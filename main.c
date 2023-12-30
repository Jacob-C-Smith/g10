/** !
 * Example g10 program
 * 
 * @author Jacob Smith
 * @file main.c
 */

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

// sync module
#include <sync/sync.h>

// http module
#include <http/http.h>

// json module
#include <json/json.h>

// web module
#include <web/web.h>
#include <web/route.h>

// g10
#include <g10/g10.h>
#include <g10/entity.h>
#include <g10/user_code.h>

/** !
 *  This gets called once per frame
 * 
 * @param p_instance : the instance
 * 
 * @return 1 
*/
int user_code_main ( g_instance *const p_instance )
{

    // Print the name of the instance
    printf("%s\n", p_instance->_name);

    // Success
    return 1;
}

int index_router ( http_message *p_request, http_message *p_response )
{

    // Success
    return 1;
}

// Entry point
int main ( int argc, const char *const argv[] )
{

    // Supress compiler warnings
    (void) argc;
    (void) argv;

    // Initialized data
    g_instance *p_instance = 0;
    mat4        _mat       = { 0 };
    entity *p_entity = (void *) 0;
    json_value *p_value = (void *) 0;
    char p_buf[4096] = {0};

    // Set up g10
    if ( g_init(&p_instance, "instance.json") == 0 ) goto failed_to_initialize_g10;

    // Set a user code callback
    user_code_callback_set(p_instance, user_code_main);

    // Call the user code
    user_code_callback(p_instance);

    // Start the server
    //g_start_server(p_instance);

    // Set the running flag
    p_instance->running = true;

    g_load_file("entity.json", p_buf, false);
    parse_json_value(p_buf, (void *) 0, &p_value);

    entity_from_json(&p_entity, p_value);
    
    extern int g_sdl2_poll_window ( g_instance *p_instance );

    // Main loop
    while ( p_instance->running )
    {

        // Input
        g_sdl2_poll_window (p_instance );
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
                printf("Error: Failed to initialize G10\n");

                // Error
                return EXIT_FAILURE;

            failed_to_teardown_g10:
                
                // Write an error message to standard out
                log_warning("Error: Failed to teardown G10\n");

                // Error
                return EXIT_FAILURE;
        }
    }
}
