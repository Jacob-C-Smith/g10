/** !
 * Shader implementation
 * 
 * @file g10/shader.h
 * 
 * @author Jacob Smith
 */

// Header 
#include <g10/shader.h>

int shader_from_json ( shader **pp_shader, json_value *p_value )
{

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        return g_vulkan_shader_construct(pp_shader, p_value);
    #elif defined G10_BUILD_WITH_OPENGL

        // External functions
        extern int g_opengl_shader_construct ( shader **pp_shader, json_value *p_value );

        // Done
        return g_opengl_shader_construct(pp_shader, p_value);
    #endif
}

int shader_bind ( shader *p_shader )
{

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        return g_vulkan_shader_bind(p_shader);
    #elif defined G10_BUILD_WITH_OPENGL

        // External functions
        extern int g_opengl_shader_bind ( shader *p_shader );

        // Done
        return g_opengl_shader_bind(p_shader);
    #endif
}
