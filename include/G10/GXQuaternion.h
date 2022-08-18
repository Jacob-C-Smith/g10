#pragma once

#include <stdio.h>
#include <math.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

DLLEXPORT quaternion    identity_quaternion                 ( void );                      // ✅ Creates quaternion (1,0,0,0)

DLLEXPORT quaternion    quaternion_from_euler_angle         ( vec3       v );// ✅ Creates a quaternion from a vector in euler angles

DLLEXPORT vec3          euler_angle_from_quaternion         ( quaternion q );
DLLEXPORT quaternion    quaternion_inverse                  ( quaternion q );
DLLEXPORT quaternion    multiply_quaternion_quaternion_vec4 ( quaternion q1, quaternion q2 ); // ✅ Multiplies two quaternions as vectors

DLLEXPORT quaternion    multiply_quaternion_quaternion      ( quaternion q1, quaternion q2 );       // ✅ Multiplies two quaternions

DLLEXPORT mat4          rotation_mat4_from_quaternion       ( quaternion q );                       // ✅ Creates a rotation matrix from a quaternion
DLLEXPORT void          rotate_vec3_by_quaternion           ( vec3* r, vec3       v, quaternion q );

DLLEXPORT quaternion    normalize_quaternion                ( quaternion q );
DLLEXPORT quaternion    q_slerp                             ( quaternion q0, quaternion q1, float     delta_time );