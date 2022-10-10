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

    update_controlee_camera(instance->delta_time);

    //Rotate teapot
    static float r = 0.f;

    GXEntity_t* entity = get_entity(instance->active_scene, "entity");
    entity->transform->rotation = quaternion_from_euler_angle((vec3) { 0.f, r, 0.f });

    r += instance->delta_time * 5;

    entity->transform->location.x = sin(r / 10) * 5;
    entity->transform->location.y = cos(r / 10) * 5;

    printf("Teapot pos: %.2f, %.2f, %.2f\r",
        entity->transform->location.x,
        entity->transform->location.y,
        entity->transform->location.z);

    return 0;
}

int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t  *instance               = 0;
    GXScene_t     *scene                  = 0;
    GXShader_t    *shader                 = 0;
    
    GXClient_t    *client                 = 0;
     
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

    // Network testing
    {

        // Command testing
        {
            GXCommand_t *cfd_command = 0;
            u8           cfd_data[]  = { 0x05, 0x00, 0x34, 0x12, 0x89, 0x67, 'h' , 'e' , 'l' , 'l', 'o', 0x00 };
            //                           ID1   ID2   CHN1  CHN2  LEN   LEN   C[0], C[1], C[2], C[3] C[4] C[5]

            GXCommand_t *dfc_command = 0;
            u8          *dfc_data    = 0;
            
            dfc_command = calloc(1, sizeof(GXCommand_t));

            dfc_command->type = chat;
            dfc_command->chat.chat         = "hello";
            dfc_command->chat.chat_channel = chat_channel_all;

            command_from_data(&cfd_command, &cfd_data);
            data_from_command(&dfc_data   , dfc_command);

            printf("");
        }

        if (instance->server)
            start_server(instance->server);

        if (connect_to_server)
        {
            //SDL_Delay(5000);
            //connect_client(&client);
        }
    }

    instance->running = true;

    // Start the game loop
    g_start_schedule(instance, schedule_name);
    
    // Exit 
    g_exit(instance);

    return 0;
}