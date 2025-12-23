// header guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <transform.h>

// structure definitions
struct aabb_s
{
    vec3 _min;
    vec3 _max;
};

// function declarations
int aabb_from_bounds ( aabb *p_aabb, vec3 min, vec3 max );
int aabb_from_transform ( aabb *p_aabb, transform *p_transform );

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