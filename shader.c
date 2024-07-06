/** !
 * Shader implementation
 * 
 * @file g10/shader.h
 * 
 * @author Jacob Smith
 */

// Header 
#include <g10/shader.h>

int shader_from_json ( shader **pp_shader, const char *const p_name, json_value *p_value )
{

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    shader     *p_shader   = (void *) 0;

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        g_vulkan_shader_construct(&p_shader, p_value);
    #elif defined G10_BUILD_WITH_OPENGL

        // External functions
        extern int g_opengl_shader_construct ( shader **pp_shader, json_value *p_value );

        // Construct the shader
        if ( g_opengl_shader_construct(&p_shader, p_value) == 0 ) goto failed_to_create_shader_opengl;
    #endif

    // Store the maximum quantity of draw items
    p_shader->max = 4;

    // Grow the allocation
    p_shader = G10_REALLOC(p_shader, sizeof(shader) + ( sizeof(void *) * p_shader->max) );

    // Store the name
    if ( p_name )
    {

        // Initialized data
        size_t len = strlen(p_name);

        // Error check
        if ( len == 0   ) goto name_property_is_too_short;
        if ( len  > 255 ) goto name_property_is_too_long;

        // Copy the render pass name
        strncpy(p_shader->_name, p_name, len);

        // Store a null terminator
        p_shader->_name[len] = '\0';
    }

    // Return a pointer to the caller
    *pp_shader = p_shader;

    // Add the shader to the shader cache
    dict_add(p_instance->cache.p_shaders, p_name, p_shader);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Parameter \"p_name\" less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Parameter \"p_name\" must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
        }

        // OpenGL errors
        {
            failed_to_create_shader_opengl:
                #ifndef NDEBUG
                    log_error("[g10] [shader] [opengl] Failed to construct shader in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
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
