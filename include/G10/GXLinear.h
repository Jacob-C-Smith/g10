#pragma once

// TODO: Phase out all of these functions in favor of SIMD accelerated funcitons.

#include <G10/GXtypedef.h>
#include <G10/G10.h>

// Adds vector a to vector b. Returns new vector
DLLEXPORT void add_vec3 ( vec3 *r, vec3 a, vec3 b );

// Subtracts vector a from vector b. Returns new vector
DLLEXPORT void sub_vec3 ( vec3 *r, vec3 a, vec3 b );

// Computes the dot product of 2 3D vectors
DLLEXPORT float length ( vec3 a );

DLLEXPORT float dot_product_vec3(vec3 a, vec3 b);

// ✅ Computes the cross product of 2 3D vectors
DLLEXPORT vec3 cross_product_vec3 ( vec3 a, vec3 b );

// ✅ Multiplies the components of one vector by the components of another
DLLEXPORT void mul_vec3_vec3 ( vec3 *r, vec3 a, vec3 b );

// ✅ Divides the components of a vector by a scalar
DLLEXPORT void div_vec3_f( vec3 *r, vec3 a, float s );

// ✅ Multiplies a vector by a scalar value
DLLEXPORT vec3 mul_vec3_f ( vec3 v, float s );

// ✅ Normailizes a vector
DLLEXPORT vec3 normalize ( vec3 v );

// ✅ Multiplies a matrix by a vector
DLLEXPORT vec2 mul_mat2_vec2 ( mat2 m, vec2 v );

// ✅ Multiplies a matrix by a vector
DLLEXPORT vec4 mul_mat4_vec4 ( mat4 m, vec4 v );

// ✅ Multiplies a matrix by a matrix
DLLEXPORT mat2 mul_mat2_mat2 ( mat2 m, mat2 n );

// ✅ Multiplies a matrix by a matrix
DLLEXPORT mat4 mul_mat4_mat4 ( mat4 m, mat4 n );

// ✅ Computes the inverse of a matrix
DLLEXPORT mat2 rcp_mat2(mat2 m);

// ✅ Computes the inverse of a matrix
DLLEXPORT mat4 rcp_mat4(mat4 m);

// ✅ Returns the identity matrix
DLLEXPORT mat2 identity_mat2();

// ✅ Returns the identity matrix
DLLEXPORT mat4 identity_mat4 ( );

// ✅ Computes a translation matrix from a translation vector
DLLEXPORT mat4 translation_mat4 ( vec3 v );

DLLEXPORT mat4 scale_mat4 ( vec3 scale );

// ✅ Computes a rotation matrix from a rotation vector
DLLEXPORT mat4 rotation_mat4_from_vec3 ( vec3 rotation );

// ✅ Computes a model matrix from a location, rotation, and scale vector.
DLLEXPORT mat4 model_mat4_from_vec3 ( vec3 location, vec3 rotation, vec3 scale );
