#include <skybox.h>
#include <camera.h>
#include <uniform.h>
#include <sampler.h>

// external functions
extern int g_sdl3_texture_load ( texture **pp_texture, const char *p_path );
extern int g_sdl3_texture_load_cubemap ( texture **pp_texture, const json_value *p_value );
extern int g_sdl3_geometry_from_json ( geometry **pp_geometry, const json_value *p_value );

int skybox_from_json ( skybox **pp_skybox, json_value *p_value )
{
    if ( pp_skybox == NULL || p_value == NULL ) return 0;

    skybox *p_skybox = default_allocator(0, sizeof(skybox));
    if ( !p_skybox ) return 0;
    memset(p_skybox, 0, sizeof(skybox));

    dict *p_dict = p_value->object;
    json_value *p_name = NULL,
               *p_texture = NULL,
               *p_geometry = NULL,
               *p_pipeline_name = NULL;

    dict_get(p_dict, "name"    , (void **)&p_name);
    dict_get(p_dict, "texture" , (void **)&p_texture);
    dict_get(p_dict, "geometry", (void **)&p_geometry);
    dict_get(p_dict, "pipeline", (void **)&p_pipeline_name);

    if ( p_name ) strncpy(p_skybox->_name, p_name->string, 63);
    
    // Load texture
    if ( p_texture )
    {
        if ( p_texture->type == JSON_VALUE_STRING )
            g_sdl3_texture_load(&p_skybox->p_texture, p_texture->string);
        else
            g_sdl3_texture_load_cubemap(&p_skybox->p_texture, p_texture);
    }

    // Load geometry
    if ( p_geometry )
        g_sdl3_geometry_from_json(&p_skybox->p_geometry, p_geometry);

    if ( p_pipeline_name )
        p_skybox->pipeline = p_pipeline_name->string;

    *pp_skybox = p_skybox;
    return 1;
}

int skybox_bind ( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable )
{
    skybox *p_skybox = (skybox *)p_drawable;
    if ( !p_skybox ) return 0;

    // Bind texture to sampler slot 0
    sampler *p_sampler = NULL;
    array_index(p_pipeline->p_samplers, 0, (void **)&p_sampler);

    if ( p_skybox->p_texture && p_sampler )
    {
        SDL_BindGPUFragmentSamplers(
            p_render_pass->p_handle,
            p_sampler->idx, 
            &(SDL_GPUTextureSamplerBinding){
                .sampler = p_sampler->p_handle,
                .texture = p_skybox->p_texture->p_handle
            },
            1
        );
    }

    // Bind geometry
    geometry_bind(p_render_pass, p_skybox->p_geometry);

    return 1;
}

int skybox_draw ( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable )
{
    skybox *p_skybox = (skybox *)p_drawable;
    if ( !p_skybox || !p_skybox->p_geometry ) return 0;

    if ( p_skybox->p_geometry->p_index_handle )
        SDL_DrawGPUIndexedPrimitives(p_render_pass->p_handle, p_skybox->p_geometry->index_count * 3, 1, 0, 0, 0);
    else
        SDL_DrawGPUPrimitives(p_render_pass->p_handle, p_skybox->p_geometry->vertex_count, 1, 0, 0);

    return 1;
}

int skybox_camera_bind ( render_pass *p_render_pass, pipeline *p_pipeline )
{
    g_instance *p_instance = g_active_instance();
    camera *p_camera = p_instance->context.p_scene->p_active_camera;
    if ( !p_camera ) return 0;

    // Create a modified camera struct for skybox (no translation)
    camera sky_camera = *p_camera;
    
    // Zero out translation in view matrix
    // Column major: [3][0], [3][1], [3][2] are m, n, o
    sky_camera.matrix._view.m = 0.0f;
    sky_camera.matrix._view.n = 0.0f;
    sky_camera.matrix._view.o = 0.0f;

    uniform *p_camera_uniform = NULL;
    array_index(p_pipeline->p_uniforms, 2, (void **)&p_camera_uniform);

    if ( p_camera_uniform )
        uniform_set_pack_push(p_camera_uniform, &sky_camera, (fn_pack *)camera_pack);

    return 1;
}
