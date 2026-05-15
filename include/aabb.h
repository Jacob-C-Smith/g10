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

// structure definitions
struct aabb_s
{
    vec3 _min;
    vec3 _max;
};

#include <g10.h>

// function declarations
int aabb_bind ( render_pass *p_render_pass, pipeline *p_pipeline, aabb *p_aabb );
int aabb_draw ( render_pass *p_render_pass, pipeline *p_pipeline, aabb *p_aabb );

/// constructors
/** 
 * Construct an axis aligned bounding box from bounds
 * 
 * @param p_aabb pointer to AABB
 * @param min    the minimum bound
 * @param max    the maximum bound
 * 
 * @return 1 on success, 0 on error
 */
int aabb_from_bounds ( aabb *p_aabb, vec3 min, vec3 max );

/// print
/** 
 *  Print a textual representation of an aabb to standard output
 * 
 * @param p_aabb the aabb
 * 
 * @return 1 on success, 0 on failure
 */
int aabb_info ( aabb *p_aabb );

/// key accessor
/** 
 *  Get the name of an aabb
 * 
 * @param p_aabb the aabb
 * 
 * @return the name of the aabb
 */
const char *aabb_key_accessor ( const aabb *const p_aabb );

/// equality
/** 
 * Test the equality of two aabbs
 * 
 * @param p_a the aabb
 * 
 * @return the name of the aabb
 */
int aabb_equality ( const aabb *p_a, const aabb *p_b );

/// intersection
/**
 * Test the intersection of two aabbs
 *
 * @param p_a the first aabb
 * @param p_b the second aabb
 *
 * @return 1 if they intersect, 0 otherwise
 */
int aabb_intersect ( const aabb *p_a, const aabb *p_b );

/// contains
/**
 * Test if an aabb contains a point
 *
 * @param p_aabb the aabb
 * @param point  the point
 *
 * @return 1 if contained, 0 otherwise
 */
int aabb_contains ( const aabb *p_aabb, vec3 point );

/// cull frustum
/**
 * Test if an aabb is outside a frustum (should be culled)
 *
 * @param p_aabb the aabb
 * @param planes the 6 frustum planes
 *
 * @return 1 if outside (cull), 0 if inside/intersecting (keep)
 */
int aabb_cull_frustum ( const aabb *p_aabb, const vec4 planes[6] );