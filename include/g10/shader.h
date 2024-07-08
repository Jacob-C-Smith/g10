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

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/transform.h>

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

