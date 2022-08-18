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

int main ( int argc, const char *argv[] )
{

    // Initialized data
    GXInstance_t  *instance      = 0;
    GXScene_t     *scene         = 0;
    GXSchedule_t  *game_schedule = 0;
    GXShader_t    *shader        = 0;
    
    // Create a debug instance
    g_init(&instance, "G10/Debug instance.json");

    // Game setup
    {

        // Set up binds                                                 
        {

            // Add an exit bind
            GXBind_t *exit_bind = find_bind(instance->input, "QUIT");
            register_bind_callback(exit_bind, &g_user_exit);
        }

        // Set up AI
        {

            // Initialized data
            GXEntity_t *entity = get_entity(instance->active_scene, "entity");
            GXAI_t     *ai     = entity->ai;

            // Pre AI update
            //set_ai_pre_update_callback(ai, &goomba_preupdate);

            // AI update
            //add_ai_state_callback(ai, "ATTACK", &goomba_state_attack);
            //add_ai_state_callback(ai, "RUN LEFT", &goomba_state_run_left);
        }
        
        // Get the schedule
        game_schedule = dict_get(instance->schedules, "Simple Schedule");
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
                    g_print_log("\t\t%d %s\n", i, entities[i]);

                g_print_log("\tactive camera: \"%s\"\n", (instance->active_scene->active_camera) ? (instance->active_scene->active_camera->name) : "(null)");

            }

        }
    }

    instance->running = true;

    // Start the game loop
    start_schedule(game_schedule);
    
    // Exit 
    g_exit(instance);

    return 0;
}