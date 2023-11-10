/** !
 * Linear algebra
 * 
 * @file g10/linear.h
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// G10
#include <g10/gtypedef.h>
#include <g10/g10.h>

// 2D vectors
/** !
 * Add vector a to vector b; Store result
 *
 * @param p_result a + b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec2_sub_vec2
 * @sa vec2_mul_vec2
 * @sa vec2_div_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_add_vec2 ( vec2 *p_result, vec2 a, vec2 b );

/** !
 * Subtract vector a from vector b; Store result
 *
 * @param p_result a - b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec2_add_vec2
 * @sa vec2_mul_vec2
 * @sa vec2_div_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_sub_vec2 ( vec2 *p_result, vec2 a, vec2 b );

/** !
 * Multiply vector a by vector b; Store result
 *
 * @param p_result a * b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec2_add_vec2
 * @sa vec2_sub_vec2
 * @sa vec2_div_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_mul_vec2 ( vec2 *p_result, vec2 a, vec2 b );

/** !
 * Divide vector a from vector b; Store result
 *
 * @param p_result a / b 
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec2_add_vec2
 * @sa vec2_sub_vec2
 * @sa vec2_mul_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_div_vec2 ( vec2 *p_result, vec2 a, vec2 b );

/** !
 * Scale a vector; Store result
 * 
 * @param p_result return
 * @param v        the vector
 * @param s        the scalar
 * 
 * @return void
*/
DLLEXPORT u0 vec2_mul_scalar ( vec2 *p_result, vec2 v, float s );

/** !
 * Promote a vec2 to a vec3; Store result
 *
 * @param p_result return
 * @param v        the vec2
 *
 * @sa vec2_to_vec4
 * 
 * @return void
 */
DLLEXPORT u0 vec2_to_vec3 ( vec3 *p_result, vec2 v );

/** !
 * Promote a vec2 to a vec4; Store result
 *
 * @param p_result return
 * @param v        the vec2
 *
 * @sa vec2_to_vec3
 * 
 * @return void
 */
DLLEXPORT u0 vec2_to_vec4 ( vec4 *p_result, vec2 v );

// 3D vectors
/** !
 * Add vector a to vector b; Store result
 *
 * @param p_result a + b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa sub_vec3
 */
DLLEXPORT u0 vec3_add_vec3 ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Subtract vector a from vector b; Store result
 *
 * @param p_result a - b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec3_add_vec3
 * @sa vec3_mul_vec3
 * @sa vec3_div_vec3
 * 
 * @return void
 */
DLLEXPORT u0 vec3_sub_vec3 ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Multiply vector a by vector b; Store result
 *
 * @param p_result a * b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec3_add_vec3
 * @sa vec3_sub_vec3
 * @sa vec3_div_vec3
 * 
 * @return void
 */
DLLEXPORT u0 vec3_mul_vec3 ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Divide vector a by vector b; Store result
 *
 * @param p_result a / b
 * @param a        vector a
 * @param b        vector b
 *
 * @sa vec3_add_vec3
 * @sa vec3_sub_vec3
 * @sa vec3_mul_vec3
 * 
 * @return void
 */
DLLEXPORT u0 vec3_div_vec3 ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Scale a vector; Store result
 * 
 * @param p_result return
 * @param v        the vector
 * @param s        the scalar
 * 
 * @return void
*/
DLLEXPORT u0 vec3_mul_scalar ( vec3 *p_result, vec3 v, float s );

/** !
 * Demote a vec3 to a vec2; Store result
 *
 * @param p_result return
 * @param v        the vec3
 *
 * @sa vec3_to_vec4
 * 
 * @return void
 */
DLLEXPORT u0 vec3_to_vec2 ( vec2 *p_result, vec3 v );

/** !
 * Promote a vec3 to a vec4; Store result
 *
 * @param p_result return
 * @param v        the vec3
 *
 * @sa vec3_to_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec3_to_vec4 ( vec4 *p_result, vec3 v );

/** !
 * Compute the dot product of a and b
 *
 * @param p_result return
 * @param a        first vector
 * @param b        second vector
 *
 * @return void
 */
DLLEXPORT u0 vec3_dot_product ( float *p_result, vec3 a, vec3 b );

/** !
 * Compute the cross product of a and b; Store result 
 *
 * @param p_result return
 * @param a        first vector
 * @param b        second vector
 *
 * @return void
 */
DLLEXPORT u0 vec3_cross_product ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Scale a vector; Store result
 *
 * @param p_result return
 * @param v        vector
 * @param s        scalar
 *
 * @return void
 */
DLLEXPORT u0 vec3_mul_scalar ( vec3 *p_result, vec3 v, float s );

/** !
 * Compute the dot product of a vec3
 *
 * @param v the vec3
 *
 * @return magnitude of a
 */
DLLEXPORT u0 vec3_length ( float *p_result, vec3 v );

/** !
 * Normailize a vector; Store result
 *
 * @param v vector
 *
 * @return void
 */
DLLEXPORT u0 vec3_normalize ( vec3 *p_result, vec3 v );

// 4D vector
// TODO: 
/*
DLLEXPORT u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b );
DLLEXPORT u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b );
DLLEXPORT u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b );
DLLEXPORT u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b );
DLLEXPORT float vec4_length ( vec4 v );

*/

