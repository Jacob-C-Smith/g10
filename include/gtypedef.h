/** !
 * Type definitions for g10
 * 
 * @file g10/gtypedef.h
 * 
 * @author Jacob Smith
 */

// header guard
#pragma once

// void
typedef void u0;

// integer
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

// floating point
typedef float  f32;
typedef double f64;

// vector
typedef struct { float x, y; }       vec2;
typedef struct { float x, y, z; }    vec3;
typedef struct { float x, y, z, w; } vec4;

// 2x2 matrix
typedef struct { float a, b, c, d; } mat2;

// 3x3 matrix
typedef struct { float a, b, c, d, e, f, g, h, i; } mat3;

// 4x4 matrix
typedef struct { float a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p; } mat4;

// quaternions
typedef struct { float u, i, j, k; } quaternion;

// structure declarations
struct aabb_s;
struct attachment_s;
struct bv_s;
struct camera_s;
struct entity_s;
struct framebuffer_s;
struct g_instance_s;
struct geometry_s;
struct light_s;
struct material_s;
struct pipeline_s;
struct renderer_s;
struct render_pass_s;
struct scene_s;
struct skybox_s;
struct sampler_s;
struct transform_s;
struct texture_s;
struct uniform_s;
struct input_s;
struct input_bind_s;

// struct ai_s;
// struct bv_s;
// struct cull_operation_s;
// struct mesh_s;
// union  mesh_data_u;
// struct mesh_create_info_s;
// struct server_s;
// struct shell_s;

// type definitions
typedef struct aabb_s        aabb;
typedef struct attachment_s  attachment;
typedef struct bv_s           bv;
typedef struct camera_s      camera;
typedef struct entity_s      entity;
typedef struct framebuffer_s framebuffer;
typedef struct g_instance_s  g_instance;
typedef struct geometry_s    geometry;
typedef struct light_s       light;
typedef struct material_s    material;
typedef struct pipeline_s    pipeline;
typedef struct renderer_s    renderer;
typedef struct render_pass_s render_pass;
typedef struct scene_s       scene;
typedef struct skybox_s      skybox;
typedef struct sampler_s     sampler;
typedef struct transform_s   transform;
typedef struct texture_s     texture;
typedef struct uniform_s     uniform;
typedef struct input_s       input;
typedef struct input_bind_s  input_bind;

// typedef struct ai_s               ai;
// typedef struct cull_operation_s   cull_operation;
// typedef struct mesh_s             mesh;
// typedef struct mesh_data_s        mesh_data;
// typedef struct mesh_create_info_s mesh_create_info;
// typedef struct server_s           server;
// typedef struct shell_s            shell;

// function pointers
typedef int (fn_bv_resize)( bv *p_bv );
typedef int (fn_bv_bounds)( bv *p_bv, vec3 *p_min, vec3 *p_max );
typedef int (fn_render_early)( g_instance *p_instance );
typedef int (fn_render_late)( g_instance *p_instance );
typedef int (fn_render_draw)( g_instance *p_instance );
typedef int (fn_render_pass_bind)( renderer *p_renderer );
typedef int (fn_render_pass_draw)( renderer *p_renderer );
typedef int (fn_pipeline_bind_once)( render_pass *p_render_pass, pipeline *p_pipeline );
typedef int (fn_pipeline_bind_each)( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable );
typedef int (fn_pipeline_cull)( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable );
typedef int (fn_pipeline_draw)( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable );
typedef int (fn_user_code)( g_instance *p_instance );
typedef int (fn_camera_controller)( camera *p_camera );

// typedef int (*fn_bv_bounds_getter)( void *p_value, vec3 *p_min, vec3 *p_max );
// typedef int (*fn_cull_operation)( void *p_object );