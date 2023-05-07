#include <G10/GXScene.h>

void init_scene(void)
{

    // Initialized data
    GXInstance_t* p_instance = g_get_active_instance();

    p_instance->mutexes.load_entity = SDL_CreateMutex();
}

int create_scene ( GXScene_t **pp_scene )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (pp_scene == (void *) 0 )
                goto no_scene;
        #endif
    }

    // Initialized data
    GXScene_t *p_scene = calloc(1, sizeof(GXScene_t));

    // Error checking
    if( p_scene == (void *) 0 )
        goto no_mem;

    // Return the allocated memory
    *pp_scene = p_scene;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("Null pointer provided for \"pp_scene\" in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_scene ( GXScene_t **pp_scene, const char *path )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_scene == (void *) 0 )
                goto no_scene;
        #endif
    }

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len + 1, sizeof(char));

    // Error checking
    if ( text == (void *) 0 )
        goto no_mem;
    
    // Load the file from the file system
    if ( g_load_file(path, text, true) == 0 )
        goto failed_to_load_file;

    // Load the scene as a JSON token
    if ( load_scene_as_json(pp_scene, text) == 0 )
        goto failed_to_load_scene;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("Null pointer provided for \"pp_scene\" in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load file in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to load scene in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                #endif
                return 0;
        }
    }
}

int load_scene_as_json ( GXScene_t **pp_scene, char *text )
{
    
    // Argument checking
    {
        #ifndef NDEBUG
            if (pp_scene == (void *)0)
                goto no_scene;
            if (text == (void *)0)
                goto no_text;
            if (len == 0)
                goto no_len;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance          = g_get_active_instance();
    GXScene_t    *p_scene           = 0;
    dict         *scene_json_object = 0;
    JSONValue_t  *p_value           = 0;

    // JSON array of object text
    char  *name           = 0;
    array *p_entities     = 0,
          *p_cameras      = 0,
          *p_lights       = 0,
          *p_skyboxes     = 0,
          *p_light_probes = 0;

    // Allocate a scene
    create_scene(pp_scene);

    // Get a pointer to the scene
    p_scene = *pp_scene;

    // Parse the text into a value
    parse_json_value(text, len, &p_value);

    // Parse the JSON
    if(p_value->type == JSONobject) {

        // Initialized data
        dict *p_dict = p_value->object;

        // Set the name
        name = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "name")), JSONstring);

        // Get array of entity objects and paths
        p_entities = (array *) JSON_VALUE(((JSONValue_t *) dict_get(p_dict, "entities")), JSONarray);
        p_cameras  = (array *) JSON_VALUE(((JSONValue_t *) dict_get(p_dict, "cameras")) , JSONarray);
        p_lights   = (array *) JSON_VALUE(((JSONValue_t *) dict_get(p_dict, "lights"))  , JSONarray);
    }

    // Set the name
    if ( name ) {

        // Initialized data
        size_t name_len = strlen(name);

        // Allocate memory
        p_scene->name = calloc(name_len + 1, sizeof(char));

        // Error checking
        {
            #ifndef NDEBUG
                if ( p_scene->name == (void *)0 )
                    goto no_mem;
            #endif
        }

        // Copy the name
        strncpy(p_scene->name, name, name_len);
    }

    // Load entities
    if ( p_entities ) 
    {
        GXThread_t  **entity_loading_threads = calloc(p_instance->loading_thread_count, sizeof(void *));

        size_t        len         = 0;
        JSONValue_t **pp_entities = 0;
         
        array_get(p_entities, 0, &len);
        pp_entities = calloc(len+1, sizeof(JSONValue_t *));
        array_get(p_entities, pp_entities, 0);

        if (p_instance->queues.load_entity)
            queue_destroy(p_instance->queues.load_entity);

        queue_construct(&p_instance->queues.load_entity, len+1);

        for (size_t i = 0; i < len; i++)
            queue_enqueue(p_instance->queues.load_entity, pp_entities[i]->string);
        
        dict_construct(&p_scene->entities, len);
        dict_construct(&p_scene->actors, len);
        dict_construct(&p_scene->ais, len);

        extern int load_entity_from_queue(GXInstance_t *p_instance);

        p_instance->context.loading_scene = p_scene;

        for (size_t i = 0; i < p_instance->loading_thread_count; i++)
        {
            GXThread_t *thread = 0;

            create_thread(&entity_loading_threads[i]);

            thread = entity_loading_threads[i];
            
            thread->thread = SDL_CreateThread(load_entity_from_queue, 0, p_instance);

        }

        for (size_t i = 0; i < p_instance->loading_thread_count; i++)
        {
            int r_stat = 0;

            SDL_WaitThread(entity_loading_threads[i]->thread, &r_stat);
        }
    }	

    // Load cameras
    if ( cameras ) {
        for (size_t i = 0; cameras[i]; i++)
        {
            GXCamera_t *camera = 0;
            
            if (*cameras[i] == '{')
                load_camera_as_json(&camera, cameras[i], strlen(cameras[i]));
            else
                load_camera(&camera, cameras[i]);

            if (camera)
                p_scene->active_camera = camera;
        }
    }

    // Load lights
    if ( lights ) {

    }

    // Load light probes
    if ( light_probes ) {

    }

    // Construct a bounding volume hierarchy tree from the entities in the scene
    construct_bvh_from_scene(&p_scene->bvh, p_scene);

    // Allocate a list to store collisions
    p_scene->collisions = calloc(16, sizeof (void *));

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"pp_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"no_text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;

            no_len:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"len\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;

        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
                #endif
                return 0;
        }
    }
}

