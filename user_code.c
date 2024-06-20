#include <g10/user_code.h>

int user_code_callback ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Check for a user callback function, so to avoid calling a null pointer
    if ( p_instance->context.pfn_user_code_callback )

        // Call the callback function
        p_instance->context.pfn_user_code_callback(p_instance);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [user code] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int user_code_callback_set ( g_instance *p_instance, fn_user_code_callback pfn_user_code_callback )
{

    // Argument check
    if ( p_instance             == (void *) 0 ) goto no_instance;
    if ( pfn_user_code_callback == (void *) 0 ) goto no_user_code_callback;

    // Set the instance's user code callback function to the specified user code callback function
    p_instance->context.pfn_user_code_callback = pfn_user_code_callback;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [user code] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_user_code_callback:
                #ifndef NDEBUG
                    log_error("[g10] [user code] Null pointer provided for parameter \"pfn_user_code_callback\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
