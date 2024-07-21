/** !
 * Type definitions for g10
 * 
 * @file g10/gtypedef.h
 * 
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
struct ai_s;
struct attachment_s;
struct bv_s;
struct camera_s;
struct cull_operation_s;
struct entity_s;
struct g_instance_s;
struct input_s;
struct input_bind_s;
struct mesh_s;
union  mesh_data_u;
struct mesh_create_info_s;
struct renderer_s;
struct render_pass_s;
struct scene_s;
struct server_s;
struct shader_s;
struct shell_s;
struct transform_s;

// Type definitions
typedef struct ai_s               ai;
typedef struct attachment_s       attachment;
typedef struct bv_s               bv;
typedef struct camera_s           camera;
typedef struct cull_operation_s   cull_operation;
typedef struct entity_s           entity;
typedef struct g_instance_s       g_instance;
typedef struct input_s            input;
typedef struct input_bind_s       input_bind;
typedef struct mesh_s             mesh;
typedef struct mesh_data_s        mesh_data;
typedef struct mesh_create_info_s mesh_create_info;
typedef struct renderer_s         renderer;
typedef struct render_pass_s      render_pass;
typedef struct scene_s            scene;
typedef struct server_s           server;
typedef struct shader_s           shader;
typedef struct shell_s            shell;
typedef struct transform_s        transform;

typedef int (*fn_bv_bounds_getter)( void *p_value, vec3 *p_min, vec3 *p_max );
typedef int (*fn_camera_controller)( camera *p_camera );
typedef int (*fn_cull_operation)( void *p_object );
typedef int (*fn_shader_on_bind)( shader *p_shader, void *p_on_bind );
typedef int (*fn_shader_on_material)( u0 );
typedef int (*fn_shader_on_draw)( void *p_draw_item );
typedef int (*fn_user_code_callback)( g_instance *p_instance );