int         append_entity      ( GXScene_t  *p_scene, GXEntity_t    *entity)
{

    // Argument check
    {
        #ifndef NDEBUG
            if (p_scene == (void *)0)
                goto no_scene;
            if (p_scene->entities == (void*)0)
                goto no_entities;
            if (entity == (void *)0)
                goto no_entity;
        #endif
        if (entity->name == 0)
            goto no_name;
    }

    // Add the entity to the scene
    dict_add(p_scene->entities, entity->name, entity);

    // If the entity has a rigidbody, add it to the actor list
    if (entity->rigidbody)
        dict_add(p_scene->actors, entity->name, entity);

    // If the entity has an AI, add it to the AI list
    if (entity->ai)
        dict_add(p_scene->ais, entity->name, entity);

    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_scene:
                #ifndef NDEBUG 
                    g_print_error("[G10] [Scene] Null pointer provided for \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_entities:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] No entity dictionary in \"p_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_entity:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_name:
                g_print_error("[G10] [Scene] \"entity\" has no name in call to function \"%s\"\n", __FUNCTION__);
                return 0;
        }
    }

}

int         append_camera      ( GXScene_t  *scene, GXCamera_t    *camera)
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if (scene == (void *)0)
                goto no_scene;
            if (scene->cameras == (void*)0)
                goto no_cameras;
            if (camera == (void *)0)
                goto no_camera;
        #endif
        if (camera->name == 0)
            goto no_name;
    }

    // Add the camera to the scene
    dict_add(scene->cameras, camera->name, camera);

    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_scene:
                #ifndef NDEBUG 
                    g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_cameras:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] No camera dictionary in scene, in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_name:
                    g_print_error("[G10] [Scene] \"camera\" has no name in call to function \"%s\"\n", __FUNCTION__);
                return 0;
        }
    }
}

int         append_light       ( GXScene_t  *scene, GXLight_t     *light)
{
    // Argument check
    {
        #ifndef NDEBUG
            if (scene == (void *)0)
                goto no_scene;
            if (scene->lights == (void*)0)
                goto no_lights;
            if (light == (void *)0)
                goto no_light;
        #endif
        if (light->name == 0)
            goto no_name;
    }

    // Add the entity to the scene
    dict_add(scene->lights, light->name, light);

    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_scene:
                #ifndef NDEBUG 
                    g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_lights:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] No light dictionary in scene, in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_light:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"light\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_name:
                g_print_error("[G10] [Scene] \"light\" has no name in call to function \"%s\"\n", __FUNCTION__);
                return 0;
        }
    }
}

int         append_collision   ( GXScene_t  *scene, GXCollision_t *collision)
{
    size_t i = 0;
    
    while (scene->collisions[++i]);

    scene->collisions[i-1] = collision;

    // Success
    return 1;
}

int         draw_scene         ( GXScene_t  *scene )
{

    // Initialized data
    GXInstance_t             *p_instance                = g_get_active_instance();
    VkCommandBufferBeginInfo  begin_info              = { 0 };
    VkClearValue              clear_color             = { {{1.f, 1.f, 1.f, 0.0f}} };

    VkRenderPassBeginInfo     render_pass_begin_info  = { 0 };

    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


    vkBeginCommandBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], &begin_info);

    // Set up the first render pass
    {
        render_pass_begin_info.sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_begin_info.renderPass          = p_instance->vulkan.render_pass;
        render_pass_begin_info.framebuffer         = p_instance->vulkan.swap_chain_framebuffers[p_instance->vulkan.image_index];
        render_pass_begin_info.renderArea.offset.x = 0;
        render_pass_begin_info.renderArea.offset.y = 0;
        render_pass_begin_info.renderArea.extent   = p_instance->vulkan.swap_chain_extent;
        render_pass_begin_info.clearValueCount     = 2;
        render_pass_begin_info.pClearValues        = p_instance->context.renderer->clear_colors;
    }

    // Start the render pass
    vkCmdBeginRenderPass(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    
    // Get a list of entities
    size_t       entity_count = dict_values(scene->entities, 0);
    GXEntity_t **entities     = calloc(entity_count, sizeof(void*));

    dict_values(scene->entities, entities);

    // Draw each entity
    for (size_t i = 0; i < entity_count; i++)
        draw_entity(entities[i]);

    free(entities);

    // End the render pass
    vkCmdEndRenderPass(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame]);

    // End the command buffer
    vkEndCommandBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame]);

    // Success
    return 1;
}

