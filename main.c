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

// data
vec3 c = { 0.3f, 0.3f, 0.3f };

// forward declarations
int poll_input( g_instance *p_instance ); 
int program_pipelines ( g_instance *p_instance );
int program_renderer ( g_instance *p_instance );

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

    // program the renderer
    program_renderer(p_instance);

    // program the pipelines
    program_pipelines(p_instance);

    // setup
    {

        // initialized data
        pipeline *p_pipeline = dict_get(p_instance->cache.p_pipeline, "color");
        geometry *p_geometry = NULL;
        json_value *p_value = NULL;

        // load geometry from json
        {
            size_t file_len = load_file("assets/geometry/tri.json", (void *) 0, true);
            char *p_file_contents = default_allocator(0, (file_len + 1) * sizeof(char));
            
            load_file("assets/geometry/tri.json", p_file_contents, true);

            // parse the file into a json value
            json_value_parse(p_file_contents, 0, (json_value **const)&p_value);

            extern int g_sdl3_geometry_from_json ( geometry **pp_geometry, const json_value *p_value );

            g_sdl3_geometry_from_json(&p_geometry, p_value);
        }

        // add geometry to pipeline
        // array_add(p_pipeline->p_static_draw_list, (void *)p_geometry);

        geometry_info(p_geometry);
    }

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
    uniform *p_color = NULL;
    
    array_index(p_pipeline->p_uniforms, 0, (void **)&p_color);
    
    p_color->p_data = &c;
    
    uniform_pack(p_color);
    
    SDL_PushGPUFragmentUniformData(p_instance->graphics.sdl3.command_buffer, 0, p_color->_buffer, sizeof(vec3));
    
    // log
    log_info("[pipeline] Binding %s\n", p_pipeline->_name);
    camera_info(p_camera);
    
    // success
    return 1;
}

int bind_each ( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    entity *p_entity = (geometry *)p_drawable;

    // log
    log_info("[pipeline] [%s] [each] Binding \"%s\"\n", p_pipeline->_name, p_entity->_name);
    aabb_info(&p_entity->p_geometry->_bounds);

    // bind the geometry
    extern int g_sdl3_geometry_bind ( render_pass *p_render_pass, geometry *p_geometry );
    g_sdl3_geometry_bind(p_render_pass, p_entity->p_geometry);

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

    // success
    return 1;
}

int program_renderer ( g_instance *p_instance )
{

    // initialized data
    renderer *p_renderer = p_instance->context.p_renderer;
    render_pass *p_pass = NULL;

    // store the first pass
    array_index(p_renderer->p_passes, 0, &p_pass);

    // set early
    //

    // set late
    //

    // success
    return 1;
}