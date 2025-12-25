// header
#include <linear.h>

// function definitions
u0 vec2_add_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{

    // store the sum
    *p_result = (vec2)
    {
        .x = a.x + b.x,
        .y = a.y + b.y
    };

    // done
    return;
}

u0 vec2_sub_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{
    
    // store the difference
    *p_result = (vec2)
    {
        .x = a.x - b.x,
        .y = a.y - b.y
    };

    // done
    return;
}

u0 vec2_mul_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{

    // store the product
    *p_result = (vec2)
    {
        .x = a.x * b.x,
        .y = a.y * b.y
    };

    // done
    return;
}

u0 vec2_div_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{

    // store the quotient
    *p_result = (vec2)
    {
        .x = a.x / b.x,
        .y = a.y / b.y
    };

    // done
    return;
}

u0 vec2_mul_scalar ( vec2 *p_result, vec2 v, float s )
{

    // store the scaled vector
    *p_result = (vec2)
    {
        .x = v.x * s,
        .y = v.y * s
    };

    // done
    return;
}

u0 vec2_to_vec3 ( vec3 *p_result, vec2 v )
{

    // store the vec2 as a vec3
    *p_result = (vec3)
    {
        .x = v.x,
        .y = v.y,
        .z = 0
    };

    // done
    return;
}

u0 vec2_to_vec4 ( vec4 *p_result, vec2 v )
{

    // store the vec2 as a vec4
    *p_result = (vec4)
    {
        .x = v.x,
        .y = v.y,
        .z = 0,
        .w = 0
    };

    // done
    return;
}

u0 vec2_length ( float *p_result, vec2 v )
{

    // store the length of the vector
    *p_result = sqrtf(
        v.x * v.x +
        v.y * v.y
    );

    // done
    return;
}

u0 vec3_add_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // store the sum
    *p_result = (vec3)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };

    // done
    return;
}

u0 vec3_sub_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // store the difference
    *p_result = (vec3)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };

    // done
    return;
}

u0 vec3_mul_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // store the product
    *p_result = (vec3)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z
    };

    // done
    return;
}

u0 vec3_div_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{

    // store the quotient
    *p_result = (vec3) 
    {
        .x = a.x / b.x,
        .y = a.y / b.y,
        .z = a.z / b.z
    };

    // done
    return;
}

u0 vec3_mul_scalar ( vec3 *p_result, vec3 v, float s )
{

    // store the scaled vector
    *p_result = (vec3)
    {
        .x = v.x * s,
        .y = v.y * s,
        .z = v.z * s
    };

    // done
    return;
}

u0 vec3_to_vec2 ( vec2 *p_result, vec3 v )
{
    
    // store the vec3 as a vec2
    *p_result = (vec2)
    {
        .x = v.x,
        .y = v.y
    };

    // done
    return;
}

u0 vec3_to_vec4 ( vec4 *p_result, vec3 v )
{

    // store the vec3 as a vec4
    *p_result = (vec4)
    {
        .x = v.x,
        .y = v.y,
        .z = v.z,
        .w = 0
    };

    // done
    return;
}

u0 vec3_dot_product ( float *p_result, vec3 a, vec3 b )
{

    // store the dot product
    *p_result = (
        a.x * b.x +
        a.y * b.y +
        a.z * b.z
    );
    
    // done
    return;
}

u0 vec3_cross_product ( vec3 *p_result, vec3 a, vec3 b )
{

    // store the cross product
    *p_result = (vec3)
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };

    // done
    return;
}
 
u0 vec3_length ( float *p_result, vec3 v )
{

    // store the length of the vector
    *p_result = sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );

    // done
    return;
}

u0 vec3_normalize ( vec3 *p_result, vec3 v )
{

    // Uninitialized data
    float l;

    // compute the length of the vector
    vec3_length(&l, v);

    // store the normalized vector
    *p_result = (vec3)
    {
        .x = v.x / l,
        .y = v.y / l,
        .z = v.z / l
    };

    // done
    return;
}

int vec3_pack ( void *p_buffer, vec3 *p_v )
{

    // done
    return pack_pack
    (
        p_buffer, 
        "%3f32", 
        
        p_v->x, 
        p_v->y, 
        p_v->z
    );
}

u0 vec4_add_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{
    // store the sum
    *p_result = (vec4)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
        .w = a.w + b.w
    };

    // done
    return;
}

