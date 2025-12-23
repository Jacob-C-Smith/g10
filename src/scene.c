// header
#include <scene.h>

// function definitions
int scene_from_json ( scene **pp_scene, json_value *p_value )
{

    // error check
    if ( NULL == pp_scene ) goto no_scene;

    scene *p_scene = default_allocator(0, sizeof(scene));

    dict *p_dict = p_value->object;
    json_value *p_name = dict_get(p_dict, "name"),
               *p_entities = dict_get(p_dict, "entities");
    
    // store the name
    strncpy(p_scene->_name, p_name->string, 63);

    // construct an entity list
    dict_construct(&p_scene->entities, 64, NULL);

    // construct entities
    {

        // initialized data
        array *p_array = p_entities->list;
        size_t len = array_size(p_array);

        // iterate through each entity
        for (size_t i = 0; i < len; i++)
        {
            
            // initialized data
            json_value *p_value = NULL;
            entity *p_entity = NULL;

            // get the i'th attachment
            array_index(p_array, i, &p_value);

            // construct an entity from a json value
            entity_from_json(&p_entity, p_value);

            // add the entity to the scene
            dict_add(p_scene->entities, p_entity->_name, p_entity);
        }
        
    }

    // return a pointer to the caller
    *pp_scene = p_scene;

    // done
    return 1;
    
    no_scene: return 0;
}
