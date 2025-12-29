// header
#include <scene.h>

// function definitions
int scene_from_json ( scene **pp_scene, json_value *p_value )
{

    // error check
    if ( NULL == pp_scene ) goto no_scene;

    scene *p_scene = default_allocator(0, sizeof(scene));

    // type check
    if ( JSON_VALUE_STRING == p_value->type )
    {

        size_t file_len = load_file(p_value->string, (void *) 0, true);
        char *p_file_contents = default_allocator(0, (file_len + 1) * sizeof(char));
        
        // error check
        if ( file_len == 0 ) return 0;

        // load the file
        if ( load_file(p_value->string, p_file_contents, true) == 0 ) return 0;

        // parse the json value
        json_value_parse(p_file_contents, NULL, &p_value);
    }

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

int scene_info ( scene *p_scene )
{

    // error check
    if ( NULL == p_scene ) goto no_scene;

    // print the scene
    logger_pad(), log_info("Scene @%p\n", p_scene),

    logger_push(),
    logger_pad(), printf("name - %s\n", p_scene->_name),

    logger_pad(), printf("entities: \n"),
    logger_push(),
    dict_foreach(p_scene->entities, (void (*)(const void *const, size_t i))entity_info);
    logger_pop(),
    
    logger_pop();

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Null pointer provided for parameter \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
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