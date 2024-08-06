#include <g10/g10.h>
#include <log/log.h>

#ifdef G10_BUILD_WITH_SDL2

// Static data
dict *p_sdl2_key_lookup = (void *) 0;
dict *p_sdl2_key_scancode = (void *) 0;

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>

struct 
{
    char _name[64];
    SDL_Scancode _scancode;
    bool _active;
} _key_lookup[SDL_NUM_SCANCODES] = 
{

    // Invalid
    [SDL_SCANCODE_UNKNOWN] = { ._name = "INVALID", ._active = false, ._scancode = SDL_SCANCODE_UNKNOWN },

    // Letters
    [SDL_SCANCODE_A] = { ._name = "A", ._active = false, ._scancode = SDL_SCANCODE_A },
    [SDL_SCANCODE_B] = { ._name = "B", ._active = false, ._scancode = SDL_SCANCODE_B },
    [SDL_SCANCODE_C] = { ._name = "C", ._active = false, ._scancode = SDL_SCANCODE_C },
    [SDL_SCANCODE_D] = { ._name = "D", ._active = false, ._scancode = SDL_SCANCODE_D },
    [SDL_SCANCODE_E] = { ._name = "E", ._active = false, ._scancode = SDL_SCANCODE_E },
    [SDL_SCANCODE_F] = { ._name = "F", ._active = false, ._scancode = SDL_SCANCODE_F },
    [SDL_SCANCODE_G] = { ._name = "G", ._active = false, ._scancode = SDL_SCANCODE_G },
    [SDL_SCANCODE_H] = { ._name = "H", ._active = false, ._scancode = SDL_SCANCODE_H },
    [SDL_SCANCODE_I] = { ._name = "I", ._active = false, ._scancode = SDL_SCANCODE_I },
    [SDL_SCANCODE_J] = { ._name = "J", ._active = false, ._scancode = SDL_SCANCODE_J },
    [SDL_SCANCODE_K] = { ._name = "K", ._active = false, ._scancode = SDL_SCANCODE_K },
    [SDL_SCANCODE_L] = { ._name = "L", ._active = false, ._scancode = SDL_SCANCODE_L },
    [SDL_SCANCODE_M] = { ._name = "M", ._active = false, ._scancode = SDL_SCANCODE_M },
    [SDL_SCANCODE_N] = { ._name = "N", ._active = false, ._scancode = SDL_SCANCODE_N },
    [SDL_SCANCODE_O] = { ._name = "O", ._active = false, ._scancode = SDL_SCANCODE_O },
    [SDL_SCANCODE_P] = { ._name = "P", ._active = false, ._scancode = SDL_SCANCODE_P },
    [SDL_SCANCODE_Q] = { ._name = "Q", ._active = false, ._scancode = SDL_SCANCODE_Q },
    [SDL_SCANCODE_R] = { ._name = "R", ._active = false, ._scancode = SDL_SCANCODE_R },
    [SDL_SCANCODE_S] = { ._name = "S", ._active = false, ._scancode = SDL_SCANCODE_S },
    [SDL_SCANCODE_T] = { ._name = "T", ._active = false, ._scancode = SDL_SCANCODE_T },
    [SDL_SCANCODE_U] = { ._name = "U", ._active = false, ._scancode = SDL_SCANCODE_U },
    [SDL_SCANCODE_V] = { ._name = "V", ._active = false, ._scancode = SDL_SCANCODE_V },
    [SDL_SCANCODE_W] = { ._name = "W", ._active = false, ._scancode = SDL_SCANCODE_W },
    [SDL_SCANCODE_X] = { ._name = "X", ._active = false, ._scancode = SDL_SCANCODE_X },
    [SDL_SCANCODE_Y] = { ._name = "Y", ._active = false, ._scancode = SDL_SCANCODE_Y },
    [SDL_SCANCODE_Z] = { ._name = "Z", ._active = false, ._scancode = SDL_SCANCODE_Z },

    [SDL_SCANCODE_1] = { ._name = "1", ._active = false, ._scancode = SDL_SCANCODE_1 },
    [SDL_SCANCODE_2] = { ._name = "2", ._active = false, ._scancode = SDL_SCANCODE_2 },
    [SDL_SCANCODE_3] = { ._name = "3", ._active = false, ._scancode = SDL_SCANCODE_3 },
    [SDL_SCANCODE_4] = { ._name = "4", ._active = false, ._scancode = SDL_SCANCODE_4 },
    [SDL_SCANCODE_5] = { ._name = "5", ._active = false, ._scancode = SDL_SCANCODE_5 },
    [SDL_SCANCODE_6] = { ._name = "6", ._active = false, ._scancode = SDL_SCANCODE_6 },
    [SDL_SCANCODE_7] = { ._name = "7", ._active = false, ._scancode = SDL_SCANCODE_7 },
    [SDL_SCANCODE_8] = { ._name = "8", ._active = false, ._scancode = SDL_SCANCODE_8 },
    [SDL_SCANCODE_9] = { ._name = "9", ._active = false, ._scancode = SDL_SCANCODE_9 },
    [SDL_SCANCODE_0] = { ._name = "0", ._active = false, ._scancode = SDL_SCANCODE_0 },

    // White space
    [SDL_SCANCODE_RETURN]    = { ._name = "RETURN"   , ._active = false, ._scancode = SDL_SCANCODE_RETURN },
    [SDL_SCANCODE_ESCAPE]    = { ._name = "ESCAPE"   , ._active = false, ._scancode = SDL_SCANCODE_ESCAPE },
    [SDL_SCANCODE_BACKSPACE] = { ._name = "BACKSPACE", ._active = false, ._scancode = SDL_SCANCODE_BACKSPACE },
    [SDL_SCANCODE_TAB]       = { ._name = "TAB"      , ._active = false, ._scancode = SDL_SCANCODE_TAB },
    [SDL_SCANCODE_SPACE]     = { ._name = "SPACE"    , ._active = false, ._scancode = SDL_SCANCODE_SPACE },

    // Symbols
    [SDL_SCANCODE_MINUS]        = { ._name = "MINUS"        , ._active = false, ._scancode = SDL_SCANCODE_MINUS },
    [SDL_SCANCODE_EQUALS]       = { ._name = "EQUALS"       , ._active = false, ._scancode = SDL_SCANCODE_EQUALS },
    [SDL_SCANCODE_LEFTBRACKET]  = { ._name = "LEFT BRACKET" , ._active = false, ._scancode = SDL_SCANCODE_LEFTBRACKET },
    [SDL_SCANCODE_RIGHTBRACKET] = { ._name = "RIGHT BRACKET", ._active = false, ._scancode = SDL_SCANCODE_RIGHTBRACKET },
    [SDL_SCANCODE_BACKSLASH]    = { ._name = "BACKSLASH"    , ._active = false, ._scancode = SDL_SCANCODE_BACKSLASH }, 
    [SDL_SCANCODE_NONUSHASH]    = { ._name = "NONUSHASH"    , ._active = false, ._scancode = SDL_SCANCODE_NONUSHASH }, 
    [SDL_SCANCODE_SEMICOLON]    = { ._name = "SEMICOLON"    , ._active = false, ._scancode = SDL_SCANCODE_SEMICOLON },
    [SDL_SCANCODE_APOSTROPHE]   = { ._name = "APOSTROPHE"   , ._active = false, ._scancode = SDL_SCANCODE_APOSTROPHE },
    [SDL_SCANCODE_GRAVE]        = { ._name = "GRAVE"        , ._active = false, ._scancode = SDL_SCANCODE_GRAVE }, 
    [SDL_SCANCODE_COMMA]        = { ._name = "COMMA"        , ._active = false, ._scancode = SDL_SCANCODE_COMMA },
    [SDL_SCANCODE_PERIOD]       = { ._name = "PERIOD"       , ._active = false, ._scancode = SDL_SCANCODE_PERIOD },
    [SDL_SCANCODE_SLASH]        = { ._name = "SLASH"        , ._active = false, ._scancode = SDL_SCANCODE_SLASH },

    // F keys
    [SDL_SCANCODE_F1]   = { ._name = "F1", ._active = false, ._scancode = SDL_SCANCODE_F1},
    [SDL_SCANCODE_F2]   = { ._name = "F2", ._active = false, ._scancode = SDL_SCANCODE_F2},
    [SDL_SCANCODE_F3]   = { ._name = "F3", ._active = false, ._scancode = SDL_SCANCODE_F3},
    [SDL_SCANCODE_F4]   = { ._name = "F4", ._active = false, ._scancode = SDL_SCANCODE_F4},
    [SDL_SCANCODE_F5]   = { ._name = "F5", ._active = false, ._scancode = SDL_SCANCODE_F5},
    [SDL_SCANCODE_F6]   = { ._name = "F6", ._active = false, ._scancode = SDL_SCANCODE_F6},
    [SDL_SCANCODE_F7]   = { ._name = "F7", ._active = false, ._scancode = SDL_SCANCODE_F7},
    [SDL_SCANCODE_F8]   = { ._name = "F8", ._active = false, ._scancode = SDL_SCANCODE_F8},
    [SDL_SCANCODE_F9]   = { ._name = "F9", ._active = false, ._scancode = SDL_SCANCODE_F9},
    [SDL_SCANCODE_F10]  = { ._name = "F10", ._active = false, ._scancode = SDL_SCANCODE_F10},
    [SDL_SCANCODE_F11]  = { ._name = "F11", ._active = false, ._scancode = SDL_SCANCODE_F11},
    [SDL_SCANCODE_F12]  = { ._name = "F12", ._active = false, ._scancode = SDL_SCANCODE_F12},

    // Special
    [SDL_SCANCODE_CAPSLOCK]    = { ._name = "CAPS LOCK"   , ._active = false, ._scancode = SDL_SCANCODE_SLASH },
    [SDL_SCANCODE_PRINTSCREEN] = { ._name = "PRINT SCREEN", ._active = false, ._scancode = SDL_SCANCODE_PRINTSCREEN },
    [SDL_SCANCODE_SCROLLLOCK]  = { ._name = "SCROLL LOCK" , ._active = false, ._scancode = SDL_SCANCODE_SCROLLLOCK },
    [SDL_SCANCODE_PAUSE]       = { ._name = "PAUSE"       , ._active = false, ._scancode = SDL_SCANCODE_PAUSE },
    [SDL_SCANCODE_INSERT]      = { ._name = "INSERT"      , ._active = false, ._scancode = SDL_SCANCODE_INSERT },
    
    // Navigation
    [SDL_SCANCODE_HOME]         = { ._name = "HOME"     , ._active = false, ._scancode = SDL_SCANCODE_HOME },
    [SDL_SCANCODE_PAGEUP]       = { ._name = "PAGE UP"  , ._active = false, ._scancode = SDL_SCANCODE_PAGEUP },
    [SDL_SCANCODE_DELETE]       = { ._name = "DELETE"   , ._active = false, ._scancode = SDL_SCANCODE_DELETE },
    [SDL_SCANCODE_END]          = { ._name = "END"      , ._active = false, ._scancode = SDL_SCANCODE_END },
    [SDL_SCANCODE_PAGEDOWN]     = { ._name = "PAGE DOWN", ._active = false, ._scancode = SDL_SCANCODE_PAGEDOWN },
    [SDL_SCANCODE_RIGHT]        = { ._name = "RIGHT"    , ._active = false, ._scancode = SDL_SCANCODE_RIGHT },
    [SDL_SCANCODE_LEFT]         = { ._name = "LEFT"     , ._active = false, ._scancode = SDL_SCANCODE_LEFT },
    [SDL_SCANCODE_DOWN]         = { ._name = "DOWN"     , ._active = false, ._scancode = SDL_SCANCODE_DOWN },
    [SDL_SCANCODE_UP]           = { ._name = "UP"       , ._active = false, ._scancode = SDL_SCANCODE_UP },
    [SDL_SCANCODE_NUMLOCKCLEAR] = { ._name = "NUM LOCK" , ._active = false, ._scancode = SDL_SCANCODE_NUMLOCKCLEAR },
    
    // Keypad
    [SDL_SCANCODE_KP_DIVIDE]   = { ._name = "KEYPAD DIVIDE"  , ._active = false, ._scancode = SDL_SCANCODE_KP_DIVIDE },
    [SDL_SCANCODE_KP_MULTIPLY] = { ._name = "KEYPAD MULTIPLY", ._active = false, ._scancode = SDL_SCANCODE_KP_MULTIPLY },
    [SDL_SCANCODE_KP_MINUS]    = { ._name = "KEYPAD MINUS"   , ._active = false, ._scancode = SDL_SCANCODE_KP_MINUS },
    [SDL_SCANCODE_KP_PLUS]     = { ._name = "KEYPAD PLUS"    , ._active = false, ._scancode = SDL_SCANCODE_KP_PLUS },
    [SDL_SCANCODE_KP_ENTER]    = { ._name = "KEYPAD ENTER"   , ._active = false, ._scancode = SDL_SCANCODE_KP_ENTER },
    [SDL_SCANCODE_KP_1]        = { ._name = "KEYPAD 1"       , ._active = false, ._scancode = SDL_SCANCODE_KP_1 },
    [SDL_SCANCODE_KP_2]        = { ._name = "KEYPAD 2"       , ._active = false, ._scancode = SDL_SCANCODE_KP_2 },
    [SDL_SCANCODE_KP_3]        = { ._name = "KEYPAD 3"       , ._active = false, ._scancode = SDL_SCANCODE_KP_3 },
    [SDL_SCANCODE_KP_4]        = { ._name = "KEYPAD 4"       , ._active = false, ._scancode = SDL_SCANCODE_KP_4 },
    [SDL_SCANCODE_KP_5]        = { ._name = "KEYPAD 5"       , ._active = false, ._scancode = SDL_SCANCODE_KP_5 },
    [SDL_SCANCODE_KP_6]        = { ._name = "KEYPAD 6"       , ._active = false, ._scancode = SDL_SCANCODE_KP_6 },
    [SDL_SCANCODE_KP_7]        = { ._name = "KEYPAD 7"       , ._active = false, ._scancode = SDL_SCANCODE_KP_7 },
    [SDL_SCANCODE_KP_8]        = { ._name = "KEYPAD 8"       , ._active = false, ._scancode = SDL_SCANCODE_KP_8 },
    [SDL_SCANCODE_KP_9]        = { ._name = "KEYPAD 9"       , ._active = false, ._scancode = SDL_SCANCODE_KP_9 },
    [SDL_SCANCODE_KP_0]        = { ._name = "KEYPAD 0"       , ._active = false, ._scancode = SDL_SCANCODE_KP_0 },
    [SDL_SCANCODE_KP_PERIOD]   = { ._name = "KEYPAD PERIOD"  , ._active = false, ._scancode = SDL_SCANCODE_KP_PERIOD },

    // Modifier
    [SDL_SCANCODE_LCTRL]  = { ._name = "LEFT CONTROL" , ._active = false, ._scancode = SDL_SCANCODE_LCTRL },
    [SDL_SCANCODE_LSHIFT] = { ._name = "LEFT SHIFT"   , ._active = false, ._scancode = SDL_SCANCODE_LSHIFT },
    [SDL_SCANCODE_LALT]   = { ._name = "LEFT ALT"     , ._active = false, ._scancode = SDL_SCANCODE_LALT },
    [SDL_SCANCODE_RCTRL]  = { ._name = "RIGHT CONTROL", ._active = false, ._scancode = SDL_SCANCODE_RCTRL },
    [SDL_SCANCODE_RSHIFT] = { ._name = "RIGHT SHIFT"  , ._active = false, ._scancode = SDL_SCANCODE_RSHIFT },
    [SDL_SCANCODE_RALT]   = { ._name = "RIGHT ALT"    , ._active = false, ._scancode = SDL_SCANCODE_RALT }
};