u0 vec4_sub_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{

    // store the difference
    *p_result = (vec4)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
        .w = a.w - b.w
    };

    // done
    return;
}

u0 vec4_mul_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{

    // store the product
    *p_result = (vec4)
    {
        .x = a.x * b.x,
        .y = a.y * b.y,
        .z = a.z * b.z,
        .w = a.w * b.w
    };

    // done
    return;
}

u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{

    // store the quotient
    *p_result = (vec4)
    {
        .x = a.x / b.x,
        .y = a.y / b.y,
        .z = a.z / b.z,
        .w = a.w / b.w
    };

    // done
    return;
}

u0 vec4_to_vec2 ( vec2 *p_result, vec4 v )
{
    // store the vec4 as a vec2
    *p_result = (vec2)
    {
        .x = v.x,
        .y = v.y
    };

    // done
    return;
}

u0 vec4_to_vec3 ( vec3 *p_result, vec4 v )
{
    // store the vec4 as a vec3
    *p_result = (vec3)
    {
        .x = v.x,
        .y = v.y,
        .z = v.z
    };

    // done
    return;
}

u0 vec4_length ( float *p_result, vec4 v )
{
    // store the length of the vector
    *p_result = sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z +
        v.w * v.w 
    );

    // done
    return;
}

u0 mat2_mul_vec2 ( vec2 *p_result, mat2 m, vec2 v )
{

    // store the product
    *p_result = (vec2)
    {
        m.a * v.x + m.b * v.y,
        m.c * v.x + m.d * v.y
    };

    // done
    return;
}

u0 mat2_mul_mat2 ( mat2 *p_result, mat2 m, mat2 n )
{

    // store the product
    *p_result = (mat2)
    {
        .a = m.a * n.a + m.b * n.c, .b = m.a * n.b + m.b * n.d,
        .c = m.c * n.a + m.d * n.c, .d = m.c * n.b + m.d * n.d
    };

    // done
    return;
}

u0 mat2_transpose ( mat2 *p_result, mat2 m )
{
    
    // store the transpose
    *p_result = (mat2)
    {
        .a = m.a, .b = m.c,
        .c = m.b, .d = m.d
    };

    // done
    return;
}

u0 mat2_identity ( mat2 *p_result )
{

    // store the identity matrix
    *p_result = (mat2)
    {
        .a = 1.f, .b = 0.f,
        .c = 0.f, .d = 1.f
    };

    // done
    return;
}

u0 mat2_to_mat3 ( mat3 *p_result, mat2 m )
{

    // store the mat2 as a mat3
    *p_result = (mat3)
    {
        .a = m.a, .b = m.b, .c = 0,
        .d = m.c, .e = m.d, .f = 0,
        .g = 0  , .h = 0  , .i = 1
    };

    // done
    return;
}

u0 mat2_to_mat4 ( mat4 *p_result, mat2 m )
{
    
    // store the mat2 as a mat4
    *p_result = (mat4)
    {
        .a = m.a, .b = m.b, .c = 0.f, .d = 0.f,
        .e = m.c, .f = m.d, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // done
    return;
}

u0 mat3_mul_vec3 ( vec3 *p_result, mat3 m, vec3 v )
{

    // store the product
    *p_result = (vec3)
    {
        .x = m.a * v.x + m.b * v.y + m.c * v.z,
        .y = m.d * v.x + m.e * v.y + m.f * v.z,
        .z = m.g * v.x + m.h * v.y + m.i * v.z
    };

    // done
    return;
}

u0 mat3_mul_mat3 ( mat3 *p_result, mat3 m, mat3 n )
{

    // store the product
    *p_result = (mat3)
    {
        .a = (m.a * n.a + m.b * n.d + m.c * n.g), .b = (m.a * n.b + m.b * n.e + m.c * n.h), .c = (m.a * n.c + m.b * n.f + m.c * n.i),
        .d = (m.d * n.a + m.e * n.d + m.f * n.g), .e = (m.d * n.b + m.e * n.e + m.f * n.h), .f = (m.d * n.c + m.e * n.f + m.f * n.i),
        .g = (m.g * n.a + m.h * n.d + m.i * n.g), .h = (m.g * n.b + m.h * n.e + m.i * n.h), .i = (m.g * n.c + m.h * n.f + m.i * n.i),
    };

    // done
    return;
}

u0 mat3_transpose ( mat3 *p_result, mat3 m )
{
    
    // store the transpose
    *p_result = (mat3)
    {
        .a = m.a, .b = m.d, .c = m.g,
        .d = m.b, .e = m.e, .f = m.h,
        .g = m.c, .h = m.f, .i = m.i,
    };

    // done
    return;
}

u0 mat3_identity ( mat3 *p_result )
{

    // store the identity matrix
    *p_result = (mat3)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, 
        .d = 0.f, .e = 1.f, .f = 0.f, 
        .g = 0.f, .h = 0.f, .i = 1.f
    };

    // done
    return;
}

