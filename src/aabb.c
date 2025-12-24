#include <aabb.h>

int aabb_from_bounds ( aabb *p_aabb, vec3 min, vec3 max )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;

    // initialized data
    g_instance *p_instance = g_active_instance();
    pipeline *p_pipeline = dict_get(p_instance->cache.p_pipeline, "aabb");

    // populate the struct
    *p_aabb = (aabb)
    {
        ._min = min,
        ._max = max
    };

    if ( p_pipeline )
        array_add(p_pipeline->p_static_draw_list, p_aabb);

    // done
    return 1;
    
    no_aabb: return 0;
}

int aabb_from_transform ( aabb *p_aabb, transform *p_transform )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;

    // TODO: 
    //

    // done
    return 1;
    
    no_aabb: return 0;
}

int aabb_bind ( render_pass *p_render_pass, pipeline *p_pipeline, aabb *p_aabb )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    uniform *p_m = NULL;
    mat4 _m = { 0 };

    // get the transform uniform
    array_index(p_pipeline->p_uniforms, 1, (void **)&p_m);

    // bind model matrix
    uniform_set_pack_push(p_m, &_m, (fn_pack *)mat4_pack);

    // bind the geometry
    geometry_bind(p_render_pass, dict_get(p_instance->cache.p_geometry, "cube"));

    // success
    return 1;
}

// function definitions
int aabb_info ( aabb *p_aabb )
{

    printf("AABB @%p\n", p_aabb);
    printf(" - min : < %.2f, %.2f, %.2f >\n", p_aabb->_min.x, p_aabb->_min.y, p_aabb->_min.z);
    printf(" - max : < %.2f, %.2f, %.2f >\n", p_aabb->_max.x, p_aabb->_max.y, p_aabb->_max.z);
    
    return 1;
}

const char *aabb_key_accessor ( const aabb *const p_aabb )
{

    // done
    return 0;
}

int aabb_equality ( const aabb *p_a, const aabb *p_b )
{

    return p_a == p_b;
}
