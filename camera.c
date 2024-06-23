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

    // Success
    return 1;
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
            .x = eye.y - target.y,
            .x = eye.z - target.z
        }
    );

    // Compute the left vector
    vec3_cross_product(&_scratch, up, _forward);

    // Normalize the left vector
    vec3_normalize(&_left, _scratch);

    // (Re)compute the up vector
    vec3_cross_product(&_up, _forward, _left);

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
                return 0;
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
                return 0;
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
