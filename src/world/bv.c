// header file
#include <bv.h>
#include <aabb.h>
#include <entity.h>
#include <scene.h>
#include <float.h>

// adapter functions
static int aabb_bind_adapter ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    return aabb_bind(p_render_pass, p_pipeline, (aabb *) p_bv->p_data);
}

static int aabb_draw_adapter ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv )
{
    return aabb_draw(p_render_pass, p_pipeline, (aabb *) p_bv->p_data);
}

static int aabb_info_adapter ( bv *p_bv )
{
    return aabb_info((aabb *) p_bv->p_data);
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

// constructors
int bv_from_entity ( bv **pp_bv, entity *p_entity )
{
    if ( NULL == pp_bv || NULL == p_entity ) return 0;
    
    aabb *p_aabb = default_allocator(0, sizeof(aabb));
    if ( NULL == p_aabb ) return 0;

    aabb_from_entity(p_aabb, p_entity);
    return bv_from_aabb(pp_bv, p_aabb);
}

int bv_from_scene ( bv **pp_bv, scene *p_scene )
{
    if ( NULL == pp_bv || NULL == p_scene ) return 0;
    
    size_t entity_count = 0;
    dict_size(p_scene->entities, &entity_count);
    
    if ( entity_count == 0 ) return 0;

    entity **entities = default_allocator(0, entity_count * sizeof(entity *));
    dict_values(p_scene->entities, (void **)entities, entity_count);

    vec3 min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
    vec3 max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for ( size_t i = 0; i < entity_count; i++ )
    {
        if ( entities[i]->p_bounds )
        {
            aabb *p_aabb = (aabb *) entities[i]->p_bounds->p_data;
            if ( p_aabb->_min.x < min.x ) min.x = p_aabb->_min.x;
            if ( p_aabb->_min.y < min.y ) min.y = p_aabb->_min.y;
            if ( p_aabb->_min.z < min.z ) min.z = p_aabb->_min.z;
            if ( p_aabb->_max.x > max.x ) max.x = p_aabb->_max.x;
            if ( p_aabb->_max.y > max.y ) max.y = p_aabb->_max.y;
            if ( p_aabb->_max.z > max.z ) max.z = p_aabb->_max.z;
        }
    }

    default_allocator(entities, 0);

    aabb *p_scene_aabb = default_allocator(0, sizeof(aabb));
    aabb_from_bounds(p_scene_aabb, min, max);
    return bv_from_aabb(pp_bv, p_scene_aabb);
}

// constructor
int bv_from_aabb ( bv **pp_bv, aabb *p_aabb )
{
    if ( NULL == pp_bv ) return 0;

    bv *p_bv = default_allocator(0, sizeof(bv));
    if ( NULL == p_bv ) return 0;

    *p_bv = (bv)
    {
        .p_data   = p_aabb,
        .pfn_bind = aabb_bind_adapter,
        .pfn_draw = aabb_draw_adapter,
        .pfn_info = aabb_info_adapter
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
    if ( p_bv->p_data )
        default_allocator(p_bv->p_data, 0);

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