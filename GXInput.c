#include <G10/GXInput.h>

// Abstract keys into keyboard file.
const kn_t keys[] =
{
    {
        SDL_SCANCODE_UNKNOWN,
        ""
    },
    {
        1,
        ""
    },
    {
        2,
        ""
    },
    {
        3,
        ""
    },
    {
        SDL_SCANCODE_A,
        "A"
    },
    {
        SDL_SCANCODE_B,
        "B"
    },
    {
        SDL_SCANCODE_C,
        "C"
    },
    {
        SDL_SCANCODE_D,
        "D"
    },
    {
        SDL_SCANCODE_E,
        "E"
    },
    {
        SDL_SCANCODE_F,
        "F"
    },
    {
        SDL_SCANCODE_G,
        "G"
    },
    {
        SDL_SCANCODE_H,
        "H"
    },
    {
        SDL_SCANCODE_I,
        "I"
    },
    {
        SDL_SCANCODE_J,
        "J"
    },
    {
        SDL_SCANCODE_K,
        "K"
    },
    {
        SDL_SCANCODE_L,
        "L"
    },
    {
        SDL_SCANCODE_M,
        "M"
    },
    {
        SDL_SCANCODE_N,
        "N"
    },
    {
        SDL_SCANCODE_O,
        "O"
    },
    {
        SDL_SCANCODE_P,
        "P"
    },
    {
        SDL_SCANCODE_Q,
        "Q"
    },
    {
        SDL_SCANCODE_R,
        "R"
    },
    {
        SDL_SCANCODE_S,
        "S"
    },
    {
        SDL_SCANCODE_T,
        "T"
    },
    {
        SDL_SCANCODE_U,
        "U"
    },
    {
        SDL_SCANCODE_V,
        "V"
    },
    {
        SDL_SCANCODE_W,
        "W"
    },
    {
        SDL_SCANCODE_X,
        "X"
    },
    {
        SDL_SCANCODE_Y,
        "Y"
    },
    {
        SDL_SCANCODE_Z,
        "Z"
    },
    {
        SDL_SCANCODE_1,
        "1"
    },
    {
        SDL_SCANCODE_2,
        "2"
    },
    {
        SDL_SCANCODE_3,
        "3"
    },
    {
        SDL_SCANCODE_4,
        "4"
    },
    {
        SDL_SCANCODE_5,
        "5"
    },
    {
        SDL_SCANCODE_6,
        "6"
    },
    {
        SDL_SCANCODE_7,
        "7"
    },
    {
        SDL_SCANCODE_8,
        "8"
    },
    {
        SDL_SCANCODE_9,
        "9"
    },
    {
        SDL_SCANCODE_0,
        "0"
    },
    {
        SDL_SCANCODE_RETURN,
        "RETURN"
    },
    {
        SDL_SCANCODE_ESCAPE,
        "ESCAPE"
    },
    {
        SDL_SCANCODE_BACKSPACE,
        "BACKSPACE"
    },
    {
        SDL_SCANCODE_TAB,
        "TAB"
    },
    {
        SDL_SCANCODE_SPACE,
        "SPACE"
    },
    {
        SDL_SCANCODE_MINUS,
        "MINUS"
    },
    {
        SDL_SCANCODE_EQUALS,
        "EQUALS"
    },
    {
        SDL_SCANCODE_LEFTBRACKET,
        "LEFT BRACKET"
    },
    {
        SDL_SCANCODE_RIGHTBRACKET,
        "RIGHT BRACKET"
    },
    {
        SDL_SCANCODE_BACKSLASH,
        "BACKSLASH"
    },
    {
        50,
        ""
    },
    {
        SDL_SCANCODE_SEMICOLON,
        "SEMICOLON"
    },
    {
        SDL_SCANCODE_APOSTROPHE,
        "APOSTROPHE"
    },
    {
        SDL_SCANCODE_GRAVE,
        "TILDE"
    },
    {
        SDL_SCANCODE_COMMA,
        "COMMA"
    },
    {
        SDL_SCANCODE_PERIOD,
        "PERIOD"
    },
    {
        SDL_SCANCODE_PERIOD,
        "PERIOD"
    },
    {
        SDL_SCANCODE_SLASH,
        "SLASH"
    },
    {
        SDL_SCANCODE_CAPSLOCK,
        "CAPS LOCK"
    },
    {
        SDL_SCANCODE_F1,
        "F1"
    },
    {
        SDL_SCANCODE_F2,
        "F2"
    },
    {
        SDL_SCANCODE_F3,
        "F3"
    },
    {
        SDL_SCANCODE_F4,
        "F4"
    },
    {
        SDL_SCANCODE_F5,
        "F5"
    },
    {
        SDL_SCANCODE_F6,
        "F6"
    },
    {
        SDL_SCANCODE_F7,
        "F7"
    },
    {
        SDL_SCANCODE_F8,
        "F8"
    },
    {
        SDL_SCANCODE_F9,
        "F9"
    },
    {
        SDL_SCANCODE_F10,
        "F10"
    },
    {
        SDL_SCANCODE_F11,
        "F11"
    },
    {
        SDL_SCANCODE_F12,
        "F12"
    },
    {
        SDL_SCANCODE_PRINTSCREEN,
        "PRINT SCREEN"
    },
    {
        SDL_SCANCODE_SCROLLLOCK,
        "SCROLL LOCK"
    },
    {
        SDL_SCANCODE_PAUSE,
        "PAUSE"
    },
    {
        SDL_SCANCODE_INSERT,
        "INSERT"
    },
    {
        SDL_SCANCODE_HOME,
        "HOME"
    },
    {
        SDL_SCANCODE_PAGEUP,
        "PAGE UP"
    },
    {
        SDL_SCANCODE_DELETE,
        "DELETE"
    },
    {
        SDL_SCANCODE_END,
        "END"
    },
    {
        SDL_SCANCODE_PAGEDOWN,
        "PAGE DOWN"
    },
    {
        SDL_SCANCODE_RIGHT,
        "RIGHT"
    },
    {
        SDL_SCANCODE_LEFT,
        "LEFT"
    },
    {
        SDL_SCANCODE_DOWN,
        "DOWN"
    },
    {
        SDL_SCANCODE_UP,
        "UP"
    },
    {
        SDL_SCANCODE_NUMLOCKCLEAR,
        "NUMLOCK"
    },
    {
        SDL_SCANCODE_KP_DIVIDE,
        "KEYPAD DIVIDE"
    },
    {
        SDL_SCANCODE_KP_MULTIPLY,
        "KEYPAD MULTIPLY"
    },
    {
        SDL_SCANCODE_KP_MINUS,
        "KEYPAD MINUS"
    },
    {
        SDL_SCANCODE_KP_PLUS,
        "KEYPAD PLUS"
    },
    {
        SDL_SCANCODE_KP_ENTER,
        "KEYPAD ENTER"
    },
    {
        SDL_SCANCODE_KP_1,
        "KEYPAD 1"
    },
    {
        SDL_SCANCODE_KP_2,
        "KEYPAD 2"
    },
    {
        SDL_SCANCODE_KP_3,
        "KEYPAD 3"
    },
    {
        SDL_SCANCODE_KP_4,
        "KEYPAD 4"
    },
    {
        SDL_SCANCODE_KP_5,
        "KEYPAD 5"
    },
    {
        SDL_SCANCODE_KP_6,
        "KEYPAD 6"
    },
    {
        SDL_SCANCODE_KP_7,
        "KEYPAD 7"
    },
    {
        SDL_SCANCODE_KP_8,
        "KEYPAD 8"
    },
    {
        SDL_SCANCODE_KP_9,
        "KEYPAD 9"
    },
    {
        SDL_SCANCODE_KP_0,
        "KEYPAD 0"
    },
    {
        SDL_SCANCODE_KP_PERIOD,
        "KEYPAD PERIOD"
    },
    {
        100,
        ""
    },
    {
        SDL_SCANCODE_LCTRL,
        "LEFT CONTROL"
    },
    {
        SDL_SCANCODE_LSHIFT,
        "LEFT SHIFT"
    },
    {
        SDL_SCANCODE_LALT,
        "LEFT ALT"
    },
    {
        SDL_SCANCODE_RCTRL,
        "RIGHT CONTROL"
    },
    {
        SDL_SCANCODE_RSHIFT,
        "RIGHT SHIFT"
    },
    {
        SDL_SCANCODE_RALT,
        "RIGHT ALT"
    },
    {
        254,
        "MOUSE UP"
    },
    {
        0,
        0
    }
};

