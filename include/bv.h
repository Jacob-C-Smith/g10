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

// structure definitions
struct bv_s
{
    void         *p_data;
    fn_bv_bind   *pfn_bind;
    fn_bv_draw   *pfn_draw;
    fn_bv_info   *pfn_info;
    fn_bv_resize *pfn_resize;
    fn_bv_bounds *pfn_bounds;
};

#include <g10.h>

// dispatcher functions
int bv_bind ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );
int bv_draw ( render_pass *p_render_pass, pipeline *p_pipeline, bv *p_bv );
int bv_info ( bv *p_bv );

/// constructors
int bv_from_scene ( bv **pp_bv, scene *p_scene );
int bv_from_entity ( bv **pp_bv, entity *p_entity );
int bv_from_aabb ( bv **pp_bv, aabb *p_aabb );

// resize
fn_bv_resize bv_entity_resize;

/// destructors
int bv_destroy ( bv **pp_bv );
