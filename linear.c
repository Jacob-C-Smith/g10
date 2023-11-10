#include <g10/linear.h>

u0 vec2_add_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{

    // Store the sum
    *p_result = (vec2)
    {
        .x = a.x + b.x,
        .y = a.y + b.y
    };

    // Done
    return;
}

u0 vec2_sub_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{
    
    // Store the difference
    *p_result = (vec2)
    {
        .x = a.x - b.x,
        .y = a.y - b.y
    };

    // Done
    return;
}

u0 vec2_mul_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{

    // Store the product
    *p_result = (vec2)
    {
        .x = a.x * b.x,
        .y = a.y * b.y
    };

    // Done
    return;
}

u0 vec2_div_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{

    // Store the quotient
    *p_result = (vec2)
    {
        .x = a.x / b.x,
        .y = a.y / b.y
    };

    // Done
    return;
}

u0 vec3_add_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // Store the sum
    *p_result = (vec3)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };

    // Done
    return;
}

u0 vec3_sub_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // Store the difference
    *p_result = (vec3)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };

    // Done
    return;
}

u0 vec3_mul_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // Store the product
    *p_result = (vec3)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z
    };

    // Done
    return;
}

u0 vec3_div_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // Store the quotient
    *p_result = (vec3) 
    {
        .x = a.x / b.x,
        .y = a.y / b.y,
        .z = a.z / b.z
    };

    // Done
    return;
}

float length ( vec3 a )
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float dot_product_vec3 ( vec3 a, vec3 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

u0 cross_product_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // Store the cross product
    *p_result = (vec3)
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };

    // Done
    return;
}

u0 div_vec3_f ( vec3 *p_result, vec3 a, float s )
{

    // TODO: Describe
    *p_result = (vec3) 
    {
        .x = a.x / s,
        .y = a.y / s,
        .z = a.z / s
    };

    // Done
    return;
}

u0 mul_vec3_f ( vec3 *p_result, vec3 v, float s )
{
    
    // TODO: Describe
    *p_result = (vec3)
    {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };

    // Done
    return;
}

vec3 normalize ( vec3 v )
{

    // Initialized data
    float vl = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

    // TODO: Describe
    return (vec3)
    {
        .x = v.x / vl,
        .y = v.y / vl,
        .z = v.z / vl
    };
}

vec2 mul_mat2_vec2 ( mat2 m, vec2 v)
{
    return (vec2)
    {
        m.a * v.x + m.b * v.y,
        m.c * v.x + m.d * v.y
    };
}

u0 mul_mat4_vec4 ( vec4 *p_result, mat4 m, vec4 v )
{
    *p_result = (vec4)
    {
        .x = m.a * v.x + m.b * v.y + m.c * v.z + m.d * v.w,
        .y = m.e * v.x + m.f * v.y + m.g * v.z + m.h * v.w,
        .z = m.i * v.x + m.j * v.y + m.k * v.z + m.l * v.w,
        .w = m.m * v.x + m.n * v.y + m.o * v.z + m.p * v.w
    };
}

u0 mat2_mul_mat2 ( mat2 *p_result, mat2 m, mat2 n )
{

    // Store the product
    *p_result = (mat2)
    {
        .a = m.a * n.a + m.b * n.c, .b = m.a * n.b + m.b * n.d,
        .c = m.c * n.a + m.d * n.c, .d = m.c * n.b + m.d * n.d
    };

    // Done
    return;
}

u0 mat4_mul_mat4 ( mat4 *p_result, mat4 m, mat4 n )
{

    // Store the product
    *p_result = (mat4)
    {
        .a = (m.a * n.a + m.b * n.e + m.c * n.i + m.d * n.m), .b = (m.a * n.b + m.b * n.f + m.c * n.j + m.d * n.n), .c = (m.a * n.c + m.b * n.g + m.c * n.k + m.d * n.o), .d = (m.a * n.d + m.b * n.h + m.c * n.l + m.d * n.p),
        .e = (m.e * n.a + m.f * n.e + m.g * n.i + m.h * n.m), .f = (m.e * n.b + m.f * n.f + m.g * n.j + m.h * n.n), .g = (m.e * n.c + m.f * n.g + m.g * n.k + m.h * n.o), .h = (m.e * n.d + m.f * n.h + m.g * n.l + m.h * n.p),
        .i = (m.i * n.a + m.j * n.e + m.k * n.i + m.l * n.m), .j = (m.i * n.b + m.j * n.f + m.k * n.j + m.l * n.n), .k = (m.i * n.c + m.j * n.g + m.k * n.k + m.l * n.o), .l = (m.i * n.d + m.j * n.h + m.k * n.l + m.l * n.p),
        .m = (m.m * n.a + m.n * n.e + m.o * n.i + m.p * n.m), .n = (m.m * n.b + m.n * n.f + m.o * n.j + m.p * n.n), .o = (m.m * n.c + m.n * n.g + m.o * n.k + m.p * n.o), .p = (m.m * n.d + m.n * n.h + m.o * n.l + m.p * n.p)
    };

    // Done
    return;
}

