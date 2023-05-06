/** !
 * @file G10/GXCollider.h
 * @author Jacob Smith
 * 
 * Entity colliders
 */

// Include guard
#pragma once

// Standard library
#include <stdbool.h>

// G10
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
/** !
 *  Allocate memory for a collider
 *
 * @param pp_collider : return
 *
 * @sa destroy_collider
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_collider ( GXCollider_t **pp_collider );

// Constructors
/** !
 *  Load a collider from a JSON file
 *
 * @param pp_ai : return
 * @param path  : The path to a JSON file containing a collider object
 *
 * @sa load_collider_as_json
 * @sa create_collider
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_collider ( GXCollider_t **pp_collider, const char *path );

/** !
 *  Load an AI from JSON text
 *
 * @param pp_ai      : return
 * @param text : The AI JSON object text
 *
 * @sa load_ai
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_collider_as_json ( GXCollider_t **pp_collider, char *text );

// Callbacks

DLLEXPORT int add_aabb_start_collision_callback ( GXCollider_t *collider, void* function_pointer );
DLLEXPORT int add_aabb_collision_callback ( GXCollider_t *collider, void* function_pointer );
DLLEXPORT int add_aabb_end_collision_callback ( GXCollider_t *collider, void* function_pointer );

//DLLEXPORT int add_start_collision_callback ( GXCollider_t *collider, void* function_pointer );
//DLLEXPORT int add_collision_callback ( GXCollider_t *collider, void* function_pointer );
//DLLEXPORT int add_end_collision_callback ( GXCollider_t *collider, void* function_pointer );

// Deallocators
/** !
 *  Destroy a collider
 *
 * @param p_collider : Pointer to collider pointer
 *
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_collider ( GXCollider_t **pp_collider );