int         scene_info         ( GXScene_t *p_scene )
{
    size_t       entity_count    = dict_keys(p_scene->entities, 0),
                 re              = entity_count,
                 fs              = 1,
                 le              = 0;
    char       **entity_names    = calloc(entity_count, sizeof(void *));
    GXEntity_t **entity_pointers = calloc(entity_count, sizeof(void *));

    while (re >= 10)
    {
        re /= 10;
        fs++;
    }

    dict_keys(p_scene->entities, entity_names);
    dict_values(p_scene->entities, entity_pointers);

    for (size_t i = 0; i < entity_count; i++)
    {
        size_t el = strlen(entity_names[i]);

        le = (el > le) ? el : le;
    }

    g_print_log("\n - Scene info -\n");
    g_print_log("name     : \"%s\"\n", p_scene->name);
    g_print_log("entities :\n");

    for (size_t i = 0; i < entity_count; i++)
        g_print_log("           [%.*i] \"%s\"\n", fs, i, entity_names[i]);

    printf("\n");

    free(entity_names);
    free(entity_pointers);

    return 0;
}

GXEntity_t *get_entity         ( GXScene_t  *scene, const char     name[] )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(scene==(void*)0)
                goto no_scene;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Success OR null pointer if name is not in scene->entities
    return (GXEntity_t *) dict_get(scene->entities, (char *)name);

    // Error handling
    {
        no_scene:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
    }
}

GXCamera_t *get_camera         ( GXScene_t  *scene, const char     name[])
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if(scene==(void*)0)
                goto no_scene;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    return (GXCamera_t *) dict_get(scene->cameras, (char *)name);

    // Error handling
    {
        no_scene:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
    }
}

GXLight_t  *get_light          ( GXScene_t  *scene, const char     name[])
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if(scene==(void*)0)
                goto no_scene;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    return (GXLight_t *) dict_get(scene->lights, (char *) name);

    // Error handling
    {
        no_scene:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Scene] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
    }
}

int         set_active_camera  ( GXScene_t  *scene, const char     name[])
{

    // Argument check
    {
        #ifndef NDEBUG
            if(scene == (void *)0)
                goto no_scene;
            if (name == (void *)0)
                goto no_name;
        #endif
    }

    // Initialized data
    GXCamera_t *c = (GXCamera_t *) dict_get(scene->cameras, (char *) name);

    // Is the requested camera real?
    if (c)

        // Set the active camera
        scene->active_camera = c;

    else
        
        // Error
        goto failed_to_find_camera;

    return 0;

    // Error handling
    {

        // Argument check
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }

        // Data errors
        {
            failed_to_find_camera:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scene] Failed to find camera \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif
                return 0;
        }
    }
}

int         destroy_scene      ( GXScene_t  *scene )
{
    
    free(scene->name);

    if (scene->entities)
    {
        size_t entity_count = dict_keys(scene->entities, 0);
        GXEntity_t **entities = calloc(entity_count, sizeof(void *));

        dict_values(scene->entities, entities);

        for (size_t i = 0; i < entity_count; i++)
            destroy_entity(entities[i]);
        
        free(entities);

        dict_destroy(scene->entities);

    }

    if (scene->cameras)
    {
        size_t camera_count = dict_keys(scene->cameras, 0);
        GXCamera_t **cameras = calloc(camera_count, sizeof(void*));

        dict_values(scene->cameras, cameras);

        // TODO:
        for (size_t i = 0; i < camera_count; i++)
            ;// destroy_camera(cameras[i]);

        free(cameras);

        dict_destroy(scene->cameras);

    }

    if (scene->lights)
    {
        size_t light_count = dict_keys(scene->lights, 0);
        GXLight_t** lights = calloc(light_count, sizeof(void*));

        dict_values(scene->lights, lights);


        // TODO:
        for (size_t i = 0; i < light_count; i++)
            ;// destroy_light(lights[i]);

        free(lights);

        dict_destroy(scene->lights);
    }


    free(scene);

    return 0;
}
