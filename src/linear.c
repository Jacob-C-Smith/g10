// header
#include <linear.h>

#define DEG_TO_RAD ((double)M_PI/(double)180.0)

// function definitions
u0 vec2_add_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{
    *p_result = (vec2){ a.x + b.x, a.y + b.y };
}

u0 vec2_sub_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{
    *p_result = (vec2){ a.x - b.x, a.y - b.y };
}

u0 vec2_mul_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{
    *p_result = (vec2){ a.x * b.x, a.y * b.y };
}

u0 vec2_div_vec2 ( vec2 *p_result, vec2 a, vec2 b )
{
    *p_result = (vec2){ a.x / b.x, a.y / b.y };
}

u0 vec2_mul_scalar ( vec2 *p_result, vec2 v, float s )
{
    *p_result = (vec2){ v.x * s, v.y * s };
}

u0 vec2_to_vec3 ( vec3 *p_result, vec2 v )
{
    *p_result = (vec3){ v.x, v.y, 0 };
}

u0 vec2_to_vec4 ( vec4 *p_result, vec2 v )
{
    *p_result = (vec4){ v.x, v.y, 0, 0 };
}

u0 vec2_length ( float *p_result, vec2 v )
{
    *p_result = sqrtf(v.x * v.x + v.y * v.y);
}

u0 vec3_add_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3){ a.x + b.x, a.y + b.y, a.z + b.z };
}

u0 vec3_sub_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3){ a.x - b.x, a.y - b.y, a.z - b.z };
}

u0 vec3_mul_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3){ a.x * b.x, a.y * b.y, a.z * b.z };
}

u0 vec3_div_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3){ a.x / b.x, a.y / b.y, a.z / b.z };
}

u0 vec3_mul_scalar ( vec3 *p_result, vec3 v, float s )
{
    *p_result = (vec3){ v.x * s, v.y * s, v.z * s };
}

u0 vec3_to_vec2 ( vec2 *p_result, vec3 v )
{
    *p_result = (vec2){ v.x, v.y };
}

u0 vec3_to_vec4 ( vec4 *p_result, vec3 v )
{
    *p_result = (vec4){ v.x, v.y, v.z, 0 };
}

u0 vec3_dot_product ( float *p_result, vec3 a, vec3 b )
{
    *p_result = a.x * b.x + a.y * b.y + a.z * b.z;
}

u0 vec3_cross_product ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
 
