/** !
 * Example g10 program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// sync module
#include <sync/sync.h>

// json module
#include <json/json.h>

// g10
#include <g10/g10.h>
#include <g10/entity.h>
#include <g10/camera.h>
#include <g10/mesh.h>
#include <g10/user_code.h>

/** !
 *  This gets called once per frame
 * 
 * @param p_instance the active instance
 * 
 * @return 1 
*/
int user_code_main ( g_instance *const p_instance );

/** !
 * Set up the example
 * 
 * @param p_instance the active instance
 * 
 * @return 1
 */
int game_setup ( g_instance *p_instance );

// TODO: Abstract this into "input_poll" so it's platform agnostic
// External functions
extern int g_sdl2_poll_window ( g_instance *p_instance );

// Entry point
int main ( int argc, const char *const argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    g_instance *p_instance = 0;

    // Initialize g10
    if ( g_init(&p_instance, "resources/instance.json") == 0 ) goto failed_to_initialize_g10;

    // Set up the example
    game_setup(p_instance);

    // Block
    while ( p_instance->running )
    {

        // Input
        g_sdl2_poll_window(p_instance);
        
        // User code
        user_code_callback(p_instance);

        // Render
        renderer_render(p_instance);

        // Present
        renderer_present(p_instance);
    }

    // Clean up g10
    if ( g_exit(&p_instance) == 0 ) goto failed_to_teardown_g10;

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

        // g10 errors
        {
            failed_to_initialize_g10:
                
                // Write an error message to standard out
                log_error("Error: Failed to initialize G10!\n");

                // Error
                return EXIT_FAILURE;

            failed_to_teardown_g10:
                
                // Write an error message to standard out
                log_warning("Error: Failed to teardown G10!\n");

                // Error
                return EXIT_FAILURE;
        }
    }
}

int user_code_main ( g_instance *const p_instance )
{

    // Print the name of the instance
    // printf("%s\n", p_instance->_name);

    // Success
    return 1;
}

int game_setup ( g_instance *p_instance )
{

    // Initialized data
    scene  *p_scene = p_instance->context.p_scene;
    entity *p_entity = scene_entity_get(p_scene, "entity");

    // Print g10 info
    g_info(p_instance);

    // Print renderer info
    renderer_info(p_instance->context.p_renderer);

    // Set a user code callback
    user_code_callback_set(p_instance, user_code_main);

    // Add a plane to the entity
    mesh_shape_construct(&p_entity->p_mesh_data, G10_BASE_MESH_PLANE, 0);

    // TODO: Move this somewhere less stupid
    p_entity->p_shader->count = 1;
    p_entity->p_shader->_p_draw_items[0] = p_entity->p_mesh_data;

    // Set the running flag
    p_instance->running = true;

    // Log
    log_info("Game setup is complete!\n");

    // Success
    return 1;
}
