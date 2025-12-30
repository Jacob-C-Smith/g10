#include <material.h>

// external functions
extern int g_sdl3_texture_load ( texture **pp_texture, const char *p_path );
int g_sdl3_texture_from_color ( texture **pp_texture, f32 r, f32 g, f32 b, f32 a );

int material_from_json ( material **pp_material, json_value *p_value )
{
    if ( pp_material == NULL || p_value == NULL ) return 0;

    // allocate
    material *p_material = default_allocator(0, sizeof(material));
    if ( !p_material ) return 0;
    
    // clear
    memset(p_material, 0, sizeof(material));

    dict *p_dict = p_value->object;

    // Name
    json_value *p_name = dict_get(p_dict, "name");
    if ( p_name && p_name->type == JSON_VALUE_STRING )
        strncpy(p_material->_name, p_name->string, 63);

    // --- Albedo ---
    json_value *p_albedo = dict_get(p_dict, "albedo");
    if ( p_albedo )
    {
        if ( p_albedo->type == JSON_VALUE_STRING )
            g_sdl3_texture_load(&p_material->p_albedo_map, p_albedo->string);
        else if ( p_albedo->type == JSON_VALUE_ARRAY )
        {
            array *list = p_albedo->list;
            json_value *r=0, *g=0, *b=0;
            array_index(list, 0, &r);
            array_index(list, 1, &g);
            array_index(list, 2, &b);
            if(r) p_material->albedo_color.x = (float)r->number;
            if(g) p_material->albedo_color.y = (float)g->number;
            if(b) p_material->albedo_color.z = (float)b->number;

            g_sdl3_texture_from_color(&p_material->p_albedo_map, r->number, g->number, b->number, 1.0);
        }
    }

    // --- Normal ---
    json_value *p_normal = dict_get(p_dict, "normal");
    if ( p_normal )
    {
        if ( p_normal->type == JSON_VALUE_STRING )
            g_sdl3_texture_load(&p_material->p_normal_map, p_normal->string);
        else if ( p_normal->type == JSON_VALUE_ARRAY )
        {
            array *list = p_normal->list;
            json_value *r=0, *g=0, *b=0;
            array_index(list, 0, &r);
            array_index(list, 1, &g);
            array_index(list, 2, &b);
            if(r) p_material->albedo_color.x = (float)r->number;
            if(g) p_material->albedo_color.y = (float)g->number;
            if(b) p_material->albedo_color.z = (float)b->number;

            g_sdl3_texture_from_color(&p_material->p_normal_map, r->number, g->number, b->number, 1.0);
        }
    }

    // --- Roughness ---
    json_value *p_roughness = dict_get(p_dict, "roughness");
    if ( p_roughness )
    {
        if ( p_roughness->type == JSON_VALUE_STRING )
             g_sdl3_texture_load(&p_material->p_roughness_map, p_roughness->string);
        else if ( p_roughness->type == JSON_VALUE_NUMBER )
            p_material->roughness_value = (float)p_roughness->number;
    }

    // --- Metallic ---
    json_value *p_metallic = dict_get(p_dict, "metallic");
    if ( p_metallic )
    {
        if ( p_metallic->type == JSON_VALUE_STRING )
             g_sdl3_texture_load(&p_material->p_metallic_map, p_metallic->string);
        else if ( p_metallic->type == JSON_VALUE_NUMBER )
            p_material->metallic_value = (float)p_metallic->number;
    }

    *pp_material = p_material;
    return 1;
}

int material_bind ( render_pass *p_render_pass, pipeline *p_pipeline, material *p_material )
{
    if ( !p_material ) return 0;


    sampler *p_texture_sampler = NULL;
    sampler *p_normal_sampler = NULL;

    array_index(p_pipeline->p_samplers, 0, &p_texture_sampler);
    array_index(p_pipeline->p_samplers, 1, &p_normal_sampler);

    if ( p_material->p_albedo_map )
    {
        SDL_BindGPUFragmentSamplers(
            p_render_pass->p_handle,
            p_texture_sampler->idx, 
            &(SDL_GPUTextureSamplerBinding){
                .sampler = p_texture_sampler->p_handle,
                .texture = p_material->p_albedo_map->p_handle
            },
            1
        );
    }

    if ( p_material->p_normal_map )
    {
        SDL_BindGPUFragmentSamplers(
            p_render_pass->p_handle,
            p_normal_sampler->idx, 
            &(SDL_GPUTextureSamplerBinding){
                .sampler = p_normal_sampler->p_handle,
                .texture = p_material->p_normal_map->p_handle
            },
            1
        );
    }

    return 1;
}

int material_info ( material *p_material )
{
    logger_pad(), log_info("Material @%p\n", p_material, p_material->_name);
    return 1;
}
