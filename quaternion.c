
// Header
#include <g10/quaternion.h>

// Function definitions
u0 quaternion_identity ( quaternion *p_result )
{

    // Store the identity quaternion
    *p_result = (quaternion)
    {
        .u = 1.f,
        .i = 0.f,
        .j = 0.f,
        .k = 0.f
    };

    // Done
    return;
}

u0 quaternion_from_euler_angle ( quaternion *p_result, vec3 euler_angle );
u0 quaternion_to_euler_angle ( vec3 *p_result, quaternion q )
{
    *p_result = (vec3)
    {
        .x = 0,
        .y = 0,
        .z = 0
    };

    // Done
    return;
}
u0 quaternion_inverse ( quaternion *p_result, quaternion q );
u0 quaternion_multiply_quaternion ( quaternion *p_result, quaternion q1, quaternion q2 );
u0 quaternion_to_rotation_mat4 ( mat4 *p_result, quaternion q );
u0 quaternion_normalize ( quaternion *p_result, quaternion q );
u0 quaternion_slerp ( quaternion *p_result, quaternion q1, quaternion q2, float delta_time );