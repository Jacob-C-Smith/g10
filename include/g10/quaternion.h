/** !
 * Quaternion
 * 
 * @file g10/quaternion.h
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

// Function declarations
/** !
 * Store an identity quaternion
 *
 * @param p_result return
 * 
 * @return void
 */
DLLEXPORT u0 quaternion_identity ( quaternion *p_result );

DLLEXPORT u0 quaternion_from_euler_angle ( quaternion *p_result, vec3 euler_angle );
DLLEXPORT u0 quaternion_to_euler_angle ( vec3 *p_result, quaternion q );
DLLEXPORT u0 quaternion_inverse ( quaternion *p_result, quaternion q );
DLLEXPORT u0 quaternion_multiply_quaternion ( quaternion *p_result, quaternion q1, quaternion q2 );
DLLEXPORT u0 quaternion_to_rotation_mat4 ( mat4 *p_result, quaternion q );
DLLEXPORT u0 quaternion_normalize ( quaternion *p_result, quaternion q );
DLLEXPORT u0 quaternion_slerp ( quaternion *p_result, quaternion q1, quaternion q2, float delta_time );