u0 mat2_transpose ( mat2 *p_result, mat2 m )
{
    
    // Store the transpose
    *p_result = (mat2)
    {
        .a = m.a, .b = m.c,
        .c = m.b, .d = m.d
    };

    // Done
    return;
}

u0 mat4_transpose ( mat4 *p_result, mat4 m )
{

    // Store the transpose
    *p_result = (mat4)
    {
        .a = m.a, .b = m.e, .c = m.i, .d = m.m,
        .e = m.b, .f = m.f, .g = m.j, .h = m.n,
        .i = m.c, .j = m.g, .k = m.k, .l = m.o,
        .m = m.d, .n = m.h, .o = m.l, .p = m.p
    };

    // Done
    return;
}

u0 mat2_identity ( mat2 *p_result )
{

    // Store the identity matrix
    *p_result = (mat2)
    {
        .a = 1.f, .b = 0.f,
        .c = 0.f, .d = 1.f
    };

    // Done
    return;
}

u0 mat4_identity ( mat4 *p_result )
{

    // Store the identity matrix
    *p_result = (mat4)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f,
        .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // Done
    return;
}

u0 mat4_translation ( mat4 *p_result, vec3 v )
{

    // Store the translation matrix
    *p_result = (mat4)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f,
        .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = v.x, .n = v.y, .o = v.z, .p = 1.f
    };

    // Done
    return;
}

u0 mat4_scale ( mat4 *p_result, vec3 scale )
{

    // Store the scale matrix
    *p_result = (mat4)
    {
        .a = scale.x, .b = 0      , .c = 0      , .d = 0,
        .e = 0      , .f = scale.y, .g = 0      , .h = 0,
        .i = 0      , .j = 0      , .k = scale.z, .l = 0,
        .m = 0      , .n = 0      , .o = 0      , .p = 1
    };

    // Done
    return;
};

u0 mat4_rotation_from_vec3 ( mat4 *p_result, vec3 rotation )
{

    // Store the rotation matrix
    *p_result = (mat4)
    {
        cos(rotation.x) + pow(rotation.x, 2) * (1 - cos(rotation.x))                 , rotation.x* rotation.y* (1 - cos(rotation.y)) - rotation.z * sin(rotation.y)   , rotation.x* rotation.z* (1 - cos(rotation.z)) + rotation.y * sin(rotation.z), 0,
        rotation.y * rotation.x * (1 - cos(rotation.x)) + rotation.z * sin(rotation.x), cos(rotation.y) + pow(rotation.y, 2) * (1 - cos(rotation.y))                  , rotation.y* rotation.z* (1 - cos(rotation.z)) - rotation.x * sin(rotation.z), 0,
        rotation.y * rotation.x * (1 - cos(rotation.x)) + rotation.z * sin(rotation.x), rotation.y * rotation.x * (1 - cos(rotation.x)) + rotation.z * sin(rotation.x), cos(rotation.x) + pow(rotation.x, 2) * (1 - cos(rotation.x))                , 0,
        0                                                                             , 0                                                                             , 0                                                                           , 1
    };

    // Done
    return;
}

u0 mat4_model_from_vec3 ( mat4 *p_result, vec3 location, vec3 rotation, vec3 scale )
{

    // Initialized data
    mat4 _location,
         _rotation,
         _scale,
         _location_scale;

    // Compute the translation matrix
    mat4_translation(&_location, location);

    // Compute the rotation matrix
    mat4_rotation_from_vec3(&_rotation, location);

    // Compute the scale matrix
    mat4_scale(&_scale, scale);

    // Compute the location scale matrix
    mat4_mul_mat4(&_location_scale, _scale, _location);

    // Compute the model matrix
    mat4_mul_mat4(p_result, _location_scale, _rotation);

    // Done
    return;
}
