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
        
        // program color pipeline
        ok &= program_pipeline("color", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_bind_each *)entity_bind
        );

        // program uv pipeline
        ok &= program_pipeline("uv", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_bind_each *)entity_bind
        );

        // program normal pipeline
        ok &= program_pipeline("normal", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_bind_each *)entity_bind
        );
        
        // program tbn pipeline
        ok &= program_pipeline("tbn", 
            (fn_pipeline_bind_once *)camera_bind_active,
            (fn_pipeline_bind_each *)entity_bind
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

    // initialized data
    entity *p_entity = dict_get(p_instance->context.p_scene->entities, "octahedron");

    // update the camera
    camera_controller_first_person_update(p_instance->context.p_scene->p_active_camera);

    // p_entity->p_transform->rotation.x += 0.7f;
    // p_entity->p_transform->rotation.y += 0.7f;
    // p_entity->p_transform->rotation.z += 0.7f;

    if ( p_entity->p_transform->rotation.x > 360) p_entity->p_transform->rotation.x -= 360;
    if ( p_entity->p_transform->rotation.y > 360) p_entity->p_transform->rotation.y -= 360;
    if ( p_entity->p_transform->rotation.z > 360) p_entity->p_transform->rotation.z -= 360;

    // p_entity = dict_get(p_instance->context.p_scene->entities, "Cylinder");

    // p_entity->p_transform->rotation.x += 0.7f;
    // p_entity->p_transform->rotation.y += 0.7f;
    // p_entity->p_transform->rotation.z += 0.7f;

    if ( p_entity->p_transform->rotation.x > 360) p_entity->p_transform->rotation.x -= 360;
    if ( p_entity->p_transform->rotation.y > 360) p_entity->p_transform->rotation.y -= 360;
    if ( p_entity->p_transform->rotation.z > 360) p_entity->p_transform->rotation.z -= 360;

    // success

    // success
    return 1;
}