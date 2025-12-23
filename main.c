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
#include <pipeline.h>
#include <geometry.h>

// sdl3
#include <SDL3/SDL.h>

int poll_input( g_instance *p_instance ) 
{

    // poll events
    while ( SDL_PollEvent(&p_instance->window.sdl3.event) )
    {
        if ( p_instance->window.sdl3.event.type == SDL_EVENT_QUIT )
            p_instance->running = false;
    }

}

// entry point
int main( int argc, const char *argv[] ) 
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    g_instance *p_instance = NULL;

    // initialize g10
    if ( g_init(&p_instance, "assets/instance.json") == 0 ) goto failed_to_initialize_g10;

    // setup
    {

        // initialized data
        geometry *p_geometry = NULL;
        json_value *p_value = NULL;
        size_t file_len = load_file("assets/geometry/cube.json", (void *) 0, true);
        char *p_file_contents = default_allocator(0, (file_len + 1) * sizeof(char));
        
        load_file("assets/geometry/cube.json", p_file_contents, true);

        // parse the file into a json value
        json_value_parse(p_file_contents, 0, (json_value **const)&p_value);

        extern int g_sdl3_geometry_from_json ( geometry **pp_geometry, const json_value *p_value );

        g_sdl3_geometry_from_json(&p_geometry, p_value);
        
        geometry_info(p_geometry);
    }

    // set running state
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
