/** !
 * Camera implementation
 * 
 * @file camera.c
 * 
 * @author Jacob Smith
 */

// Header file
#include <g10/camera.h>

// Function declarations
/** !
 * Allocate memory for a camera
 *
 * @param pp_camera result
 *
 * @return 1 on success, 0 on error
 */
int camera_create ( camera **const pp_camera );

// Function definitions
int camera_create ( camera **const pp_camera )
{

    // Argument check
    if ( pp_camera == (void *) 0 ) goto no_camera;

    // Initialized data
    camera *p_camera = G10_REALLOC(0, sizeof(camera));

    // Error check
    if ( p_camera == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_camera, 0, sizeof(camera));

    // Return a pointer to the caller
    *pp_camera = p_camera;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int camera_from_json ( camera **const pp_camera, const char *const p_name, const json_value *const p_value )
{

    // Argument check
    if ( pp_camera     ==        (void *) 0 ) goto no_camera;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    size_t i = 0;
    camera *p_camera = (void *) 0;
    camera _camera = { 0 };
    vec3 location, orientation;
    float fov, near_clip, far_clip, aspect_ratio;
    mat4 view, projection;
    dict *const p_dict = p_value->object;
    json_value *p_scratch[4] = { 0 };
    const json_value *const p_location    = dict_get(p_dict, "location"),
                     *const p_orientation = dict_get(p_dict, "orientation"),
                     *const p_fov         = dict_get(p_dict, "fov"),
                     *const p_scale       = dict_get(p_dict, "scale"),
                     *const p_clip        = dict_get(p_dict, "clip");
    
    // Extra check
    if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [camera] Consider adding a \"$schema\" property to the camera\n");

    // Property check
    if ( p_location    == (void *) 0 ) goto no_location_property;
    if ( p_orientation == (void *) 0 ) goto no_orientation_property;
    if ( p_clip        == (void *) 0 ) goto no_clip_property;
    if ( ! ( p_fov || p_scale )      ) goto no_fov_property;
    if ( p_fov && p_scale            ) goto fov_and_scale_property;

    // Type check
    if ( p_location->type    != JSON_VALUE_ARRAY  ) goto wrong_location_type;
    if ( p_orientation->type != JSON_VALUE_ARRAY  ) goto wrong_orientation_type;
    if ( p_clip->type        != JSON_VALUE_OBJECT ) goto wrong_clip_type;

    // Parse the location
    {

        // Initialized data
        array *p_array = p_location->list;
        size_t len     = array_size(p_array);

        // Error check
        if ( len != 3 ) goto wrong_location_len;

        // Dump the location values
        array_slice(p_array, (void **) &p_scratch, 0, 2);

        // Error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto location_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto location_element_was_wrong_type;  
        }

        // Store the vector
        location = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number                
        };
    }
    
    // Parse the orientation
    {

        // Initialized data
        array *p_array = p_orientation->list;
        size_t len     = array_size(p_array);

        // Error check
        if ( len != 3 ) goto wrong_orientation_len;

        // Dump the p_orientation values
        array_slice(p_array, (void **)&p_scratch, 0, 2);

        // Error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto orientation_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto orientation_element_was_wrong_type;  
        }

        // Store the vector
        orientation = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number
        };
    }

    // Parse the clip
    {

        // Initialized data
        dict *const p_dict = p_clip->object;
        const json_value *const p_near = dict_get(p_dict, "near"),
                         *const p_far  = dict_get(p_dict, "far");

        // Property check
        if ( p_near == (void *) 0 ) goto no_clip_near_property;
        if ( p_far  == (void *) 0 ) goto no_clip_far_property;

        // Type check
        if ( p_near->type != JSON_VALUE_NUMBER ) goto wrong_clip_near_type;
        if ( p_far->type  != JSON_VALUE_NUMBER ) goto wrong_clip_far_type;

        // Store the distance to the near clipping plane
        near_clip = p_near->number;

        // Store the distance to the far clipping plane
        far_clip = p_far->number;

        // Error check
        if ( near_clip > far_clip ) goto clip_bounds_error;
    }

    // Parse the lens ...
    if ( p_fov )

        // ... as field of view ...
        goto parse_fov;

    // ... or ...
    else
        
        // ... as a orthographic scale
        goto parse_scale;

    lens_parsed:

    // Construct a camera
    _camera = (camera)
    {
        ._name = { 0 },
        .pfn_camera_controller = camera_controller_orbit_update,
        .dirty = true,
        .view  = 
        {
            .location = location,
            .target   = orientation,
            .up       = (vec3) { 0.f, 0.f, 1.f }
        },
        .projection = 
        {
            .fov          = fov,
            .near_clip    = near_clip,
            .far_clip     = far_clip,
            .aspect_ratio = (p_instance) ? ((double)p_instance->window.width / (double)p_instance->window.height) : (16.0 / 9.0)
        },
        .matrix = 
        {
            ._projection = { 0 },
            ._view = { 0 }
        }
    };

    // Compute the view matrix
    camera_matrix_view
    (
        &_camera.matrix._view,
        _camera.view.location,
        _camera.view.target,
        _camera.view.up
    );
    
    // Compute the projection matrix
    camera_matrix_projection_perspective
    (
        &_camera.matrix._projection,
        _camera.projection.fov,
        _camera.projection.aspect_ratio,
        _camera.projection.near_clip,
        _camera.projection.far_clip
    );

    // Copy the name
    if ( p_name ) goto copy_name;

    // Done
    done:

    // Allocate memory for camera
    camera_create(&p_camera);

    // Error check
    if ( p_camera == (void *) 0 ) goto no_mem;

    // Copy the camera 
    memcpy(p_camera, &_camera, sizeof(camera));

    // Return a pointer to the caller
    *pp_camera = p_camera;

    // Success
    return 1;

    // Copy the name of the camera
    copy_name:
    {

        // Initialized data
        size_t len = strlen(p_name);

        // Error check
        if ( len > 63 ) goto name_too_long;
        if ( len == 0 ) goto name_too_short;

        // Copy the name
        strncpy(_camera._name, p_name, len);

        goto done;
    }

    parse_fov:
    {

        // Type check
        if ( p_fov->type != JSON_VALUE_NUMBER ) goto wrong_fov_type;

        // Store the fov
        fov = p_fov->number;
        
        // Error check
        if ( fov <  1.0 ) goto fov_too_short;
        if ( fov > 90.0 ) goto fov_too_long;

        // 

        // Done
        goto lens_parsed;
    }

    parse_scale:
    {

        // Type check
        if ( p_scale->type != JSON_VALUE_NUMBER ) goto wrong_scale_type;

        // Store the fov
        fov = p_scale->number;
        
        // Error check
        if ( fov < 0.0010 ) goto fov_too_short;
        if ( fov > 1000.0 ) goto fov_too_long;

        // Done
        goto lens_parsed;
    }

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            name_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_name\" must be at least 1 character in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            name_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_name\" must be less than 63 characters in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            no_location_property:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" is missing required property \"location\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            no_orientation_property:

                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" is missing required property \"orientation\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            no_clip_property:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" is missing required property \"clip\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            no_clip_near_property:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" is missing required property \"clip.near\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            no_clip_far_property:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" is missing required property \"clip.far\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;
            
            no_fov_property:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" is missing required property \"fov\" - OR - \"scale\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            fov_and_scale_property:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Parameter \"p_value\" contains mutually exclusive \"fov\" - AND - \"scale\" properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_location_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"location\" of camera object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_orientation_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"orientation\" of camera object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_clip_type: 
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"clip\" of camera object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_location_len:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"location\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_orientation_len:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"orientation\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            location_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Element %d of \"location\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            orientation_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Element %d of \"orientation\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_clip_near_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"clip.near\" of camera object must be of type [ number ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_clip_far_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"clip.far\" of camera object must be of type [ number ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            clip_bounds_error:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"clip.far\" must be greater than property \"clip.near\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_fov_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"fov\" of parameter \"p_value\" must be of type [ number ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            fov_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"fov\" of parameter \"p_value\" must be greater than 1.0 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            fov_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"fov\" of parameter \"p_value\" must be less than 90.0 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            wrong_scale_type:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"scale\" of parameter \"p_value\" must be of type [ number ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            scale_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"scale\" of parameter \"p_value\" must be greater than 0.001 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;

            scale_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Property \"scale\" of parameter \"p_value\" must be less than 1000.0 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/camera.json\n");
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