u0 vec3_length ( float *p_result, vec3 v )
{
    *p_result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

u0 vec3_normalize ( vec3 *p_result, vec3 v )
{
    float l;
    vec3_length(&l, v);
    if (l > 0.0f) {
        float inv_l = 1.0f / l;
        *p_result = (vec3){ v.x * inv_l, v.y * inv_l, v.z * inv_l };
    } else {
        *p_result = (vec3){ 0, 0, 0 };
    }
}

int vec3_pack ( void *p_buffer, vec3 *p_v )
{
    return pack_pack(p_buffer, "%3f32", p_v->x, p_v->y, p_v->z);
}

u0 vec4_add_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{
    *p_result = (vec4){ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

u0 vec4_sub_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{
    *p_result = (vec4){ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

u0 vec4_mul_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{
    *p_result = (vec4){ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w };
}

u0 vec4_div_vec4 ( vec4 *p_result, vec4 a, vec4 b )
{
    *p_result = (vec4){ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w };
}

u0 vec4_to_vec2 ( vec2 *p_result, vec4 v )
{
    *p_result = (vec2){ v.x, v.y };
}

u0 vec4_to_vec3 ( vec3 *p_result, vec4 v )
{
    *p_result = (vec3){ v.x, v.y, v.z };
}

u0 vec4_length ( float *p_result, vec4 v )
{
    *p_result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

// Matrix operations - COLUMN MAJOR STORAGE
// Structure layout:
// a e i m (Col 0)
// b f j n (Col 1)
// c g k o (Col 2)
// d h l p (Col 3)

u0 mat2_mul_vec2 ( vec2 *p_result, mat2 m, vec2 v )
{
    // Col-Major: x' = a*x + c*y, y' = b*x + d*y
    // Struct: a, b (Col 0); c, d (Col 1)
    // Actually struct is usually linear a,b,c,d.
    // If a,b is Col 0 and c,d is Col 1:
    // x' = m.a * v.x + m.c * v.y
    // y' = m.b * v.x + m.d * v.y
    *p_result = (vec2){
        m.a * v.x + m.c * v.y,
        m.b * v.x + m.d * v.y
    };
}

u0 mat2_mul_mat2 ( mat2 *p_result, mat2 m, mat2 n )
{
    // Result = M * N
    // Col 0 = M * N_col0
    // N_col0 = (n.a, n.b)
    // res.a (0,0) = Row0_M . Col0_N = (m.a, m.c) . (n.a, n.b) = m.a*n.a + m.c*n.b
    // res.b (1,0) = Row1_M . Col0_N = (m.b, m.d) . (n.a, n.b) = m.b*n.a + m.d*n.b
    
    *p_result = (mat2){
        .a = m.a * n.a + m.c * n.b,
        .b = m.b * n.a + m.d * n.b,
        
        .c = m.a * n.c + m.c * n.d,
        .d = m.b * n.c + m.d * n.d
    };
}

u0 mat2_transpose ( mat2 *p_result, mat2 m )
{
    *p_result = (mat2){
        .a = m.a, .b = m.c,
        .c = m.b, .d = m.d
    };
}

u0 mat2_identity ( mat2 *p_result )
{
    *p_result = (mat2){
        .a = 1.f, .b = 0.f,
        .c = 0.f, .d = 1.f
    };
}

u0 mat2_to_mat3 ( mat3 *p_result, mat2 m )
{
    *p_result = (mat3){
        .a = m.a, .b = m.b, .c = 0,
        .d = m.c, .e = m.d, .f = 0,
        .g = 0  , .h = 0  , .i = 1
    };
}

u0 mat2_to_mat4 ( mat4 *p_result, mat2 m )
{
    *p_result = (mat4){
        .a = m.a, .b = m.b, .c = 0.f, .d = 0.f,
        .e = m.c, .f = m.d, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };
}

// 3x3 Matrix - Column Major
// a d g (Col 0)
// b e h (Col 1)
// c f i (Col 2)
u0 mat3_mul_vec3 ( vec3 *p_result, mat3 m, vec3 v )
{
    // x' = Row0 . v = (a, d, g) . v
    *p_result = (vec3){
        .x = m.a * v.x + m.d * v.y + m.g * v.z,
        .y = m.b * v.x + m.e * v.y + m.h * v.z,
        .z = m.c * v.x + m.f * v.y + m.i * v.z
    };
}

u0 mat3_mul_mat3 ( mat3 *p_result, mat3 m, mat3 n )
{
    // Result = M * N
    *p_result = (mat3){
        // Col 0
        .a = m.a * n.a + m.d * n.b + m.g * n.c,
        .b = m.b * n.a + m.e * n.b + m.h * n.c,
        .c = m.c * n.a + m.f * n.b + m.i * n.c,
        
        // Col 1
        .d = m.a * n.d + m.d * n.e + m.g * n.f,
        .e = m.b * n.d + m.e * n.e + m.h * n.f,
        .f = m.c * n.d + m.f * n.e + m.i * n.f,
        
        // Col 2
        .g = m.a * n.g + m.d * n.h + m.g * n.i,
        .h = m.b * n.g + m.e * n.h + m.h * n.i,
        .i = m.c * n.g + m.f * n.h + m.i * n.i
    };
}

u0 mat3_transpose ( mat3 *p_result, mat3 m )
{
    *p_result = (mat3){
        .a = m.a, .b = m.d, .c = m.g,
        .d = m.b, .e = m.e, .f = m.h,
        .g = m.c, .h = m.f, .i = m.i
    };
}

u0 mat3_identity ( mat3 *p_result )
{
    *p_result = (mat3){
        .a = 1.f, .b = 0.f, .c = 0.f, 
        .d = 0.f, .e = 1.f, .f = 0.f, 
        .g = 0.f, .h = 0.f, .i = 1.f
    };
}

u0 mat3_to_mat2 ( mat2 *p_result, mat3 m )
{
    *p_result = (mat2){
        .a = m.a, .b = m.b,
        .c = m.d, .d = m.e
    };
}

u0 mat3_to_mat4 ( mat4 *p_result, mat3 m )
{
    *p_result = (mat4){
        .a = m.a, .b = m.b, .c = m.c, .d = 0.f,
        .e = m.d, .f = m.e, .g = m.f, .h = 0.f,
        .i = m.g, .j = m.h, .k = m.i, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };
}

int mat3_pack ( void *p_buffer, mat3 *p_m )
{
    return pack_pack(p_buffer, "%9f32", 
        p_m->a, p_m->b, p_m->c,
        p_m->d, p_m->e, p_m->f,
        p_m->g, p_m->h, p_m->i
    );
}

// 4x4 Matrix - Column Major
// a e i m (Col 0)
// b f j n (Col 1)
// c g k o (Col 2)
// d h l p (Col 3)

u0 mat4_mul_vec4 ( vec4 *p_result, mat4 m, vec4 v )
{
    // x' = Row0 . v = (a, e, i, m) . v
    *p_result = (vec4){
        .x = m.a * v.x + m.e * v.y + m.i * v.z + m.m * v.w,
        .y = m.b * v.x + m.f * v.y + m.j * v.z + m.n * v.w,
        .z = m.c * v.x + m.g * v.y + m.k * v.z + m.o * v.w,
        .w = m.d * v.x + m.h * v.y + m.l * v.z + m.p * v.w
    };
}

u0 mat4_mul_mat4 ( mat4 *p_result, mat4 m, mat4 n )
{
    // Result = M * N
    // Res_Col0 = M * N_Col0
    
    *p_result = (mat4){
        // Col 0
        .a = m.a * n.a + m.e * n.b + m.i * n.c + m.m * n.d,
        .b = m.b * n.a + m.f * n.b + m.j * n.c + m.n * n.d,
        .c = m.c * n.a + m.g * n.b + m.k * n.c + m.o * n.d,
        .d = m.d * n.a + m.h * n.b + m.l * n.c + m.p * n.d,

        // Col 1
        .e = m.a * n.e + m.e * n.f + m.i * n.g + m.m * n.h,
        .f = m.b * n.e + m.f * n.f + m.j * n.g + m.n * n.h,
        .g = m.c * n.e + m.g * n.f + m.k * n.g + m.o * n.h,
        .h = m.d * n.e + m.h * n.f + m.l * n.g + m.p * n.h,

        // Col 2
        .i = m.a * n.i + m.e * n.j + m.i * n.k + m.m * n.l,
        .j = m.b * n.i + m.f * n.j + m.j * n.k + m.n * n.l,
        .k = m.c * n.i + m.g * n.j + m.k * n.k + m.o * n.l,
        .l = m.d * n.i + m.h * n.j + m.l * n.k + m.p * n.l,

        // Col 3
        .m = m.a * n.m + m.e * n.n + m.i * n.o + m.m * n.p,
        .n = m.b * n.m + m.f * n.n + m.j * n.o + m.n * n.p,
        .o = m.c * n.m + m.g * n.n + m.k * n.o + m.o * n.p,
        .p = m.d * n.m + m.h * n.n + m.l * n.o + m.p * n.p
    };
}

u0 mat4_transpose ( mat4 *p_result, mat4 m )
{
    *p_result = (mat4){
        .a = m.a, .b = m.e, .c = m.i, .d = m.m,
        .e = m.b, .f = m.f, .g = m.j, .h = m.n,
        .i = m.c, .j = m.g, .k = m.k, .l = m.o,
        .m = m.d, .n = m.h, .o = m.l, .p = m.p
    };
}

u0 mat4_inverse ( mat4 *p_result, mat4 m )
{
    // Mapping for Col-Major
    // m00=a, m10=b, m20=c, m30=d (Col 0)
    // m01=e, m11=f, m21=g, m31=h (Col 1)
    // m02=i, m12=j, m22=k, m32=l (Col 2)
    // m03=m, m13=n, m23=o, m33=p (Col 3)

    float m00 = m.a, m10 = m.b, m20 = m.c, m30 = m.d;
    float m01 = m.e, m11 = m.f, m21 = m.g, m31 = m.h;
    float m02 = m.i, m12 = m.j, m22 = m.k, m32 = m.l;
    float m03 = m.m, m13 = m.n, m23 = m.o, m33 = m.p;

    float a00 = m00 * m11 - m01 * m10;
    float a01 = m00 * m12 - m02 * m10;
    float a02 = m00 * m13 - m03 * m10;
    float a03 = m01 * m12 - m02 * m11;
    float a04 = m01 * m13 - m03 * m11;
    float a05 = m02 * m13 - m03 * m12;
    float a06 = m20 * m31 - m21 * m30;
    float a07 = m20 * m32 - m22 * m30;
    float a08 = m20 * m33 - m23 * m30;
    float a09 = m21 * m32 - m22 * m31;
    float a10 = m21 * m33 - m23 * m31;
    float a11 = m22 * m33 - m23 * m32;

    float det = a00 * a11 - a01 * a10 + a02 * a09 + a03 * a08 - a04 * a07 + a05 * a06;

    if (det == 0.0f)
    {
        mat4_identity(p_result);
        return;
    }

    float invDet = 1.0f / det;

    *p_result = (mat4)
    {
        .a = ( m11 * a11 - m12 * a10 + m13 * a09) * invDet,
        .b = (-m10 * a11 + m12 * a08 - m13 * a07) * invDet,
        .c = ( m10 * a10 - m11 * a08 + m13 * a06) * invDet,
        .d = (-m10 * a09 + m11 * a07 - m12 * a06) * invDet,
        
        .e = (-m01 * a11 + m02 * a10 - m03 * a09) * invDet,
        .f = ( m00 * a11 - m02 * a08 + m03 * a07) * invDet,
        .g = (-m00 * a10 + m01 * a08 - m03 * a06) * invDet,
        .h = ( m00 * a09 - m01 * a07 + m02 * a06) * invDet,
        
        .i = ( m31 * a05 - m32 * a04 + m33 * a03) * invDet,
        .j = (-m30 * a05 + m32 * a02 - m33 * a01) * invDet,
        .k = ( m30 * a04 - m31 * a02 + m33 * a00) * invDet,
        .l = (-m30 * a03 + m31 * a01 - m32 * a00) * invDet,
        
        .m = (-m21 * a05 + m22 * a04 - m23 * a03) * invDet,
        .n = ( m20 * a05 - m22 * a02 + m23 * a01) * invDet,
        .o = (-m20 * a04 + m21 * a02 - m23 * a00) * invDet,
        .p = ( m20 * a03 - m21 * a01 + m22 * a00) * invDet
    };
}

u0 mat4_identity ( mat4 *p_result )
{
    *p_result = (mat4){
        .a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f,
        .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };
}

u0 mat4_to_mat2 ( mat2 *p_result, mat4 m )
{
    *p_result = (mat2){
        .a = m.a, .b = m.b,
        .c = m.e, .d = m.f
    };
}

u0 mat4_to_mat3 ( mat3 *p_result, mat4 m )
{
    *p_result = (mat3){
        .a = m.a, .b = m.b, .c = m.c, 
        .d = m.e, .e = m.f, .f = m.g, 
        .g = m.i, .h = m.j, .i = m.k
    };
}

u0 mat4_translation ( mat4 *p_result, vec3 location )
{
    // Col 3 = tx, ty, tz, 1 -> m, n, o, p
    *p_result = (mat4){
        .a = 1.f       , .b = 0.f       , .c = 0.f       , .d = 0.f,
        .e = 0.f       , .f = 1.f       , .g = 0.f       , .h = 0.f,
        .i = 0.f       , .j = 0.f       , .k = 1.f       , .l = 0.f,
        .m = location.x, .n = location.y, .o = location.z, .p = 1.f
    };
}

u0 mat4_scale ( mat4 *p_result, vec3 scale )
{
    *p_result = (mat4){
        .a = scale.x, .b = 0      , .c = 0      , .d = 0,
        .e = 0      , .f = scale.y, .g = 0      , .h = 0,
        .i = 0      , .j = 0      , .k = scale.z, .l = 0,
        .m = 0      , .n = 0      , .o = 0      , .p = 1
    };
}

u0 mat4_rotation_from_vec3 ( mat4 *p_result, vec3 rotation )
{
    float cx = cosf(rotation.x * DEG_TO_RAD), sx = sinf(rotation.x * DEG_TO_RAD);
    float cy = cosf(rotation.y * DEG_TO_RAD), sy = sinf(rotation.y * DEG_TO_RAD);
    float cz = cosf(rotation.z * DEG_TO_RAD), sz = sinf(rotation.z * DEG_TO_RAD);

    // XYZ Rotation - Column Major
    // Rx * Ry * Rz ? Or Rz * Ry * Rx ?
    // Standard Euler usually Rz * Ry * Rx (Roll Pitch Yaw) for intrinsic?
    // Let's assume standard XYZ application order: X then Y then Z.
    // M = Rz * Ry * Rx
    
    // Result of RzRyRx in Col Major:
    // Row 0: cy*cz,              -cy*sz,             sy
    // Row 1: cx*sz + sx*sy*cz,   cx*cz - sx*sy*sz,  -sx*cy
    // Row 2: sx*sz - cx*sy*cz,   sx*cz + cx*sy*sz,   cx*cy
    
    // Transpose of Row-Major gives Col-Major memory layout for same matrix.
    // m.a (0,0) = cy*cz
    // m.b (1,0) = cx*sz + sx*sy*cz
    // m.c (2,0) = sx*sz - cx*sy*cz
    // m.d (3,0) = 0
    
    // m.e (0,1) = -cy*sz
    // m.f (1,1) = cx*cz - sx*sy*sz
    // m.g (2,1) = sx*cz + cx*sy*sz
    // m.h (3,1) = 0

    // m.i (0,2) = sy
    // m.j (1,2) = -sx*cy
    // m.k (2,2) = cx*cy
    // m.l (3,2) = 0
    
    *p_result = (mat4){
        .a = cy * cz,               .b = cx * sz + sx * sy * cz, .c = sx * sz - cx * sy * cz, .d = 0.f,
        .e = -cy * sz,              .f = cx * cz - sx * sy * sz, .g = sx * cz + cx * sy * sz, .h = 0.f,
        .i = sy,                    .j = -sx * cy,               .k = cx * cy,                .l = 0.f,
        .m = 0.f,                   .n = 0.f,                    .o = 0.f,                    .p = 1.f
    };
}

u0 mat4_model_from_vec3 ( mat4 *p_result, vec3 location, vec3 rotation, vec3 scale )
{
    mat4 _location = { 0 }, _rotation = { 0 }, _scale = { 0 };
    mat4 _rs = { 0 };

    mat4_translation(&_location, location);
    mat4_rotation_from_vec3(&_rotation, rotation);
    mat4_scale(&_scale, scale);

    // M = T * R * S
    mat4_mul_mat4(&_rs, _rotation, _scale);
    mat4_mul_mat4(p_result, _location, _rs);
}

u0 mat4_model_from_bounds ( mat4 *p_result, vec3 min, vec3 max )
{
    vec3 scale = {
        .x = max.x - min.x,
        .y = max.y - min.y,
        .z = max.z - min.z
    };
    vec3 location = {
        .x = (max.x + min.x) / 2.0f,
        .y = (max.y + min.y) / 2.0f,
        .z = (max.z + min.z) / 2.0f
    };
    vec3 rotation = { 0.0f, 0.0f, 0.0f };

    mat4_model_from_vec3(p_result, location, rotation, scale);
}

int mat4_pack ( void *p_buffer, mat4 *p_m )
{
    return pack_pack(p_buffer, "%16f32", 
        p_m->a, p_m->b, p_m->c, p_m->d,
        p_m->e, p_m->f, p_m->g, p_m->h,
        p_m->i, p_m->j, p_m->k, p_m->l,
        p_m->m, p_m->n, p_m->o, p_m->p
    );
}
