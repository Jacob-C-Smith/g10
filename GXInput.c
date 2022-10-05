#pragma once

#include <G10/GXInput.h>

// Abstract keys into keyboard file. Same w gamepad
const kn_t keys[] = {
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

int init_input ( void )
{
    // Make a hash table of keys
    dict_construct(&key_dict, 512);

    // Construct the hash table
    for (size_t i = 0; keys[i].code; i++)
        dict_add(key_dict, keys[i].name, 0);

    // Is there a controller?
    if (SDL_IsGameController(0)) 
        controller = SDL_GameControllerOpen(0);

    return 0;
}

int          create_bind               ( GXBind_t    **bind )
{
	// Argument check
	{
		#ifndef NDEBUG
			if(bind == (void *)0)
				goto no_bind;
		#endif
	}

	GXBind_t *ret = calloc(1, sizeof(GXBind_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void *)0)
				goto no_mem;
		#endif
	}

    *bind = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_bind:
			#ifndef NDEBUG
				printf("[G10] [Bind] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}

		// Standard library errors
		{
			no_mem:
			#ifndef NDEBUG
				printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}
	}
}

int          create_input              ( GXInput_t   **input )
{
	// Argument check
	{
		#ifndef NDEBUG
			if(input == (void *)0)
				goto no_input;
		#endif
	}

	GXInput_t *ret = calloc(1, sizeof(GXInput_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void *)0)
				goto no_mem;
		#endif
	}

    *input = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_input:
			#ifndef NDEBUG
				printf("[G10] [Input] Null pointer provided for \"input\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}

		// Standard library errors
		{
			no_mem:
			#ifndef NDEBUG
				printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}
	}
}

int          load_input                ( GXInput_t   **input, const char    path[] )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( path == (void*)0 )
                goto noPath;
        #endif
    }

    // Uninitialized data
    u8*          data;
    size_t       i;

    // Initialized data
    GXInput_t    *ret          = 0;

    // Load up the file
    {
        i    = g_load_file(path, 0, false);
        data = calloc(i + 1, sizeof(char));
        g_load_file(path, data, false);
    }

    // Construct the input
    load_input_as_json_n(&ret, data, i);

    // Error checking
    {
        #ifndef NDEBUG
            if(ret == (void*)0)
                goto no_ret;
        #endif
    }

    *input = ret;

    return 1;

    // Error handling
    {

        // Argument errors
        {
            noPath:
                #ifndef NDEBUG
                    g_print_log("[G10] [Input] No path provided to function \"%s\"\n", __FUNCSIG__);
                #endif
            return 0;
        }

        // G10 errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Failed to parse file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
                #endif
            return 0;
        }
        
    }
}

int          load_input_as_json        ( GXInput_t   **input, char         *token )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (token == (void*)0)
                goto no_token;
        #endif
    }

    // Initialized data
    size_t     len = strlen(token);
    GXInput_t* ret = 0;
    load_input_as_json_n(&ret, token, len);

    // Error checking
    {
        #ifndef NDEBUG
            if ( ret == (void*)0 )
                goto no_ret;
        #endif
    }

    *input = ret;

    return 1;

    // Error handling
    {

        // Debug only branches
        {
            #ifndef NDEBUG

                // Argument errors
                {
                    no_token:
                            g_print_error("[G10] [Input] Null pointer provided for \"token\" in call to function \"%s\"\n", __FUNCSIG__);
                        return 0;
                }

                // G10 errors
                {
                    no_ret:
                            g_print_error("[G10] [Input] Failed to parse \"token\" in call to function \"%s\"\n", __FUNCSIG__);
                        return 0;
                }
            #endif
        }
    }
}
 
