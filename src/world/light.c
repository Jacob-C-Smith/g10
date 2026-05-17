#include <light.h>
#include <scene.h>
#include <uniform.h>
#include <pipeline.h>
#include <data/dict.h>
#include <data/array.h>
#include <string.h>

const char *light_key_accessor ( const light *const p_light )
{
    return p_light->_name;
}

int light_from_json ( light **pp_light, json_value *p_value )
{
    if ( !pp_light || !p_value || p_value->type != JSON_VALUE_OBJECT ) return 0;

    light *p_light = calloc(1, sizeof(light));
    if ( !p_light ) return 0;

    dict *p_dict = p_value->object;
    json_value *p_name = NULL, *p_type = NULL, *p_loc = NULL, *p_dir = NULL, 
               *p_col = NULL, *p_int = NULL, *p_rad = NULL, *p_in = NULL, *p_out = NULL;

    dict_get(p_dict, "name", (void**)&p_name);
    dict_get(p_dict, "type", (void**)&p_type);
    dict_get(p_dict, "location", (void**)&p_loc);
    dict_get(p_dict, "direction", (void**)&p_dir);
    dict_get(p_dict, "color", (void**)&p_col);
    dict_get(p_dict, "intensity", (void**)&p_int);
    dict_get(p_dict, "radius", (void**)&p_rad);
    dict_get(p_dict, "inner_angle", (void**)&p_in);
    dict_get(p_dict, "outer_angle", (void**)&p_out);

    if ( p_name ) strncpy(p_light->_name, p_name->string, 63);
    if ( p_type ) p_light->type = (g10_light_type)p_type->integer;

    if ( p_loc && p_loc->type == JSON_VALUE_ARRAY ) {
        json_value *pv = NULL;
        array_index(p_loc->list, 0, (void**)&pv); if (pv) p_light->location.x = (float)pv->number;
        array_index(p_loc->list, 1, (void**)&pv); if (pv) p_light->location.y = (float)pv->number;
        array_index(p_loc->list, 2, (void**)&pv); if (pv) p_light->location.z = (float)pv->number;
    }

    if ( p_dir && p_dir->type == JSON_VALUE_ARRAY ) {
        json_value *pv = NULL;
        array_index(p_dir->list, 0, (void**)&pv); if (pv) p_light->direction.x = (float)pv->number;
        array_index(p_dir->list, 1, (void**)&pv); if (pv) p_light->direction.y = (float)pv->number;
        array_index(p_dir->list, 2, (void**)&pv); if (pv) p_light->direction.z = (float)pv->number;
    }

    if ( p_col && p_col->type == JSON_VALUE_ARRAY ) {
        json_value *pv = NULL;
        array_index(p_col->list, 0, (void**)&pv); if (pv) p_light->color.x = (float)pv->number;
        array_index(p_col->list, 1, (void**)&pv); if (pv) p_light->color.y = (float)pv->number;
        array_index(p_col->list, 2, (void**)&pv); if (pv) p_light->color.z = (float)pv->number;
    }

    if ( p_int ) p_light->intensity = (float)p_int->number;
    if ( p_rad ) p_light->radius = (float)p_rad->number;
    if ( p_in ) p_light->inner_angle = (float)p_in->number;
    if ( p_out ) p_light->outer_angle = (float)p_out->number;

    *pp_light = p_light;
    return 1;
}

int lighting_pack ( void *p_buffer, scene *p_scene )
{
    if ( !p_buffer || !p_scene ) return 0;

    lighting_uniform_t lu = { 0 };
    lu.ambient_color = (vec4){ 0.1f, 0.1f, 0.1f, 1.0f }; // Default ambient

    if ( p_scene->lights ) {
        dict *p_lights = p_scene->lights;
        size_t count = 0;
        dict_size(p_lights, &count);
        if ( count > 16 ) count = 16;

        lu.light_count_and_pad.x = (float)count;

        light *pp_lights[16] = { 0 };
        dict_values(p_lights, (void**)pp_lights, 16);

        for ( size_t i = 0; i < count; i++ ) {
            light *p_light = pp_lights[i];

            lu.lights[i].position = (vec4){ p_light->location.x, p_light->location.y, p_light->location.z, (p_light->type == G10_LIGHT_TYPE_DIRECTIONAL) ? 0.0f : 1.0f };
            lu.lights[i].color    = (vec4){ p_light->color.x, p_light->color.y, p_light->color.z, p_light->intensity };
            lu.lights[i].direction = (vec4){ p_light->direction.x, p_light->direction.y, p_light->direction.z, p_light->inner_angle };
            lu.lights[i].params   = (vec4){ p_light->radius, p_light->outer_angle, (float)p_light->type, 0.0f };
        }
    }

    memcpy(p_buffer, &lu, sizeof(lighting_uniform_t));
    return sizeof(lighting_uniform_t);
}

int lighting_bind_active ( render_pass *p_render_pass, pipeline *p_pipeline )
{
    (void)p_render_pass;
    g_instance *p_instance = g_active_instance();
    scene *p_scene = p_instance->context.p_scene;

    uniform *p_lighting = NULL;
    // lighting is index 3 in default.json
    array_index(p_pipeline->p_uniforms, 3, (void**)&p_lighting);

    if ( p_lighting )
        uniform_set_pack_push(p_lighting, p_scene, (fn_pack *)lighting_pack);

    return 1;
}

int lighting_bind_once ( render_pass *p_render_pass, pipeline *p_pipeline )
{
    camera_bind_active(p_render_pass, p_pipeline);
    lighting_bind_active(p_render_pass, p_pipeline);
    return 1;
}
