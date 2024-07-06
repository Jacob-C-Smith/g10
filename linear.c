/** !
 * Linear algebra for g10
 * 
 * @author Jacob Smith
 * @file linear.c
 */

// Header
#include <g10/linear.h>

// Function definitions
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

u0 vec2_mul_scalar ( vec2 *p_result, vec2 v, float s )
{

    // Store the scaled vector
    *p_result = (vec2)
    {
        .x = v.x * s,
        .y = v.y * s
    };

    // Done
    return;
}

u0 vec2_to_vec3 ( vec3 *p_result, vec2 v )
{

    // Store the vec2 as a vec3
    *p_result = (vec3)
    {
        .x = v.x,
        .y = v.y,
        .z = 0
    };

    // Done
    return;
}

u0 vec2_to_vec4 ( vec4 *p_result, vec2 v )
{

    // Store the vec2 as a vec4
    *p_result = (vec4)
    {
        .x = v.x,
        .y = v.y,
        .z = 0,
        .w = 0
    };

    // Done
    return;
}

u0 vec2_length ( float *p_result, vec2 v )
{

    // Store the length of the vector
    *p_result = sqrtf(
        v.x * v.x +
        v.y * v.y
    );

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

u0 vec3_mul_scalar ( vec3 *p_result, vec3 v, float s )
{

    // Store the scaled vector
    *p_result = (vec3)
    {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };

    // Done
    return;
}

u0 vec3_to_vec2 ( vec2 *p_result, vec3 v )
{
    
    // Store the vec3 as a vec2
    *p_result = (vec2)
    {
        .x = v.x,
        .y = v.y
    };

    // Done
    return;
}

u0 vec3_to_vec4 ( vec4 *p_result, vec3 v )
{

    // Store the vec3 as a vec4
    *p_result = (vec4)
    {
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 0
    };

    // Done
    return;
}

u0 vec3_dot_product ( float *p_result, vec3 a, vec3 b )
{

    // Store the dot product
    *p_result = (
        a.x * b.x +
        a.y * b.y +
        a.z * b.z
    );
    
    // Done
    return;
}

u0 vec3_cross_product ( vec3 *p_result, vec3 a, vec3 b )
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
 
u0 vec3_length ( float *p_result, vec3 v )
{

    // Store the length of the vector
    *p_result = sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );

    // Done
    return;
}

u0 vec3_normalize ( vec3 *p_result, vec3 v )
{

    // Uninitialized data
    float l;

    // Compute the length of the vector
    vec3_length(&l, v);

    // Store the normalized vector
    *p_result = (vec3)
    {
        .x = v.x / l,
        .y = v.y / l,
        .z = v.z / l
    };

    // Done
    return;
}

u0 vec4_add_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{
    // Store the sum
    *p_result = (vec4)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
        .w = a.w + b.w
    };

    // Done
    return;
}

u0 vec4_sub_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{

    // Store the difference
    *p_result = (vec4)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
        .w = a.w - b.w
    };

    // Done
    return;
}

u0 vec4_mul_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{

    // Store the product
    *p_result = (vec4)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z,
        .w = a.w * b.w
    };

    // Done
    return;
}

u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{

    // Store the quotient
    *p_result = (vec4)
    {
        .x = a.x / b.x,
        .y = a.y / b.y,
        .z = a.z / b.z,
        .w = a.w / b.w
    };

    // Done
    return;
}

u0 vec4_to_vec2 ( vec2 *p_result, vec4 v )
{
    // Store the vec4 as a vec2
    *p_result = (vec2)
    {
        .x = v.x,
        .y = v.y
    };

    // Done
    return;
}

u0 vec4_to_vec3 ( vec3 *p_result, vec4 v )
{
    // Store the vec4 as a vec3
    *p_result = (vec3)
    {
        .x = v.x,
        .y = v.y,
        .z = v.z
    };

    // Done
    return;
}

u0 vec4_length ( float *p_result, vec4 v )
{
    // Store the length of the vector
    *p_result = sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z +
        v.w * v.w 
    );

    // Done
    return;
}