dict *key_dict = 0;

SDL_GameController *controller;

void init_input ( void )
{

    // Make a hash table of keys
    dict_construct(&key_dict, 512);

    // Construct the hash table
    for (size_t i = 0; keys[i].code; i++)
        dict_add(key_dict, (char *)keys[i].name, 0);

    // Is there a controller?
    if ( SDL_IsGameController(0) )
        controller = SDL_GameControllerOpen(0);

}

int create_input ( GXInput_t **pp_input )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_input == (void *) 0 ) goto no_input;
		#endif
	}

    // Initialized data
	GXInput_t *p_input = calloc(1, sizeof(GXInput_t));

	// Error checking
    if ( p_input == (void *) 0 )
	    goto no_mem;

    // Return an input pointer to the caller
    *pp_input = p_input;

    // Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_input:
			    #ifndef NDEBUG
			    	printf("[G10] [Input] Null pointer provided for \"input\" in call to function \"%s\"\n", __FUNCTION__);
			    #endif

                // Error
			    return 0;
		}

		// Standard library errors
		{
			no_mem:
			    #ifndef NDEBUG
			    	printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
			    #endif

                // Error
			    return 0;
		}
	}
}

int load_input ( GXInput_t **pp_input, const char path[] )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_input == (void *) 0 ) goto no_input;
            if ( path     == (void *) 0 ) goto no_path;
        #endif
    }

    // Uninitialized data
    size_t       i    = g_load_file(path, 0, true);
    u8*          text = calloc(i + 1, sizeof(char));

    // Initialized data
    GXInput_t    *p_input = 0;

    // Load the file
    if ( g_load_file(path, text, true) == 0 )
        goto failed_to_load_file;

    // Construct the input from JSON text
    if ( load_input_as_json_text(pp_input, text) == 0 )
        goto failed_to_parse_input;

    // Cleanup the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_log("[G10] [Input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_log("[G10] [Input] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_parse_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to parse file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_input_as_json_text ( GXInput_t **pp_input, char *text )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( text == (void *) 0 ) goto no_text;
        #endif
    }

    // Initialized data
    JSONValue_t *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto failed_to_parse_json_value;

    if ( load_input_as_json_value(pp_input, p_value) == 0 )
        goto failed_to_parse_input_as_json_value;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_parse_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to parse JSON value in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_parse_input_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to load input in call to function \"%s\"", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_input_as_json_value ( GXInput_t **pp_input, JSONValue_t *p_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_input == (void *) 0 ) goto no_input;
            if ( p_value  == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXInput_t *p_input           = 0;
    char      *name              = 0;
    float      mouse_sensitivity = 0.f;
    array     *p_binds           = 0;

    // Parse the input as an object
    if ( p_value->type == JSONobject )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Get the name
        name = (char *) ((JSONValue_t *)dict_get(p_dict, "name"))->string;

        // Get the mouse sense
        mouse_sensitivity = (float) ((JSONValue_t *)dict_get(p_dict, "mouse sensitivity"))->floating;

        // Get an array of binds for the input
        p_binds = (array *) ((JSONValue_t *)dict_get(p_dict, "binds"))->list;

        // Check for missing data
        if ( !(name && mouse_sensitivity && p_binds) )
            goto missing_properties;

    }

    // Parse the input as a path
    else if ( p_value->type == JSONstring )
    {

        // Load the input as a path
        if ( load_input(pp_input, p_value->string) == 0 )
            goto failed_to_load_input_from_path;

        goto exit;
    }

    // Construct the input
    {

        // Allocate memory for an input
        if ( create_input(pp_input) == 0 )
            goto failed_to_load_input;

        // Get a pointer
        p_input = *pp_input;

        // Copy the name
        {

            // Initialized data
            size_t name_len = strlen(name);

            // Allocate memory for the name
            p_input->name = calloc(name_len+1, sizeof(char));

            // Error checking
            if ( p_input->name == (void *) 0 )
                goto no_mem;

            // Copy the string
            strncpy(p_input->name, name, name_len);
        }

        // Set the mouse sense
        p_input->mouse_sensitivity = mouse_sensitivity;

        // Parse the binds
        {

			// Initialized data
			JSONValue_t **pp_elements          = 0;
			size_t        vector_element_count = 0;

			// Get the quantity of elements
			array_get(p_binds, 0, &vector_element_count );

			// Allocate an array for the elements
			pp_elements = calloc(vector_element_count+1, sizeof(JSONValue_t *));

			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the array
			array_get(p_binds, pp_elements, 0 );

            // TODO: Check return
            // Allocate a dictionary
            dict_construct(&p_input->binds, vector_element_count);
            dict_construct(&p_input->bind_lut, vector_element_count);

			// Iterate over each element
            for (size_t i = 0; i < vector_element_count; i++)
            {

                // Initialized data
                JSONValue_t *i_element = pp_elements[i];
                GXBind_t    *p_bind    = 0;

                // Load the bind from the JSON value
                if ( load_bind_as_json_value(&p_bind, i_element) == 0 )
                    goto failed_to_load_bind_as_json_value;

                // Add the bind to the input
                dict_add(p_input->binds, p_bind->name, p_bind);
                dict_add(p_input->bind_lut, p_bind->name, p_bind);
            }

			// Clean the scope
			free(pp_elements);
        }
    }

    exit:

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // TODO: G10 errors
        {
            failed_to_load_input_from_path:
            missing_properties:
            failed_to_load_input:
            failed_to_load_bind_as_json_value:
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

int load_bind_as_json_value ( GXBind_t **pp_bind, JSONValue_t *p_value )
{
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_bind == (void *) 0 ) goto no_bind;
            if ( p_value == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXBind_t  *p_bind    = 0;
    char      *name      = 0;
    array     *p_keys    = 0;
    size_t     key_count = 0;
    char     **p_keys_array = 0;

    // Parse the bind as an object
    if ( p_value->type == JSONobject )
    {

        // Initialized data
        dict *p_dict = p_value->object;

        // Get the name
        name = (char *) ((JSONValue_t *)dict_get(p_dict, "name"))->string;

        // Get an array of binds for the bind
        p_keys = (array *) ((JSONValue_t *)dict_get(p_dict, "keys"))->list;

        // Check for missing data
        if ( !(name && p_keys) )
            goto missing_properties;

    }

    // Construct the bind
    {

        // Allocate memory for an bind
        if ( create_bind(pp_bind) == 0 )
            goto failed_to_load_bind;

        // Get a pointer
        p_bind = *pp_bind;

        // Copy the name
        {

            // Initialized data
            size_t name_len = strlen(name);

            // Allocate memory for the name
            p_bind->name = calloc(name_len+1, sizeof(char));

            // Error checking
            if ( p_bind->name == (void *) 0 )
                goto no_mem;

            // Copy the string
            strncpy(p_bind->name, name, name_len);
        }

        // Parse the keys
        {

			// Initialized data
			JSONValue_t **pp_elements  = 0;
			size_t        key_count    = 0;

			// Get the quantity of elements
			array_get(p_keys, 0, &key_count );

			// Allocate an array for the elements
			pp_elements = calloc(key_count+1, sizeof(JSONValue_t *));

			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the array
			array_get(p_keys, pp_elements, 0 );

            // Allocate memory for an array of keys
            p_keys_array = calloc(key_count+1, sizeof(char *));

            // Error checking
			if ( keys == (void *) 0 )
				goto no_mem;

			// Iterate over each element
            for (size_t i = 0; i < key_count; i++)
            {

                // Initialized data
                JSONValue_t *i_element = pp_elements[i];
                size_t  key_len = 0;
                char   *key     = 0;

                if ( i_element->type != JSONstring )
                    goto failed_to_parse_key;

                // Compute the length of the string
                key_len = strlen(i_element->string);

                // Allocate memory for a copy of the string
                key = calloc(key_len+1, sizeof(char));

                // Error checking
			    if ( key == (void *) 0 )
				    goto no_mem;

                // Copy the string
                strncpy(key, i_element->string, key_len);

                // Store the key pointer in the list of keys
                p_keys_array[i] = key;
            }

			// Clean the scope
			free(pp_elements);
        }

        *p_bind = (GXBind_t)
        {
            .name = name,
            .keys = p_keys_array,
            .key_count = key_count,
            .callback_max = 2,
            .callbacks    = calloc(2, sizeof(void *))
        };
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"pp_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // TODO: G10 errors
        {
            missing_properties:
            failed_to_load_bind:
            failed_to_parse_key:
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

/*
int load_bind_as_json ( GXBind_t **pp_bind, char *token )
{

    return 1;
    /*
    // Argument check
    {
        #ifndef NDEBUG
            if ( bind  == (void *) 0 )
                goto no_bind;
            if ( token == (void *) 0 )
                goto no_token;
        #endif
    }

    // Initialized data
    GXBind_t    *ret         = 0;
    dict        *json_tokens = 0;
    JSONToken_t *t           = 0;

    char        *name        = 0,
               **keys        = 0;

    // Construct the bind
    {
        // TODO: Improve
        parse_json(token, strlen(token), &json_tokens);

        t    = (JSONToken_t *) dict_get(json_tokens, "name");
        name = JSON_VALUE(t, JSONstring);

        t    = (JSONToken_t *) dict_get(json_tokens, "keys");
        keys = JSON_VALUE(t, JSONarray);

    }

    if ( construct_bind(bind, name, keys) == 0)
        goto failed_to_construct_bind;

    // Success
    return 1;

    // Error handling
    {

        // G10 errors
        {
            // TODO: Implement
            failed_to_construct_bind:
            no_bind:
                return 0;
        }

        // Argument errors
        {
            no_token:
            #ifndef NDEBUG
                g_print_error("[G10] [Bind] Null pointer provided for \"token\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }
    }
}
*/
/*
int construct_bind ( GXBind_t** pp_bind, char* name, char** keys)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(bind == (void *)0)
                goto no_bind;
            if(name == (void *)0)
                goto no_name;
            if (keys == (void*)0)
                goto no_keys;
        #endif
    }

    // Initialized data
    GXBind_t *b = 0;

    // Allocate for a bind
    create_bind(bind);

    b = *bind;

    // Error handling
    {
        #ifndef NDEBUG
            if(b == (void *)0)
                goto no_b;
        #endif
    }

    // Construct the bind
    {
        b->name = name;
        b->keys = keys;

        while (keys[++b->key_count]);

        b->callback_max = 2,
        b->callbacks    = calloc(b->callback_max, sizeof(void*));

    }


    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            no_keys:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Null pointer provided for \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            no_b:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Failed to allocate a bind in call to function \"%s\"\n", __FUNCTION__);
                #endif
            return 0;
        }
    }
}
*/

int create_bind ( GXBind_t **pp_bind )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_bind == (void *) 0 ) goto no_bind;
		#endif
	}

    // Initialized data
	GXBind_t *p_bind = calloc(1, sizeof(GXBind_t));

	// Error checking
    if ( p_bind == (void *) 0 )
	    goto no_mem;

    // Return a bind pointer to the caller
    *pp_bind = p_bind;

    // Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_bind:
			    #ifndef NDEBUG
			    	printf("[G10] [Bind] Null pointer provided for \"p_bind\" in call to function \"%s\"\n", __FUNCTION__);
			    #endif

                // Error
			    return 0;
		}

		// Standard library errors
		{
			no_mem:
			    #ifndef NDEBUG
			    	printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
			    #endif

                // Error
			    return 0;
		}
	}
}

