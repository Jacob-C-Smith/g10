#include <G10/GXAI.h>

void init_ai                   ( void )
{

	// Initialized data
	GXInstance_t *p_instance = g_get_active_instance();

	// Create instance mutexes for AI tasks
	p_instance->mutexes.ai_cache     = SDL_CreateMutex();
	p_instance->mutexes.ai_preupdate = SDL_CreateMutex();
	p_instance->mutexes.ai_update    = SDL_CreateMutex();

	// Exit
	return;
}

int create_ai ( GXAI_t **pp_ai )
{
	// Argument check
	{
		#ifndef NDEBUG
			if(pp_ai == (void *)0)
				goto no_ai;
		#endif
	}

	// Initialized data
	GXAI_t *p_ai = calloc(1, sizeof(GXAI_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(p_ai == (void *)0)
				goto no_mem;
		#endif
	}

	// Write the return value
	*pp_ai = p_ai;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_ai ( GXAI_t **pp_ai, char *path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_ai == (void *) 0 )
				goto no_ai;
			if ( path  == (void *) 0 )
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  file_len  = g_load_file(path, 0, false);
	char   *file_data = calloc(file_len+1, sizeof(char));

	// Error checking
	{
		#ifndef NDEBUG
			if (file_data == (void *) 0 )
				goto no_mem;
		#endif
	}

	// Load the file contents into memory
	if ( g_load_file(path, file_data, false) == 0 )
		goto failed_to_load_file;

	// Parse the file contents into an AI struct
	if ( load_ai_as_json(pp_ai, file_data, file_len) == 0)
		goto failed_to_construct_ai_from_file_json;

	// Free the file data
	free(file_data);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// G10 errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					free(file_data);
					g_print_error("[G10] [AI] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif	
				return 0;

			failed_to_construct_ai_from_file_json:
				#ifndef NDEBUG
					free(file_data);
					g_print_error("[G10] [AI] Failed to construct AI from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif	
				return 0;
		}
	}
}

int load_ai_as_json ( GXAI_t **pp_ai, char *text, size_t len )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_ai == (void *)0 )
				goto no_ai;
			if ( text == (void *)0 )
				goto no_token_text;
		#endif
	}

	// Initialized data
	GXInstance_t *instance      = g_get_active_instance();
	size_t        state_count   = 0;
	char         *name          = 0,
			     *initial_state = 0;
	array        *p_states      = 0;
	dict         *ai_dict       = 0;
	int           ret           = 1;

	// Parse the JSON
	{

		// Initialized data
		JSONValue_t *p_value = 0;

		// Parse the JSON text into a dictionary
		if ( parse_json_value(text, len, &p_value) ) 
			goto failed_to_parse_json;

		// Get the name 
		name = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_value->object, "name")), JSONstring);

		// Get the states
		p_states = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_value->object, "states")), JSONarray);

		// Get the initial state
		initial_state = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_value->object, "initial state")), JSONstring);

	}

	// Construct the AI
	{

		// Initialized data
		GXAI_t* p_ai = 0;

		// Check the cache
		{
			// Lock the AI cache mutex
			SDL_LockMutex(instance->mutexes.ai_cache);

			// Initialized data
			GXAI_t* p_cache_ai = g_find_ai(instance, name);
			
			// If the AI is in the cache, copy it
			if (p_cache_ai)
			{

				// Make a copy of the cached ai
				copy_ai(pp_ai, p_cache_ai);

				// Write the return
				p_ai = *pp_ai;

				// Set the initial state
				goto set_initial_state;
			}
		}

		// Allocate memory for an AI
		create_ai(pp_ai);

		// Get a pointer to the allocated memory
		p_ai = *pp_ai;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for the name
			p_ai->name      = calloc(name_len+1, sizeof(char));

			// Error checking
			{
				#ifndef NDEBUG
					if(p_ai->name == (void *)0)
						goto no_mem;
				#endif
			}

			// Copy the name 
			strncpy(p_ai->name, name, name_len);
		}

		// Initialize each state
		{

			// Initialized data
			size_t        state_count = 0;
			JSONValue_t **states      = 0;

			// Get the states
			if(p_states)
			{
				array_get(p_states, 0, &state_count );
				states = calloc(state_count+1, sizeof(JSONValue_t *));
				array_get(p_states, states, 0 );
			}

			// Construct a dictionary for the states
			dict_construct(&p_ai->states, state_count);

			// Iterate over each state
			for (size_t i = 0; i < state_count; i++)
			{

				// Initialized data
				size_t  state_len   = strlen(states[i]);

				// Allocate for the state name
				char   *state_name = calloc(state_len + 1, sizeof(char));
				
				// Error checking
				{
					#ifndef NDEBUG
						if(state_name == (void *)0)
							goto no_mem;
					#endif
				}

				strncpy(state_name, states[i], state_len);

				// Add the state name to the state dictionary
				dict_add(p_ai->states, state_name, 0);
			}
		}

		// Cache the AI
		g_cache_ai(instance, p_ai);

		// Set the initial state
		set_initial_state:
		{
			
			// Initialized data
			size_t current_state_len = strlen(initial_state);

			// Allocate for the initial state name
			p_ai->current_state = calloc(current_state_len + 1, sizeof(char));

			// Error checking
			{
				#ifndef NDEBUG
					if( p_ai->current_state == (void *) 0 )
						goto no_mem;
				#endif
			}

			// Copy the initial state name
			strncpy(p_ai->current_state, initial_state, current_state_len);

		}

		SDL_UnlockMutex(instance->mutexes.ai_cache);
	}

	free_memory:
	return ret;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				ret = 0;
				goto free_memory;
			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				ret = 0;
				goto free_memory;
			failed_to_parse_json:
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int add_ai_state_callback ( GXAI_t *p_ai , char *state_name, int (*function_pointer) ( GXEntity_t *entity ) )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_ai             == (void *) 0 )
				goto no_ai;
			if ( state_name       == (void *) 0 )
				goto no_state_name;
			if ( function_pointer == (void *) 0 )
				goto no_function_pointer;
		#endif
	}

	// Update the state callback
	dict_add(p_ai->states, state_name, function_pointer);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_state_name:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"state_name\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_function_pointer:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"function_pointer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			return 0;
		}
	}
}

