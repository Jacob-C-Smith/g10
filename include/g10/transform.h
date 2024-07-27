/** !
 * Include header for transform.
 * 
 * @file g10/transform.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// log module
#include <log/log.h>

// array module
#include <array/array.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// g10
#include <g10/gtypedef.h>
#include <g10/linear.h>
#include <g10/quaternion.h>

// Structure definitions
struct transform_s
{
    bool dirty;

    vec3 location;
    vec3 rotation;
    vec3 scale;

    mat4 model;

    transform *p_parent;

    transform *p_childern[];
};

// Function definitions

// Constructors
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
DLLEXPORT int transform_construct (
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
DLLEXPORT int transform_from_json
(
    transform        **pp_transform,
    const json_value  *p_value
);

// Accessors
/** !
 * Get the local 4x4 model matrix from a transform
 * 
 * @param p_transform    the transform
 * @param p_model_matrix return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int transform_get_matrix_local ( 
    transform *p_transform, 
    mat4      *p_model_matrix
);

/** !
 * Get the world 4x4 model matrix from a transform by
 * applying each parent transformation
 * 
 * @param p_transform    the transform
 * @param p_model_matrix return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int transform_get_matrix_world ( 
    transform *p_transform, 
    mat4      *p_model_matrix
);

/** !
 * Print info about a transform
 * 
 * @param p_transform the transform
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int transform_info ( const transform *const p_transform );

// Destructorss
/** !
 * Destroy and deallocate a transform
 * 
 * @param pp_transform pointer to transform pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int transform_destroy ( transform **pp_transform );
