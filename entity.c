/** !
 * Entity
 * 
 * @author Jacob Smith
 * @file entity.c
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

    // Zero set
    memset(p_entity, 0, sizeof(entity));

    // Return a pointer to the caller
    *pp_entity = p_entity;

    // Success
    return 1;

    no_entity:
    no_mem:
    
        // Error
        return 0;
}

int entity_from_json ( entity **pp_entity, json_value *p_value )
{

    // Initialized data
    entity   _entity = { 0 },
           *p_entity = (void *) 0;

    // Error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto entity_value_is_wrong_type;

    // Parse the json value into an instance
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

    name_property_is_too_short:
    name_property_is_too_long:
    entity_value_is_wrong_type:
    missing_properties:
    name_property_is_wrong_type:
    failed_to_construct_transform:
    failed_to_allocate_entity:

        // Error
        return 0;
}