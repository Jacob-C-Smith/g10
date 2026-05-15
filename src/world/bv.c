// header file
#include <bv.h>
#include <aabb.h>
#include <entity.h>
#include <scene.h>
#include <float.h>

// forward declarations for BVH adapters
static int bvh_intersect_adapter ( bv *p_a, bv *p_b );

// adapter functions
static int aabb_bind_adapter ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    return aabb_bind(p_render_pass, p_pipeline, (aabb *) p_bv->p_data[0]);
}

static int aabb_draw_adapter ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    return aabb_draw(p_render_pass, p_pipeline, (aabb *) p_bv->p_data[0]);
}

static int aabb_info_adapter ( bv *p_bv )
{
    return aabb_info((aabb *) p_bv->p_data[0]);
}

static int aabb_intersect_adapter ( bv *p_a, bv *p_b )
{
    if ( !p_a || !p_b ) return 0;
    
    if ( p_b->pfn_intersect == aabb_intersect_adapter ) {
        return aabb_intersect((aabb *) p_a->p_data[0], (aabb *) p_b->p_data[0]);
    } else if ( p_b->pfn_intersect == bvh_intersect_adapter ) {
        return p_b->pfn_intersect(p_b, p_a);
    }
    return 0;
}

static int aabb_contains_adapter ( bv *p_bv, vec3 point )
{
    if ( !p_bv ) return 0;
    return aabb_contains((aabb *) p_bv->p_data[0], point);
}

static int aabb_bounds_adapter ( bv *p_bv, vec3 *p_min, vec3 *p_max )
{
    if ( !p_bv || !p_min || !p_max ) return 0;
    aabb *p_aabb = (aabb *) p_bv->p_data[0];
    *p_min = p_aabb->_min;
    *p_max = p_aabb->_max;
    return 1;
}

static int aabb_cull_adapter ( bv *p_bv, const vec4 planes[6] )
{
    if ( !p_bv ) return 0;
    return aabb_cull_frustum((aabb *) p_bv->p_data[0], planes);
}

// bvh adapter functions
static int bvh_bind_adapter ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    if ( !p_bv ) return 0;
    for ( int i = 0; i < 4; i++ ) {
        if ( p_bv->p_data[i] ) bv_bind(p_render_pass, p_pipeline, (bv *)p_bv->p_data[i]);
    }
    return 1;
}

static int bvh_draw_adapter ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    if ( !p_bv ) return 0;

    g_instance *p_instance = g_active_instance();
    if ( p_instance && p_instance->context.p_scene && p_instance->context.p_scene->p_active_camera ) {
        camera *p_camera = p_instance->context.p_scene->p_active_camera;
        if ( bv_cull(p_bv, p_camera->frustum.planes) ) return 1; // Cull entire branch
    }

    for ( int i = 0; i < 4; i++ ) {
        if ( p_bv->p_data[i] ) bv_draw(p_render_pass, p_pipeline, (bv *)p_bv->p_data[i]);
    }
    return 1;
}

static int bvh_cull_adapter ( bv *p_bv, const vec4 planes[6] )
{
    if ( !p_bv ) return 0;
    vec3 min, max;
    if ( bv_bounds(p_bv, &min, &max) ) {
        aabb temp = { ._min = min, ._max = max };
        return aabb_cull_frustum(&temp, planes);
    }
    return 0;
}

static int bvh_bounds_adapter ( bv *p_bv, vec3 *p_min, vec3 *p_max )
{
    if ( !p_bv || !p_min || !p_max ) return 0;
    
    vec3 min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
    vec3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    int has_bounds = 0;

    for ( int i = 0; i < 4; i++ ) {
        if ( p_bv->p_data[i] ) {
            vec3 child_min, child_max;
            if ( bv_bounds((bv *)p_bv->p_data[i], &child_min, &child_max) ) {
                if ( child_min.x < min.x ) min.x = child_min.x;
                if ( child_min.y < min.y ) min.y = child_min.y;
                if ( child_min.z < min.z ) min.z = child_min.z;
                if ( child_max.x > max.x ) max.x = child_max.x;
                if ( child_max.y > max.y ) max.y = child_max.y;
                if ( child_max.z > max.z ) max.z = child_max.z;
                has_bounds = 1;
            }
        }
    }

    if ( has_bounds ) {
        *p_min = min;
        *p_max = max;
        return 1;
    }
    return 0;
}