u0 camera_matrix_view ( mat4 *const p_view, vec3 eye, vec3 target, vec3 up )
{
    
    // Argument check
    if ( p_view == (void *) 0 ) goto no_view;

    // Initialized data
    vec3 _forward = { 0 },
         _left    = { 0 },
         _up      = { 0 },
         _scratch = { 0 };
    
    // Compute the forward vector
    vec3_normalize (
        &_forward,
        (vec3)
        {
            .x = eye.x - target.x,
            .y = eye.y - target.y,
            .z = eye.z - target.z
        }
    );

    // Compute the left vector
    vec3_cross_product(&_scratch, up, _forward);

    // Normalize the left vector
    vec3_normalize(&_left, _scratch);

    // (Re)compute the up vector
    vec3_cross_product(&_up, _left, _forward);

    //vec3_mul_scalar(&_up, _up, -1.0f);

    // Store the orientation
    *p_view = (mat4)
    {
        .a = _left.x, .b = _up.x, .c = _forward.x, .d = 0,
        .e = _left.y, .f = _up.y, .g = _forward.y, .h = 0,
        .i = _left.z, .j = _up.z, .k = _forward.z, .l = 0,
        .m =       0, .n =     0, .o =          0, .p = 1,        
    };

    // Store the location
    vec3_dot_product(&p_view->m, _left   , eye);
    vec3_dot_product(&p_view->n, _up     , eye);
    vec3_dot_product(&p_view->o, _forward, eye);
    p_view->m *= -1;
    p_view->n *= -1;
    p_view->o *= -1;

    // Done
    return;

    // Error handling
    {

        // Argument errors
        {
            no_view:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"p_view\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return;
        }
    }
}

