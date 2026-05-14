// header
#include <transform.h>

int transform_create ( transform **pp_transform )
{

    // argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // initialized data
    transform *p_transform = default_allocator(0, sizeof(transform));

    // error check
    if ( p_transform == 0 ) goto no_mem;

    // initialize
    memset(p_transform, 0, sizeof(transform));

    // return a pointer to the caller
    *pp_transform = p_transform;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
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
    // argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // initialized data
    transform  _transform  = { 0 },
              *p_transform = (void *) 0;

    // populate the transform 
    _transform = (transform)
    {
        .location   = location,
        .rotation   = rotation,
        .scale      = scale,
        .model      = { 0 },
    };

    // compute the model matrix
    mat4_model_from_vec3(
        &_transform.model,
        _transform.location,
        _transform.rotation,
        _transform.scale
    );

    // allocate a transform
    if ( transform_create(&p_transform) == 0 ) goto failed_to_allocate_transform;

    // copy the transform from the stack to the heap
    memcpy(p_transform, &_transform, sizeof(transform));

    // return a pointer to the caller
    *pp_transform = p_transform;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }

        // g10 errors
        {
            failed_to_allocate_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to allocate transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
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
     
    // argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;
    if ( p_path       == (void *) 0 ) goto no_path;

    // initialized data
    size_t       len         = load_file(p_path, 0, false);
    char        *p_text      = default_allocator(0, sizeof(char) * len);
    transform   *p_transform = (void *) 0;
    json_value  *p_value     = (void *) 0;

    // load the file
    if ( load_file(p_path, p_text, false) == 0 ) goto failed_to_load_file;

    // parse the file into a json value
    if ( json_value_parse(p_text, 0, &p_value) == 0 ) goto failed_to_parse_json;

    // construct the transform
    if ( transform_from_json(&p_transform, p_value) == 0 ) goto failed_to_construct_transform;

    // return a pointer to the caller
    *pp_transform = p_transform;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // g10 errors
        {
            failed_to_construct_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to construct transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to parse json in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Failed to load file \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // error
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

    // argument check
    if ( pp_transform  ==        (void *) 0 ) goto no_transform;
    if ( p_value       ==        (void *) 0 ) goto no_value;

    // accept check
    if ( p_value->type == JSON_VALUE_STRING ) goto parse_as_path;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    size_t i = 0;
    transform *p_transform = (void *) 0;
    transform _transform = { 0 };
    vec3 location, rotation, scale;
    mat4 model;
    dict *const p_dict = p_value->object;
    json_value *p_scratch[4] = { 0 };
    json_value *p_location   = NULL,
               *p_rotation   = NULL,
               *p_quaternion = NULL,
               *p_scale      = NULL;

    dict_get(p_dict, "location"  , (void **)&p_location);
    dict_get(p_dict, "rotation"  , (void **)&p_rotation);
    dict_get(p_dict, "quaternion", (void **)&p_quaternion);
    dict_get(p_dict, "scale"     , (void **)&p_scale);

    // property check
    if ( p_location   == (void *) 0 )       goto no_location_property;
    if ( ! ( p_rotation || p_quaternion ) ) goto no_rotation_property;
    if ( p_scale      == (void *) 0 )       goto no_scale_property;

    // type check
    if ( p_location->type != JSON_VALUE_ARRAY ) goto wrong_location_type;
    if ( p_scale->type    != JSON_VALUE_ARRAY ) goto wrong_scale_type;

    // parse the location
    {

        // initialized data
        array *p_array = p_location->list;
        size_t len     = array_size(p_array);

        // error check
        if ( len != 3 ) goto wrong_location_len;

        // dump the location values
        array_slice(p_array, (void **) &p_scratch, 0, 2);

        // error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto location_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto location_element_was_wrong_type;  
        }

        // store the vector
        location = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number                
        };
    }

    // parse the rotation
    {

        // initialized data
        array *p_array = p_rotation->list;
        size_t len     = array_size(p_array);

        // error check
        if ( len != 3 ) goto wrong_location_len;

        // dump the location values
        array_slice(p_array, (void **) &p_scratch, 0, 2);

        // error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto rotation_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto rotation_element_was_wrong_type;  
        }

        // store the vector
        rotation = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number                
        };
    }
    
    // parse the scale
    {

        // initialized data
        array *p_array = p_scale->list;
        size_t len     = array_size(p_array);

        // error check
        if ( len != 3 ) goto wrong_scale_len;

        // dump the p_scale values
        array_slice(p_array, (void **)&p_scratch, 0, 2);

        // error check
        for ( i = 0; i < len; i++ ) 
        {
            if ( p_scratch[i]       ==        (void *) 0 ) goto scale_element_was_wrong_type;
            if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto scale_element_was_wrong_type;  
        }

        // store the vector
        scale = (vec3)
        {
            .x = (float) p_scratch[0]->number,
            .y = (float) p_scratch[1]->number,
            .z = (float) p_scratch[2]->number
        };
    }       
        
    // calculate the model matrix
    mat4_model_from_vec3(&model, location, rotation, scale);

    // construct a transform
    _transform = (transform)
    {
        .location = location,
        .rotation = rotation,
        .scale    = scale,
        .model    = model
    };

    // allocate memory for transform
    transform_create(&p_transform);

    // error check
    if ( p_transform == (void *) 0 ) goto no_mem;

    // copy the transform 
    memcpy(p_transform, &_transform, sizeof(transform));

    // done
    done:

    // return a pointer to the caller
    *pp_transform = p_transform;

    // success
    return 1;

    // parse the transform as a path
    parse_as_path:
    {

        // initialized data
        const char *const p_path = p_value->string;

        // construct a transform
        //if ( transform_from_path(&p_transform, ) == 0 ) goto failed_to_construct_transform;

        // done
        goto done;
    } 

    // error handling
    {

        // argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" must be of type [ object | string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            no_location_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" is missing required property \"location\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            no_rotation_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" is missing required property \"rotation\" - OR - \"quaternion\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            no_scale_property:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Parameter \"p_value\" is missing required property \"scale\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            wrong_location_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] \"location\" property of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            wrong_location_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"location\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            location_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"location\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            wrong_rotation_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] \"rotation\" property of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            wrong_rotation_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"rotation\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            rotation_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"rotation\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            wrong_scale_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] \"scale\" property of transform object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            wrong_scale_len:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Property \"scale\" of parameter \"p_value\" must be of length 3 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;

            scale_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Element %d of \"scale\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int transform_get_matrix_local ( 
    transform *p_transform, 
    mat4 *p_model_matrix
)
{
    
    // argument check
    if ( p_transform    == (void *) 0 ) goto no_transform;
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // copy the local matrix
    memcpy(p_model_matrix, &p_transform->model, sizeof(mat4));

    // success
    return 1;

    // error handling
    {

        // argument error
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }
    }
}

