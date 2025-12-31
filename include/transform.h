// header guard
#pragma once

// standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// gsdk
/// core
#include <core/log.h>

/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// g10
#include <gtypedef.h>
#include <linear.h>

// structure definitions
struct transform_s
{
    vec3 location;
    vec3 rotation;
    vec3 scale;

    mat4 model;

    transform *p_parent;
};

// function definitions
/// constructors
/** !
 * Construct a transform from immediate parameters
 * 
 * @param pp_transform return
 * @param location     the location of the transform
 * @param rotation     the rotation of the transform
 * @param scale        the scale of the transform
 * 
 * @return 1 on success, 0 on error
 */
int transform_construct
(
    transform **pp_transform,
    vec3        location,
    vec3        rotation,
    vec3        scale,
    transform  *p_parent
);

/** !
 * Construct a transform from a json value
 * 
 * @param pp_transform return
 * @param p_value      the json value
 * 
 * @return 1 on success, 0 on error
*/
int transform_from_json
(
    transform  **pp_transform,
    json_value  *p_value
);

/// accessors
/** !
 * Get the local 4x4 model matrix from a transform
 * 
 * @param p_transform    the transform
 * @param p_model_matrix return
 * 
 * @return 1 on success, 0 on error
 */
int transform_get_matrix_local ( 
    transform *p_transform, 
    mat4      *p_model_matrix
);

/// bind
int transform_bind ( render_pass *p_render_pass, pipeline *p_pipeline, transform *p_transform );

/// info
int transform_info ( transform *p_transform );

/** !
 * Construct a transform from an AABB
 * 
 * @param p_transform the transform
 * @param p_aabb      the AABB
 * 
 * @return 1 on success, 0 on error
 */
int transform_from_aabb ( transform *p_transform, aabb *p_aabb );

/// destructors
/** !
 * Destroy and deallocate a transform
 * 
 * @param pp_transform pointer to transform pointer
 * 
 * @return 1 on success, 0 on error
*/
int transform_destroy ( transform **pp_transform );