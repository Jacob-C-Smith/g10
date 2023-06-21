// Standard library
#include <stdio.h>

// G10
#include <G10/G10.h>
#include <G10/GXScheduler.h>
#include <G10/GXUserCode.h>
#include <G10/GXEntity.h>

// This gets called once a frame
int user_code_callback ( GXInstance_t *p_instance )
{

    // Success
    return 1;
}

// This gets called after g_init and before g_start_schedule
int game_initialization ( GXInstance_t *p_instance )
{
    
    // Set the user code callback
    add_user_code_callback(p_instance, &user_code_callback);

    // Success
    return 1;
}

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t *p_instance        = 0;
    GXEntity_t   *p_entity          = 0;
    GXAI_t       *p_ai              = 0;
    const char   *instance_path     = "G10/debug client instance.json",
                 *schedule_name     = "Client Schedule";

    // Parse command line arguments
    for ( size_t i = 0; i < argc; i++ )
    {

        // Path to instance
        if ( strcmp("-instance", argv[i]) == 0 )
            instance_path = argv[++i];

        // Name of schedule
        if ( strcmp("-schedule", argv[i]) == 0 )
            schedule_name = argv[++i];
    }

    // Create an instance
    if ( g_init(&p_instance, instance_path) == 0 )
        goto failed_to_initialize_g10;
    
    // Set up the game itself
    (void)game_initialization(p_instance);
    
    // Start the game
    (void)g_start_schedule(p_instance, schedule_name);

    // Stop the game
    (void)g_stop_schedule(p_instance);

    // Exit
    (void)g_exit(&p_instance);

    // Return
    return EXIT_SUCCESS;

    // Error handling
    {

        // G10 Errors
        {
            failed_to_initialize_g10:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to initialize G10 in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return EXIT_FAILURE;
        }
    }
}
