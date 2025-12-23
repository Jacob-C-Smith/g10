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

// sdl3
#include <SDL3/SDL.h>

// forward declarations
int entity_bind ( render_pass *p_render_pass, pipeline *p_pipeline, entity *p_entity );
int program_pipelines ( g_instance *p_instance );
int program_renderer ( g_instance *p_instance );

// entry point
int main ( int argc, const char *argv[] ) 
{
    
    // unused
    (void) argc, (void) argv;

    // initialized data
    g_instance *p_instance = NULL;

    // initialize g10
    if ( 0 == g_init(&p_instance, "assets/instance.json") ) goto failed_to_initialize_g10;

    // program the renderer
    program_renderer(p_instance);

    // program the pipelines
    program_pipelines(p_instance);

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

int program_pipelines ( g_instance *p_instance )
{

    // program color pipeline
    program_pipeline("color", 
        (fn_pipeline_bind_once *)camera_bind_active,
        (fn_pipeline_bind_each *)entity_bind
    );

    // program aabb pipeline
    program_pipeline("aabb", 
        (fn_pipeline_bind_once *)camera_bind_active,
        (fn_pipeline_bind_each *)aabb_bind
    );

    // success
    return 1;
}

int program_renderer ( g_instance *p_instance )
{

    // initialized data
    renderer *p_renderer = p_instance->context.p_renderer;
    render_pass *p_pass = NULL;

    // success
    return 1;
}