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

    // Log the clean exit
    log_info("Game over!\n");

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

        // g10 errors
        {
            failed_to_initialize_g10:
                
                // Write an error message to standard out
                log_error("Error: Failed to initialize g10!\n");

                // Error
                return EXIT_FAILURE;

            failed_to_teardown_g10:
                
                // Write an error message to standard out
                log_warning("Error: Failed to teardown g10!\n");

                // Error
                return EXIT_FAILURE;
        }
    }
}

int user_code_main ( g_instance *const p_instance )
{

    // Initialized data
    static int c = 0;
    camera *p_camera = p_instance->context.p_scene->context.p_camera;
    entity *p_ball_1   = dict_get(p_instance->context.p_scene->data.entities, "ball 1"),
           *p_ball_2   = dict_get(p_instance->context.p_scene->data.entities, "ball 2");
    mesh_data *p_ball_data_1 = p_ball_1->p_mesh->_p_meshes[0],
              *p_ball_data_2 = p_ball_2->p_mesh->_p_meshes[0];

    // Exit the game?
    if ( input_bind_value("QUIT") ) g_stop();

    // Toggle mouse locking
    if ( input_bind_value("MOUSE LOCK") ) input_mouse_lock_toggle();

    // Update the camera controller
    p_camera->pfn_camera_controller(p_camera);

    // Animate the ball
    p_ball_data_1->p_transform->location.z = 6.0 + 3.0 * sinf((float) c / (float)60);
    p_ball_data_2->p_transform->location.z = 6.0 + 3.0 * cosf((float) c / (float)60);

    // Increment the counter
    c++;

    // Success
    return 1;
}


int game_setup ( g_instance *const p_instance )
{

    // Initialized data
    shell *p_shell = (void *) 0; 

    // Set a user code callback
    user_code_callback_set(p_instance, user_code_main);
    
    // Set the running flag
    p_instance->running = true;
    
    // Log
    log_info("Game setup is complete!\n");

    // Construct a shell
    shell_construct(&p_instance->p_shell);

    // Success
    return 1;
}