u0 mat2_mul_vec2 ( vec2 *p_result, mat2 m, vec2 v )
{

    // Store the product
    *p_result = (vec2)
    {
        m.a * v.x + m.b * v.y,
        m.c * v.x + m.d * v.y
    };

    // Done
    return;
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

u0 mat2_to_mat3 ( mat3 *p_result, mat2 m )
{

    // Store the mat2 as a mat3
    *p_result = (mat3)
    {
        .a = m.a, .b = m.b, .c = 0,
        .d = m.c, .e = m.d, .f = 0,
        .g = 0  , .h = 0  , .i = 1
    };

    // Done
    return;
}

u0 mat2_to_mat4 ( mat4 *p_result, mat2 m )
{
    
    // Store the mat2 as a mat4
    *p_result = (mat4)
    {
        .a = m.a, .b = m.b, .c = 0.f, .d = 0.f,
        .e = m.c, .f = m.d, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // Done
    return;
}

u0 mat3_mul_vec3 ( vec3 *p_result, mat3 m, vec3 v )
{

    // Store the product
    *p_result = (vec3)
    {
        .x = m.a * v.x + m.b * v.y + m.c * v.z,
        .y = m.d * v.x + m.e * v.y + m.f * v.z,
        .z = m.g * v.x + m.h * v.y + m.i * v.z
    };

    // Done
    return;
}

u0 mat3_mul_mat3 ( mat3 *p_result, mat3 m, mat3 n )
{

    // Store the product
    *p_result = (mat3)
    {
        .a = (m.a * n.a + m.b * n.d + m.c * n.g), .b = (m.a * n.b + m.b * n.e + m.c * n.h), .c = (m.a * n.c + m.b * n.f + m.c * n.i),
        .d = (m.d * n.a + m.e * n.d + m.f * n.g), .e = (m.d * n.b + m.e * n.e + m.f * n.h), .f = (m.d * n.c + m.e * n.f + m.f * n.i),
        .g = (m.g * n.a + m.h * n.d + m.i * n.g), .h = (m.g * n.b + m.h * n.e + m.i * n.h), .i = (m.g * n.c + m.h * n.f + m.i * n.i),
    };

    // Done
    return;
}

u0 mat3_transpose ( mat3 *p_result, mat3 m )
{
    
    // Store the transpose
    *p_result = (mat3)
    {
        .a = m.a, .b = m.d, .c = m.g,
        .d = m.b, .e = m.e, .f = m.h,
        .g = m.c, .h = m.f, .i = m.i,
    };

    // Done
    return;
}

u0 mat3_identity ( mat3 *p_result )
{

    // Store the identity matrix
    *p_result = (mat3)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, 
        .d = 0.f, .e = 1.f, .f = 0.f, 
        .g = 0.f, .h = 0.f, .i = 1.f
    };

    // Done
    return;
}

u0 mat3_to_mat2 ( mat2 *p_result, mat3 m )
{

    // Store the mat2 as a mat3
    *p_result = (mat2)
    {
        .a = m.a, .b = m.b,
        .c = m.d, .d = m.e
    };

    // Done
    return;
}

u0 mat3_to_mat4 ( mat4 *p_result, mat3 m )
{
    
    // Store the mat2 as a mat4
    *p_result = (mat4)
    {
        .a = m.a, .b = m.b, .c = m.c, .d = 0.f,
        .e = m.d, .f = m.e, .g = m.f, .h = 0.f,
        .i = m.g, .j = m.h, .k = m.i, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // Done
    return;
}

u0 mat4_mul_vec4 ( vec4 *p_result, mat4 m, vec4 v )
{

    // Store the product
    *p_result = (vec4)
    {
        .x = m.a * v.x + m.b * v.y + m.c * v.z + m.d * v.w,
        .y = m.e * v.x + m.f * v.y + m.g * v.z + m.h * v.w,
        .z = m.i * v.x + m.j * v.y + m.k * v.z + m.l * v.w,
        .w = m.m * v.x + m.n * v.y + m.o * v.z + m.p * v.w
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

u0 mat4_to_mat2 ( mat2 *p_result, mat4 m )
{

    // Store the mat4 as a mat2
    *p_result = (mat2)
    {
        .a = m.a, .b = m.b,
        .c = m.e, .d = m.f
    };

    // Done
    return;
}

u0 mat4_to_mat3 ( mat3 *p_result, mat4 m )
{
    
    // Store the mat4 as a mat3
    *p_result = (mat3)
    {
        .a = m.a, .b = m.b, .c = m.c, 
        .d = m.e, .e = m.f, .f = m.g, 
        .g = m.i, .h = m.j, .i = m.k
    };

    // Done
    return;
}

u0 mat4_translation ( mat4 *p_result, vec3 location )
{

    // Store the translation matrix
    *p_result = (mat4)
    {
        .a = 1.f       , .b = 0.f       , .c = 0.f       , .d = 0.f,
        .e = 0.f       , .f = 1.f       , .g = 0.f       , .h = 0.f,
        .i = 0.f       , .j = 0.f       , .k = 1.f       , .l = 0.f,
        .m = location.x, .n = location.y, .o = location.z, .p = 1.f
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
}

u0 mat4_rotation_from_vec3 ( mat4 *p_result, vec3 rotation )
{

    // Store the rotation matrix
    *p_result = (mat4)
    {
        cosf(rotation.x) + powf(rotation.x, 2) * (1 - cosf(rotation.x))                 , rotation.x * rotation.y * (1 - cosf(rotation.y)) - rotation.z * sinf(rotation.y), rotation.x * rotation.z * (1 - cosf(rotation.z)) + rotation.y * sinf(rotation.z), 0,
        rotation.y * rotation.x * (1 - cosf(rotation.x)) + rotation.z * sinf(rotation.x), cosf(rotation.y) + powf(rotation.y, 2) * (1 - cosf(rotation.y))                 , rotation.y * rotation.z * (1 - cosf(rotation.z)) - rotation.x * sinf(rotation.z), 0,
        rotation.y * rotation.x * (1 - cosf(rotation.x)) + rotation.z * sinf(rotation.x), rotation.y * rotation.x * (1 - cosf(rotation.x)) + rotation.z * sinf(rotation.x), cosf(rotation.x) + powf(rotation.x, 2) * (1 - cosf(rotation.x))                 , 0,
        0                                                                               , 0                                                                               , 0                                                                               , 1
    };

    // Done
    return;
}

u0 mat4_model_from_vec3 ( mat4 *p_result, vec3 location, vec3 rotation, vec3 scale )
{

    // Uninitialized data
    mat4 _location,
         _rotation,
         _scale,
         _location_scale;

    // Compute the translation matrix
    mat4_translation(&_location, location);

    // Compute the rotation matrix
    mat4_rotation_from_vec3(&_rotation, rotation);

    // Compute the scale matrix
    mat4_scale(&_scale, scale);

    // Compute the location-scale matrix
    mat4_mul_mat4(&_location_scale, _scale, _location);

    // Compute the model matrix
    mat4_mul_mat4(p_result, _location_scale, _rotation);

    // Done
    return;
}
