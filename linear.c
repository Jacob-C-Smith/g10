#include <g10/linear.h>

u0 add_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3)
    {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}

u0 sub_vec3 ( vec3 *p_result, vec3 a, vec3 b )
{
    *p_result = (vec3)
    {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}

float length ( vec3 a )
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float dot_product_vec3( vec3 a, vec3 b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross_product_vec3 ( vec3 a, vec3 b )
{
    return (vec3)
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

u0 mul_vec3_vec3 ( vec3 *r, vec3 a, vec3 b )
{
    r->x = a.x * b.x,
    r->y = a.y * b.y,
    r->z = a.z * b.z;
}

u0 div_vec3_f( vec3 *r, vec3 a, float s )
{
    r->x = a.x / s,
    r->y = a.y / s,
    r->z = a.z / s;
}

vec3 mul_vec3_f ( vec3 v, float s )
{
    return (vec3)
    {
        v.x * s,
        v.y * s,
        v.z * s,
        v.w * s
    };
}

vec3 normalize ( vec3 v )
{
    float vl = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w));
    return (vec3)
    {
        .x = v.x / vl,
        .y = v.y / vl,
        .z = v.z / vl,
        .w = v.w / vl
    };
}

vec2 mul_mat2_vec2 ( mat2 m, vec2 v)
{
    return (vec2)
    {
        m.a*v.x+m.b*v.y,
        m.c*v.x+m.d*v.y
    };
}

vec4 mul_mat4_vec4 ( mat4 m, vec4 v )
{
    return (vec4)
    {
        .x = m.a* v.x + m.b * v.y + m.c * v.z + m.d * v.w,
        .y = m.e* v.x + m.f * v.y + m.g * v.z + m.h * v.w,
        .z = m.i* v.x + m.j * v.y + m.k * v.z + m.l * v.w,
        .w = m.m* v.x + m.n * v.y + m.o * v.z + m.p * v.w
    };
}

mat2 mul_mat2_mat2 ( mat2 m, mat2 n )
{
    return (mat2)
    {
        .a = m.a* n.a + m.b * n.c, .b = m.a* n.b + m.b * n.d,
        .c = m.c* n.a + m.d * n.c, .d = m.c* n.b + m.d * n.d
    };
}

mat4 mul_mat4_mat4 ( mat4 m, mat4 n )
{
    return (mat4)
    {
        .a = (m.a * n.a + m.b * n.e + m.c * n.i + m.d * n.m), .b = (m.a * n.b + m.b * n.f + m.c * n.j + m.d * n.n), .c = (m.a * n.c + m.b * n.g + m.c * n.k + m.d * n.o), .d = (m.a * n.d + m.b * n.h + m.c * n.l + m.d * n.p),
        .e = (m.e * n.a + m.f * n.e + m.g * n.i + m.h * n.m), .f = (m.e * n.b + m.f * n.f + m.g * n.j + m.h * n.n), .g = (m.e * n.c + m.f * n.g + m.g * n.k + m.h * n.o), .h = (m.e * n.d + m.f * n.h + m.g * n.l + m.h * n.p),
        .i = (m.i * n.a + m.j * n.e + m.k * n.i + m.l * n.m), .j = (m.i * n.b + m.j * n.f + m.k * n.j + m.l * n.n), .k = (m.i * n.c + m.j * n.g + m.k * n.k + m.l * n.o), .l = (m.i * n.d + m.j * n.h + m.k * n.l + m.l * n.p),
        .m = (m.m * n.a + m.n * n.e + m.o * n.i + m.p * n.m), .n = (m.m * n.b + m.n * n.f + m.o * n.j + m.p * n.n), .o = (m.m * n.c + m.n * n.g + m.o * n.k + m.p * n.o), .p = (m.m * n.d + m.n * n.h + m.o * n.l + m.p * n.p)
    };
}

mat2 rcp_mat2( mat2 m )
{
    return (mat2)
    {
        .a = m.a, .b = m.c,
        .c = m.b, .d = m.d
    };
}

mat4 rcp_mat4( mat4 m )
{
    return (mat4)
    {
        .a = (m.a), .b = (m.e), .c = (m.i), .d = (m.m),
        .e = (m.b), .f = (m.f), .g = (m.j), .h = (m.n),
        .i = (m.c), .j = (m.g), .k = (m.k), .l = (m.o),
        .m = (m.d), .n = (m.h), .o = (m.l), .p = (m.p)
    };
}

mat2 identity_mat2()
{
    return (mat2)
    {
        .a = 1.f, .b = 0.f,
        .c = 0.f, .d = 1.f
    };
}

mat4 identity_mat4 ( )
{
    return (mat4)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f,
        .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = 0.f, .n = 0.f, .o = 0.f, .p = 1.f
    };
}

mat4 translation_mat4 ( vec3 v )
{
    return (mat4)
    {
        .a = 1.f, .b = 0.f, .c = 0.f, .d = 0.f,
        .e = 0.f, .f = 1.f, .g = 0.f, .h = 0.f,
        .i = 0.f, .j = 0.f, .k = 1.f, .l = 0.f,
        .m = v.x, .n = v.y, .o = v.z, .p = 1.f
    };
}

mat4 scale_mat4 ( vec3 scale )
{
    return (mat4)
    {
        .a = scale.x, .b = 0      , .c = 0      , .d = 0,
        .e = 0      , .f = scale.y, .g = 0      , .h = 0,
        .i = 0      , .j = 0      , .k = scale.z, .l = 0,
        .m = 0      , .n = 0      , .o = 0      , .p = 1
    };
};

mat4 rotation_mat4_from_vec3 ( vec3 rotation )
{
    return (mat4)
    {
        cos(rotation.x) + pow(rotation.x, 2) * (1 - cos(rotation.x))                 , rotation.x* rotation.y* (1 - cos(rotation.y)) - rotation.z * sin(rotation.y)   , rotation.x* rotation.z* (1 - cos(rotation.z)) + rotation.y * sin(rotation.z), 0,
        rotation.y * rotation.x * (1 - cos(rotation.x)) + rotation.z * sin(rotation.x), cos(rotation.y) + pow(rotation.y, 2) * (1 - cos(rotation.y))                  , rotation.y* rotation.z* (1 - cos(rotation.z)) - rotation.x * sin(rotation.z), 0,
        rotation.y * rotation.x * (1 - cos(rotation.x)) + rotation.z * sin(rotation.x), rotation.y * rotation.x * (1 - cos(rotation.x)) + rotation.z * sin(rotation.x), cos(rotation.x) + pow(rotation.x, 2) * (1 - cos(rotation.x))                , 0,
        0                                                                             , 0                                                                             , 0                                                                           , 1
    };
}

mat4 model_mat4_from_vec3 ( vec3 location, vec3 rotation, vec3 scale )
{
    return mul_mat4_mat4(mul_mat4_mat4(scale_mat4(scale), translation_mat4(location)), rotation_mat4_from_vec3(rotation));
}