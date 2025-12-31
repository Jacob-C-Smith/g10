// header
#include <entity.h>
#include <material.h>

static float frand(void)
{
    return (float)rand() / (float)RAND_MAX;
}

void hsv_to_rgb(float h, float s, float v, float *r, float *g, float *b)
{
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h * 6.0f, 2.0f) - 1.0f));
    float m = v - c;

    float rp, gp, bp;

    if (h < 1.0f/6.0f)      { rp = c; gp = x; bp = 0; }
    else if (h < 2.0f/6.0f) { rp = x; gp = c; bp = 0; }
    else if (h < 3.0f/6.0f) { rp = 0; gp = c; bp = x; }
    else if (h < 4.0f/6.0f) { rp = 0; gp = x; bp = c; }
    else if (h < 5.0f/6.0f) { rp = x; gp = 0; bp = c; }
    else                    { rp = c; gp = 0; bp = x; }

    *r = rp + m;
    *g = gp + m;
    *b = bp + m;
}

void random_vibrant_color(float *r, float *g, float *b)
{
    float h = frand();          // random hue
    float s = 0.9f + 0.1f * frand(); // high saturation
    float v = 0.9f + 0.1f * frand(); // high brightness

    hsv_to_rgb(h, s, v, r, g, b);
}

// function definitions
int entity_info ( entity *p_entity )
{

    // print the entity
    logger_pad(), log_info("Entity @%p\n", p_entity),

    logger_push(),
    logger_pad(), printf("name     - %s\n", p_entity->_name),
    logger_pad(), printf("pipeline - %s\n", p_entity->pipeline),    
    
    logger_pad(), printf("geometry:\n"),
    logger_push();

    if(p_entity->p_geometry) geometry_info(p_entity->p_geometry);
    logger_pop(),

    logger_pad(), printf("transform:\n"),
    logger_push(),
    transform_info(p_entity->p_transform),
    logger_pop();

    logger_pad(), printf("material:\n");
    logger_push();
    if ( p_entity->p_material ) material_info(p_entity->p_material);
    logger_pop();

    logger_pop();
    
    return 1;
}

int entity_from_json ( entity **pp_entity, json_value *p_value )
{

    // error check
    if ( NULL == pp_entity ) goto no_entity;

    // initialized data
    g_instance *p_instance = g_active_instance();
    entity *p_entity = default_allocator(0, sizeof(entity));

    dict *p_dict = p_value->object;
    json_value *p_name          = dict_get(p_dict, "name"),
               *p_transform     = dict_get(p_dict, "transform"),
               *p_geometry      = dict_get(p_dict, "geometry"),
               *p_material      = dict_get(p_dict, "material"),
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

    // Parse Material
    if ( p_material && p_material->type == JSON_VALUE_STRING )
    {
        // Load material from file
        const char *path = p_material->string;
        size_t len = load_file(path, 0, false);
        
        if ( len > 0 )
        {
            // Allocate buffer
            char *buf = default_allocator(0, len + 1);
            if ( buf )
            {
                load_file(path, buf, false);
                buf[len] = '\0'; // Null terminate

                json_value *mat_json = 0;
                if ( json_value_parse(buf, 0, &mat_json) )
                {
                    material_from_json(&p_entity->p_material, mat_json);
                    
                    // Ideally free mat_json here if logic existed
                }
                
                // Free buffer
                // default_allocator(buf, 0); 
            }
        }
    }

    // return a pointer to the caller
    *pp_entity = p_entity;

    // done
    return 1;
    
    no_entity: return 0;
}

int entity_bind ( render_pass *p_render_pass, pipeline *p_pipeline, entity *p_entity )
{

    // transform
    transform_bind(p_render_pass, p_pipeline, p_entity->p_transform);
  
    // material
    if ( p_entity->p_material ) material_bind(p_render_pass, p_pipeline, p_entity->p_material);
    
    // bind the geometry
    geometry_bind(p_render_pass, p_entity->p_geometry);

    // success
    return 1;
}