int          load_input_as_json_n      ( GXInput_t   **input, char         *token_text   , size_t len )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if (token_text == (void*)0)
                goto no_token;
        #endif
    }

    // Initialized data
    GXInput_t   *ret               = 0;
    dict        *input_json_object = 0;
    JSONToken_t *token             = 0;

    // JSON results
    char        *name        = 0;
    char       **bind_tokens = 0;

    // Allocate an input struct
    create_input(&ret);

    // Parse the JSON
    {
        // Parse JSON
        parse_json(token_text, len, &input_json_object);

        // A JSON token
        JSONToken_t* token = 0;

        // Set the name
        token = dict_get(input_json_object, "name");
        name = token->value.n_where;

        // Set the binds
        token = dict_get(input_json_object, "binds");
        bind_tokens = token->value.a_where;
    }

    // Construct Input
    {

        // Set name
        {
            if(name)
            {
                size_t name_len = strlen(name);

                ret->name = calloc(len + 1, sizeof(char));

                // Error checking
                {
                    #ifndef NDEBUG
                        if(ret->name == (void*)0)
                            goto no_mem;
                    #endif
                }

                strncpy(ret->name, name, len);
            }
            
        }

        // Set binds
        {
            size_t bind_count = 0;
            while (bind_tokens[bind_count++]);

            dict_construct(&ret->binds, bind_count * 2);
            dict_construct(&ret->bind_lut, 110);

            if (bind_tokens)
            {
                // Iterate through JSON array
                for (size_t j = 0; bind_tokens[j]; j++)
                {
                    // Load a bind as a JSON object
                    GXBind_t* bind = 0;
                    
                    load_bind_as_json(&bind, bind_tokens[j]);

                    append_bind(ret, bind);
                }
            }
        }
    }

    // Warnings
    {
        #ifndef NDEBUG
            if (bind_tokens == (void*)0)
                g_print_error("[G10] [Input] No binds in input set \"%s\"\n", ret->name);
        #endif
    }

    *input = ret;

    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_token:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] Null pointer provided for \"token\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // JSON type errors
        {
            name_type_error:
            binds_type_error:
            return 0;
        }

        // Parsing errors
        {
            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input] No \"name\" token provided in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"", __FUNCSIG__);    
                #endif
                return 0;
        }

    }
}

