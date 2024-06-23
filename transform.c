/** !
 * Transform implementation
 * 
 * @file transform.c
 * 
 * @author Jacob Smith
 */

// Header
#include <g10/transform.h>

int transform_create ( transform **pp_transform )
{

    // Argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // Initialized data
    transform *p_transform = G10_REALLOC(0, sizeof(transform));

    // Error check
    if ( p_transform == 0 ) goto no_mem;

    // Initialize
    memset(p_transform, 0, sizeof(transform));

    // Return a pointer to the caller
    *pp_transform = p_transform;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
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

int transform_construct (
    transform **pp_transform,
    vec3        location,
    vec3        rotation,
    vec3        scale,
    transform  *p_parent
)
{
    // Argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // Initialized data
    transform  _transform  = { 0 },
              *p_transform = (void *) 0;

    // Populate the transform 
    _transform = (transform)
    {
        .location   = location,
        .rotation   = rotation,
        .scale      = scale,
        .model      = { 0 },
        .p_childern = 0,
        .p_parent   = 0
    };

    // Compute the model matrix
    mat4_model_from_vec3(
        &_transform.model,
        _transform.location,
        _transform.rotation,
        _transform.scale
    );

    // Allocate a transform
    if ( transform_create(&p_transform) == 0 ) goto failed_to_allocate_transform;

    // Copy the transform from the stack to the heap
    memcpy(p_transform, &_transform, sizeof(transform));

    // Return a pointer to the caller
    *pp_transform = p_transform;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to allocate transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int transform_from_path
(
    transform **pp_transform,
    const char *p_path
)
{
     
    // Argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;
    if ( p_path       == (void *) 0 ) goto no_path;

    // Initialized data
    size_t       len         = g_load_file(p_path, 0, false);
    char        *p_text      = G10_REALLOC(0, sizeof(char) * len);
    transform   *p_transform = (void *) 0;
    json_value  *p_value     = (void *) 0;

    // Load the file
    if ( g_load_file(p_path, p_text, false) == 0 ) goto failed_to_load_file;

    // Parse the file into a json value
    if ( json_value_parse(p_text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Construct the transform
    if ( transform_from_json(&p_transform, p_value) == 0 ) goto failed_to_construct_transform;

    // Return a pointer to the caller
    *pp_transform = p_transform;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_construct_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to construct transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to parse json in call to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to load file \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int transform_from_json
(
    transform  **pp_transform,
    json_value  *p_value
)
{

    // Argument check
    if ( pp_transform  ==        (void *) 0 ) goto no_transform;
    if ( p_value       ==        (void *) 0 ) goto no_value;

    // Accept check
    if ( p_value->type == JSON_VALUE_STRING ) goto parse_as_path;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    size_t i = 0;
    transform *p_transform = (void *) 0;
    transform _transform = { 0 };
    vec3 location, rotation, scale;
    mat4 model;
    dict *const p_dict = p_value->object;
    json_value *p_scratch[4] = { 0 };
    const json_value *const p_location   = dict_get(p_dict, "location"),
                     *const p_rotation   = dict_get(p_dict, "rotation"),
                     *const p_quaternion = dict_get(p_dict, "quaternion"),
                     *const p_scale      = dict_get(p_dict, "scale");
    
    // Extra check
    if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [transform] Consider adding a \"$schema\" property to the transform\n");

    // Property check
    if ( p_location   == (void *) 0 )       goto no_location_property;
    if ( ! ( p_rotation || p_quaternion ) ) goto no_rotation_property;
    if ( p_rotation && p_quaternion )       goto rotation_and_quaternion_property;
    if ( p_scale      == (void *) 0 )       goto no_scale_property;

    // Type check
    if ( p_location->type != JSON_VALUE_ARRAY ) goto wrong_location_type;
    if ( p_scale->type    != JSON_VALUE_ARRAY ) goto wrong_scale_type;

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
    
    // Parse the rotation ...
    if ( p_rotation )

        // ... as an euler angle ...
        goto parse_rotation;

    // ... or ...
    else
        
        // ... as a quaternion
        goto parse_quaternion;

    rotation_parsed:

    // Parse the scale
    {

        // Initialized data
        array *p_array = p_scale->list;
        size_t len     = array_size(p_array);

        // Error check
        if ( len != 3 ) goto wrong_scale_len;

        // Dump the p_scale values
        array_slice(p_array, (void **)&p_scratch, 0, 2);

        // Error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto scale_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto scale_element_was_wrong_type;  
        }

        // Store the vector
        scale = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number
        };
    }
        
    // Calculate the model matrix
    mat4_model_from_vec3(&model, location, (vec3){ 0, 0, 0 }, scale);

    // Construct a transform
    _transform = (transform)
    {
        .location = location,
        .rotation = { 0, 0, 0 },
        .scale    = scale,
        .model    = model
    };

    // Allocate memory for transform
    transform_create(&p_transform);

    // Error check
    if ( p_transform == (void *) 0 ) goto no_mem;

    // Copy the transform 
    memcpy(p_transform, &_transform, sizeof(transform));

    // Done
    done:

    // Return a pointer to the caller
    *pp_transform = p_transform;

    // Success
    return 1;

    // Parse the transform as a path
    parse_as_path:
    {

        // Initialized data
        const char *const p_path = p_value->string;

        // Construct a transform
        //if ( transform_from_path(&p_transform, ) == 0 ) goto failed_to_construct_transform;

        // Done
        goto done;
    } 

    parse_rotation:
    {

        // Type check
        if ( p_rotation->type != JSON_VALUE_ARRAY ) goto wrong_rotation_type;

        // Initialized data
        array *p_array = p_rotation->list;
        size_t len     = array_size(p_array);

        // Error check
        if ( len != 3 ) goto wrong_rotation_len;

        // Dump the p_rotation values
        array_slice(p_array, (void **)&p_scratch, 0, 2);

        // Error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto rotation_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto rotation_element_was_wrong_type;  
        }

        // Store the vector
        rotation = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number
        };

        // Done
        goto rotation_parsed;
    }


    parse_quaternion:
    {

        // Type check
        if ( p_quaternion->type != JSON_VALUE_ARRAY ) goto wrong_quaternion_type;

        // Initialized data
        array *p_array = p_quaternion->list;
        size_t len     = array_size(p_array);

        // Error check
        if ( len != 4 ) goto wrong_quaternion_len;

        // Dump the p_quaternion values
        array_slice(p_array, (void **)&p_scratch, 0, 3);

        // Error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto quaternion_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto quaternion_element_was_wrong_type;  
        }

        // Store the quaternion
        quaternion _q = (quaternion)
        {
            .u = (float) p_scratch[0]->number,
            .i = (float) p_scratch[1]->number,
            .j = (float) p_scratch[2]->number,
            .k = (float) p_scratch[3]->number,
        };

        // Store the rotation
        quaternion_to_euler_angle(&rotation, _q);

        // Done
        goto rotation_parsed;
    }

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" must be of type [ object | string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            no_location_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" is missing required property \"location\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            no_rotation_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" is missing required property \"rotation\" - OR - \"quaternion\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            rotation_and_quaternion_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" contains mutually exclusive \"rotation\" - AND - \"quaternion\" properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            no_scale_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" is missing required property \"scale\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_location_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"location\" of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_location_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"location\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            location_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"location\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_rotation_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"rotation\" of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_rotation_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"rotation\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            rotation_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"rotation\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_quaternion_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"quaternion\" of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_quaternion_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"quaternion\" of parameter \"p_value\" must be of length 4 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            quaternion_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"quaternion\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_scale_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"scale\" of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            wrong_scale_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"scale\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // Error
                return 0;

            scale_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"scale\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
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