int register_bind_callback ( GXBind_t *p_bind, void *function_pointer )
{

    // Argument check
    {
        if ( p_bind           == (void *) 0 ) goto no_bind;
        if ( function_pointer == (void *) 0 ) goto no_function_pointer;
    }

    // Allocate memory for bind callbacks
    if (p_bind->callback_max == 0)
    {
        p_bind->callback_max   = 2,
        p_bind->callbacks      = calloc(p_bind->callback_max, sizeof(void*));
    }

    if (p_bind->callback_count + 1 > p_bind->callback_max)
    {
        p_bind->callback_max *= 2;
        void **callbacks    = calloc(p_bind->callback_max, sizeof(void*)),
              *t            = p_bind->callbacks;

        memcpy(callbacks, p_bind->callbacks, p_bind->callback_count * sizeof(void*));
        p_bind->callbacks = callbacks;

        free(t);
    }

    p_bind->callbacks[p_bind->callback_count++] = function_pointer;

    // Success
    return 1;

    // Error handling
    {
        no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"bind\" in call to function \"%s\"\n",__FUNCTION__);
            #endif

            // Error
            return 0;

        no_function_pointer:
            #ifndef NDEBUG
                g_print_warning("[G10] [Input] Null pointer provided for \"function_pointer\" in call to function \"%s\"\n",__FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int unregister_bind_callback ( GXBind_t *p_bind, void *function_pointer )
{
    // TODO: Argument check
    // TODO
    return 0;
    // TODO: Error handling
}

SDL_Scancode find_key ( const char *name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( name == (void *) 0 ) goto no_name;
        #endif
    }

    // Return the corresponding keycode
    return (SDL_Scancode)(size_t)dict_get(key_dict, (char *)name);

    // Error handling
    {
        no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] No name provided to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return (SDL_Scancode)0;
    }
}

