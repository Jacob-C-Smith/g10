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
                 *schedule_name     = "Client Schedule",
                 *client_name       = 0;
    bool          connect_to_server = false;

    // Parse command line arguments
    for (size_t i = 0; i < argc; i++)
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

    // Game setup
    {

        // Set up input 
        {

            // Initialized data
            GXBind_t *exit_bind  = find_bind(p_instance->input, "QUIT"),
                     *lock_mouse = find_bind(p_instance->input, "TOGGLE LOCK MOUSE"),
                     *play_sound = find_bind(p_instance->input, "PLAY SOUND"),
                     *mouse_up   = find_bind(p_instance->input, "MOUSE UP"),
                     *mouse_down = find_bind(p_instance->input, "MOUSE DOWN");

            // If quit is fired, exit the game loop
            register_bind_callback(exit_bind, &g_user_exit);
            
            // Set up the camera controller
            {
                // First person controller
                // camera_controller_from_camera(instance, instance->context.scene->active_camera);
               
                // Third person controller. Thanks Aiden :)
                // aps_3rdpersonctrl_from_camera_and_entity(instance, instance->active_scene->active_camera, get_entity(instance->active_scene, "player1"));
            }
             
        }

        // Set up user code
        add_user_code_callback(p_instance, &user_code_callback);
    }
    
    // Start the game 
    g_start_schedule(p_instance, schedule_name);
    
    // Stop the game
    g_stop_schedule(p_instance);

    // Exit 
    g_exit(&p_instance);

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
