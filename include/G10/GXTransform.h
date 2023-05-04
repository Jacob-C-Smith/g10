#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// JSON parsing
#include <dict/dict.h>
#include <json/json.h>

// G10 includes
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>
#include <G10/GXQuaternion.h>

struct GXTransform_s
{
	vec3       location;
	quaternion rotation;
	vec3       scale;

	mat4       model_matrix;
};


// Allocators
/* !
 *  Allocate memory for a transform
 *
 * @param pp_transform : return
 *
 * @sa destroy_transform
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  create_transform          ( GXTransform_t **pp_transform );

// Constructors
/* !
 *  Load a transform from the filesystem
 *
 * @param pp_transform : return
 * @param path         : The path to a JSON file containing an transform object
 *
 * @sa load_transform_as_json
 * @sa create_transform
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  load_transform            ( GXTransform_t** pp_transform, const char* path);

/* !
 *  Load a transform from JSON text
 *
 * @param pp_transform : return
 * @param token_text   : The transform JSON object text
 * @param len          : The length of the transform JSON object text
 *
 * @sa load_transform_as_json
 * @sa create_transform
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  load_transform_as_json    ( GXTransform_t **pp_transform, char *object_text, size_t len);

/* !
 *  Construct a transform from a location vector, a rotation quaternion, and a scale vector
 *
 * @param pp_transform : return
 * @param location     : Location of the transform
 * @param rotation     : Rotation of the transform
 * @param scale        : Scale of the transform
 * 
 * @sa destroy_transform
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  construct_transform       ( GXTransform_t **pp_transform, vec3 location, quaternion rotation, vec3 scale );

// Getter
/* !
 *  Compute a model matrix from the transform
 *
 * @param p_transform : Pointer to transform
 * @param r           : return
 *
 * @sa construct_transform
 *
 */
DLLEXPORT void transform_model_matrix   ( GXTransform_t   *p_transform, mat4* r);

// Deallocators
/* !
 *  Free a transform and all its contents
 *
 * @param p_transform : Pointer to transform
 *
 * @sa create_transform
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  destroy_transform         ( GXTransform_t  *p_transform );

