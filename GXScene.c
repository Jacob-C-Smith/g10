#include <G10/GXScene.h>

int create_scene(GXScene_t** scene)
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( scene == (void *) 0 )
				goto no_scene;
		#endif
	}

	// Initialized data
	GXScene_t *ret = calloc(1, sizeof(GXScene_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void*)0)
				goto no_mem;
		#endif

	}

	*scene = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("Null pointer provided for \"scene\" in call to function \"%s\"", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_scene(GXScene_t** scene, const char path[])
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( scene == (void *) 0 )
				goto no_scene;
		#endif
	}

	// Initialized data
	char      *token_text     = 0;
	size_t     token_text_len = 0;

	// Load the file from the path
	{
		token_text_len = g_load_file(path, 0, false);
		token_text     = calloc(token_text_len + 1, sizeof(char));

		// Error checking
		{
			#ifndef NDEBUG
				if(token_text == (void*)0)
					goto no_mem;
			#endif
		}
		
		g_load_file(path, token_text, false);
	}

	// Load the scene as a JSON token
	load_scene_as_json_n(scene, token_text, token_text_len); 

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("Null pointer provided for \"scene\" in call to function \"%s\"", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_scene_as_json(GXScene_t** scene, char* token_text)
{
	// Argument checking
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (token_text == (void *)0)
				goto no_token_text;
		#endif
	}

	// Initialized data
	size_t token_text_len = strlen(token_text);
	
	// Load the scene
	load_scene_as_json_n(scene, token_text, token_text_len);

	return 1;

	// Error handling
	{

		// Argument checking
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"no_token_text\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int entity_thread()
{
	size_t j = 0;
	for (size_t i = 0; i < 10000000000; i++)
		j++;
	
	printf("[DONE]\n");
	fflush(stdout);

	return 1;
}

int load_scene_as_json_n(GXScene_t** scene, char* token_text, size_t len)
{
	// Argument checking
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (token_text == (void *)0)
				goto no_token_text;
			if (len == 0)
				goto no_len;
		#endif
	}

	// Initialized data
	size_t        token_text_len    = strlen(token_text);
	GXScene_t    *i_scene           = 0;
	dict         *scene_json_object = 0;
	JSONToken_t  *token             = 0;

	// JSON array of object text
	char        **entities          = 0,
		        **cameras           = 0,
		        **lights            = 0,
		        **skyboxes          = 0,
		        **light_probes      = 0;

	// Allocate a scene
	create_scene(scene);

	// Get an internal pointer to the scene
	i_scene = *scene;

	// Parse the JSON
	parse_json(token_text, len, &scene_json_object);

	// Set the name
	token         = dict_get(scene_json_object, "name");
	i_scene->name = (token) ? token->value.n_where : 0;

	// Get array of entity objects and paths
	token         = dict_get(scene_json_object, "entities");
	entities      = (token) ? token->value.a_where : 0;

	// Get an array of camera objects and paths
	token         = dict_get(scene_json_object, "cameras");
	cameras       = (token) ? token->value.a_where : 0;

	// Get an array of light objects and paths
	token         = dict_get(scene_json_object, "lights");
	lights        = (token) ? token->value.a_where : 0;

	// Load entities
	if(entities) 
	{
		GXInstance_t *instance             = g_get_active_instance();
		char         *entity_text          = entities[0];
		GXEntity_t   *entity               = 0;
		size_t        loading_thread_count = instance->loading_thread_count;
		SDL_Thread  **threads              = 0;
		
		// TODO: Thread
		threads = calloc(loading_thread_count + 1, sizeof(void*));

		// TODO: Spawn some worker threads to load entities
		for (size_t i = 0; i < loading_thread_count; i++)
		{
			//threads[i] = SDL_CreateThread(entity_thread, "", (void *)0);
		}

		for (size_t i = 0; i < loading_thread_count; i++)
		{
			int ret = 0;
			SDL_WaitThread(threads[i], &ret);
		}

		//{
		//	// Differentiate between objects and paths
		//	if (*entity_text == '{')
		//		load_entity_as_json(&entity, entity_text);
		//	else
		//		load_entity(&entity, entity_text);
		//}

		//// TODO: Atomize

		//// Add the entity to the dictionary
		//dict_add(i_scene->entities, entity->name, entity);

	}	

	// Load cameras
	if(cameras) {
		
	}

	// Load Lights
	if(lights) {

	}

	// Load Light Probes
	if(light_probes) {

	}

	return 1;

	// Error handling
	{

		// Argument checking
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"no_token_text\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_len:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"len\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

		}
	}
}

int append_entity(GXScene_t* scene, GXEntity_t* entity)
{

	// Argument check
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (scene->entities == (void*)0)
				goto no_entities;
			if (entity == (void *)0)
				goto no_entity;
		#endif
		if (entity->name == 0)
			goto no_name;
	}

	// Add the entity to the scene
	dict_add(scene->entities, entity->name, entity);

	return 1;

	// Error handling
	{

		// Argument check
		{
			no_scene:
				#ifndef NDEBUG 
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_entities:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] No entity dictionary in scene in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
				return 0;
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_name:
				g_print_error("[G10] [Scene] \"entity\" has no name in call to function \"%s\"\n", __FUNCSIG__);
				return 0;
		}
	}

}
