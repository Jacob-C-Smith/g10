/** !
 * Entity
 * 
 * @file entity.c
 * 
 * @author Jacob Smith
 */

// Header
#include <g10/entity.h>

// Function definitions
int entity_create ( entity **pp_entity )
{

    // Argument check
    if ( pp_entity == (void *) 0 ) goto no_entity;

    // Initialized data
    entity *p_entity = G10_REALLOC(0, sizeof(entity));

    // Error check
    if ( p_entity == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_entity, 0, sizeof(entity));

    // Return a pointer to the caller
    *pp_entity = p_entity;

    // Success
    return 1;
        
    // Error handling
    {

        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Null pointer provided for parameter \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
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

int entity_from_json ( entity **pp_entity, json_value *p_value )
{

    // Argument check
    if ( p_value       ==        (void *) 0 ) goto no_entity;
    if ( pp_entity     ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto value_is_wrong_type;

    // Initialized data
    entity   _entity = { 0 },
           *p_entity = (void *) 0;

    // Parse the json value into an entity
    {

        // Initialized data
        const dict *const p_dict = p_value->object;
        const json_value *p_name_value      = dict_get(p_dict, "name"),
                         *p_transform_value = dict_get(p_dict, "transform");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [entity] Consider adding a \"$schema\" property to the entity\n");

        // Check for missing properties
        if ( ! ( p_name_value ) ) goto missing_properties;

        // Error check
        if ( p_name_value->type != JSON_VALUE_STRING ) goto name_property_is_wrong_type;

        // Store the name
        {

            // Initialized data
            size_t len = strlen(p_name_value->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the instance name
            strncpy(_entity._name, p_name_value->string, 255);

            // Store a null terminator
            _entity._name[len] = '\0';
        }

        // Store the transform
        if ( transform_from_json(&_entity.p_transform, p_transform_value) == 0 ) goto failed_to_construct_transform;
    }

    // Allocate memory on the heap
    if ( entity_create(&p_entity) == (void *) 0 ) goto failed_to_allocate_entity;

    // Copy the struct to the heap
    memcpy(p_entity, &_entity, sizeof(entity));

    // Return a pointer to the caller
    *pp_entity = p_entity;

    // Success
    return 1;

    // Error handling
    {


        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Null pointer provided for parameter \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Null pointer provided for parameter \"no_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/entity.json\n");
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_construct_transform:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Failed to construct transform in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_allocate_entity:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Failed to allocate entity in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [entity] \"name\" property of entity object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/entity.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [entity] \"name\" property of entity object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/entity.json\n");
                #endif

                // Error
                return 0;

            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [entity] \"name\" property of entity object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/entity.json\n");
                #endif

                // Error
                return 0;

            missing_properties:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Missing required properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/entity.json\n");
                #endif

                // Error
                return 0;
        }
    }
}

int entity_bounding_box ( entity *p_entity, vec3 *p_min, vec3 *p_max )
{

    // Argument check
    if ( p_entity == (void *) 0 ) goto no_entity;
    if ( p_min    == (void *) 0 ) goto no_min;
    if ( p_max    == (void *) 0 ) goto no_max;
    
    // Initialized data
    //

    // Success
    return 1;

    // Error handling
    {


        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_min:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Null pointer provided for parameter \"p_min\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_max:
                #ifndef NDEBUG
                    log_error("[g10] [entity] Null pointer provided for parameter \"p_max\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}