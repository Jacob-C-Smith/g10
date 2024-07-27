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
        extern int g_opengl_shader_construct ( shader **pp_shader, const json_value *p_value );

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

    // Add the shader to the shader cache
    dict_add(p_instance->cache.p_shaders, p_name, p_shader);

    // Return a pointer to the caller
    *pp_shader = p_shader;

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

int shader_from_json_2 ( shader **pp_shader, const char *const p_name, const json_value *p_value )
{
 
    // Initialized data
    g_instance *p_instance   = g_get_active_instance();
    shader     *p_shader     = (void *) 0;
    node_graph *p_node_graph = (void *) 0;

    // Unused
    (void) p_node_graph;

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        g_vulkan_shader_construct(&p_shader, p_value);
    #elif defined G10_BUILD_WITH_OPENGL

        // External functions
        extern int g_opengl_shader_construct ( shader **pp_shader, const json_value *p_value );
 
        // Construct the shader
        if ( g_opengl_shader_construct(&p_shader, p_value) == 0 ) goto failed_to_create_shader_opengl;
    #else
        return 0;
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


int shader_draw_item_add ( shader *p_shader, void *p_draw_item )
{

    // Argument check
    if ( p_shader    == (void *) 0 ) goto no_shader;
    if ( p_draw_item == (void *) 0 ) goto no_draw_item;

    
    if ( p_shader->count == p_shader->max ) goto grow_allocation;

    done:

    // Store the draw item
    p_shader->_p_draw_items[p_shader->count] = p_draw_item;

    // Increment the draw item count
    p_shader->count++;

    // Success
    return 1;

    grow_allocation:
    {
        
        // Increase the maximum quantity of draw items
        p_shader->max += 128;

        // Grow the allocation
        p_shader = G10_REALLOC(p_shader, sizeof(shader) + ( sizeof(void *) + p_shader->max ));

        // Error check
        if ( p_shader == (void *) 0 ) goto no_mem;

        // Done
        goto done;
    }

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_draw_item:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_draw_item\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int shader_bind ( shader *p_shader )
{

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    fn_shader_on_bind pfn_shader_on_bind = p_shader->functions.pfn_shader_on_bind;

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        return g_vulkan_shader_bind(p_shader);
    #elif defined G10_BUILD_WITH_OPENGL

        // External functions
        extern int g_opengl_shader_bind ( shader *p_shader );
        
        // Bind the shader
        g_opengl_shader_bind(p_shader);
        
        // Bind the camera
        pfn_shader_on_bind(p_shader, (void *)p_instance->context.p_scene->context.p_camera);
    #endif

    // Success
    return 1;
}

int shader_info ( const shader *const p_shader )
{

    // Argument check
    if ( p_shader == (void *) 0 ) goto no_shader;

    // Print the shader
    printf("Shader:\n");
    printf(" - name : %s\n", p_shader->_name);
    printf(" - draw items : \n");

    // Print each render pass
    for (size_t i = 0; i < p_shader->count; i++)
    {

        // Initialized data
        void *p_draw_item = p_shader->_p_draw_items[i];
        
        // Print the render pass
        printf("    [%zu] : %p\n", i, p_draw_item);
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int shader_bind_camera ( shader *p_shader, camera *p_camera )
{
    
    // Argument check
    if ( p_shader == (void *) 0 ) goto no_shader;
    if ( p_camera == (void *) 0 ) goto no_camera;

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        return g_vulkan_shader_bind(p_shader);
    #elif defined G10_BUILD_WITH_OPENGL
        
        // External functions
        extern int g_opengl_shader_bind_camera ( shader *p_shader, const camera *const p_camera, ... );

        g_opengl_shader_bind_camera(p_shader, p_camera);

    #endif
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int shader_bind_transform ( shader *p_shader, transform *p_transform )
{
    
    // Argument check
    if ( p_shader    == (void *) 0 ) goto no_shader;
    if ( p_transform == (void *) 0 ) goto no_transform;

    // Graphics API specific implementation
    #ifdef G10_BUILD_WITH_VULKAN
    
        // Done
        return g_vulkan_shader_bind(p_shader);
    #elif defined G10_BUILD_WITH_OPENGL
        
        // External functions
        extern int g_opengl_shader_bind_transform ( shader *p_shader, const transform *const p_transform, ... );

        // Bind the transform
        g_opengl_shader_bind_transform(p_shader, p_transform);
    #endif
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
