/** !
 * Transform implementation
 * 
 * @file transform.c
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

    // Zero set
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
                    printf("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
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
    vec3        scale
)
{
    // Argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // Initialized data
    transform *p_transform = (void *) 0;

    // Allocate a transform
    if ( transform_create(&p_transform) == 0 ) goto failed_to_allocate_transform;

    // Store the location
    p_transform->location = location;

    // Store the rotation
    p_transform->rotation = rotation;

    // Store the scale
    p_transform->scale = scale;

    // Compute the model matrix
    mat4_model_from_vec3(
        p_transform->model_matrix,
        p_transform->location,
        p_transform->rotation,
        p_transform->scale
    );

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
                    printf("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_transform:
                #ifndef NDEBUG
                    printf("[g10] [transform] Failed to allocate transform in call to function \"%s\"\n", __FUNCTION__);
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
    size_t len = g_load_file(p_path, 0, false);
    char *p_text = G10_REALLOC(0, sizeof(char) * len);
    json_value *p_value = (void *) 0;

    // Load the file
    if ( g_load_file(p_path, p_text, false) == 0 ) goto failed_to_load_file;

    // Parse the file into a json value
    if ( parse_json_value(p_text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // Construct the transform
    if ( transform_from_json(&p_transform, p_value) == 0 ) goto failed_to_construct_transform;

    // Return a pointer to the caller
    *pp_transform = p_transform;

    // Success
    return 1;

    // TODO: Error handling
    {

        // Argument errors
        no_transform:
        no_path:

        // JSON errors
        failed_to_parse_json:

        // G10 errors
        failed_to_construct_transform:

        // Standard library errors
        failed_to_load_file:
            return 0;
    }
}

int transform_from_json
(
    transform  **pp_transform,
    json_value  *p_value
)
{

    // Argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;
    if ( p_value      == (void *) 0 ) goto no_value;

    // Initialized data
    transform *p_transform = (void *) 0;
    vec3 location;
    vec3 rotation;
    vec3 scale;

    // Parse the json value as a path
    if ( p_value->type == JSON_VALUE_STRING ) 
    {

        // Initialized data
        const char *const p_path = p_value->string;

        // Construct a transform
        if ( transform_from_path(&p_transform) == 0 ) goto failed_to_construct_transform;

        // Done
        goto done;

    } 

    // Parse the json value as an object
    else if ( p_value->type == JSON_VALUE_OBJECT ) 
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_location   = dict_get(p_value->object, "location"),
                   *p_rotation   = dict_get(p_value->object, "rotation"),
        //         *p_quaternion = (void *) 0,
                   *p_scale      = dict_get(p_value->object, "scale");
        json_value *p_scratch[4] = { 0 };

        // Property check
        if ( ! ( p_location && p_rotation && p_scale ) ) goto missing_properties;

        // Parse the location
        if ( p_location->type == JSON_VALUE_ARRAY )
        {

            // Initialized data
            array *p_array = p_location->list;
            size_t len     = array_size(p_array);

            // Error check
            if ( len != 3 ) goto wrong_location_len;

            // Dump the location values
            if ( array_slice(p_array, &p_scratch, 0, 2) == 0 ) goto failed_to_slice;

            // Error checking
            for (size_t i = 0; i < len; i++) 
            {
                if ( p_scratch[i]       == 0 ) goto location_element_was_wrong_type;
                if ( p_scratch[i]->type == JSON_VALUE_NUMBER ) goto location_element_was_wrong_type;  
            }

            // Store the vector
            location = (vec3)
            {
                .x = p_scratch[0]->number,
                .y = p_scratch[1]->number,
                .z = p_scratch[2]->number                
            }
        }

        // Default
        else goto wrong_location_type;
    
        // Parse the scale
        if ( p_scale->type != JSON_VALUE_ARRAY )
        {

            // Initialized data
            array *p_array = p_scale->list;
            size_t len     = array_size(p_array);

            // Error check
            if ( len != 3 ) goto wrong_scale_len;

            // Dump the p_scale values
            if ( array_slice(p_array, &p_scratch, 0, 2) == 0 ) goto failed_to_slice;

            // Error checking
            for (size_t i = 0; i < len; i++) 
            {
                if ( p_scratch[i]       == 0 ) goto scale_element_was_wrong_type;
                if ( p_scratch[i]->type == JSON_VALUE_NUMBER ) goto scale_element_was_wrong_type;  
            }

            // Store the vector
            scale = (vec3)
            {
                .x = p_scratch[0]->number,
                .y = p_scratch[1]->number,
                .z = p_scratch[2]->number                
            }
        }
        
        // Default
        else goto wrong_scale_type;
    }

    // Default
    else goto wrong_type;

    // Done
    done:

    // Return a pointer to the caller
    *pp_transform = p_transform;

    // Success
    return 1;

    // TODO:
    wrong_type:
    wrong_location_type:
    wrong_scale_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    printf("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n")
                #endif

                // Error
                return 0;
            
            no_value:
                #ifndef NDEBUG
                    printf("[g10] [transform] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n")
                #endif

                // Error
                return 0;
        }
    }
}


int transform_get_matrix ( 
    transform *p_transform, 
    mat4      *p_model_matrix
)
{

    // Argument check
    if ( p_transform    == (void *) 0 ) goto no_transform;
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // Copy the model matrix
    memcpy(p_model_matrix, &p_transform->model_matrix, sizeof(mat4));

    // Success
    return 1;
    
    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    printf("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            no_return:
                #ifndef NDEBUG
                    printf("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
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

    // Free the memory
    G10_REALLOC(p_transform, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    printf("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
