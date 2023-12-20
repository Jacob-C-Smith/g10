/** !
 * Type definitions for g10
 * 
 * @file g10/gtypedef.h
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Void
typedef void u0;

// Integers
typedef unsigned char      u8;
typedef signed   char      s8;
typedef          char      i8;
typedef unsigned short     u16;
typedef signed   short     s16;
typedef          short     i16;
typedef unsigned int       u32;
typedef signed   int       s32;
typedef          int       i32;
typedef unsigned long long u64;
typedef signed   long long s64;
typedef          long long i64;

// Floats
typedef float  f32;
typedef double f64;

// Vectors
typedef struct { float x, y; }       vec2;
typedef struct { float x, y, z; }    vec3;
typedef struct { float x, y, z, w; } vec4;

// 2x2 Matrix
typedef struct { float a, b, c, d; } mat2;

// 3x3 Matrix
typedef struct { float a, b, c, d, e, f, g, h, i; } mat3;

// 4x4 Matrix
typedef struct { float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p; } mat4;

// Quaternions
typedef struct { float u, i, j, k; } quaternion;

// Forward declarations
struct g_instance_s;
struct server_s;

// Type definitions
typedef struct g_instance_s g_instance;
typedef struct server_s server;

typedef int (*fn_user_code_callback)( g_instance *p_instance );