static int bvh_info_adapter ( bv *p_bv )
{
    if ( !p_bv ) return 0;
    vec3 min, max;
    if ( bv_bounds(p_bv, &min, &max) ) {
        logger_pad(); log_info("BVH Node @%p Bounds: min <%.2f, %.2f, %.2f>, max <%.2f, %.2f, %.2f>\n", p_bv, min.x, min.y, min.z, max.x, max.y, max.z);
    } else {
        logger_pad(); log_info("BVH Node @%p\n", p_bv);
    }
    logger_push();
    for ( int i = 0; i < 4; i++ ) {
        if ( p_bv->p_data[i] ) bv_info((bv *)p_bv->p_data[i]);
    }
    logger_pop();
    return 1;
}

static int bvh_intersect_adapter ( bv *p_a, bv *p_b )
{
    if ( !p_a || !p_b ) return 0;
    for ( int i = 0; i < 4; i++ ) {
        if ( p_a->p_data[i] ) {
            if ( bv_intersect((bv *)p_a->p_data[i], p_b) ) return 1;
        }
    }
    return 0;
}

static int bvh_contains_adapter ( bv *p_bv, vec3 point )
{
    if ( !p_bv ) return 0;
    for ( int i = 0; i < 4; i++ ) {
        if ( p_bv->p_data[i] ) {
            if ( bv_contains((bv *)p_bv->p_data[i], point) ) return 1;
        }
    }
    return 0;
}

// dispatcher functions
int bv_bind ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    if ( p_bv && p_bv->pfn_bind )
        return p_bv->pfn_bind(p_render_pass, p_pipeline, p_bv);
    return 0;
}

int bv_draw ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    if ( p_bv && p_bv->pfn_draw )
        return p_bv->pfn_draw(p_render_pass, p_pipeline, p_bv);
    return 0;
}

int bv_info ( bv *p_bv )
{
    if ( p_bv && p_bv->pfn_info )
        return p_bv->pfn_info(p_bv);
    return 0;
}

int bv_intersect ( bv *p_a, bv *p_b )
{
    if ( p_a && p_a->pfn_intersect )
        return p_a->pfn_intersect(p_a, p_b);
    return 0;
}

int bv_contains ( bv *p_bv, vec3 point )
{
    if ( p_bv && p_bv->pfn_contains )
        return p_bv->pfn_contains(p_bv, point);
    return 0;
}

int bv_bounds ( bv *p_bv, vec3 *p_min, vec3 *p_max )
{
    if ( p_bv && p_bv->pfn_bounds )
        return p_bv->pfn_bounds(p_bv, p_min, p_max);
    return 0;
}

int bv_cull ( bv *p_bv, const vec4 planes[6] )
{
    if ( p_bv && p_bv->pfn_cull )
        return p_bv->pfn_cull(p_bv, planes);
    return 0;
}

int bv_cull_pipeline ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    g_instance *p_instance = g_active_instance();
    if ( !p_instance || !p_instance->context.p_scene || !p_instance->context.p_scene->p_active_camera ) return 0;
    
    camera *p_camera = p_instance->context.p_scene->p_active_camera;
    return bv_cull(p_bv, p_camera->frustum.planes);
}

// constructors
int bv_from_entity ( bv **pp_bv, entity *p_entity )
{
    if ( NULL == pp_bv || NULL == p_entity ) return 0;
    
    aabb *p_aabb = default_allocator(0, sizeof(aabb));
    if ( NULL == p_aabb ) return 0;

    aabb_from_entity(p_aabb, p_entity);
    bv_from_aabb(pp_bv, p_aabb);
    if (*pp_bv) {
        (*pp_bv)->p_user_data = p_entity;
    }
    return 1;
}

