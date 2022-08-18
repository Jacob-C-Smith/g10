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
int     create_bv                ( GXBV_t **bv );                                       // Creates an empty bounding volume

// Constructors
int     construct_bv             ( GXBV_t   **bv, vec3       min,     vec3        max);
int     construct_bv_from_bvs    ( GXBV_t   **bv, GXBV_t     *a     , GXBV_t      *b );  // 
int     construct_bvh_from_scene ( GXBV_t   **bv, GXScene_t  *scene );                          // 

float   bv_distance               ( GXBV_t *a, GXBV_t *b );

// Tree navigation
GXBV_t *find_closest_bv          ( GXBV_t     *bvh   , GXBV_t      *bv );
GXBV_t *find_parent_bv           ( GXBV_t     *bvh   , GXBV_t      *bv );

// Tree resizing
int     resize_bv                ( GXBV_t     *bv );

// Debugging
int     print_bv                 ( FILE       *f     , GXBV_t     *bv       , size_t      d );

// Getters
size_t  get_entities_from_bv     ( GXBV_t     *bv, queue **entity_queue );

// Collision detection
bool    aabb_intersect           ( GXBV_t      *a    , GXBV_t      *b );  // 

// Destructors
int     destroy_bv               ( GXBV_t      *bv ); // Destroyes a bounding volume and all its childern
