#include <g10/g10.h>

// Uninitialized data
FILE* log_file;

// Initialized data
static g_instance *active_instance = 0;

int g_init ( g_instance **pp_instance, const char *p_path )
{

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
                    printf("[g10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    printf("[g10] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