int bv_from_scene ( bv **pp_bv, scene *p_scene )
{
    if ( NULL == pp_bv || NULL == p_scene ) return 0;
    
    size_t entity_count = 0;
    dict_size(p_scene->entities, &entity_count);
    
    if ( entity_count == 0 ) return 0;

    entity **entities = default_allocator(0, entity_count * sizeof(entity *));
    dict_values(p_scene->entities, (void **)entities, entity_count);

    bv **children = default_allocator(0, entity_count * sizeof(bv *));
    size_t child_count = 0;

    for ( size_t i = 0; i < entity_count; i++ )
    {
        if ( entities[i]->p_bounds )
        {
            children[child_count++] = entities[i]->p_bounds;
        }
    }

    default_allocator(entities, 0);

    if ( child_count == 0 ) {
        default_allocator(children, 0);
        return 0;
    }

    size_t current_count = child_count;
    bv **current_level = children;

    while ( current_count > 1 )
    {
        size_t next_count = (current_count + 3) / 4;
        bv **next_level = default_allocator(0, next_count * sizeof(bv *));
        
        for ( size_t i = 0; i < next_count; i++ )
        {
            bv *p_node = default_allocator(0, sizeof(bv));
            *p_node = (bv)
            {
                .p_data        = { 0, 0, 0, 0 },
                .p_parent      = 0,
                .p_user_data   = 0,
                .pfn_bind      = bvh_bind_adapter,
                .pfn_draw      = bvh_draw_adapter,
                .pfn_info      = bvh_info_adapter,
                .pfn_intersect = bvh_intersect_adapter,
                .pfn_contains  = bvh_contains_adapter,
                .pfn_bounds    = bvh_bounds_adapter,
                .pfn_cull      = bvh_cull_adapter
            };
            
            for ( size_t j = 0; j < 4; j++ )
            {
                size_t child_idx = i * 4 + j;
                if ( child_idx < current_count )
                {
                    p_node->p_data[j] = current_level[child_idx];
                    current_level[child_idx]->p_parent = p_node;
                }
            }
            next_level[i] = p_node;
        }
        
        default_allocator(current_level, 0);
        current_level = next_level;
        current_count = next_count;
    }

    *pp_bv = current_level[0];
    default_allocator(current_level, 0);

    return 1;
}

// constructor
int bv_from_aabb ( bv **pp_bv, aabb *p_aabb )
{
    if ( NULL == pp_bv ) return 0;

    bv *p_bv = default_allocator(0, sizeof(bv));
    if ( NULL == p_bv ) return 0;

    *p_bv = (bv)
    {
        .p_data        = { p_aabb, 0, 0, 0 },
        .p_parent      = 0,
        .pfn_bind      = aabb_bind_adapter,
        .pfn_draw      = aabb_draw_adapter,
        .pfn_info      = aabb_info_adapter,
        .pfn_intersect = aabb_intersect_adapter,
        .pfn_contains  = aabb_contains_adapter,
        .pfn_bounds    = aabb_bounds_adapter,
        .pfn_cull      = aabb_cull_adapter
    };

    *pp_bv = p_bv;
    return 1;
}

// function definitions
int bv_destroy ( bv **pp_bv )
{

    // argument check
    if ( pp_bv == (void *) 0 ) goto no_bv;

    // initialized data
    bv *p_bv = *pp_bv;

    // release the data
    for ( int i = 0; i < 4; i++ ) {
        if ( p_bv->p_data[i] ) {
            // Note: properly freeing the contents depends on whether it's an AABB or a child BV.
            // If it's a BVH node, we recursively call bv_destroy. If it's an AABB, we free it.
            // For now we assume if it's an intermediate node, it destroys children.
            if ( p_bv->pfn_intersect == bvh_intersect_adapter ) {
                bv *child = (bv *)p_bv->p_data[i];
                bv_destroy(&child);
            } else if ( i == 0 ) {
                default_allocator(p_bv->p_data[i], 0);
            }
        }
    }

    // release the structure
    default_allocator(p_bv, 0);

    // set to null
    *pp_bv = (void *) 0;

    // success
    return 1;

    // error handling
    {
        no_bv: return 0;
    }
}