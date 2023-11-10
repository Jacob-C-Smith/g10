/** !
 * Linear algebra operations
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

/** !
 * Add vector a to vector b; Store result
 *
 * @param p_result : a + b
 * @param a        : vector a
 * @param b        : vector b
 *
 * @sa vec2_sub_vec2
 * @sa vec2_mul_vec2
 * @sa vec2_div_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_add_vec2 ( vec2 *r, vec2 a, vec2 b );

/** !
 * Subtract vector a from vector b; Store result
 *
 * @param p_result : a - b
 * @param a        : vector a
 * @param b        : vector b
 *
 * @sa vec2_add_vec2
 * @sa vec2_mul_vec2
 * @sa vec2_div_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_sub_vec2 ( vec2 *r, vec2 a, vec2 b );

/** !
 * Multiply vector a by vector b; Store result
 *
 * @param p_result : a * b
 * @param a        : vector a
 * @param b        : vector b
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
 * @param p_result : a / b 
 * @param a        : vector a
 * @param b        : vector b
 *
 * @sa vec2_add_vec2
 * @sa vec2_sub_vec2
 * @sa vec2_mul_vec2
 * 
 * @return void
 */
DLLEXPORT u0 vec2_div_vec2 ( vec2 *p_result, vec2 a, vec2 b );

/** !
 * Add vector a to vector b; Store result
 *
 * @param p_result : a + b
 * @param a        : vector a
 * @param b        : vector b
 *
 * @sa sub_vec3
 */
DLLEXPORT u0 vec3_add_vec3 ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Subtract vector a from vector b; Store result
 *
 * @param p_result : a - b
 * @param a        : vector a
 * @param b        : vector b
 *
 * @sa vec3_add_vec3
 * @sa vec3_mul_vec3
 * @sa vec3_div_vec3
 * 
 * @return void
 */
DLLEXPORT u0 vec3_sub_vec3 ( vec3 *r, vec3 a, vec3 b );

/** !
 * Multiply vector a by vector b; Store result
 *
 * @param p_result : a * b
 * @param a        : vector a
 * @param b        : vector b
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
 * @param p_result : a / b
 * @param a        : vector a
 * @param b        : vector b
 *
 * @sa vec3_add_vec3
 * @sa vec3_sub_vec3
 * @sa vec3_mul_vec3
 * 
 * @return void
 */
DLLEXPORT u0 vec3_div_vec3 ( vec3 *p_result, vec3 a, vec3 b );

/** !
 * Compute the dot product of a vec3
 *
 * @param a : the vec3
 *
 * @return magnitude of a
 */
DLLEXPORT float length ( vec3 a );

/** !
 * Computes the dot product of two 3D vectors
 *
 * @param a : first vector
 * @param b : second vector
 *
 * @return a DOT b
 */
DLLEXPORT float dot_product_vec3 ( vec3 a, vec3 b );

/** !
 * Computes the cross product of 2 3D vectors
 *
 * @param a : first vector
 * @param b : second vector
 *
 * @return a CROSS b
 */
DLLEXPORT u0 vec3_cross_product ( vec3 a, vec3 b );

/** !
 * Multiplies the components of one vector by the components of another, writes to return
 *
 * @param r : return
 * @param a : first vector
 * @param b : second vector
 *
 */
DLLEXPORT u0 mul_vec3_vec3 ( vec3 *r, vec3 a, vec3 b );

/** !
 * Divides the components of a vector by a scalar
 *
 * @param r : return
 * @param a : vector
 * @param s : scalar
 *
 */
DLLEXPORT u0 div_vec3_f( vec3 *p_result, vec3 a, float s );

/** !
 * Multiplies a vector by a scalar value
 *
 * @param v : vector
 * @param s : scalar
 *
 * @return vector multiplied by scalar
 */
DLLEXPORT u0 mul_vec3_f ( vec3 *p_result, vec3 v, float s );

/** !
 * Normailizes a vector
 *
 * @param v : vector
 *
 * @return normalized vector
 */
DLLEXPORT vec3 normalize ( vec3 v );

/** !
 * Multiplies a 2x2 matrix by a vector
 *
 * |1 2| . | 1, 1 | = | 3, 7 |
 * |3 4|
 *
 * @param m : 2x2 matrix
 * @param v : vector
 *
 * @return vector times matrix
 */
DLLEXPORT vec2 mul_mat2_vec2 ( mat2 m, vec2 v );

/** !
 * Multiplies a matrix by a vector
 *
 * @param m : 4x4 matrix
 * @param v : vector
 *
 * @return vector times matrix
 */
DLLEXPORT u0 mul_mat4_vec4 ( vec4 *p_result, mat4 m, vec4 v );

/** !
 * Multiplies a matrix by a matrix
 *
 * @param m : 2x2 matrix
 * @param n : 2x2 matrix
 *
 * @return m times n
 */
DLLEXPORT u0 mat2_mul_mat2 ( mat2 *p_result, mat2 m, mat2 n );

/** !
 * Multiplies a matrix by a matrix
 *
 * @param m : 4x4 matrix
 * @param n : 4x4 matrix
 *
 * @return m times n
 */
DLLEXPORT u0 mat4_mul_mat4 ( mat4 *p_result, mat4 m, mat4 n );

/** !
 * Computes the inverse of a matrix
 *
 * @param m : 2x2 matrix
 *
 * @return inverse of m
 */
DLLEXPORT u0 mat2_transpose ( mat2 *p_result, mat2 m );

/** !
 * Computes the inverse of a matrix
 *
 * @param m : 4x4 matrix
 *
 * @return inverse of m
 */
DLLEXPORT u0 mat4_transpose ( mat4 *p_result, mat4 m );

/**
 * Returns the identity matrix
 *
 * @return 2x2 identity matrix
 */
DLLEXPORT u0 mat2_identity ( mat2 *p_result );

/**
 * Returns the identity matrix
 *
 * @return 4x4 identity matrix
 */
DLLEXPORT u0 mat4_identity ( mat4 *p_result );

/**
 * Computes a translation matrix from a translation vector
 *
 * @param v : translation vector
 *
 * @sa scale_mat4
 * @sa rotation_mat4_from_vec3
 *
 * @return 4x4 translation matrix
 */
DLLEXPORT u0 mat4_translation ( mat4 *p_result, vec3 v );

/**
 * Computes a scale matrix from a scale vector
 *
 * @param scale : scale vector
 *
 * @sa translation_mat4
 * @sa rotation_mat4_from_vec3
 *
 * @return 4x4 scale matrix
 */
DLLEXPORT u0 mat4_scale ( mat4 *p_result, vec3 scale );

/**
 * Computes a rotation matrix from a rotation vector
 *
 * @param rotation : rotation vector
 *
 * @sa translation_mat4
 * @sa scale_mat4
 *
 * @return 4x4 rotation matrix
 */
DLLEXPORT u0 mat4_rotation_from_vec3 ( mat4 *p_result, vec3 rotation );

/**!
 * Computes a model matrix from a location, rotation, and scale vector.
 *
 * @param location : location
 * @param rotation : rotation
 * @param scale    : scale
 *
 * @return 4x4 model matrix
 */
DLLEXPORT u0 mat4_model_from_vec3 ( mat4 *p_result, vec3 location, vec3 rotation, vec3 scale );
