#include <stdio.h>

#include <SDL2/SDL_gamecontroller.h>

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
#include <G10/GXCameraController.h>

int user_code_callback(GXInstance_t* instance)
{

    // Update the camera controller
    update_controlee_camera(instance->delta_time);

    static float r = 0.f;

    GXEntity_t* entity = get_entity(instance->active_scene, "entity");
    entity->transform->rotation = quaternion_from_euler_angle((vec3) { 0.f, r, 0.f });

    r += instance->delta_time * 5;

    return 0;
}

int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t  *instance               = 0;
    GXScene_t     *scene                  = 0;
    GXShader_t    *shader                 = 0;

    dict          *command_line_arguments = 0;

    char *instance_path = "G10/debug server instance.json";
    char *schedule_name = "Server Schedule";

    // Parse command line arguments
    {

        // Initialized data
        char *instance_load_path = 0;

        // Construct a dictionary from the command line arguments
        dict_from_keys(&command_line_arguments, argv, argc);
        
        // Iterate over each command line argument
        for (size_t i = 0; i < argc; i++)
        {
            if (strcmp("-instance", argv[i]) == 0)
                instance_path = argv[++i];

            if (strcmp("-schedule", argv[i]) == 0)
                schedule_name = argv[++i];
        }

    }

    // Create a debug instance
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
                camera_controller_from_camera(instance, instance->active_scene->active_camera);
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

        load_shader(&shader, "G10/shaders/G10 triangle camera.json");

        // Generate lists
        {

            // Entity list
            dict_keys(instance->active_scene->entities, entities);

        }

        // Print scene info
        {

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

    instance->running = true;

    // Start the game loop
    start_server(instance->server);
    g_start_schedule(instance, schedule_name);
    
    // Exit 
    g_exit(instance);

    return 0;
}