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
DLLEXPORT int     create_bv                ( GXBV_t   **pp_bv );                                       // Creates an empty bounding volume

// Constructors
DLLEXPORT int     construct_bv             ( GXBV_t   **pp_bv, vec3       min,     vec3        max);
DLLEXPORT int     construct_bv_from_bvs    ( GXBV_t   **bv, GXBV_t     *a     , GXBV_t      *b );  // 
DLLEXPORT int     construct_bvh_from_scene ( GXBV_t   **bv, GXScene_t  *scene );                          // 

DLLEXPORT float   bv_distance               ( GXBV_t *a, GXBV_t *b );

// Tree navigation
DLLEXPORT GXBV_t *find_parent_bv           ( GXBV_t     *bvh   , GXBV_t      *bv );
DLLEXPORT int     insert_bv                ( GXBV_t     *bvh   , GXBV_t      *bv );

// Tree resizing
DLLEXPORT int     resize_bv                ( GXBV_t     *bv );

// Debugging
DLLEXPORT int     print_bv                 ( FILE       *f     , GXBV_t     *bv       , size_t      d );

// Getters
DLLEXPORT size_t  get_entities_from_bv     ( GXBV_t     *bv, queue **entity_queue );

// Collision detection
DLLEXPORT bool    aabb_intersect           ( GXBV_t      *a    , GXBV_t      *b );  // 

// Destructors
DLLEXPORT int     destroy_bv               ( GXBV_t      *bv ); // Destroyes a bounding volume and all its childern
