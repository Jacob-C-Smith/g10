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

// Forward declarations
struct g_instance_s;

// Type definitions
typedef struct g_instance_s g_instance;