﻿/** !
 * Mesh
 * 
 * @file g10/mesh.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// log module
#include <log/log.h>

// sync module
#include <sync/sync.h>

// hash cache module
#include <hash_cache/hash.h>

// array module
#include <array/array.h>

// queue module
#include <queue/queue.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// parallel module
#include <parallel/parallel.h>
#include <parallel/schedule.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#ifdef G10_BUILD_WITH_VULKAN
#elif defined G10_BUILD_WITH_OPENGL
    #include <glad/glad.h>
#endif

// Enumeration definitions
enum mesh_shapes_e
{
    G10_BASE_MESH_PLANE,
    G10_BASE_MESH_CUBE,
    G10_BASE_MESH_CIRCLE,
    G10_BASE_MESH_SPHERE,
    G10_BASE_MESH_CYLINDER,
    G10_BASE_MESH_CONE,
    G10_BASE_MESH_QUANTITY
};

// Structure definitions
struct mesh_s
{
    size_t quantity;
    mesh_data *_p_meshes[];
};

struct mesh_create_info_s
{
    struct { size_t quantity; const f32 *p_data; } verticies;
    struct { size_t quantity; const u32 *p_data; } elements;
};

struct mesh_data_s
{

    char _name[63 + 1];
    transform *p_transform;
    #ifdef G10_BUILD_WITH_VULKAN
        struct
        {
            
        } vulkan;
    #elif defined G10_BUILD_WITH_OPENGL
        struct
        {
            GLuint vertex_buffers[32];
            GLuint vertex_arrays[32];
            GLuint element_arrays[32];
            size_t indices;
        } opengl;
    #endif
};

DLLEXPORT int mesh_shape_construct
(
    mesh_data          **pp_mesh_data,
    enum mesh_shapes_e   type,
    transform           *p_transform
);

DLLEXPORT int mesh_from_json ( mesh **pp_mesh, const json_value *const p_value, transform *p_parent_transform );

DLLEXPORT int mesh_draw ( shader *p_shader, mesh_data *p_mesh_data );