int process_input ( GXInstance_t *p_instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_instance        == (void *) 0 ) goto no_instance;
            if ( p_instance->input == (void *) 0 ) goto no_inputs;
        #endif
    }

    // TODO: Refactor bind fires into a queue.
    queue *p_update_binds_queue = 0;

    // TODO: Reimplement for other libraries?

    // Poll for events
    while ( SDL_PollEvent(&p_instance->sdl2.event) )
    {

        // Switch on the event type
        switch (p_instance->sdl2.event.type)
        {

            // Mouse events
            case SDL_MOUSEMOTION:
            {

                // TODO: Uncomment
                // Don't fire binds if the mouse isn't lockced
                //if (!SDL_GetRelativeMouseMode())
                //    break;

                // Initialized data
                u8                   button = 0;
                int                  x_rel  = p_instance->sdl2.event.motion.xrel,
                                     y_rel  = p_instance->sdl2.event.motion.yrel;
                callback_parameter_t input  = (callback_parameter_t)
                {
                    .input_state               = MOUSE,
                    .inputs.mouse_state.xrel   = (s32) (x_rel * p_instance->input->mouse_sensitivity),
                    .inputs.mouse_state.yrel   = (s32) (y_rel * p_instance->input->mouse_sensitivity),
                    .inputs.mouse_state.button = 0
                };

                // TODO: Fire binds
                printf("%3d %3d \r", x_rel, y_rel);

                // -X, mouse left
                if ( x_rel < 0 )
                    queue_enqueue(p_instance->input->p_key_queue, "MOUSE LEFT");

                // +X, mouse right
                if ( 0 < x_rel )
                    queue_enqueue(p_instance->input->p_key_queue, "MOUSE RIGHT");

                // -Y, mouse up
                if ( y_rel < 0 )
                    queue_enqueue(p_instance->input->p_key_queue, "MOUSE UP");

                // +Y, mouse down
                if ( 0 < y_rel )
                    queue_enqueue(p_instance->input->p_key_queue, "MOUSE DOWN");

            }
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                // Don't fire binds if the mouse isn't lockced
                if ( !SDL_GetRelativeMouseMode() )
                    break;

                // Initialized data
                u8 button = 0;
                int                  x_rel = p_instance->sdl2.event.motion.xrel,
                                     y_rel = p_instance->sdl2.event.motion.yrel;
                callback_parameter_t input = (callback_parameter_t)
                {
                    .input_state               = MOUSE,
                    .inputs.mouse_state.xrel   = (s32) (x_rel * p_instance->input->mouse_sensitivity),
                    .inputs.mouse_state.yrel   = (s32) (y_rel * p_instance->input->mouse_sensitivity),
                    .inputs.mouse_state.button = button
                };
            }
            break;

            // Keyboard events

            // Key up
            // Key down
            case SDL_KEYDOWN:
            {

            }

            // Window resize
            case SDL_WINDOWEVENT:
                switch (p_instance->sdl2.event.window.event)
                {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    {
                        // TODO: Uncomment
                        g_window_resize(p_instance);
                        break;
                    }
                    default:
                        break;
                }
                break;

            // Quit
            case SDL_QUIT:
            {
                g_user_exit( (callback_parameter_t) { 0, {0} }, p_instance);
                break;
            }
        }
    }
    /*
    const u8* keyboard_state = SDL_GetKeyboardState(NULL);

    // Iterate over each key
    for (size_t i = 0; i < 110; i++)
    {

        // If the key is down...
        if ( keyboard_state[i] )
        {

            // Initialized data
            GXBind_t* bind = (GXBind_t *) dict_get(p_instance->input->bind_lut, (char*)keys[i].name);


            if ( bind )
            {
                bind->active = true;
                p_instance.input.inputs.key.depressed = true;
                fire_bind(bind, p_input, p_instance);
            }
        }
    }*/


    // Game controller
    if ( controller )
    {

        // Initialized data
        callback_parameter_t input = (callback_parameter_t)
        {
            .input_state = GAMEPAD,

            .inputs.gamepad_state.left_trigger  = (float)(SDL_GameControllerGetAxis(controller, 4) / (float)SHRT_MAX),
            .inputs.gamepad_state.right_trigger = (float)(SDL_GameControllerGetAxis(controller, 5) / (float)SHRT_MAX),

            .inputs.gamepad_state.left_bumper   = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
            .inputs.gamepad_state.right_bumper  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),

            .inputs.gamepad_state.A = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A),
            .inputs.gamepad_state.B = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B),
            .inputs.gamepad_state.X = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X),
            .inputs.gamepad_state.Y = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y),

            .inputs.gamepad_state.dpad_down  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN),
            .inputs.gamepad_state.dpad_left  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT),
            .inputs.gamepad_state.dpad_right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT),
            .inputs.gamepad_state.dpad_up    = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP),

            .inputs.gamepad_state.start  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START),
            .inputs.gamepad_state.select = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK),

            .inputs.gamepad_state.left_stick = (vec2)
            {
                .x = (float)(SDL_GameControllerGetAxis(controller, 2) / (float)SHRT_MAX),
                .y = (float)(SDL_GameControllerGetAxis(controller, 3) / (float)SHRT_MAX)
            },
            .inputs.gamepad_state.right_stick = (vec2)
            {
                .x = (float)(SDL_GameControllerGetAxis(controller, 0) / (float)SHRT_MAX),
                .y = (float)(SDL_GameControllerGetAxis(controller, 1) / (float)SHRT_MAX)
            }
        };

    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"p_instance\" in call to \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_inputs:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] No input in \"p_instance\" in call to \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int append_bind ( GXInput_t *p_input, GXBind_t *p_bind )
{

    // TODO: Argument check

    for (size_t i = 0; i < p_bind->key_count; i++)
    {
        GXBind_t* b = (GXBind_t *) dict_get(p_input->bind_lut, p_bind->keys[i]);
        if (b)
        {
            while (b->next)
                b = b->next;
            b->next = p_bind;
        }
        else
            dict_add(p_input->bind_lut, p_bind->keys[i], p_bind);

    }

    dict_add(p_input->binds, p_bind->name, p_bind);

    // Success
    return 1;

    // TODO: Error handling
}

