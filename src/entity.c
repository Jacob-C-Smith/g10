// header
#include <entity.h>

// function definitions
int entity_from_json ( entity **pp_entity, json_value *p_value )
{

    // error check
    if ( NULL == pp_entity ) goto no_entity;

    // initialized data
    g_instance *p_instance = g_active_instance();
    entity *p_entity = default_allocator(0, sizeof(entity));

    dict *p_dict = p_value->object;
    json_value *p_name          = dict_get(p_dict, "name"),
               *p_color         = dict_get(p_dict, "color"),
               *p_texture       = dict_get(p_dict, "texture"),
               *p_normal        = dict_get(p_dict, "normal"),
               *p_transform     = dict_get(p_dict, "transform"),
               *p_geometry      = dict_get(p_dict, "geometry"),
               *p_pipeline_name = dict_get(p_dict, "pipeline");

    // store the name
    strncpy(p_entity->_name, p_name->string, 63);

    // construct a transform
    transform_from_json(&p_entity->p_transform, p_transform);

    // add the entity to the pipeline's draw list
    if ( p_pipeline_name )
    {

        // initialized data
        pipeline *p_pipeline = dict_get(p_instance->cache.p_pipeline, p_pipeline_name->string);

        // store the pipeline name
        p_entity->pipeline = p_pipeline->_name;
        
        // add the entity to the pipeline's static draw list
        array_add(p_pipeline->p_static_draw_list, p_entity);
    }

    if ( p_geometry )
    {

        // initialized data
        geometry *p_geom = NULL;

        extern int g_sdl3_geometry_from_json ( geometry **pp_geometry, const json_value *p_value );
        g_sdl3_geometry_from_json(&p_geom, p_geometry);

        // store the geometry in the entity's transform
        p_entity->p_geometry = p_geom;
    }

    // default to pitch white
    p_entity->color = (vec3)
    {
        .x = 1.0f,
        .y = 1.0f,
        .z = 1.0f
    };

    // parse the color
    if ( p_color )
    {

        // initialized data
        array *p_array = p_color->list;
        json_value *p_r = NULL,
                   *p_g = NULL,
                   *p_b = NULL;

        // TODO: type check
        //
        
        // store the red, green, and blue components
        array_index(p_array, 0, &p_r),
        array_index(p_array, 1, &p_g),
        array_index(p_array, 2, &p_b);

        // store the color
        p_entity->color = (vec3)
        {
            .x = (float) p_r->number,
            .y = (float) p_g->number,
            .z = (float) p_b->number
        };
    }
    
    // parse the texture
    if ( p_texture )
    {

        // initialized data
        char *p_path = p_texture->string;

        extern int g_sdl3_texture_load ( texture **pp_texture, const char *p_path );
        g_sdl3_texture_load(&p_entity->p_texture, p_path);
    }

    // parse the normal
    if ( p_normal )
    {

        // initialized data
        char *p_path = p_normal->string;

        extern int g_sdl3_texture_load ( texture **pp_normal, const char *p_path );
        g_sdl3_texture_load(&p_entity->p_normal, p_path);
    }

    // return a pointer to the caller
    *pp_entity = p_entity;

    // done
    return 1;
    
    no_entity: return 0;
}

int entity_bind ( render_pass *p_render_pass, pipeline *p_pipeline, entity *p_entity )
{

    // initialized data
    g_instance *p_instance = g_active_instance();

    // color 
    if ( 0 == strcmp(p_pipeline->_name, "color") )
    {
    
        // initialized data
        uniform *p_color = NULL;

        // get the color uniform
        array_index(p_pipeline->p_uniforms, 0, (void **)&p_color);

        // bind color
        uniform_set_pack_push(p_color, &p_entity->color, (fn_pack *)vec3_pack);
    } 
    else if ( 
        0 == strcmp(p_pipeline->_name, "texture") ||
        0 == strcmp(p_pipeline->_name, "tbn")
    )
    {
    
        // initialized data
        sampler *p_sampler_1 = NULL;
        sampler *p_sampler_2 = NULL;

        // get the texture uniform
        array_index(p_pipeline->p_samplers, 0, (void **)&p_sampler_1);
        array_index(p_pipeline->p_samplers, 1, (void **)&p_sampler_2);

        SDL_BindGPUFragmentSamplers(
            p_render_pass->p_handle,
            0,
            &(SDL_GPUTextureSamplerBinding)
            {
                .sampler = p_sampler_1->p_handle,
                .texture = p_entity->p_texture->p_handle
            },
            1
        );
        SDL_BindGPUFragmentSamplers(
            p_render_pass->p_handle,
            1,
            &(SDL_GPUTextureSamplerBinding)
            {
                .sampler = p_sampler_2->p_handle,
                .texture = p_entity->p_normal->p_handle
            },
            1
        );
    }

    // transform
    transform_bind(p_render_pass, p_pipeline, p_entity->p_transform);
  
    // bind the geometry
    geometry_bind(p_render_pass, p_entity->p_geometry);

    // success
    return 1;
}
