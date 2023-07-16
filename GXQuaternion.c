#include <G10/GXQuaternion.h>

quaternion identity_quaternion ( void )
{

    // Success
    return (quaternion)
    {
        .u = 1.f,
        .i = 0.f,
        .j = 0.f,
        .k = 0.f
    };
}

quaternion quaternion_from_euler_angle ( vec3 v )
{

    // Initialized data
    float sx = sinf(to_radians(v.x) * .5f),
          sy = sinf(to_radians(v.y) * .5f),
          sz = sinf(to_radians(v.z) * .5f),
          cx = cosf(to_radians(v.x) * .5f),
          cy = cosf(to_radians(v.y) * .5f),
          cz = cosf(to_radians(v.z) * .5f);

    // Success
    return (quaternion)
    {
        .u = (cz * cx * cy + sz * sx * sy),
        .i = (sz * cx * cy - cz * sx * sy),
        .j = (cz * sx * cy + sz * cx * sy),
        .k = (cz * cx * sy - sz * sx * cy)
    };
}

vec3 euler_angle_from_quaternion ( quaternion q )
{

    // Initialized data
    float wD = acosf(q.u / 2),
          iD = sinf(q.i / wD),
          jD = sinf(q.j / wD),
          kD = sinf(q.k / wD);

    // Success
    return (vec3)
    {
        iD,
        jD,
        kD
    };
}

quaternion quaternion_inverse ( quaternion q )
{

    // Success
    return (quaternion)
    {
        .u =  q.u,
        .i = -q.i,
        .j = -q.j,
        .k = -q.k
    };
}

quaternion multiply_quaternion_quaternion_vec4 ( quaternion q1, quaternion q2 )
{

    // Success
    return (quaternion)
    {
        .u = (-q1.i * q2.i - q1.j * q2.j - q1.k * q2.k),
        .i = ( q1.j * q2.k - q1.k * q2.j),
        .j = ( q1.k * q2.i - q1.i * q2.k),
        .k = ( q1.i * q2.j - q1.j * q2.i)
    };
}

quaternion multiply_quaternion_quaternion ( quaternion q1, quaternion q2 )
{

    // Success
    return (quaternion)
    {
        .u = (q1.u * q2.u - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k),
        .i = (q1.u * q2.i + q1.i * q2.u + q1.j * q2.k - q1.k * q2.j),
        .j = (q1.u * q2.j + q1.j * q2.u + q1.k * q2.i - q1.i * q2.k),
        .k = (q1.u * q2.k + q1.k * q2.u + q1.i * q2.j - q1.j * q2.i)
    };
}

mat4 rotation_mat4_from_quaternion ( quaternion q )
{
    /*
        (q.u * q.u + q.i * q.i - q.j * q.j - q.k * q.k), (2 * q.i * q.j - 2 * q.k * q.u)                , (2 * q.i * q.k + 2 * q.j * q.u)                , 0,
        (2 * q.i * q.j + 2 * q.k * q.u)                , (q.u * q.u - q.i * q.i + q.j * q.j - q.k * q.k), (2 * q.j * q.k - 2 * q.i * q.u)                , 0,
        (2 * q.i * q.k - 2 * q.j * q.u)                , (2 * q.j * q.k + 2 * q.i * q.u)                , (q.u * q.u - q.i * q.i - q.j * q.j + q.k * q.k), 0,
        0                                              , 0                                              , 0                                              , 1
    */
    /*
        (q.u * q.u + q.i * q.i - q.j * q.j - q.k * q.k), (2 * q.i * q.j + 2 * q.k * q.u)                , (2 * q.i * q.k - 2 * q.j * q.u)                , 0,
        (2 * q.i * q.j - 2 * q.k * q.u)                , (q.u * q.u - q.i * q.i + q.j * q.j - q.k * q.k), (2 * q.j * q.k + 2 * q.i * q.u)                , 0,
        (2 * q.i * q.k + 2 * q.j * q.u)                , (2 * q.j * q.k - 2 * q.i * q.u)                , (q.u * q.u - q.i * q.i - q.j * q.j + q.k * q.k), 0,
        0                                              , 0                                              , 0                                              , 1
    */
    return (mat4)
    {
        (q.u * q.u + q.i * q.i - q.j * q.j - q.k * q.k), (2 * q.i * q.j + 2 * q.k * q.u)                , (2 * q.i * q.k - 2 * q.j * q.u)                , 0,
        (2 * q.i * q.j - 2 * q.k * q.u)                , (q.u * q.u - q.i * q.i + q.j * q.j - q.k * q.k), (2 * q.j * q.k + 2 * q.i * q.u)                , 0,
        (2 * q.i * q.k + 2 * q.j * q.u)                , (2 * q.j * q.k - 2 * q.i * q.u)                , (q.u * q.u - q.i * q.i - q.j * q.j + q.k * q.k), 0,
        0                                              , 0                                              , 0                                              , 1
    };
}

void rotate_vec3_by_quaternion ( vec3* r, vec3 v, quaternion q )
{

    // Initialized data
    quaternion p  = (quaternion)
    { 
        .u = 0.f,
        .i = v.x,
        .j = v.y,
        .k = v.z
    };
    quaternion pP = normalize_quaternion(
        multiply_quaternion_quaternion(
            multiply_quaternion_quaternion(q, p), quaternion_inverse(q)
        )
    );

    // Return a value to the caller
    *r = (vec3)
    {
        .x = pP.i,
        .y = pP.j,
        .z = pP.k
    };

    // Success
    return;
}

quaternion normalize_quaternion ( quaternion q )
{

    // Initialized data
    float      vl  = sqrtf((q.i * q.i) + (q.j * q.j) + (q.k * q.k));
    quaternion ret =
    {
        .u = 0.f,
        .i = 0.f,
        .j = 0.f,
        .k = 0.f
    };

    // Success
    return (quaternion)
    {
        .u = 0.f,
        .i = q.i / vl,
        .j = q.j / vl,
        .k = q.k / vl
    };
}

quaternion q_slerp ( quaternion q0, quaternion q1, float delta_time )
{
    // Uninitialized data
    float sinht,
          ht,
          rA,
          rB;

    // Initialized data
    // The cosine of the half angle of the quaternions
    float cosht = q0.u * q1.u + q0.i * q1.i + q0.j * q1.j + q0.k * q1.k;

    // TODO: Argument check?
    // If the half angle is zero, do nothing
    if (fabsf(cosht) >= 1.0)
        return q0;

    // Compute the half angle and the sin of the half angle
    ht    = acosf(cosht);
    sinht = sqrtf(1.f - cosht * cosht);

    // If theta = 180, rotate around any axis
    if (fabsf(sinht) < 0.001f)
        return (quaternion)
        {
            .u = (q0.u * 0.5f + q1.u * 0.5f),
            .i = (q0.i * 0.5f + q1.i * 0.5f),
            .j = (q0.j * 0.5f + q1.j * 0.5f),
            .k = (q0.k * 0.5f + q1.k * 0.5f)
        };

    // Compute the ratios to step by
    rA = sinf((1 - delta_time) * ht) / sinht,
    rB = sinf(delta_time * ht) / sinht;

    // Success
    return (quaternion)
    {
        .u = (q0.u * rA + q1.u * rB),
        .i = (q0.i * rA + q1.i * rB),
        .j = (q0.j * rA + q1.j * rB),
        .k = (q0.k * rA + q1.k * rB)
    };
}