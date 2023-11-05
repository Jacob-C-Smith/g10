/** !
 * Example g10 program
 * 
 * @author Jacob Smith
 * @file main.c
 */

// Standard library
#include <stdio.h>

// g10
#include <g10/g10.h>

// Entry point
int main ( int argc, const char *const argv[] )
{

    // Initialized data
    g_instance *p_instance = 0;

    // Set up g10
    if ( g_init(&p_instance, "instance.json") == 0 ) goto failed_to_initialize_g10;

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
                printf("Error: Failed to teardown G10\n");

                // Error
                return EXIT_FAILURE;
        }
    }
}
