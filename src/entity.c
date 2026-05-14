// header
#include <entity.h>
#include <material.h>

// key accessor
const char *entity_key_accessor ( const entity *const p_entity )
{
    return p_entity->_name;
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
    json_value *p_name          = NULL,
               *p_transform     = NULL,
               *p_geometry      = NULL,
               *p_material      = NULL,
               *p_pipeline_name = NULL;

    dict_get(p_dict, "name"     , (void **)&p_name);
    dict_get(p_dict, "transform", (void **)&p_transform);
    dict_get(p_dict, "geometry" , (void **)&p_geometry);
    dict_get(p_dict, "material" , (void **)&p_material);
    dict_get(p_dict, "pipeline" , (void **)&p_pipeline_name);

    // store the name
    strncpy(p_entity->_name, p_name->string, 63);

    // construct a transform
    transform_from_json(&p_entity->p_transform, p_transform);

    // add the entity to the pipeline's draw list
    if ( p_pipeline_name )
    {

        // initialized data
        pipeline *p_pipeline = NULL;
        dict_get(p_instance->cache.p_pipeline, p_pipeline_name->string, (void **)&p_pipeline);

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

        // TODO: clean this up later so no dangling pointers
        p_entity->p_geometry->p_local_transform->p_parent = p_entity->p_transform;

        aabb_from_entity(&p_entity->p_geometry->_bounds, p_entity);

        {
            pipeline *p_pipeline = NULL;
            dict_get(p_instance->cache.p_pipeline, "aabb", (void **)&p_pipeline);
            if ( p_pipeline )
                array_add(p_pipeline->p_static_draw_list, &p_entity->p_geometry->_bounds);
        }

    }

    // parse Material
    if ( p_material && p_material->type == JSON_VALUE_STRING )
    {
        // load material from file
        const char *path = p_material->string;
        size_t len = load_file(path, 0, false);
        
        if ( len > 0 )
        {
            // Allocate buffer
            char *buf = default_allocator(0, len + 1);
            if ( buf )
            {
                load_file(path, buf, false);
                buf[len] = '\0'; // null terminate

                json_value *mat_json = 0;
                if ( json_value_parse(buf, 0, &mat_json) )
                {
                    material_from_json(&p_entity->p_material, mat_json);
                    
                    // ideally free mat_json here if logic existed
                }
                
                // free buffer
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

int aabb_from_entity ( aabb *p_aabb, entity *p_entity )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;
    if ( NULL == p_entity ) goto no_entity;
    if ( NULL == p_entity->p_geometry ) goto no_geometry;
    if ( NULL == p_entity->p_transform ) goto no_transform;

    // initialized data
    vec3 min = p_entity->p_geometry->_bounds._min;
    vec3 max = p_entity->p_geometry->_bounds._max;
    mat4 model = p_entity->p_transform->model;

    // corners of the aabb
    vec3 corners[8] = 
    {
        { min.x, min.y, min.z },
        { max.x, min.y, min.z },
        { min.x, max.y, min.z },
        { max.x, max.y, min.z },
        { min.x, min.y, max.z },
        { max.x, min.y, max.z },
        { min.x, max.y, max.z },
        { max.x, max.y, max.z }
    };

    // transform the corners
    vec3 new_min = {  3.402823e+38F,  3.402823e+38F,  3.402823e+38F };
    vec3 new_max = { -3.402823e+38F, -3.402823e+38F, -3.402823e+38F };

    for (size_t i = 0; i < 8; i++)
    {
        vec4 v_in = { corners[i].x, corners[i].y, corners[i].z, 1.0f };
        vec4 v_out;
        
        mat4_mul_vec4(&v_out, model, v_in);

        if ( v_out.x < new_min.x ) new_min.x = v_out.x;
        if ( v_out.y < new_min.y ) new_min.y = v_out.y;
        if ( v_out.z < new_min.z ) new_min.z = v_out.z;

        if ( v_out.x > new_max.x ) new_max.x = v_out.x;
        if ( v_out.y > new_max.y ) new_max.y = v_out.y;
        if ( v_out.z > new_max.z ) new_max.z = v_out.z;
    }

    // store the result
    p_aabb->_min = new_min;
    p_aabb->_max = new_max;

    // success
    return 1;

    no_aabb:
    no_entity:
    no_geometry:
    no_transform:
        return 0;
}

int entity_bind ( render_pass *p_render_pass, pipeline *p_pipeline, entity *p_entity )
{

    // transform
    transform_bind(p_render_pass, p_pipeline, p_entity->p_geometry->p_local_transform);
  
    // material
    if ( p_entity->p_material ) material_bind(p_render_pass, p_pipeline, p_entity->p_material);
    
    // bind the geometry
    geometry_bind(p_render_pass, p_entity->p_geometry);

    // success
    return 1;
}

int entity_draw ( render_pass *p_render_pass, pipeline *p_pipeline, entity *p_entity )
{

    // draw geometry
    if ( p_entity->p_geometry )

    // no parts -> skip
    if ( p_entity->p_geometry->_parts[0].p_handle )
    {
        for (size_t i = 0; i < 4; i++)
        {

            // no part -> skip
            if (p_entity->p_geometry->_parts[i].p_handle == NULL) continue;
            
            SDL_BindGPUIndexBuffer(
                p_render_pass->p_handle,
                &(SDL_GPUBufferBinding)
                {
                    .buffer = p_entity->p_geometry->_parts[i].p_handle,
                    .offset = 0
                },
                SDL_GPU_INDEXELEMENTSIZE_32BIT
            );

            SDL_DrawGPUIndexedPrimitives(
                p_render_pass->p_handle, 
                p_entity->p_geometry->_parts[i].index_count * 3, 
                1,
                0, 
                0, 
                0
            );
        }
    }
    else if ( p_entity->p_geometry->p_index_handle )
        SDL_DrawGPUIndexedPrimitives(p_render_pass->p_handle, p_entity->p_geometry->index_count * 3, 1, 0, 0, 0);
    else
        SDL_DrawGPUPrimitives(p_render_pass->p_handle, p_entity->p_geometry->vertex_count, 1, 0, 0);

    // success
    return 1;
}