u0 mat3_to_mat2 ( mat2 *p_result, mat3 m )
{

    // store the mat2 as a mat3
    *p_result = (mat2)
    {
        .a = m.a, .b = m.b,
        .c = m.d, .d = m.e
    };

    // done
    return;
}

u0 mat3_to_mat4 ( mat4 *p_result, mat3 m )
{
    
    // store the mat2 as a mat4
    *p_result = (mat4)
    {
        .a = m.a, .b = m.b, .c = m.c, .d = 0.f,
        .e = m.d, .f = m.e, .g = m.f, .h = 0.f,
        .i = m.g, .j = m.h, .k = m.i, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // done
    return;
}

u0 mat4_mul_vec4 ( vec4 *p_result, mat4 m, vec4 v )
{

    // store the product
    *p_result = (vec4)
    {
        .x = m.a * v.x + m.b * v.y + m.c * v.z + m.d * v.w,
        .y = m.e * v.x + m.f * v.y + m.g * v.z + m.h * v.w,
        .z = m.i * v.x + m.j * v.y + m.k * v.z + m.l * v.w,
        .w = m.m * v.x + m.n * v.y + m.o * v.z + m.p * v.w
    };

    // done
    return;
}

u0 mat4_mul_mat4 ( mat4 *p_result, mat4 m, mat4 n )
{

    // store the product
    *p_result = (mat4)
    {
        .a = (m.a * n.a + m.b * n.e + m.c * n.i + m.d * n.m), .b = (m.a * n.b + m.b * n.f + m.c * n.j + m.d * n.n), .c = (m.a * n.c + m.b * n.g + m.c * n.k + m.d * n.o), .d = (m.a * n.d + m.b * n.h + m.c * n.l + m.d * n.p),
        .e = (m.e * n.a + m.f * n.e + m.g * n.i + m.h * n.m), .f = (m.e * n.b + m.f * n.f + m.g * n.j + m.h * n.n), .g = (m.e * n.c + m.f * n.g + m.g * n.k + m.h * n.o), .h = (m.e * n.d + m.f * n.h + m.g * n.l + m.h * n.p),
        .i = (m.i * n.a + m.j * n.e + m.k * n.i + m.l * n.m), .j = (m.i * n.b + m.j * n.f + m.k * n.j + m.l * n.n), .k = (m.i * n.c + m.j * n.g + m.k * n.k + m.l * n.o), .l = (m.i * n.d + m.j * n.h + m.k * n.l + m.l * n.p),
        .m = (m.m * n.a + m.n * n.e + m.o * n.i + m.p * n.m), .n = (m.m * n.b + m.n * n.f + m.o * n.j + m.p * n.n), .o = (m.m * n.c + m.n * n.g + m.o * n.k + m.p * n.o), .p = (m.m * n.d + m.n * n.h + m.o * n.l + m.p * n.p)
    };

    // done
    return;
}

u0 mat4_transpose ( mat4 *p_result, mat4 m )
{

    // store the transpose
    *p_result = (mat4)
    {
        .a = m.a, .b = m.e, .c = m.i, .d = m.m,
        .e = m.b, .f = m.f, .g = m.j, .h = m.n,
        .i = m.c, .j = m.g, .k = m.k, .l = m.o,
        .m = m.d, .n = m.h, .o = m.l, .p = m.p
    };

    // done
    return;
}

u0 mat4_identity ( mat4 *p_result )
{

    // store the identity matrix
    *p_result = (mat4)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f,
        .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // done
    return;
}

u0 mat4_to_mat2 ( mat2 *p_result, mat4 m )
{

    // store the mat4 as a mat2
    *p_result = (mat2)
    {
        .a = m.a, .b = m.b,
        .c = m.e, .d = m.f
    };

    // done
    return;
}

u0 mat4_to_mat3 ( mat3 *p_result, mat4 m )
{
    
    // store the mat4 as a mat3
    *p_result = (mat3)
    {
        .a = m.a, .b = m.b, .c = m.c, 
        .d = m.e, .e = m.f, .f = m.g, 
        .g = m.i, .h = m.j, .i = m.k
    };

    // done
    return;
}

u0 mat4_translation ( mat4 *p_result, vec3 location )
{

    // store the translation matrix
    *p_result = (mat4)
    {
        .a = 1.f       , .b = 0.f       , .c = 0.f       , .d = 0.f,
        .e = 0.f       , .f = 1.f       , .g = 0.f       , .h = 0.f,
        .i = 0.f       , .j = 0.f       , .k = 1.f       , .l = 0.f,
        .m = location.x, .n = location.y, .o = location.z, .p = 1.f
    };

    // done
    return;
}

u0 mat4_scale ( mat4 *p_result, vec3 scale )
{

    // store the scale matrix
    *p_result = (mat4)
    {
        .a = scale.x, .b = 0      , .c = 0      , .d = 0,
        .e = 0      , .f = scale.y, .g = 0      , .h = 0,
        .i = 0      , .j = 0      , .k = scale.z, .l = 0,
        .m = 0      , .n = 0      , .o = 0      , .p = 1
    };

    // done
    return;
}

u0 mat4_rotation_from_vec3 ( mat4 *p_result, vec3 rotation )
{
    float cx = cosf(rotation.x), sx = sinf(rotation.x);
    float cy = cosf(rotation.y), sy = sinf(rotation.y);
    float cz = cosf(rotation.z), sz = sinf(rotation.z);

    // Standard Euler XYZ Rotation Matrix (Row-Major Storage, Column-Vector compatible)
    // Note: If you prefer a different order (like YXZ), you can re-order the multiplications.
    *p_result = (mat4){
        .a = cy * cz, .b = -cy * sz, .c = sy, .d = 0.f,
        .e = cx * sz + sx * sy * cz, .f = cx * cz - sx * sy * sz, .g = -sx * cy, .h = 0.f,
        .i = sx * sz - cx * sy * cz, .j = sx * cz + cx * sy * sz, .k = cx * cy, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };

    // done
    return;
}

u0 mat4_model_from_vec3 ( mat4 *p_result, vec3 location, vec3 rotation, vec3 scale )
{

    // initialized data
    mat4 _location       = { 0 },
         _rotation       = { 0 },
         _scale          = { 0 },
         _rotation_scale = { 0 };

    // compute the translation matrix
    mat4_translation(&_location, location);

    // compute the rotation matrix
    mat4_rotation_from_vec3(&_rotation, rotation);

    // compute the scale matrix
    mat4_scale(&_scale, scale);

    // Standard Model Matrix Order: Translation * Rotation * Scale
    // compute the rotation-scale matrix
    mat4_mul_mat4(&_rotation_scale, _rotation, _scale);

    // compute the model matrix
    mat4_mul_mat4(p_result, _location, _rotation_scale);

    // done
    return;
}

u0 mat4_model_from_bounds ( mat4 *p_result, vec3 min, vec3 max )
{

    // initialized data
    vec3 scale = 
    {
        .x = max.x - min.x,
        .y = max.y - min.y,
        .z = max.z - min.z
    };
    vec3 location = 
    {
        .x = (max.x + min.x) / 2.0f,
        .y = (max.y + min.y) / 2.0f,
        .z = (max.z + min.z) / 2.0f
    };
    vec3 rotation = { 0.0f, 0.0f, 0.0f };

    // compute the model matrix
    mat4_model_from_vec3(p_result, location, rotation, scale);

    // done
    return;
}

int mat4_pack ( void *p_buffer, mat4 *p_m )
{

    // done
    return pack_pack
    (
        p_buffer, 
        "%16f32", 
        
        p_m->a, p_m->b, p_m->c, p_m->d,
        p_m->e, p_m->f, p_m->g, p_m->h,
        p_m->i, p_m->j, p_m->k, p_m->l,
        p_m->m, p_m->n, p_m->o, p_m->p
    );
}