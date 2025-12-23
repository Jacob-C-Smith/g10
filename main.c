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
#include <render_pass.h>
#include <uniform.h>

// sdl3
#include <SDL3/SDL.h>

// preprocessor macros
// #define LOG_VERBOSE

// forward declarations
int poll_input( g_instance *p_instance ); 
int program_pipelines ( g_instance *p_instance );
int program_renderer ( g_instance *p_instance );

// entry point
int main ( int argc, const char *argv[] ) 
{
    
    // unused
    (void) argc;
    (void) argv;

    // initialized data
    g_instance *p_instance = NULL;

    // initialize g10
    if ( g_init(&p_instance, "assets/instance.json") == 0 ) goto failed_to_initialize_g10;

    // program the renderer
    program_renderer(p_instance);

    // program the pipelines
    program_pipelines(p_instance);

    // set running 
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

int poll_input( g_instance *p_instance ) 
{

    // poll events
    while ( SDL_PollEvent(&p_instance->window.sdl3.event) )
    {
        if ( p_instance->window.sdl3.event.type == SDL_EVENT_QUIT )
            p_instance->running = false;
    }

    // success
    return 1;
}

int bind_once ( render_pass *p_render_pass, pipeline *p_pipeline )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    scene *p_scene = p_instance->context.p_scene;
    camera *p_camera = p_scene->p_active_camera;
    
    // log
    #ifdef LOG_VERBOSE
        log_info("[pipeline] Binding %s\n", p_pipeline->_name);
    #endif

    // camera
    {

        // initialized data
        uniform *p_vp = NULL;

        // get the transform uniform
        array_index(p_pipeline->p_uniforms, 2, (void **)&p_vp);

        // bind model matrix
        uniform_set_pack_push(p_vp, p_camera, (fn_pack *)camera_pack);
    }

    // success
    return 1;
}

int bind_each ( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    entity *p_entity = (geometry *)p_drawable;

    // log
    #ifdef LOG_VERBOSE
        log_info("[pipeline] [%s] [each] Binding \"%s\"\n", p_pipeline->_name, p_entity->_name);
    #endif

    // color 
    {
    
        // initialized data
        uniform *p_color = NULL;

        // get the color uniform
        array_index(p_pipeline->p_uniforms, 0, (void **)&p_color);

        // increase brightness
        {
            p_entity->color.x += 0.005f;
            p_entity->color.y += 0.0025f;
            p_entity->color.z += 0.00125f;

            p_entity->color.x = (p_entity->color.x > 1.0f) ? 0.0f : p_entity->color.x;
            p_entity->color.y = (p_entity->color.y > 1.0f) ? 0.0f : p_entity->color.y;
            p_entity->color.z = (p_entity->color.z > 1.0f) ? 0.0f : p_entity->color.z;
        }

        // bind color
        uniform_set_pack_push(p_color, &p_entity->color, (fn_pack *)vec3_pack);
    }

    // transform
    {

        // initialized data
        uniform *p_m = NULL;

        // get the transform uniform
        array_index(p_pipeline->p_uniforms, 1, (void **)&p_m);

        // compute the model matrix
        mat4_model_from_vec3(
            &p_entity->p_transform->model,
            p_entity->p_transform->location,
            p_entity->p_transform->rotation,
            p_entity->p_transform->scale
        );
        
        // bind model matrix
        uniform_set_pack_push(p_m, &p_entity->p_transform->model, (fn_pack *)mat4_pack);
    }

    // bind the geometry
    geometry_bind(p_render_pass, p_entity->p_geometry);

    // success
    return 1;
}

int program_pipelines ( g_instance *p_instance )
{

    // initialized data
    pipeline *p_pipeline = NULL;
    
    // program color pipeline
    p_pipeline = dict_get(p_instance->cache.p_pipeline, "color"),
    pipeline_set_bind_once(p_pipeline, bind_once),
    pipeline_set_bind_each(p_pipeline, bind_each);

    // program aabb pipeline
    p_pipeline = dict_get(p_instance->cache.p_pipeline, "aabb"),
    pipeline_set_bind_once(p_pipeline, bind_once),
    pipeline_set_bind_each(p_pipeline, bind_each);

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