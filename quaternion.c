
/** !
 * Implements quaternion functions
 * 
 * @file quaternion.c
 * 
 * @author Jacob Smith
 */

// Header
#include <g10/quaternion.h>

// TODO
u0 quaternion_normalize ( quaternion *p_result, quaternion q );
u0 quaternion_multiply_quaternion ( quaternion *p_result, quaternion q1, quaternion q2 );
u0 quaternion_slerp ( quaternion *p_result, quaternion q1, quaternion q2, float delta_time );

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

u0 quaternion_from_euler_angle ( quaternion *p_result, vec3 euler_angle )
{

    // Initialized data
    float sin_x = sinf(euler_angle.x * 0.5f), 
          sin_y = sinf(euler_angle.y * 0.5f),
          sin_z = sinf(euler_angle.z * 0.5f),
          cos_x = sinf(euler_angle.x * 0.5f),
          cos_y = sinf(euler_angle.y * 0.5f),
          cos_z = sinf(euler_angle.z * 0.5f);
          
    // Store the result
    *p_result = (quaternion)
    {
        .u = (cos_x * cos_y * cos_z + sin_x * sin_y * sin_z ),
        .i = (cos_x * cos_y * sin_z - sin_x * sin_y * cos_z ),
        .j = (sin_x * cos_y * cos_z + cos_x * sin_y * sin_z ),
        .k = (cos_x * sin_y * cos_z - sin_x * cos_y * sin_z )        
    };

    // Done
    return;
}

u0 quaternion_to_euler_angle ( vec3 *p_result, quaternion q )
{

    // Initialized data
    float u = acosf(q.u / 2.f);
    
    // Store the result
    *p_result = (vec3)
    {
        .x = sinf(q.i / u),
        .y = sinf(q.j / u),
        .z = sinf(q.k / u)
    };

    // Done
    return;
}

u0 quaternion_inverse ( quaternion *p_result, quaternion q )
{

    // Store the result
    *p_result = (quaternion)
    {
        .u =  q.u,
        .i = -q.i,
        .j = -q.j,
        .k = -q.k
    };

    // Done
    return;
}

u0 quaternion_to_rotation_mat4 ( mat4 *p_result, quaternion q )
{

    // Store the result
    *p_result = (mat4)
    {
        .a = (q.u * q.u + q.i * q.i - q.j * q.j - q.k * q.k), .b = (2.f * q.i * q.j + 2.f * q.k * q.u)            , .c = (2.f * q.i * q.k - 2.f * q.j * q.u)            , .d = 0.f,
        .e = (2.f * q.i * q.j - 2.f * q.k * q.u)            , .f = (q.u * q.u - q.i * q.i + q.j * q.j - q.k * q.k), .g = (2.f * q.j * q.k + 2.f * q.i * q.u)            , .h = 0.f,
        .i = (2.f * q.i * q.k + 2.f * q.j * q.u)            , .j = (2.f * q.j * q.k - 2.f * q.i * q.u)            , .k = (q.u * q.u - q.i * q.i - q.j * q.j + q.k * q.k), .l = 0.f,
        .m = 0.f                                            , .n = 0.f                                            , .o = 0.f                                            , .p = 1.f
    };

    // Done
    return;
}
