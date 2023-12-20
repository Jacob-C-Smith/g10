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

// http module
#include <http/http.h>

// web module
#include <web/web.h>
#include <web/route.h>

// g10
#include <g10/g10.h>
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

    // Set up g10
    if ( g_init(&p_instance, "instance.json") == 0 ) goto failed_to_initialize_g10;

    // Set a user code callback
    user_code_callback_set(p_instance, user_code_main);

    // Call the user code
    user_code_callback(p_instance);

    // Start the server
    g_start_server(p_instance);

    // Set the running flag
    p_instance->running = true;

    // Main loop
    while (p_instance->running)
    {
        sleep(1);
        //printf("%d        \r", z);
        //z++;

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
