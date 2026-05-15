// header file
#include <aabb.h>

// function definitions
int aabb_from_bounds ( aabb *p_aabb, vec3 min, vec3 max )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;

    // populate the struct
    *p_aabb = (aabb)
    {
        ._min = min,
        ._max = max
    };

    // done
    return 1;
    
    no_aabb: return 0;
}

int aabb_from_transform ( aabb *p_aabb, transform *p_transform )
{

    // error check
    if ( NULL == p_aabb ) goto no_aabb;
    if ( NULL == p_transform ) goto no_transform;

    // computed data
    mat4 world_matrix = { 0 };

    // identity
    mat4_identity(&world_matrix);

    // traverse parent hierarchy
    // use a fixed stack to avoid dynamic allocation
    transform *stack[64] = { 0 };
    int top = 0;
    transform *curr = p_transform;
    
    // push transforms onto stack
    while ( curr && top < 64 ) 
    {
        stack[top++] = curr;
        curr = curr->p_parent;
    }

    // multiply matrices from root to leaf
    while ( top > 0 ) 
    {
        transform *t = stack[--top];
        mat4 temp = { 0 };

        // world = world * local
        mat4_mul_mat4(&temp, world_matrix, t->model);
        world_matrix = temp;
    }

    // define unit cube corners [-1, 1]
    vec3 corners[8] = 
    {
        { -1.0f, -1.0f, -1.0f }, { 1.0f, -1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f }, { 1.0f,  1.0f, -1.0f },
        { -1.0f, -1.0f,  1.0f }, { 1.0f, -1.0f,  1.0f },
        { -1.0f,  1.0f,  1.0f }, { 1.0f,  1.0f,  1.0f }
    };

    vec3 min_bounds = { 0 };
    vec3 max_bounds = { 0 };

    // transform first corner to initialize min/max
    {
        vec4 v4_in = { corners[0].x, corners[0].y, corners[0].z, 1.0f };
        vec4 v4_out = { 0 };
        mat4_mul_vec4(&v4_out, world_matrix, v4_in);
        min_bounds = (vec3){ v4_out.x, v4_out.y, v4_out.z };
        max_bounds = min_bounds;
    }

    // transform remaining corners
    for ( int i = 1; i < 8; ++i )
    {
        vec4 v4_in = { corners[i].x, corners[i].y, corners[i].z, 1.0f };
        vec4 v4_out = { 0 };
        mat4_mul_vec4(&v4_out, world_matrix, v4_in);
        
        if ( v4_out.x < min_bounds.x ) min_bounds.x = v4_out.x;
        if ( v4_out.y < min_bounds.y ) min_bounds.y = v4_out.y;
        if ( v4_out.z < min_bounds.z ) min_bounds.z = v4_out.z;

        if ( v4_out.x > max_bounds.x ) max_bounds.x = v4_out.x;
        if ( v4_out.y > max_bounds.y ) max_bounds.y = v4_out.y;
        if ( v4_out.z > max_bounds.z ) max_bounds.z = v4_out.z;
    }

    // populate the struct
    p_aabb->_min = min_bounds;
    p_aabb->_max = max_bounds;

    // done
    return 1;
    
    no_aabb: return 0;
    no_transform: return 0;
}

int aabb_bind ( render_pass *p_render_pass, pipeline *p_pipeline, aabb *p_aabb )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    transform _t = { 0 };

    // populate the transform
    transform_from_aabb(&_t, p_aabb);

    // transform
    transform_bind(p_render_pass, p_pipeline, &_t);
  
    // success
    return 1;
}

int aabb_draw ( render_pass *p_render_pass, pipeline *p_pipeline, aabb *p_aabb )
{

    // initialized data
    g_instance *p_instance = g_active_instance();

    SDL_DrawGPUPrimitives(p_render_pass->p_handle, 8, 1, 0, 0);
    
    // success
    return 1;
}

// function definitions
int aabb_info ( aabb *p_aabb )
{

    logger_pad(), log_info("AABB @%p\n", p_aabb);
    logger_push(),
    logger_pad(), printf("min - < %.2f, %.2f, %.2f >\n", p_aabb->_min.x, p_aabb->_min.y, p_aabb->_min.z);
    logger_pad(), printf("max - < %.2f, %.2f, %.2f >\n", p_aabb->_max.x, p_aabb->_max.y, p_aabb->_max.z);
    logger_pop();
    
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

int aabb_intersect ( const aabb *p_a, const aabb *p_b )
{
    if ( !p_a || !p_b ) return 0;
    
    if ( p_a->_max.x < p_b->_min.x || p_a->_min.x > p_b->_max.x ) return 0;
    if ( p_a->_max.y < p_b->_min.y || p_a->_min.y > p_b->_max.y ) return 0;
    if ( p_a->_max.z < p_b->_min.z || p_a->_min.z > p_b->_max.z ) return 0;

    return 1;
}

int aabb_contains ( const aabb *p_aabb, vec3 point )
{
    if ( !p_aabb ) return 0;

    if ( point.x < p_aabb->_min.x || point.x > p_aabb->_max.x ) return 0;
    if ( point.y < p_aabb->_min.y || point.y > p_aabb->_max.y ) return 0;
    if ( point.z < p_aabb->_min.z || point.z > p_aabb->_max.z ) return 0;

    return 1;
}

int aabb_cull_frustum ( const aabb *p_aabb, const vec4 planes[6] )
{
    if ( !p_aabb || !planes ) return 0;

    for ( int i = 0; i < 6; i++ )
    {
        float px = planes[i].x > 0.0f ? p_aabb->_max.x : p_aabb->_min.x;
        float py = planes[i].y > 0.0f ? p_aabb->_max.y : p_aabb->_min.y;
        float pz = planes[i].z > 0.0f ? p_aabb->_max.z : p_aabb->_min.z;

        float distance = planes[i].x * px + planes[i].y * py + planes[i].z * pz + planes[i].w;

        if ( distance < 0.0f )
            return 1; // Outside the frustum
    }

    return 0; // Inside or intersecting
}