int          load_bind_as_json         ( GXBind_t    **bind, char          *token )
{

    // Argument check
    {
        if (token == (void*)0)
            goto no_token;
    }

    GXBind_t    *ret         = 0;
    dict        *json_tokens = 0;
    JSONToken_t *t           = 0;

    char        *name        = 0,
               **keys        = 0;

    // Error checking
    {
        #ifndef NDEBUG

        #endif
    }

    // Construct the bind
    {
        // TODO: Improve
        parse_json(token, strlen(token), &json_tokens);
            
        t    = dict_get(json_tokens, "name");
        name = JSON_VALUE(t, JSONstring);

        t = dict_get(json_tokens, "keys");
        keys = JSON_VALUE(t, JSONarray);

    }

    construct_bind(bind, name, keys);

    return 1;

    // Error handling
    {
        
        // Standard library errors
        {
            no_mem:
            #ifndef NDEBUG
                g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n", __FUNCSIG__);
                return 0;
            #endif
                return 0;
        }

        // JSON type errors
        {
        name_type_error:
        keys_type_error:
            return 0;

        }

        no_name:
            return 0;


        // Argument handling
        {
            no_token:
            #ifndef NDEBUG
                g_print_error("[G10] [Bind] Null pointer provided for \"token\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int          construct_bind            ( GXBind_t** bind, char* name, char** keys)
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
        
        
    }

    return 1;

    // Error handling
    {

        // Argument check
        {
            no_bind:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_keys:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Null pointer provided for \"keys\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // G10 Errors
        {
            no_b:
                #ifndef NDEBUG
                    g_print_error("[G10] [Bind] Failed to allocate a bind in call to function \"%s\"\n", __FUNCSIG__);
                #endif
            return 0;
        }
    }
}

int          register_bind_callback    ( GXBind_t     *bind    , void                *function_pointer )
{
    // Argument check
    {
        if (bind == (void*)0)
            goto noBind;
        if (function_pointer == (void*)0)
            goto noFunPtr;
    }

    if (bind->callback_max == 0)
    {
        bind->callback_max   = 2,
        bind->callbacks      = calloc(bind->callback_max, sizeof(void*));
    }

    if (bind->callback_count + 1 > bind->callback_max)
    {
        bind->callback_max *= 2;
        void **callbacks    = calloc(bind->callback_max, sizeof(void*)),
              *t            = bind->callbacks;

        memcpy(callbacks, bind->callbacks, bind->callback_count * sizeof(void*));
        bind->callbacks = callbacks;

        free(t);
    }

    bind->callbacks[bind->callback_count++] = function_pointer;

    return 0;

    // Error handling
    {
        noBind:
        #ifndef NDEBUG
            g_print_error("[G10] [Input] Null pointer provided for \"bind\" in call to funciton \"%s\"\n",__FUNCSIG__);
        #endif
        return 0;

        noFunPtr:
        #ifndef NDEBUG
            g_print_warning("[G10] [Input] Null pointer provided for \"function_pointer\" in call to funciton \"%s\"\n",__FUNCSIG__);
        #endif
        return 0;
    }
}

int          unregister_bind_callback  ( GXBind_t     *bind    , void                *function_pointer )
{
    // TODO: Argument check
    // TODO
    return 0;
    // TODO: Error handling
}

SDL_Scancode find_key                  ( const char   *name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (name == 0)
                goto noName;    
        #endif
    }


    return (SDL_Scancode)dict_get(key_dict, name);

    // Error handling
    {
        #ifndef NDEBUG
            noName:
                g_print_error("[G10] [Input] No name provided to function \"%s\"\n", __FUNCSIG__);
                return (SDL_Scancode)0;
        #endif
    }
}

int          process_input             ( GXInstance_t *instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (instance == (void*)0)
                goto no_instance;
            if (instance->input == (void*)0)
                goto no_inputs;
        #endif  
    }

    // TODO: Refactor bind fires into a queue. 

    if (instance->ui_instance)
        if (instance->ui_instance->active_window)
            if (SDL_GetWindowFlags(instance->ui_instance->active_window->window) & SDL_WINDOW_INPUT_FOCUS)
                ui_process_input(instance->ui_instance);

    // TODO: Reimplement for other libraries?

    // Poll for events 
    while (SDL_PollEvent(&instance->event)) {
        switch (instance->event.type)
        {
            // Mouse events
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                // Don't fire binds if the mouse isn't lockced
                if (!SDL_GetRelativeMouseMode())
                    break;

                u8 button = 0;

                callback_parameter_t input;
                int                  x_rel = instance->event.motion.xrel,
                                     y_rel = instance->event.motion.yrel;
                {
                    input.input_state               = MOUSE;
                    input.inputs.mouse_state.xrel   = x_rel;
                    input.inputs.mouse_state.yrel   = y_rel;
                    input.inputs.mouse_state.button = button;
                }

                // Fire mouse motion binds
                {
                if (y_rel < 0)
                    fire_bind(dict_get(instance->input->bind_lut, "MOUSE UP"), input, instance);
                if (y_rel > 0)
                    fire_bind(dict_get(instance->input->bind_lut, "MOUSE DOWN"), input, instance);
                if (x_rel < 0)
                    fire_bind(dict_get(instance->input->bind_lut, "MOUSE LEFT"), input, instance);
                if (x_rel > 0)
                    fire_bind(dict_get(instance->input->bind_lut, "MOUSE RIGHT"), input, instance);
                }
                    
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
                switch (instance->event.window.event)
                {
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    // TODO: Uncomment
                    g_window_resize(instance);
                    break;
                }
                default:
                    break;
                }
                break;

            // Quit
            case SDL_QUIT:
            {
                instance->running = false;
                break;
            }
        }

    }

    GXBind_t* iter = instance->input->binds;
    callback_parameter_t input = { 0, {0}};
    {
        u8* keyboard_state = SDL_GetKeyboardState(NULL);

        for (size_t i = 0; i < 110; i++)
            if (keyboard_state[i])
            {
                GXBind_t *bind = dict_get(instance->input->bind_lut, keys[i].name);
                if( bind )
                    while (bind)
                    {
                        fire_bind(bind, input, instance);
                        bind = bind->next;
                    }
            }       
    }

    // Game controller
    if(controller) {
        callback_parameter_t input;
        
        // Initialize input struct
        {
            input.input_state = GAMEPAD;
        
            input.inputs.gamepad_state.left_trigger  = false;
            input.inputs.gamepad_state.right_trigger = false;

            input.inputs.gamepad_state.left_bumper   = false;
            input.inputs.gamepad_state.right_bumper  = false;

            input.inputs.gamepad_state.A             = false;
            input.inputs.gamepad_state.B             = false;
            input.inputs.gamepad_state.X             = false;
            input.inputs.gamepad_state.Y             = false;

            input.inputs.gamepad_state.dpad_down     = false;
            input.inputs.gamepad_state.dpad_left     = false;
            input.inputs.gamepad_state.dpad_right    = false;
            input.inputs.gamepad_state.dpad_up       = false;

            input.inputs.gamepad_state.start         = false;
            input.inputs.gamepad_state.select        = false;

            input.inputs.gamepad_state.left_stick    = (vec2){0.f, 0.f};
            input.inputs.gamepad_state.right_stick   = (vec2){0.f, 0.f};
        }

        // Set bumpers
        {
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
                input.inputs.gamepad_state.right_bumper = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
                input.inputs.gamepad_state.left_bumper = true;
        }

        // Set ABXY buttons
        {
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
                input.inputs.gamepad_state.A = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
                input.inputs.gamepad_state.B = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
                input.inputs.gamepad_state.X = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
                input.inputs.gamepad_state.Y = true;
        }
   
        // Set DPAD buttons
        {
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
                input.inputs.gamepad_state.A = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
                input.inputs.gamepad_state.B = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
                input.inputs.gamepad_state.X = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                input.inputs.gamepad_state.Y = true;
        }

        // Set start/select buttons
        {
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START))
                input.inputs.gamepad_state.start = true;
            if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
                input.inputs.gamepad_state.select = true;
        }

        // Set joysticks and triggers
        {

            // Set joysticks
            {

                // Set left joystick
                {
                    // X
                    input.inputs.gamepad_state.right_stick.x = (float)SDL_GameControllerGetAxis(controller, 0) / (float)SHRT_MAX;

                    // Y
                    input.inputs.gamepad_state.right_stick.y = (float)SDL_GameControllerGetAxis(controller, 1) / (float)SHRT_MAX;
                }

                // Set right joystick
                {
                    // X
                    input.inputs.gamepad_state.right_stick.x = (float)SDL_GameControllerGetAxis(controller, 2) / (float)SHRT_MAX;
                
                    // Y
                    input.inputs.gamepad_state.right_stick.y = (float)SDL_GameControllerGetAxis(controller, 3) / (float)SHRT_MAX;
                }
            }
            
            // Set triggers
            {

                // Left trigger
                input.inputs.gamepad_state.left_trigger = (float)SDL_GameControllerGetAxis(controller, 4) / (float)SHRT_MAX;
                
                // Right trigger
                input.inputs.gamepad_state.right_trigger = (float)SDL_GameControllerGetAxis(controller, 5) / (float)SHRT_MAX;
            }

        }

        // Fire binds
        {
            if (true)
            {

            }
        }
    }

    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"instance\" in call to \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_inputs:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] No input in \"instance\" in call to \"%s\"\n", __FUNCSIG__);
            #endif
             return 0;
        }

    }
}