// TODO: 
/*
DLLEXPORT float vec2_length ( vec2 v );
*/

// 2x2 matrix
/** !
 * Multiplies a 2x2 matrix by a vector; Store result
 *
 * |1 2| . | 1, 1 | = | 3, 7 |
 * |3 4|
 *
 * @param m 2x2 matrix
 * @param v vector
 *
 * @return vector times matrix
 */
DLLEXPORT vec2 mul_mat2_vec2 ( mat2 m, vec2 v );

/** !
 * Multiply m by n; Store result
 *
 * @param m mat2
 * @param n mat2
 *
 * @return m times n
 */
DLLEXPORT u0 mat2_mul_mat2 ( mat2 *p_result, mat2 m, mat2 n );

/** !
 * Compute the transpose of a 2x2 matrix; Store result
 *
 * @param p_result return
 * @param m        the matrix
 * 
 * @sa mat4_transpose
 *
 * @return void
 */
DLLEXPORT u0 mat2_transpose ( mat2 *p_result, mat2 m );

/**
 * Store a 2x2 identity matrix
 *
 * @param p_result return
 * 
 * @sa mat3_identity
 * @sa mat4_identity
 * 
 * @return void
 */
DLLEXPORT u0 mat2_identity ( mat2 *p_result );

/** !
 * Promote a mat2 to a mat3; Store result
 *
 * @param p_result return
 * @param v        the mat2
 *
 * @sa mat2_to_mat4
 * 
 * @return void
 */
DLLEXPORT u0 mat2_to_mat3 ( mat3 *p_result, mat2 m );

/** !
 * Promote a mat2 to a mat4; Store result
 *
 * @param p_result return
 * @param v        the mat2
 *
 * @sa mat2_to_mat3
 * 
 * @return void
 */
DLLEXPORT u0 mat2_to_mat4 ( mat4 *p_result, mat2 m );

// 3x3 matrix
/**
 * Store a 3x3 identity matrix
 *
 * @param p_result return
 * 
 * @sa mat2_identity
 * @sa mat4_identity
 * 
 * @return void
 */
DLLEXPORT u0 mat3_identity ( mat3 *p_result );

/** !
 * Demote a mat3 to a mat2; Store result
 *
 * @param p_result return
 * @param v        the mat3
 *
 * @sa mat3_to_mat4
 * 
 * @return void
 */
DLLEXPORT u0 mat3_to_mat2 ( mat2 *p_result, mat3 m );

/** !
 * Promote a mat3 to a mat4; Store result
 *
 * @param p_result return
 * @param v        the mat3
 *
 * @sa mat3_to_mat2
 * 
 * @return void
 */
DLLEXPORT u0 mat3_to_mat4 ( mat4 *p_result, mat3 m );

// 4x4 matrix
/** !
 * Multiply a matrix by a vector; Store result
 *
 * @param m mat4
 * @param v vector
 *
 * @return vector times matrix
 */
DLLEXPORT u0 mat4_mul_vec4 ( vec4 *p_result, mat4 m, vec4 v );

/** !
 * Multipy m by n; Store result
 *
 * @param m 4x4 matrix
 * @param n 4x4 matrix
 *
 * @return m times n
 */
DLLEXPORT u0 mat4_mul_mat4 ( mat4 *p_result, mat4 m, mat4 n );

/**
 * Store a 4x4 identity matrix
 *
 * @param p_result return
 * 
 * @sa mat2_identity
 * @sa mat3_identity
 * 
 * @return void
 */
DLLEXPORT u0 mat4_identity ( mat4 *p_result );

/** !
 * Compute the transpose of a 4x4 matrix; Store result
 *
 * @param p_result return
 * @param m        the matrix
 *
 * @return void
 */
DLLEXPORT u0 mat4_transpose ( mat4 *p_result, mat4 m );

/**
 * Compute a translation matrix from a location vector; Store result
 *
 * @param p_result return 
 * @param location location vector
 *
 * @sa scale_mat4
 * @sa rotation_mat4_from_vec3
 *
 * @return 4x4 translation matrix
 */
DLLEXPORT u0 mat4_translation ( mat4 *p_result, vec3 location );

/**
 * Compute a scale matrix from a scale vector; Store result
 *
 * @param p_result return 
 * @param scale    scale vector
 *
 * @sa translation_mat4
 * @sa rotation_mat4_from_vec3
 *
 * @return 4x4 scale matrix
 */
DLLEXPORT u0 mat4_scale ( mat4 *p_result, vec3 scale );

/**
 * Compute a rotation matrix from a rotation vector; Store result
 *
 * @param p_result return
 * @param rotation rotation vector
 *
 * @sa translation_mat4
 * @sa scale_mat4
 *
 * @return void
 */
DLLEXPORT u0 mat4_rotation_from_vec3 ( mat4 *p_result, vec3 rotation );

/**!
 * Compute a model matrix from a location, rotation, and scale vector; Store result
 *
 * @param p_result return
 * @param location location vector
 * @param rotation rotation vector
 * @param scale    scale vector
 *
 * @return void
 */
DLLEXPORT u0 mat4_model_from_vec3 ( mat4 *p_result, vec3 location, vec3 rotation, vec3 scale );