int transform_get_matrix_world_recursive ( 
    transform *p_transform,
    mat4 *p_model_matrix
)
{
    
    // argument check
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // base case
    if ( p_transform == (void *) 0 ) goto no_transform;

    // initialized data
    mat4 parent_model = { 0 };

    //transform_get_matrix_world_recursive(p_transform->parent_model, &parent_model)

    // apply the transform
    mat4_mul_mat4(p_model_matrix, parent_model, p_transform->model);

    // success
    return 1;

    // error handling
    {

        // argument error
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }
    }
}

int transform_get_matrix_world ( 
    transform *p_transform, 
    mat4 *p_model_matrix
)
{
    
    // argument check
    if ( p_transform    == (void *) 0 ) goto no_transform;
    if ( p_model_matrix == (void *) 0 ) goto no_return;

    // initialized data
    mat4 matrix_world = { 0 };

    // identity matrix
    mat4_identity(&matrix_world);

    // recursively build the world matrix
    transform_get_matrix_world_recursive(p_transform, &matrix_world);

    // copy the local matrix
    memcpy(p_model_matrix, &p_transform->model, sizeof(mat4));

    // success
    return 1;

    // error handling
    {

        // argument error
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_return:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_model_matrix\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

        }
    }
}

int transform_bind ( render_pass *p_render_pass, pipeline *p_pipeline, transform *p_transform )
{
     
    // initialized data
    uniform *p_m   = (void *) 0;
    uniform *p_inv = (void *) 0;
    transform *p_iter = p_transform;
    mat4 _accumulator = { 0 };

    // initialize 
    mat4_identity(&_accumulator);

    // get the transform uniform
    array_index(p_pipeline->p_uniforms, 1, (void **)&p_m);
    
    while ( p_iter )
    {
        mat4 _temp = { 0 };

        // accumulator = parent * accumulator
        mat4_mul_mat4(&_temp, p_iter->model, _accumulator);
        _accumulator = _temp;

        p_iter = p_iter->p_parent;
    }

    
    // bind model matrix
    if ( p_m ) uniform_set_pack_push(p_m, &_accumulator, (fn_pack *)mat4_pack);

    // bind inv normal matrix
    if ( array_index(p_pipeline->p_uniforms, 0, (void **)&p_inv) )
    {
        mat4 inv = { 0 }, inv_trans = { 0 };
        mat4_inverse(&inv, _accumulator);
        mat4_transpose(&inv_trans, inv);
        uniform_set_pack_push(p_inv, &inv_trans, (fn_pack *)mat4_pack);
    }
    
    return 1;
}

int transform_info ( transform *p_transform )
{

    logger_pad(), log_info("Transform @%p\n", p_transform);

    logger_push(),
    logger_pad(), printf("location - [ %.2f, %.2f, %.2f ]\n", p_transform->location.x, p_transform->location.y, p_transform->location.z);
    logger_pad(), printf("rotation - [ %.2f, %.2f, %.2f ]\n", p_transform->rotation.x, p_transform->rotation.y, p_transform->rotation.z);
    logger_pad(), printf("scale    - [ %.2f, %.2f, %.2f ]\n", p_transform->scale.x, p_transform->scale.y, p_transform->scale.z);
    logger_pop();

    return 1;
}

int transform_from_aabb ( transform *p_transform, aabb *p_aabb )
{

    // argument check
    if ( p_transform == (void *) 0 ) goto no_transform;
    if ( p_aabb      == (void *) 0 ) goto no_aabb;

    // rotation is zero
    p_transform->rotation = (vec3) { 0, 0, 0 };

    // location = (max + min) / 2
    vec3_add_vec3(&p_transform->location, p_aabb->_max, p_aabb->_min);
    vec3_mul_scalar(&p_transform->location, p_transform->location, 0.5f);

    // scale = (max - min) / 2
    vec3_sub_vec3(&p_transform->scale, p_aabb->_max, p_aabb->_min);
    vec3_mul_scalar(&p_transform->scale, p_transform->scale, 0.5f);

    // compute the model matrix
    mat4_model_from_vec3(
        &p_transform->model,
        p_transform->location,
        p_transform->rotation,
        p_transform->scale
    );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_aabb:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"p_aabb\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int transform_destroy ( transform **pp_transform )
{

    // argument check
    if ( pp_transform == (void *) 0 ) goto no_transform;

    // initialized data
    transform *p_transform = *pp_transform;

    // no more pointer for caller
    *pp_transform = (void *) 0;

    // release the memory
    p_transform = default_allocator(p_transform, 0);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [transform] Null pointer provided for parameter \"pp_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
