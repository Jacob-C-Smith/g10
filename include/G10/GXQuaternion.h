/** !
 * @file G10/GXQuaternion.h
 * @author Jacob Smith
 * 
 * Quaternions
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

/** !
 *  Returns identity quaternion
 *  
 *  @return 1 + 0i + 0j + 0k
 */
DLLEXPORT quaternion    identity_quaternion                 ( void );

/** !
 * Returns a given euler angle's quaternion
 * 
 * @param v : euler angle
 * 
 * @return a quaternion, corresponding to the euler angle
 */
DLLEXPORT quaternion quaternion_from_euler_angle ( vec3 v );// ✅ Creates a quaternion from a vector in euler angles

/** !
 * Returns a given quaternion's euler angle
 * 
 * @param q : quaternion
 * 
 * @return a euler angle, corresponding to the quaternion
 */
DLLEXPORT vec3 euler_angle_from_quaternion ( quaternion q );

/** !
 * Returns < u, -i, -j, -k > from quaternion
 * 
 * @param q : quaternion
 * 
 * @return q with i, j, and k elements multiplied by one
 */
DLLEXPORT quaternion quaternion_inverse ( quaternion q );

/** !
 * Multiplies two quaternions as vectors
 * 
 * @param q1 : quaternion 1
 * @param q2 : quaternion 2
 * 
 * @return TODO
 */
DLLEXPORT quaternion multiply_quaternion_quaternion_vec4 ( quaternion q1, quaternion q2 ); // ✅ Multiplies two quaternions as vectors

/** !
 * Multiplies two quaternions
 * 
 * @param q1 : quaternion 1
 * @param q2 : quaternion 2
 * 
 * @return TODO
 */
DLLEXPORT quaternion multiply_quaternion_quaternion ( quaternion q1, quaternion q2 );       // ✅ 

/** !
 * Creates a rotation matrix from a quaternion
 * 
 * @param q : quaternion
 * 
 * @return mat4
*/
DLLEXPORT mat4 rotation_mat4_from_quaternion ( quaternion q );                       // ✅

// TODO
DLLEXPORT void rotate_vec3_by_quaternion ( vec3* r, vec3 v, quaternion q );

/** !
 * Normalize i, j, and k
 * 
 * @param q : quaternion
 * 
 * @return |q|
*/
DLLEXPORT quaternion    normalize_quaternion                ( quaternion q );

/** !
 * Spherical liner interpolation between two quaternions over some time
 * 
 * @param q1         : quaternion 1
 * @param q2         : quaternion 2
 * @param delta_time : delta time
 * 
 * @return The quaternion between q1 and q2 at a given time
*/
DLLEXPORT quaternion q_slerp ( quaternion q1, quaternion q2, float delta_time );