int          append_bind               ( GXInput_t *input, GXBind_t *bind )
{
    for (size_t i = 0; i < bind->key_count; i++)
    {
        GXBind_t* b = dict_get(input->bind_lut, bind->keys[i]);
        if (b)
        {
            while (b->next)
                b = b->next;
            b->next = bind;
        }
        else
            dict_add(input->bind_lut, bind->keys[i], bind);

    }

    dict_add(input->binds, bind->name, bind);
    
    return 1;
}

int          fire_bind                 ( GXBind_t     *bind    , callback_parameter_t input, GXInstance_t* instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(bind==(void*)0)
                goto no_bind;
        #endif
    }

    for (size_t i = 0; i < bind->callback_count; i++)
    {
        void (*function)(input, instance) = bind->callbacks[i];
        function(input, instance);
    }

    return 0;
    
    // Error handling
    {
        no_bind:
        #ifndef NDEBUG
            g_print_error("[G10] [Bind] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCSIG__);
        #endif
        return 0;
    }
}

GXBind_t    *find_bind                 ( GXInput_t    *input   , char                *name )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(input == (void*)0)
                goto no_input;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    GXBind_t* i = dict_get(input->binds, name);

    return i;

    // Error handling
    {

        // Argument errors
        {
            no_input:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"input\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
    }
}

int          remove_bind               ( GXInput_t    *input   , GXBind_t            *bind )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(input == (void*)0)
                goto no_input;
            if (bind == (void*)0)
                goto no_bind;
        #endif
    }

    // TODO
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_input:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"input\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_bind:
            #ifndef NDEBUG
                g_print_error("[G10] [Input] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }
    }
}
 
int          destroy_bind              ( GXBind_t     *bind )
{
    // Argument check
    {
        if (bind == (void*)0)
            goto noBind;
    }

    // TODO: Free everything

    free(bind->name);
    
    for (size_t i = 0; i < bind->key_count; i++)
        free(bind->keys[i]);
    
    if(bind->callbacks)
        free(bind->callbacks);

    free(bind->keys);

    free(bind);

    return 0;

    // Error handling
    {
        noBind:
        #ifndef NDEBUG
            g_print_error("[G10] [Input] Null pointer provided for \"bind\" in call to function \"%s\"\n", __FUNCSIG__);
        #endif
        return 0;
    }
}

int          destroy_input             ( GXInput_t    *input )
{

    // Argument check
    {
        #ifndef NDEBUG
        if (input == (void*)0)
            goto noInput;
        #endif
    }

    // Initialized data
    size_t     bind_count = dict_values(input->binds, 0);
    GXBind_t **binds      = calloc(bind_count, sizeof(void *));

    // Free the input name
    free(input->name);

    // Get each bind
    dict_values(input->binds, binds);

    // Free each bind
    for (size_t i = 0; i < bind_count; i++)
        free(binds[i]);

    free(input);

    return 0;

    // Error handling
    {
        noInput:
        #ifndef NDEBUG
            g_print_error("[G10] [Input] Null poiner provided for \"input\" in call to function \"%s\"\n", __FUNCSIG__);
        #endif
        return 0;
    }
}
