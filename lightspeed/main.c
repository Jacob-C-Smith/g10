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
#include <transform.h>
#include <renderer.h>
#include <entity.h>
#include <pipeline.h>
#include <geometry.h>
#include <render_pass.h>
#include <uniform.h>
#include <bv.h>
#include <user_code.h>

// function declarations
int game_logic ( g_instance *p_instance );

// entry point
int main ( int argc, const char *argv[] ) 
{
    
    // unused
    (void) argc, (void) argv;

    // initialized data
    g_instance *p_instance = NULL;
    bool ok = true;

    // initialize g10
    if ( 0 == g_init(&p_instance, "assets/lightspeed.json") ) goto failed_to_initialize_g10;

    // program pipelines
    {

        // program default pipeline
        ok &= program_pipeline("default", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_bind_each *)entity_bind,
            (fn_pipeline_draw *)entity_draw
        );

        // program aabb pipeline
        ok &= program_pipeline("aabb", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_bind_each *)bv_bind,
            (fn_pipeline_draw *)bv_draw
        );
    }

    // user code 
    user_code_set_callback((fn_user_code *)game_logic);

    // logs
    instance_info(p_instance);

    // set running flag
    p_instance->running = ok;

    // main loop
    while ( p_instance->running )
    {

        // poll input
        poll_input(p_instance);
        
        // user code
        user_code(p_instance);

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

int game_logic ( g_instance *p_instance )
{

    // update the camera
    camera_controller_first_person_update(p_instance->context.p_scene->p_active_camera);
    
    // success
    return 1;
}