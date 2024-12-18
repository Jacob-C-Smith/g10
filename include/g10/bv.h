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
    size_t bv_quantity;
    bv *_p_bvs[];
};

struct bv_s
{
    void *p_object;
    bv *p_left, *p_right;
    struct { vec3 min, max; } bounds;
    fn_bv_bounds_getter *pfn_bv_bounds_getter;
};

// Constructors
/** !
 * Construct a bounding volume hierarchy from a scene 
 * 
 * @param p_scene the scene
 *   
 * @return 1 on success, 0 on error
 */
DLLEXPORT int bvh_from_scene ( scene *p_scene );

/** !
 * Construct a bounding volume from a minimum and maximum bound 
 * 
 * @param pp_bv result
 * @param min   the minimum axis aligned bounding vector
 * @param min   the maximum axis aligned bounding vector
 * 
 * @return 1 on success, 0 on error
 */
//DLLEXPORT int bv_construct ( bv **pp_bv, vec3 min, vec3 max );

/** !
 * Construct a bounding volume from an entity 
 * 
 * @param pp_bv    result
 * @param p_entity the entity
 *   
 * @return 1 on success, 0 on error
 */
DLLEXPORT int bv_from_entity ( bv **pp_bv, entity *p_entity );

/** !
 * Construct a bounding volume from a mesh 
 * 
 * @param pp_bv  result
 * @param p_mesh the mesh
 *   
 * @return 1 on success, 0 on error
 */
DLLEXPORT int bv_from_mesh ( bv **pp_bv, mesh *p_mesh );

/** !
 * Construct a bounding volume from mesh data
 * 
 * @param pp_bv       result
 * @param p_mesh_data the mesh data
 *   
 * @return 1 on success, 0 on error
 */
DLLEXPORT int bv_from_mesh_data ( bv **pp_bv, mesh_data *p_mesh_data );

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

// Printer
/** !
 * Print info about a bounding volume
 *
 * @param p_bv the bounding volume
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int bv_info ( bv *p_bv );

DLLEXPORT int bv_draw ( shader *p_shader, bv *p_bv );

// Destructors
/** !
 *  Recursively free a bounding volume
 *
 * @param pp_bv pointer to bounding volume pointer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_bv ( bv **pp_bv );
