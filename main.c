#include <stdio.h>

#include <G10/G10.h>
#include <G10/GXScheduler.h>
#include <G10/GXScene.h>
#include <G10/GXEntity.h>
#include <G10/GXUserCode.h>

// This gets called once a frame
int user_code_callback(GXInstance_t* instance)
{

    // Update the camera controller
    update_controlee_camera(instance->time.delta_time);

    return 0;
}

// Entry point 
int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t  *instance               = 0;
    GXScene_t     *scene                  = 0;
    char          *instance_path          = "G10/debug client instance.json",
                  *schedule_name          = "Client Schedule",
                  *client_name            = 0;
    bool           connect_to_server      = false;
    GXAI_t        *p_ai                   = 0;
    GXTexture_t   *abc_texture            = 0;

    // Parse command line arguments
    {

        // Iterate over each command line argument
        for (size_t i = 0; i < argc; i++)
        {

            // Path to instance
            if (strcmp("-instance", argv[i]) == 0)
                instance_path = argv[++i];

            // Name of schedule
            if (strcmp("-schedule", argv[i]) == 0)
                schedule_name = argv[++i];

            // Connect to a host
            if (strcmp("-connect", argv[i]) == 0)
                connect_to_server = true;

            // Client name 
            if (strcmp("-client_name", argv[i]) == 0)
                client_name = argv[++i];
        }
    }

    // Create an instance
    if (instance_path)
        g_init(&instance, instance_path);

    load_camera(&instance->context.scene->active_camera, "G10/camera.json");
    
    // Game setup
    {

        // Set up binds                                                 
        {

            // Add an exit bind
            GXBind_t *exit_bind  = find_bind(instance->input, "QUIT"),
                     *lock_mouse = find_bind(instance->input, "TOGGLE LOCK MOUSE");

            
            // If quit is fired, exit the game loop
            register_bind_callback(exit_bind, &g_user_exit);

            // Toggle mouse locking
            register_bind_callback(lock_mouse, &g_toggle_mouse_lock);

            // Set up the camera controller
            {

                // First person controller
                camera_controller_from_camera(instance, instance->context.scene->active_camera);

                // Third person controller. Thanks Aiden :)
                //aps_3rdpersonctrl_from_camera_and_entity(instance, instance->active_scene->active_camera, get_entity(instance->active_scene, "player1"));
            }

            // Chat test bind
            {
                //GXBind_t* chat_bind = find_bind(instance->input, "TEXT CHAT");
                //register_bind_callback(chat_bind, &chat_callback);
            }
        }
        
        // Set up user code
        {
            add_user_code_callback(instance, &user_code_callback);
        }

    }

    // Log a lot of stuff
    if(1){
        scene_info(instance->context.scene);

        bv_info(instance->context.scene->bvh, 0);

        renderer_info(instance->context.renderer);

        input_info(instance->input);
    }

    instance->running = true;

    // Start the game 
    g_start_schedule(instance, schedule_name);

    // Stop execution
    stop_schedule(instance->context.schedule);
    return 0;

    // Exit 
    g_exit(instance);

    return 1;
}
