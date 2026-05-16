/** ! 
 * Transform info
 * 
 * @file util/transform/info.c
 * 
 * @author Jacob Smith
 */

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// gsdk
/// core
#include <core/log.h>

/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// g10
/// world
#include <transform.h>

// forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc the argc parameter of the entry point
 * @param argv the argv parameter of the entry point
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[] );

// data
bool binary = false;
char *p_data = NULL;
size_t buffer_len = 4096;
size_t bytes_read = 0;

// entry point
int main ( int argc, const char *argv[] )
{
    
    // initialized data
    transform *p_transform = NULL;

    // parse command line arguments
    parse_command_line_arguments(argc, argv);

    // allocate memory
    p_data = default_allocator(0, buffer_len);

    // load the file
    while ( !feof(stdin) )
    {

        if ( buffer_len - bytes_read < 2 )
        {
            buffer_len += buffer_len;
            p_data = default_allocator(p_data, buffer_len);
        }

        p_data[bytes_read] = getchar();
        
        bytes_read++;
    }

    if ( binary )
        transform_unpack(&p_transform, p_data);
    else
    {
        json_value *v = NULL;
        json_value_parse(p_data, 0, &v);
        transform_from_json(&p_transform, v);
    }

    if ( NULL == p_transform ) return EXIT_FAILURE;

    transform_info(p_transform);
    
    // success
    return EXIT_SUCCESS;

    // error handling
    {

        // standard library errors
        {
            failed_to_open_file:

                // log the error
                log_error("Error: Failed to open file!\n");

                // error
                return EXIT_FAILURE;
        }
    }
}

void print_usage ( const char *argv0 )
{

    // argument check
    if ( NULL == argv0 ) exit(EXIT_FAILURE);

    // print a usage message to standard out
    printf("Usage: %s < --binary | --json >\n", argv0);

    // done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{

    // iterate through each command line argument
    for (size_t i = 1; i < (size_t) argc; i++)
    {
        
        // json
        if ( 0 == strcmp(argv[i], "--json") )
            binary = false;

        // binary
        else if ( 0 == strcmp(argv[i], "--binary") )
            binary = true;

        // default
        else goto invalid_arguments;
    }
    
    // success
    return;

    // error handling
    {

        // argument errors
        {
            invalid_arguments:
                
                // print a usage message to standard out
                print_usage(argv[0]);

                // abort
                exit(EXIT_FAILURE);
        }
    }
}
