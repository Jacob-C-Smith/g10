#pragma once

#include <float.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXBV.h>

struct GXBV_s {
    vec3        maximum,
                minimum;
    GXEntity_t *entity;
    GXBV_t     *left,
               *right;
};

// Allocators

/* !
 *  Allocate memory for a bounding volume
 *
 * @param pp_bv : return
 *
 * @sa destroy_bv
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int     create_bv                ( GXBV_t   **pp_bv );

// Constructors

/* !
 *  Construct a bounding volume from a minimum and a maximum
 *
 * @param pp_bv : return
 * @param min   : The minimum axis aligned bounding vector
 * @param max   : The maximum axis aligned bounding vector
 * 
 * @sa construct_bv_from_bvs
 * @sa construct_bvh_from_scene
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int     construct_bv             ( GXBV_t   **pp_bv, vec3       min,     vec3        max);

/* !
 *  Construct a bounding volume from two bounding volumes
 *
 * @param pp_bv : return
 * @param min   : The minimum axis aligned bounding vector
 * @param max   : The maximum axis aligned bounding vector
 *
 * @sa construct_bv
 * @sa construct_bvh_from_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int     construct_bv_from_bvs    ( GXBV_t   **pp_bv, GXBV_t     *a     , GXBV_t      *b );

/* !
 *  Construct a bounding volume heierarchy from a scene
 *
 * @param pp_bv : return
 * @param scene : The scene
 *
 * @sa construct_bv
 * @sa construct_bv_from_bvs
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int     construct_bvh_from_scene ( GXBV_t   **bv, GXScene_t  *scene );

/* !
 *  Compute the distance between two bounding volumes
 *
 * @param a : The first bounding volume
 * @param b : The second bounding volume
 *
 * @sa aabb_intersect
 *
 * @return distance on success, 0 on error
 */
DLLEXPORT float   bv_distance               ( GXBV_t *a, GXBV_t *b );

// Tree navigation
DLLEXPORT GXBV_t *find_parent_bv           ( GXBV_t     *bvh   , GXBV_t      *bv );
DLLEXPORT int     insert_bv                ( GXBV_t     *bvh   , GXBV_t      *bv );

// Tree resizing
DLLEXPORT int     resize_bv                ( GXBV_t     *bv );

// Logging
/* !
 *  Print info about a bounding volume
 *
 * @param p_ai  : pointer to ai to log
 *
 * @sa load_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int     bv_info                  ( GXBV_t     *p_bv       , size_t      d );

// Getters
DLLEXPORT size_t  get_entities_from_bv     ( GXBV_t     *bv, queue **entity_queue );

// Collision detection
/* !
 *  Compute if two bounding volumes are colliding
 *
 * @param a : The first bounding volume
 * @param b : The second bounding volume
 *
 * @sa bv_distance
 *
 * @return true on collision, false otherwise
 */
DLLEXPORT bool    aabb_intersect           ( GXBV_t      *a    , GXBV_t      *b );

// Destructors
DLLEXPORT int     destroy_bv               ( GXBV_t      *bv ); // Destroyes a bounding volume and all its childern
