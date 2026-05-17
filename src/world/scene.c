// header
#include <scene.h>
#include <light.h>

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
    json_value *p_name = NULL,
               *p_entities = NULL,
               *p_cameras = NULL,
               *p_lights = NULL,
               *p_skybox = NULL;
    
    dict_get(p_dict, "name"    , (void **)&p_name);
    dict_get(p_dict, "entities", (void **)&p_entities);
    dict_get(p_dict, "cameras" , (void **)&p_cameras);
    dict_get(p_dict, "lights"  , (void **)&p_lights);
    dict_get(p_dict, "skybox"  , (void **)&p_skybox);

    // store the name
    strncpy(p_scene->_name, p_name->string, 63);

    // construct an entity list
    dict_construct(&p_scene->entities, 64, NULL, (fn_key_accessor *)entity_key_accessor, NULL);
    dict_construct(&p_scene->cameras, 64, NULL, (fn_key_accessor *)camera_key_accessor, NULL);
    dict_construct(&p_scene->lights, 64, NULL, (fn_key_accessor *)light_key_accessor, NULL);

    // construct entities
    if ( p_entities )
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
            array_index(p_array, i, (void **)&p_value);

            // construct an entity from a json value
            entity_from_json(&p_entity, p_value);

            // add the entity to the scene
            dict_add(p_scene->entities, p_entity);
        }
    }

    // construct cameras
    if ( p_cameras )
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
            array_index(p_array, i, (void **)&p_value);

            // construct an camera from a json value
            camera_from_json(&p_camera, p_value);

            // add the camera to the scene
            dict_add(p_scene->cameras, p_camera);
            
            p_scene->p_active_camera = p_camera;
        }
    }

    // construct lights
    if ( p_lights )
    {

        // initialized data
        array *p_array = p_lights->list;
        size_t len = array_size(p_array);

        // iterate through each light
        for (size_t i = 0; i < len; i++)
        {
            
            // initialized data
            json_value *p_value = NULL;
            light *p_light = NULL;

            // get the i'th attachment
            array_index(p_array, i, (void **)&p_value);

            // construct a light from a json value
            light_from_json(&p_light, p_value);

            // add the light to the scene
            dict_add(p_scene->lights, p_light);
        }
    }

    // construct skybox
    if ( p_skybox )
    {
        skybox_from_json(&p_scene->p_skybox, p_skybox);
    }

    // compute the scene bounds
    bv_from_scene(&p_scene->p_bounds, p_scene);

    {
        g_instance *p_instance = g_active_instance();
        pipeline *p_pipeline = NULL;
        dict_get(p_instance->cache.p_pipeline, "aabb", (void **)&p_pipeline);
        if ( p_pipeline )
            array_add(p_pipeline->p_static_draw_list, p_scene->p_bounds);
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

    logger_pad(), printf("bounds: \n"),
    logger_push(),
    bv_info(p_scene->p_bounds),
    logger_pop(),

    logger_pad(), printf("entities: \n"),
    logger_push(),
    dict_foreach(p_scene->entities, (fn_foreach *)entity_info);
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

static void clear_dynamic_list(pipeline *p_pipeline)
{
    if ( !p_pipeline || !p_pipeline->p_dynamic_draw_list ) return;

    while(array_size(p_pipeline->p_dynamic_draw_list))
        array_remove(p_pipeline->p_dynamic_draw_list, 0, NULL);
}

static void bvh_gather_recursive(bv *p_bv, const vec4 planes[6], g_instance *p_instance)
{
    if ( !p_bv ) return;

    if ( bv_cull(p_bv, planes) ) return;

    if ( p_bv->p_user_data )
    {
        entity *p_entity = (entity *)p_bv->p_user_data;
        if ( p_entity->pipeline )
        {
            pipeline *p_pipeline = NULL;
            dict_get(p_instance->cache.p_pipeline, p_entity->pipeline, (void **)&p_pipeline);
            if ( p_pipeline && p_pipeline->p_dynamic_draw_list )
            {
                array_add(p_pipeline->p_dynamic_draw_list, p_entity);
            }
        }
    }
    else
    {
        for ( int i = 0; i < 4; i++ )
        {
            if ( p_bv->p_data[i] )
                bvh_gather_recursive((bv *)p_bv->p_data[i], planes, p_instance);
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
    
    // Clear all dynamic draw lists
    if ( p_instance->cache.p_pipeline )
        dict_foreach(p_instance->cache.p_pipeline, (fn_foreach *)clear_dynamic_list);

    if ( p_scene->p_active_camera && p_scene->p_bounds )
    {
        bvh_gather_recursive(p_scene->p_bounds, p_scene->p_active_camera->frustum.planes, p_instance);
    }

    if ( p_scene->p_skybox && p_scene->p_skybox->pipeline )
    {
        pipeline *p_pipeline = NULL;
        dict_get(p_instance->cache.p_pipeline, p_scene->p_skybox->pipeline, (void **)&p_pipeline);
        if ( p_pipeline && p_pipeline->p_dynamic_draw_list )
        {
            array_add(p_pipeline->p_dynamic_draw_list, p_scene->p_skybox);
        }
    }

    // done
    return 1;

    no_scene: return 0;
}
