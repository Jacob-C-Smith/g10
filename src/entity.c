// header
#include <entity.h>

// function definitions
int entity_from_json ( entity **pp_entity, json_value *p_value )
{

    // error check
    if ( NULL == pp_entity ) goto no_entity;

    entity *p_entity = default_allocator(0, sizeof(entity));

    dict *p_dict = p_value->object;
    json_value *p_name = dict_get(p_dict, "name"),
               *p_transform = dict_get(p_dict, "transform");

    // store the name
    strncpy(p_entity->_name, p_name->string, 63);

    // construct a transform
    transform_from_json(&p_entity->p_transform, p_transform);

    // return a pointer to the caller
    *pp_entity = p_entity;

    // done
    return 1;
    
    no_entity: return 0;
}
