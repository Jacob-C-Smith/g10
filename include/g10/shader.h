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

    // Functions
    struct
    {
        fn_shader_on_bind     pfn_shader_on_bind;
        fn_shader_on_material pfn_shader_on_material;
        fn_shader_on_draw     pfn_shader_on_draw;
    } functions;

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
DLLEXPORT int shader_from_json ( shader **pp_shader, json_value *p_value );
