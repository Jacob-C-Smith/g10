#include <stdio.h>

#include <G10/G10.h>
#include <G10/GXScheduler.h>
#include <G10/GXScene.h>
#include <G10/GXEntity.h>
#include <G10/GXShader.h>
#include <G10/GXAI.h>
#include <G10/GXTransform.h>
#include <G10/GXPart.h>
#include <G10/GXMaterial.h>
#include <G10/GXUserCode.h>

#include <G10/APS_3rdPersonCtrl.h>

// This gets called once a frame
int user_code_callback(GXInstance_t* instance)
{

    // This updates the camera controller
    update_controlee_camera(instance->delta_time);

    return 0;
}

// Entry point 
int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t  *instance               = 0;
    GXScene_t     *scene                  = 0;
    char          *instance_path          = 0;
    char          *schedule_name          = 0;
    char          *client_name            = 0;
    bool           connect_to_server      = false;

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
                camera_controller_from_camera(instance, instance->active_scene->active_camera);

                // Third person controller
                //aps_3rdpersonctrl_from_camera_and_entity(instance, instance->active_scene->active_camera, get_entity(instance->active_scene, "player1"));
            }

            //Chat test bind
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
    
    // Debugging
    {
        
        // Initialized data
        size_t       entity_count = dict_keys(instance->active_scene->entities, 0);
        GXEntity_t **entities     = calloc(entity_count + 1, sizeof(void *));

        // Generate lists
        {

            // Entity list
            dict_keys(instance->active_scene->entities, entities);

        }

        // Print scene info
        {
            g_print_log("G10 Instance: \"%s\"\n", instance->name);
            g_print_log("G10 Scene information:\n");

            // Print scene name
            g_print_log("\tname: \"%s\"\n", instance->active_scene->name);

            // Print each entity
            {
                g_print_log("\tentities:\n");

                for (size_t i = 0; i < entity_count; i++)
                    g_print_log("\t\t[%d] %s\n", i, entities[i]);

                g_print_log("\tactive camera: \"%s\"\n", (instance->active_scene->active_camera) ? (instance->active_scene->active_camera->name) : "(null)");

            }

        }
    }
    
    if (instance->server)
        start_server();

    instance->running = true;

    // Start the game loop
    g_start_schedule(instance, schedule_name);
    
    // Stop execution
    stop_schedule(instance->active_schedule);

    // Exit 
    g_exit(instance);

    return 0;
}
