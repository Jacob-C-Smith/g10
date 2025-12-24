/** 
 * g10 example
 * 
 * @file main.c
 * 
 * g10 example program
 */

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>

// g10
#include <g10.h>
#include <renderer.h>
#include <entity.h>
#include <pipeline.h>
#include <geometry.h>
#include <render_pass.h>
#include <uniform.h>
#include <aabb.h>

// entry point
int main ( int argc, const char *argv[] ) 
{
    
    // unused
    (void) argc, (void) argv;

    // initialized data
    g_instance *p_instance = NULL;

    // initialize g10
    if ( 0 == g_init(&p_instance, "assets/instance.json") ) goto failed_to_initialize_g10;

    // program color pipeline
    program_pipeline("color", 
        (fn_pipeline_bind_once *)camera_bind_active,
        (fn_pipeline_bind_each *)entity_bind
    );

    // program aabb pipeline
    program_pipeline("floor", 
        (fn_pipeline_bind_once *)camera_bind_active,
        (fn_pipeline_bind_each *)entity_bind
    );

    // set running flag
    p_instance->running = true;

    // main loop
    while ( p_instance->running )
    {

        // poll input
        poll_input(p_instance);

        // render the frame
        renderer_render(p_instance);
    }
    
    // success
    return EXIT_SUCCESS;

    // error handling
    {

        failed_to_create_renderer:
            return EXIT_FAILURE;

        failed_to_initialize_g10:
            log_error("Error: Failed to initialize g10!\n");
            return EXIT_FAILURE;

        failed_to_teardown_g10:
            log_warning("Error: Failed to teardown g10!\n");
            return EXIT_FAILURE;
    }
}