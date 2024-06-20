/** !
 * Header file for bounding volume heierarchy
 *
 * @file g10/bv.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <float.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

// Structure definitions
struct bvh_s
{
    bv *p_root;
    fn_bv_bounds_getter *pfn_bounds_getter;
};

struct bv_s
{
    void *p_data;
    bv *p_left, *p_right;
    struct { vec3 min, max; } bounds;
};

// Constructors
/** !
 * Construct a bounding volume from a minimum and maximum bound 
 * 
 * @param pp_bv result
 * @param min   the minimum axis aligned bounding vector
 * @param min   the maximum axis aligned bounding vector
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int bv_construct ( bv **pp_bv, vec3 min, vec3 max );

// Collision detection
/** !
 * Test if two bounding volumes are colliding
 *
 * @param p_a the first bounding volume
 * @param p_b the second bounding volume
 *
 * @return true on collision else false
 */
DLLEXPORT bool bv_aabb_intersect ( bv *p_a, bv *p_b );

// Destructors
/** !
 *  Recursively free a bounding volume
 *
 * @param pp_bv pointer to bounding volume pointer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_bv ( bv **pp_bv );