u0 camera_matrix_projection_perspective ( mat4 *const p_projection, float fov, float aspect, float near_clip, float far_clip )
{
    
    // Argument check
    if ( p_projection == (void *) 0 ) goto no_projection;
    
    // Initialized data
    const float tan_fov_2 = tanf(fov / 2.0);

    // Store the orientation
    *p_projection = (mat4)
    {
        .a = 1.0 / (aspect * tan_fov_2), .b = 0              , .c = 0                                                , .d =  0,
        .e = 0                         , .f = 1.0 / tan_fov_2, .g = 0                                                , .h =  0,
        .i = 0                         , .j = 0              , .k = -(far_clip + near_clip) / (far_clip - near_clip) , .l = -1,
        .m = 0                         , .n = 0              , .o = 2 * far_clip * near_clip / (near_clip - far_clip), .p =  0,        
    };

    // Done
    return;

    // Error handling
    {

        // Argument errors
        {
            no_projection:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"p_projection\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return;
        }
    }
}

int camera_info ( const camera *const p_camera )
{

    // Argument check
    if ( p_camera == (void *) 0 ) goto no_camera;

    // Print the camera
    printf("Camera:\n");
    printf(" - name : %s\n", p_camera->_name);
    printf(" - view : ");
    printf("    - location : < %f, %f, %f >\n", p_camera->view.location.x, p_camera->view.location.y, p_camera->view.location.z);
    printf("    - target   : < %f, %f, %f >\n", p_camera->view.target.x, p_camera->view.target.y, p_camera->view.target.z);
    printf("    - up       : < %f, %f, %f >\n", p_camera->view.up.x, p_camera->view.up.y, p_camera->view.up.z);
    printf(" - projection :\n");
    printf("    - fov    : %f\n", p_camera->projection.fov);
    printf("    - clip   : < %f, %f >\n", p_camera->projection.near_clip, p_camera->projection.far_clip);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"p_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int camera_destroy ( camera **const pp_camera )
{

    // Argument check
    if ( pp_camera == (void *) 0 ) goto no_camera;

    // Initialized data
    camera *p_camera = *pp_camera;

    // No more pointer for caller
    *pp_camera = (void *) 0;

    // Release memory
    p_camera = G10_REALLOC(p_camera, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
