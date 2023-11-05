/** !
 * Implements general G10 functionality
 * 
 * @file g10.c
 * @author Jacob Smith
 */

// Headers
#include <g10/g10.h>

// Static initialized data
static g_instance *active_instance = 0;

/** !
 * Initialize libraries
 * 
 * @param void
 * @return void
 */
u0 g_init_early ( void )
{

    // Initialize log library
    log_init(0, BUILD_G10_WITH_ANSI_COLOR);

    // Done
    return;
}

int g_init ( g_instance **pp_instance, const char *p_path )
{

    // Early 
    g_init_early( );

    // Argument Check
    if ( pp_instance == (void *) 0 ) goto no_instance;
    if ( p_path      == (void *) 0 ) goto no_path;

    // Initialized data
    g_instance *p_instance = 0;
    
    // Return an instance pointer to the caller
    *pp_instance = p_instance;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Memory allocator returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_exit ( g_instance **pp_instance )
{

    // Argument Check
    if ( pp_instance == (void *) 0 ) goto no_instance;

    // Initialized data
    g_instance *p_instance = *pp_instance;
    
    // No more pointer for caller
    *pp_instance = 0;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}