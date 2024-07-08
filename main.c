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
#include <g10/input.h>
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
int game_setup ( g_instance *const p_instance );

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
        input_poll(p_instance);
        
        // State change
        ai_preupdate(p_instance);

        // State
        ai_update(p_instance);

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

int game_setup ( g_instance *const p_instance )
{

    // Print g10 info
    {

        // Print instance info
        g_info(p_instance);

        // Print renderer info
        renderer_info(p_instance->context.p_renderer);
    }

    // Set a user code callback
    user_code_callback_set(p_instance, user_code_main);
    
    // Set the running flag
    p_instance->running = true;

    // Log
    log_info("Game setup is complete!\n");

    // Success
    return 1;
}