int input_info ( GXInput_t *p_input )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_input == (void *) 0 ) goto no_input;
        #endif
    }

    // Formatting
    g_print_log(" - Input info - \n");

    // Print the name
    g_print_log("name             : \"%s\"\n", p_input->name);

	// Print the mouse sensitivity
	g_print_log("mouse sensitivity: \"%.2f\"\n", p_input->mouse_sensitivity);

    // Format
    g_print_log("binds            : \n");

	// Print each bind
    size_t l         = dict_values(p_input->binds, 0);
    GXBind_t** binds = calloc(l, sizeof( GXBind_t *));

    dict_values(p_input->binds, binds);

    // TODO: Better formatting, like the scene_info function
    for (size_t i = 0; i < l; i++)
    {
        g_print_log("                   [%d] \"%s\": \n", i, binds[i]->name);
        g_print_log("                         [ ", i, binds[i]->name);


        if (binds[i]->key_count)
        {
            g_print_log("%s", binds[i]->keys[0]);

            for (size_t j = 1; j < binds[i]->key_count; j++)
            {
                g_print_log(", %s", binds[i]->keys[j]);
            }

            g_print_log(" ]\n");
        }

    }
	putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input} Null pointer provided for \"input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error handling
                return 0;
        }
    }
}

int fire_bind ( GXBind_t *p_bind, callback_parameter_t input, GXInstance_t *p_instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_bind == (void *) 0 ) goto no_bind;
        #endif
    }

    for (size_t i = 0; i < p_bind->callback_count; i++)
    {
        void (*function)(input, p_instance) = p_bind->callbacks[i];
        function(input, p_instance);
    }

    return 0;

    // Error handling
    {
        no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Bind] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