int transform_get_matrix_local ( 
    transform *p_transform, 
    mat4 *p_model_matrix
)
{
    
    // Argument check
    if ( p_transform    == (void *) 0 ) goto no_transform;
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // Copy the local matrix
    memcpy(p_model_matrix, &p_transform->model, sizeof(mat4));

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int transform_get_matrix_world_recursive ( 
    transform *p_transform,
    mat4 *p_model_matrix
)
{
    
    // Argument check
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // Base case
    if ( p_transform == (void *) 0 ) goto no_transform;

    // Initialized data
    mat4 parent_model = { 0 };

    //transform_get_matrix_world_recursive(p_transform->parent_model, &parent_model)

    // Apply the transform
    mat4_mul_mat4(p_model_matrix, parent_model, p_transform->model);

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int transform_get_matrix_world ( 
    transform *p_transform, 
    mat4 *p_model_matrix
)
{
    
    // Argument check
    if ( p_transform    == (void *) 0 ) goto no_transform;
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // Initialized data
    mat4 matrix_world = { 0 };

    // Identity matrix
    mat4_identity(&matrix_world);

    // Recursively build the world matrix
    transform_get_matrix_world_recursive(p_transform, &matrix_world);

    // Copy the local matrix
    memcpy(p_model_matrix, &p_transform->model, sizeof(mat4));

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int transform_destroy ( transform **pp_transform )
{

    // Argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // Initialized data
    transform *p_transform = *pp_transform;

    // No more pointer for caller
    *pp_transform = (void *) 0;

    // Release the memory
    p_transform = G10_REALLOC(p_transform, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
