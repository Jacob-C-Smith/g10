/** !
 * Header file for shader
 * 
 * @file g10/shader.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// node
#include <node/node.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/transform.h>

// Enumeration definitions
enum shader_vertex_attribute_type_e
{
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC2_F32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC3_F32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC4_F32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC2_BOOL,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC3_BOOL,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC4_BOOL,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC2_S32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC3_S32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC4_S32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC2_U32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC3_U32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC4_U32,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC2_F64,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC3_F64,
    SHADER_VERTEX_ATTRIBUTE_TYPE_VEC4_F64
};

// Structure definitions
struct shader_s
{

    // Name
    char _name[255+1];

    // Count
    size_t count, max;

    // Functions
    struct
    {
        fn_shader_on_bind     pfn_shader_on_bind;
        fn_shader_on_material pfn_shader_on_material;
        fn_shader_on_draw     pfn_shader_on_draw;
    } functions;

    #ifdef G10_BUILD_WITH_VULKAN
        struct
        {
            
        } vulkan;
    #elif defined G10_BUILD_WITH_OPENGL
        struct
        {
            unsigned int vertex_shader,
                         fragment_shader,
                         program;
            size_t vertex_attribute_count;
            struct 
            {
                char _name[15 + 1];
                enum shader_vertex_attribute_type_e _type;
            } vertex_attributes[16];
        } opengl;
    #endif

    void *_p_draw_items[];
};

// Constructors
/** !
 * Construct an shader from a json value
 * 
 * @param pp_shader result
 * @param p_value   the json value
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int shader_from_json ( shader **pp_shader, const char *const p_name, json_value *p_value );

DLLEXPORT int shader_from_json_2 ( shader **pp_shader, const char *const p_name, const json_value *p_value );

// Mutators
DLLEXPORT int shader_draw_item_add ( shader *p_shader, void *p_draw_item );

// Pipeline operations
/** !
 * Bind a shader
 * 
 * @param p_shader
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int shader_bind ( shader *p_shader );

// Info
/** !
 *  Print info about a shader
 *
 * @param p_shader the shader
 *
 * @sa shader_load
 * @sa shader_from_json
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int shader_info ( const shader *const p_shader );

DLLEXPORT int shader_bind_camera ( shader *p_shader, camera *p_camera );
