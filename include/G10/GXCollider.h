#pragma once

#include <stdbool.h>

#include <G10/G10.h>
#include <G10/GXBV.h>
//#include <G10/GXCollision.h>
#include <G10/GXLinear.h>


enum collider_type_e
{
    collider_invalid    = 0,
    collider_quad       = 1,
    collider_box        = 2,
    collider_sphere     = 3,
    collider_capsule    = 4,
    collider_cylinder   = 5,
    collider_cone       = 6,
    collider_convexhull = 7
};
typedef enum collider_type_e collider_type_t;

struct GXCollider_s
{

    collider_type_t  type;

    // AABB dimensions
    vec3            aabb_max,
                    aabb_min;

    // Pointer to model matrix
    mat4           *model_matrix;

    // List of points on convex hull
    vec3           *convex_hull;
    size_t          convex_hull_count;

    // Pointer to bounding volume in the scene BVH
    GXBV_t         *bv;

    // Callback data
    size_t          aabb_start_callback_count,
                    aabb_callback_count,
                    aabb_end_callback_count,

                    aabb_start_callback_max,
                    aabb_callback_max,
                    aabb_end_callback_max,
        
                    obb_start_callback_count,
                    obb_callback_count,
                    obb_end_callback_count,

                    obb_start_callback_max,
                    obb_callback_max,
                    obb_end_callback_max,
                    
                    shape_start_callback_count,
                    shape_callback_count,
                    shape_end_callback_count,

                    shape_start_callback_max,
                    shape_callback_max,
                    shape_end_callback_max;

    void          **aabb_start_callbacks,
                  **aabb_callbacks,
                  **aabb_end_callbacks,

                  **obb_start_callbacks,
                  **obb_callbacks,
                  **obb_end_callbacks,
        
                  **shape_start_callbacks,
                  **shape_callbacks,
                  **shape_end_callbacks;

    dict           *collisions;
};

// Allocators
DLLEXPORT int create_collider              ( GXCollider_t **collider );                     // Creates an empty collider

// Constructors
DLLEXPORT int load_collider                ( GXCollider_t **collider, const char   *path );       // Creates and loads a collider from a path and populates it
DLLEXPORT int load_collider_as_json        ( GXCollider_t **collider, char         *token_text , size_t len );      // Creates a collider from JSON text

// Callbacks
DLLEXPORT int add_aabb_start_collision_callback ( GXCollider_t *collider, void* function_pointer );
DLLEXPORT int add_aabb_collision_callback       ( GXCollider_t *collider, void* function_pointer );
DLLEXPORT int add_aabb_end_collision_callback   ( GXCollider_t *collider, void* function_pointer );

//DLLEXPORT int add_start_collision_callback ( GXCollider_t *collider, void* function_pointer );
//DLLEXPORT int add_collision_callback       ( GXCollider_t *collider, void* function_pointer );
//DLLEXPORT int add_end_collision_callback   ( GXCollider_t *collider, void* function_pointer );

// Destructors
DLLEXPORT int destroy_collider             ( GXCollider_t *collider );   // Destroys a collider