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

GXCommand_t* no_operation = 0;

int user_code_callback(GXInstance_t* instance)
{

    update_controlee_camera(instance->delta_time);
    GXEntity_t* e = get_entity(instance->active_scene, "entity");
    vec3 l = e->transform->location;

    printf("%.2f %.2f %.2f\r", l.x, l.y, l.z);

    if(instance->client == (void *)0 && instance->server == (void*) 0)
    {
        SDL_Delay(6000);
        connect_client("Jake");
    }

    if (instance->client)
        queue_enqueue(instance->client->send_queue, no_operation);

    return 0;
}

int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t  *instance               = 0;
    GXScene_t     *scene                  = 0;
    GXShader_t    *shader                 = 0;
         
    char *instance_path = "G10/debug server instance.json";
    char *schedule_name = "Server Schedule";

    bool connect_to_server = false;

    // Parse command line arguments
    {

        // Iterate over each command line argument
        for (size_t i = 0; i < argc; i++)
        {
            if (strcmp("-instance", argv[i]) == 0)
                instance_path = argv[++i];

            if (strcmp("-schedule", argv[i]) == 0)
                schedule_name = argv[++i];

            if (strcmp("-connect", argv[i]) == 0)
                connect_to_server = true;
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
    no_operation = calloc(1, sizeof(GXCommand_t));
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
    
    // Exit 
    g_exit(instance);

    return 0;
}