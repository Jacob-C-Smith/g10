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

    p_entity->color = (vec3)
    {
        .x = 1.0f,
        .y = 1.0f,
        .z = 1.0f
    };

    if ( p_color )
    {
        array *p_array = p_color->list;
        json_value *p_r = NULL,
                   *p_g = NULL,
                   *p_b = NULL;
        
        array_index(p_array, 0, &p_r);
        array_index(p_array, 1, &p_g);
        array_index(p_array, 2, &p_b);

        p_entity->color = (vec3)
        {
            .x = (float) p_r->number,
            .y = (float) p_g->number,
            .z = (float) p_b->number
        };
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
    transform_bind(p_render_pass, p_pipeline, p_entity->p_transform);
  
    // bind the geometry
    geometry_bind(p_render_pass, p_entity->p_geometry);

    // success
    return 1;
}