GXBind_t *find_bind ( GXInput_t *p_input, char *name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_input == (void *) 0 ) goto no_input;
            if ( name    == (void *) 0 ) goto no_name;
        #endif
    }

    // Return the bind
    return (GXBind_t *) dict_get(p_input->binds, name);

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int remove_bind ( GXInput_t *p_input, GXBind_t *p_bind )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_input == (void *) 0 ) goto no_input;
            if ( p_bind  == (void *) 0 ) goto no_bind;
        #endif
    }

    // TODO
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"p_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int destroy_bind ( GXBind_t *p_bind )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_bind == (void *) 0 ) goto no_bind;
        #endif
    }

    // TODO: Free everything

    free(p_bind->name);

    for (size_t i = 0; i < p_bind->key_count; i++)
        free(p_bind->keys[i]);

    if ( p_bind->callbacks )
        free(p_bind->callbacks);

    free(p_bind->keys);

    free(p_bind);

    // Success
    return 1;

    // Error handling
    {
        no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"p_bind\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int destroy_input ( GXInput_t *p_input )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_input == (void *) 0 ) goto no_input;
        #endif
    }

    // Initialized data
    size_t     bind_count = dict_values(p_input->binds, 0);
    GXBind_t **binds      = calloc(bind_count, sizeof(void *));

    // Free the input name
    free(p_input->name);

    // Get each bind
    dict_values(p_input->binds, binds);

    // Free each bind
    for (size_t i = 0; i < bind_count; i++)
        free(binds[i]);

    free(p_input);

    // Success
    return 1;

    // Error handling
    {
        no_input:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null poiner provided for \"input\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}
