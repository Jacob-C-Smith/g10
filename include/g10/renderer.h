/** !
 * Include header for renderer.
 * 
 * @file g10/renderer.h
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

// array module
#include <array/array.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/shader.h>

// Structure definitions
struct renderer_s
{

    // Name
    char _name[255 + 1];
    
    // Clear color
    vec3 clear_color;

    // Render passes
    size_t       render_pass_quantity;
    render_pass *_p_render_passes[];
};

struct render_pass_s
{

    // Name
    char _name[255 + 1];

    size_t shader_quantity;
    shader *_p_shaders[];
};

struct attachment_s
{

    // Name
    char _name[255 + 1];

    // Description
    char _description[255 + 1];

    #ifdef G10_BUILD_WITH_VULKAN
    struct
    {
        
    } vulkan;
    #elif defined G10_BUILD_WITH_OPENGL
        struct
        {
            int vertex_buffers[32];
            int vertex_arrays[32];
            int element_arrays[32];
        } opengl;
    #endif

};

// Function definitions

// Constructors
/** !
 * Construct a renderer from a json value
 * 
 * @param pp_renderer return
 * @param p_value     the json value
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int renderer_from_json
(
    renderer   **pp_renderer,
    json_value  *p_value
);

// Accessors
//

// Render 
/** !
 * Render the active scene using the active renderer
 * 
 * @param p_instance the active instance
 *
 * @return 1 on success, 0 on error 
 */
DLLEXPORT int renderer_render ( g_instance *p_instance );

// Present
/** !
 * Present the rendered frame
 * 
 * @param p_instance the active instance
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int renderer_present ( g_instance *p_instance );

// Destructorss
/** !
 * Destroy and deallocate a renderer
 * 
 * @param pp_renderer pointer to renderer pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int renderer_destroy ( renderer **pp_renderer );
