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
 * Adds vector a to vector b. Returns new vector
 *
 * @param r : return ( a + b )
 * @param a : a
 * @param b : b
 *
 * @sa sub_vec3
 */
DLLEXPORT u0 add_vec3 ( vec3 *r, vec3 a, vec3 b );

/** !
 * Subtracts vector a from vector b. Returns new vector
 *
 * @param r : return ( a - b )
 * @param a : a
 * @param b : b
 *
 * @sa add_vec3
 */
DLLEXPORT u0 sub_vec3 ( vec3 *r, vec3 a, vec3 b );

/** !
 * Computes the dot product of a 3D vector
 *
 * @param a : vector
 *
 * @return |a|, magnitude of a
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
DLLEXPORT float dot_product_vec3(vec3 a, vec3 b);

/** !
 * Computes the cross product of 2 3D vectors
 *
 * @param a : first vector
 * @param b : second vector
 *
 * @return a CROSS b
 */
DLLEXPORT vec3 cross_product_vec3 ( vec3 a, vec3 b );


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
DLLEXPORT u0 div_vec3_f( vec3 *r, vec3 a, float s );

/** !
 * Multiplies a vector by a scalar value
 *
 * @param v : vector
 * @param s : scalar
 *
 * @return vector multiplied by scalar
 */
DLLEXPORT vec3 mul_vec3_f ( vec3 v, float s );

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
DLLEXPORT vec4 mul_mat4_vec4 ( mat4 m, vec4 v );

/** !
 * Multiplies a matrix by a matrix
 *
 * @param m : 2x2 matrix
 * @param n : 2x2 matrix
 *
 * @return m times n
 */
DLLEXPORT mat2 mul_mat2_mat2 ( mat2 m, mat2 n );

/** !
 * Multiplies a matrix by a matrix
 *
 * @param m : 4x4 matrix
 * @param n : 4x4 matrix
 *
 * @return m times n
 */
DLLEXPORT mat4 mul_mat4_mat4 ( mat4 m, mat4 n );

/** !
 * Computes the inverse of a matrix
 *
 * @param m : 2x2 matrix
 *
 * @return inverse of m
 */
DLLEXPORT mat2 rcp_mat2 ( mat2 m );

/** !
 * Computes the inverse of a matrix
 *
 * @param m : 4x4 matrix
 *
 * @return inverse of m
 */
DLLEXPORT mat4 rcp_mat4(mat4 m);

/**
 * Returns the identity matrix
 *
 * @return 2x2 identity matrix
 */
DLLEXPORT mat2 identity_mat2();

/**
 * Returns the identity matrix
 *
 * @return 4x4 identity matrix
 */
DLLEXPORT mat4 identity_mat4 ( );

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
DLLEXPORT mat4 translation_mat4 ( vec3 v );

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
DLLEXPORT mat4 scale_mat4 ( vec3 scale );

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
DLLEXPORT mat4 rotation_mat4_from_vec3 ( vec3 rotation );

/**!
 * Computes a model matrix from a location, rotation, and scale vector.
 *
 * @param location : location
 * @param rotation : rotation
 * @param scale    : scale
 *
 * @return 4x4 model matrix
 */
DLLEXPORT mat4 model_mat4_from_vec3 ( vec3 location, vec3 rotation, vec3 scale );