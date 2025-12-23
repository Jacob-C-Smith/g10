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
               *p_entities = dict_get(p_dict, "entities"),
               *p_cameras = dict_get(p_dict, "cameras");
    
    // store the name
    strncpy(p_scene->_name, p_name->string, 63);

    // construct an entity list
    dict_construct(&p_scene->entities, 64, NULL);
    dict_construct(&p_scene->cameras, 64, NULL);

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

    // construct cameras
    {

        // initialized data
        array *p_array = p_cameras->list;
        size_t len = array_size(p_array);

        // iterate through each camera
        for (size_t i = 0; i < len; i++)
        {
            
            // initialized data
            json_value *p_value = NULL;
            camera *p_camera = NULL;

            // get the i'th attachment
            array_index(p_array, i, &p_value);

            // construct an camera from a json value
            camera_from_json(&p_camera, p_value);

            // add the camera to the scene
            dict_add(p_scene->cameras, p_camera->_name, p_camera);
            
            p_scene->p_active_camera = p_camera;
        }
    }

    // return a pointer to the caller
    *pp_scene = p_scene;

    // done
    return 1;
    
    no_scene: return 0;
}

int scene_gather_drawable 
( 
    scene *p_scene
)
{

    // argument check
    if ( NULL == p_scene ) goto no_scene;
    
    // initialized data
    g_instance *p_instance = g_active_instance();
    size_t entity_count = dict_values(p_scene->entities, 0);
    
    entity *_p_entities[64] = {0};
    dict_values(p_scene->entities, &_p_entities);

    // iterate through each entity in the scene
    for ( size_t i = 0; i < entity_count; i++ )
    {
        
        // initialized data
        pipeline *p_pipeline = NULL;
        entity *p_entity = _p_entities[i];

        // error check
        if ( NULL == p_entity ) goto no_drawable_list;

        // store the pipeline
        p_pipeline = dict_get(p_instance->cache.p_pipeline, p_entity->pipeline);
    }

    // done
    return 1;

    no_scene: return 0;
    no_drawable_list: return 0;
}