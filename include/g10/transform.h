/** !
 * Include header for transform.
 * 
 * @file g10/transform.h
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

// Structure definitions
struct transform_s
{
    vec3 location;
    vec3 rotation;
    vec3 scale;

    mat4 model;
};

// Function definitions

// Allocators
/** !
 * Allocate memory for a transfrom
 * 
 * @param pp_transform return
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int transform_create ( transform **pp_transform );

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
    vec3        scale
);

// Accessors
/** !
 * Get the 4x4 model matrix from a transform
 * 
 * @param p_transform    the transform
 * @param p_model_matrix the model matrix
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int transform_get_matrix ( 
    transform *p_transform, 
    mat4 *p_model_matrix
);

// 
/** !
 * Destroy and deallocate a transform
 * 
 * @param pp_transform pointer to transform pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int transform_destroy ( transform **pp_transform );
