/** !
 * Input implementation
 * 
 * @file input.c
 * 
 * @author Jacob Smith
 */

// Header
#include <g10/input.h>

// Function definitions
int input_from_json ( input **pp_input, const json_value *const p_value )
{

    // Argument check
    if ( pp_input == (void *) 0 ) goto no_input;
    if ( p_value  == (void *) 0 ) goto no_value;

    // Initialized data
    

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
            no_value:

                // Error
                return 0;
        }
    }
}

int input_poll ( g_instance *p_instance )
{
 
    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Window system integration depenant implementation
    #ifdef BUILD_G10_WITH_SDL2
        
        // External functions
        extern int g_sdl2_window_poll ( g_instance *p_instance );

        // Poll for input
        g_sdl2_window_poll(p_instance);
    #else

        // Others? 
    #endif

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