int set_ai_state ( GXAI_t *p_ai , const char *state_name )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_ai             == (void *) 0 )
				goto no_ai;
			if ( state_name       == (void *) 0 )
				goto no_state_name;
		#endif
	}

	// Set the state, if it is a valid state
	if(dict_get(p_ai->states, (char *) state_name))
		p_ai->current_state = (char *) state_name;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_state_name:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"state_name\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int set_ai_pre_update_callback ( GXAI_t *p_ai , int (*function_pointer) ( GXEntity_t *entity ) )
{
	
	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_ai             == (void *) 0 )
				goto no_ai;
			if ( function_pointer == (void *) 0 )
				goto no_function_pointer;
		#endif
	}

	p_ai->pre_ai = function_pointer;

	return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_function_pointer:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"function_pointer\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int pre_update_ai ( GXInstance_t *instance )
{

	// Argument check
	{
		if (instance == (void*)0)
			goto no_instance;
	}

    // Initialized data
	GXEntity_t* entity = 0;

	// Get an ai entity
	{
		// Lock the mutex 
		SDL_LockMutex(instance->mutexes.ai_preupdate);

		// Is there anything left to move?
		if (queue_empty(instance->queues.ai_preupdate))
		{

			// If not, unlock and return
			SDL_UnlockMutex(instance->mutexes.ai_preupdate);
			return 1;
		}

		queue_dequeue(instance->queues.ai_preupdate,&entity);

		SDL_UnlockMutex(instance->mutexes.ai_preupdate);
	}

    // Update the AI
    if ( entity )
		
		// Uncomment
		;//preupdate_entity_ai(entity);

    return 1;

	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_log("[G10] [AI] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int copy_ai ( GXAI_t **pp_ai, GXAI_t *p_ai )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if (pp_ai == (void *) 0)
				goto no_return;
			if (p_ai == (void*)0)
				goto no_ai;
		#endif
	}

	// Initialized data
	GXAI_t *ai = 0;

	// Allocate a new AI
	{
		create_ai(pp_ai);
		ai = *pp_ai;
	}

	// Copy the AI
	{
		ai->name          = p_ai->name;
		ai->current_state = p_ai->current_state;
		ai->pre_ai        = p_ai->pre_ai;
		ai->states        = p_ai->states;
	}

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_return:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"pp_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}
 
int ai_info ( GXAI_t *p_ai )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if (p_ai == (void*)0)
				goto no_ai;
		#endif
	}

	// Initialized data
	size_t   state_count = dict_keys(p_ai->states, 0);
	char   **state_names = calloc(state_count, sizeof (char *));

	// Formatting 
    g_print_log(" - AI info - \n");
    
    // Print the name
    g_print_log("name        : \"%s\"\n", p_ai->name);

	// Print the active state
	g_print_log("active state: \"%s\"\n", p_ai->current_state);


    // Formatting 
    g_print_log("states      : \n");

	// Get each AI state
	if(state_count)
		dict_keys(p_ai->states, state_names);

    // Iterate over each state
    for (size_t i = 0; i < state_count; i++)
    {
        
        // Print the name of each state
        g_print_log("\t[%d] \"%s\"\n", i, state_names[i]);

    }

	putchar('\n');

	// Free the list of state names
	free(state_names);

    return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int update_ai ( GXInstance_t* instance )
{

	// Argument check
	{
		if (instance == (void*)0)
			goto no_instance;
	}

    // Initialized data
	GXEntity_t* entity = 0;

	// Get an ai entity
	{
		// Lock the mutex 
		SDL_LockMutex(instance->mutexes.ai_update);

		// Is there anything left to move?
		if (queue_empty(instance->queues.ai_update))
		{

			// If not, unlock and return
			SDL_UnlockMutex(instance->mutexes.ai_update);
			return 1;
		}

		queue_dequeue(instance->queues.ai_update, &entity);

		SDL_UnlockMutex(instance->mutexes.ai_update);
	}

    // Update the AI
    if (entity)
        // Uncomment
		;//update_entity_ai(entity);

	// Success
    return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_log("[G10] [AI] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int destroy_ai ( GXAI_t *p_ai )
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if(p_ai == (void *)0)
				goto no_ai;
		#endif
	}

	// Free the AI name
	free(p_ai->name);

	// TODO: Free each key from the dictionary
	dict_destroy(p_ai->states);

	// Free the AI
	free(p_ai);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_ai:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}
