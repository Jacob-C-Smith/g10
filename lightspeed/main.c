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
#include <skybox.h>

// function declarations
int game_logic ( g_instance *p_instance );

// entry point
int main ( int argc, const char *argv[] ) 
{
    
    // unused
    (void) argc, (void) argv;

    // initialized data
    g_instance *p_instance = NULL;
    skybox *p_skybox = NULL;
    json_value *p_s = NULL;
    char _buf[1024] = { 0 };
    bool ok = true;

    // initialize g10
    if ( 0 == g_init(&p_instance, "assets/lightspeed.json") ) goto failed_to_initialize_g10;

    load_file("assets/skybox/skybox.json", _buf, true);
    json_value_parse(_buf, 0, &p_s);

    skybox_from_json(&p_skybox, p_s);
    p_instance->context.p_scene->p_skybox = p_skybox;

    // program pipelines
    {

        // program skybox pipeline
        ok &= program_pipeline("skybox", 
            (fn_pipeline_bind_once *)skybox_camera_bind,
            NULL,
            (fn_pipeline_bind_each *)skybox_bind,
            (fn_pipeline_draw *)skybox_draw
        );

        // program default pipeline
        ok &= program_pipeline("default", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_cull *)entity_cull,
            (fn_pipeline_bind_each *)entity_bind,
            (fn_pipeline_draw *)entity_draw
        );

        // program aabb pipeline
        ok &= program_pipeline("aabb", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_cull *)bv_cull_pipeline,
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

void ptr_prt(void *p_element)
{
    printf("%p, ", p_element);
}

int game_logic ( g_instance *p_instance )
{

    // update the camera
    camera_controller_first_person_update(p_instance->context.p_scene->p_active_camera);

    if ( input_bind_value("SNAPSHOT") )    
    {

        static int time = 0;
        pipeline *p_pipeline = NULL;
        dict_get(p_instance->cache.p_pipeline, "default", (void**)&p_pipeline);
        size_t l = array_size(p_pipeline->p_dynamic_draw_list);

        printf("[%d] : %d\n",time,l);

        time++;
        
    }
    
    // success
    return 1;
}