// Function definitions
int g_sdl2_init ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Initialize SDL2
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

    // Construct the key lookups
    if ( dict_construct(&p_sdl2_key_lookup  , SDL_NUM_SCANCODES, 0) == 0 ) goto failed_to_construct_dict;
    if ( dict_construct(&p_sdl2_key_scancode, SDL_NUM_SCANCODES, 0) == 0 ) goto failed_to_construct_dict;

    // Populate the key lookup
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
    {

        // Add the key
        dict_add(p_sdl2_key_lookup  , _key_lookup[i]._name, &_key_lookup[i]._active);
        dict_add(p_sdl2_key_scancode, _key_lookup[i]._name, (void *)_key_lookup[i]._scancode);
    }

    // Success
    return 1;
    
    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl2] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // dict errors
        {
            failed_to_construct_dict:
                #ifndef NDEBUG
                    log_error("[sdl2] Failed to construct key lookup in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_sdl2_window_from_json ( g_instance *p_instance, const json_value *p_value )
{

    // Argument check
    if ( p_instance    ==        (void *) 0 ) goto no_instance;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    u32 sdl2_window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    // Graphics API
    #ifdef G10_BUILD_WITH_VULKAN
        sdl2_window_flags |= SDL_WINDOW_VULKAN;
    #elif defined G10_BUILD_WITH_OPENGL
        sdl2_window_flags |= SDL_WINDOW_OPENGL;
    #endif

    // Parse the window object
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_width  = (json_value *)dict_get(p_dict, "width"),
                   *p_height = (json_value *)dict_get(p_dict, "height"),
                   *p_title  = (json_value *)dict_get(p_dict, "title");

        // Error check
        if ( p_width  == (void *) 0 ) goto no_width_property;
        if ( p_height == (void *) 0 ) goto no_height_property;
        if ( p_title  == (void *) 0 ) goto no_title_property;

        // Type check
        if ( p_width->type  != JSON_VALUE_INTEGER ) goto wrong_width_type;
        if ( p_height->type != JSON_VALUE_INTEGER ) goto wrong_height_type;
        if ( p_title->type  != JSON_VALUE_STRING  ) goto wrong_title_type;
    
        // Store the width
        p_instance->window.width = (u32) p_width->integer;

        // Store the height
        p_instance->window.height = (u32) p_height->integer;

        // Copy the title
        {

            // Initialized data
            size_t len = strlen(p_title->string);

            // Allocate memory for the string
            p_instance->window.title = G10_REALLOC(0, sizeof(char) * ( len + 1 ));

            // Error check
            if ( p_instance->window.title == (void *) 0 ) goto no_mem;

            // Initialize
            memset(p_instance->window.title, 0, sizeof(char) * ( len + 1 ));

            // Copy the string
            strncpy(p_instance->window.title, p_title->string, len);
        }
    }

    // Construct the SDL2 window
    p_instance->window.sdl2.window = SDL_CreateWindow(p_instance->window.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) p_instance->window.width, (int) p_instance->window.height, sdl2_window_flags);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            no_width_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Parameter \"p_value\" is missing required property \"width\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            no_height_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Parameter \"p_value\" is missing required property \"height\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            no_title_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Parameter \"p_value\" is missing required property \"title\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            wrong_width_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Property \"width\" of parameter \"p_value\" must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            wrong_height_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Property \"height\" of parameter \"p_value\" must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            wrong_title_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Property \"title\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_sdl2_window_poll ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // Initialized data
    input *p_input = p_instance->context.p_input;
    const u8* keyboard_state = SDL_GetKeyboardState(NULL);

    // Process each event
    while (SDL_PollEvent(&p_instance->window.sdl2.event))
    {
        
        // Switch on the event type
        switch (p_instance->window.sdl2.event.type)
        {

            // Process should exit
            case SDL_QUIT:
            {

                // Clear the running flag
                p_instance->running = false;

                // Done
                break;
            }

            case SDL_WINDOWEVENT:

                if ( p_instance->window.sdl2.event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
                {

                    // External functions
                    extern int g_window_resize ( g_instance *p_instance, u32 width, u32 height );
                    
                    // Resize the window
                    g_window_resize
                    (
                        p_instance,
                        (u32) p_instance->window.sdl2.event.window.data1,
                        (u32) p_instance->window.sdl2.event.window.data2
                    );
                }

                // Done
                break;

            // Mouse moves
            case SDL_MOUSEBUTTONDOWN:break;
            case SDL_MOUSEBUTTONUP:
            {
                
                // Done
                break;
            }
            case SDL_MOUSEMOTION:
            {

                // Initialized data
                //camera *p_camera = p_instance->context.p_scene->context.p_camera;
                //int xrel = p_instance->window.sdl2.event.motion.xrel,
                //    yrel = p_instance->window.sdl2.event.motion.yrel;
                
                // printf("[ %d, %d ]\n", p_instance->window.sdl2.event.motion.x, p_instance->window.sdl2.event.motion.y);
                
                // Done
                break;
            }
        }
    }
    
    // Update the keyboard state
    for (size_t i = 0; i < SDL_NUM_SCANCODES; i++) _key_lookup[i]._active = keyboard_state[i];

    // Update the binds
    for (size_t i = 0; i < p_input->bind_quantity; i++)
    {
        
        // Initialize data
        input_bind *p_bind = p_input->_p_binds[i];

        // Clear the input
        p_bind->value = 0.0;

        // Iterate through each scancode
        for (size_t j = 0; j < p_bind->scancode_quantity; j++)
        {

            // Initialized data
            size_t p_scancode = (size_t) p_bind->_scancodes[j];

            // Update the scancode 
            if ( _key_lookup[p_scancode]._active )
            {

                // Set the input
                p_bind->value = 1.0;

                // Done with this bind
                break;
            }
        }
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

void g_sdl2_mouse_lock_toggle ( void )
{

    // Static data
    static bool relative_mouse = false;

    // Invert the relative mouse flag
    relative_mouse = !relative_mouse;

    // Update the relative mouse mode
    SDL_SetRelativeMouseMode(relative_mouse);

    // Done
    return;
}

int g_sdl2_scancode_to_name ( const char **pp_result, void *scancode )
{

    // Argument check
    if ( pp_result == (void *) 0 ) goto no_result;

    // Return a scancode name to the caller
    *pp_result = _key_lookup[(size_t)scancode]._name;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_result:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for paramter \"pp_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_sdl2_window_destroy ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Destroy the window
    SDL_DestroyWindow(p_instance->window.sdl2.window);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl2] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
#endif