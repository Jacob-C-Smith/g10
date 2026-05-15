// header guard
#pragma once

// standard library
#include <stdio.h>
#include <math.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>

// function pointers
typedef int (fn_bv_bind)( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );
typedef int (fn_bv_draw)( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );
typedef int (fn_bv_info)( bv *p_bv );
typedef int (fn_bv_intersect)( bv *p_a, bv *p_b );
typedef int (fn_bv_contains)( bv *p_bv, vec3 point );
typedef int (fn_bv_cull)( bv *p_bv, const vec4 planes[6] );

// structure definitions
struct bv_s
{
    void         *p_data[4];
    struct bv_s  *p_parent;
    void         *p_user_data;
    fn_bv_bind   *pfn_bind;
    fn_bv_draw   *pfn_draw;
    fn_bv_info   *pfn_info;
    fn_bv_resize *pfn_resize;
    fn_bv_bounds *pfn_bounds;
    fn_bv_intersect *pfn_intersect;
    fn_bv_contains  *pfn_contains;
    fn_bv_cull      *pfn_cull;
};

#include <g10.h>

// dispatcher functions
int bv_bind ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );
int bv_draw ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );
int bv_info ( bv *p_bv );
int bv_intersect ( bv *p_a, bv *p_b );
int bv_contains ( bv *p_bv, vec3 point );
int bv_bounds ( bv *p_bv, vec3 *p_min, vec3 *p_max );
int bv_cull ( bv *p_bv, const vec4 planes[6] );
int bv_cull_pipeline ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );

/// constructors
int bv_from_scene ( bv **pp_bv, scene *p_scene );
int bv_from_entity ( bv **pp_bv, entity *p_entity );
int bv_from_aabb ( bv **pp_bv, aabb *p_aabb );

// resize
fn_bv_resize bv_entity_resize;

/// destructors
int bv_destroy ( bv **pp_bv );
