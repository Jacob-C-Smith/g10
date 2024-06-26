// Header
#include <g10/renderer.h>

// Function declarations
int renderer_pass_render ( renderer *p_renderer, render_pass *p_render_pass );

int renderer_create ( renderer **pp_renderer ) 
{

    // Argument check
    if ( pp_renderer == (void *) 0 ) goto no_renderer;

    // Initialized data
    renderer *p_renderer = G10_REALLOC(0, sizeof(renderer));

    // Error check
    if ( p_renderer == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_renderer, 0, sizeof(renderer));

    // Return a pointer to the caller
    *pp_renderer = p_renderer;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int render_pass_create ( render_pass **pp_render_pass ) 
{

    // Argument check
    if ( pp_render_pass == (void *) 0 ) goto no_render_pass;

    // Initialized data
    render_pass *p_render_pass = G10_REALLOC(0, sizeof(render_pass));

    // Error check
    if ( p_render_pass == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_render_pass, 0, sizeof(render_pass));

    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int attachment_create ( attachment **pp_attachment ) 
{

    // Argument check
    if ( pp_attachment == (void *) 0 ) goto no_attachment;

    // Initialized data
    attachment *p_attachment = G10_REALLOC(0, sizeof(attachment));

    // Error check
    if ( p_attachment == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_attachment, 0, sizeof(attachment));

    // Return a pointer to the caller
    *pp_attachment = p_attachment;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_attachment:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"pp_attachment\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int renderer_from_json ( renderer **pp_renderer, json_value *p_value )
{

    // Argument check
    if ( pp_renderer   == (void *)        0 ) goto no_renderer;
    if ( p_value       == (void *)        0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto value_is_wrong_type;

    // Initialized data
    size_t    i = 0;
    renderer  _renderer  = { 0 },
             *p_renderer = (void *) 0;

    // Parse the json value into a renderer
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name_value        = dict_get(p_dict, "name"),
                         *p_clear_color_value = dict_get(p_dict, "clear color"),
                         *p_attachments_value = dict_get(p_dict, "attachments"),
                         *p_passes_value      = dict_get(p_dict, "passes");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [renderer] Consider adding a \"$schema\" property to the renderer\n");

        // Error check
        if ( p_name_value        == (void *) 0 ) goto no_name_property;
        if ( p_attachments_value == (void *) 0 ) goto no_attachments_property;
        if ( p_passes_value      == (void *) 0 ) goto no_passes_property;

        // Type check
        if ( p_name_value->type        != JSON_VALUE_STRING ) goto name_property_is_wrong_type;
        if ( p_clear_color_value->type != JSON_VALUE_ARRAY  ) goto clear_color_property_is_wrong_type;
        if ( p_attachments_value->type != JSON_VALUE_OBJECT ) goto attachments_property_is_wrong_type;
        if ( p_attachments_value->type != JSON_VALUE_OBJECT ) goto passes_property_is_wrong_type;

        // Store the name
        {

            // Initialized data
            size_t len = strlen(p_name_value->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the instance name
            strncpy(_renderer._name, p_name_value->string, 255);

            // Store a null terminator
            _renderer._name[len] = '\0';
        }

        // Store the clear color
        {

            // Initialized data
            array      *p_array      = p_clear_color_value->list;
            size_t      len          = array_size(p_array);
            json_value *p_scratch[5] = { 0 };

            // Error check
            if ( len != 4 ) goto wrong_clear_color_len;

            // Dump the location values
            array_slice(p_array, (void **) &p_scratch, 0, 3);

            // Error check
            for ( i = 0; i < len; i++ ) 
            {
                if ( p_scratch[i]       ==        (void *) 0 ) goto clear_color_element_was_wrong_type;
                if ( p_scratch[i]->type != JSON_VALUE_NUMBER ) goto clear_color_element_was_wrong_type;  
            }

            // Store the vector
            _renderer.clear_color = (vec3)
            {
                .x = (float) p_scratch[0]->number,
                .y = (float) p_scratch[1]->number,
                .z = (float) p_scratch[2]->number
            };
        }

        // Construct attachments
        {

            // Initialized data
            dict       *p_dict          = p_attachments_value->object;
            const char *p_scratch[32]   = { 0 };
            size_t  attachment_quantity = dict_keys(p_dict, 0);

            // Error check
            if ( attachment_quantity > 32 ) goto too_many_attachments;

            // Get the name of each attachment
            dict_keys(p_dict, &p_scratch);

            // Iterate through each attachment
            for (size_t i = 0; i < attachment_quantity; i++)
            {
                
                // Initialized data
                //attachment *p_attachment       = (void *) 0;
                json_value *p_attachment_value = dict_get(p_dict, p_scratch[i]);
                
                //printf("%d : %s\n", i, p_scratch[i]);
                // Construct an attachment
                //

                // Store the attachment
                // dict_add(p_renderer->attachments.data, p_attachment->_name, p_attachment);
            }
        }

        // Construct render passes
        {

        }
    }

    // Allocate memory on the heap
    if ( renderer_create(&p_renderer) == 0 ) goto failed_to_allocate_renderer;

    // Copy the struct to the heap
    memcpy(p_renderer, &_renderer, sizeof(renderer));

    // Return a pointer to the caller
    *pp_renderer = p_renderer;

    // Success
    return 1;

    too_many_attachments:
    too_many_render_passes:

        // Error
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_allocate_renderer:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Failed to allocate renderer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"name\" property of renderer object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
            
            clear_color_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"clear color\" property of renderer object must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
            
            attachments_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"attachments\" property of renderer object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
                        
            passes_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"passes\" property of renderer object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"name\" property of renderer object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"name\" property of renderer object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            wrong_clear_color_len:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Property \"clear color\" of parameter \"p_value\" must be of length 4 in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            clear_color_element_was_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Element %d of \"clear color\" property of \"p_value\" parameter must be of type [ number ] in call to function \"%s\"\n", i, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            no_attachments_property:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" is missing required property \"attachments\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            no_passes_property:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
        }
    }
}

int attachment_from_json ( attachment **pp_attachment, json_value *p_value )
{

    // Argument check
    if ( pp_attachment == (void *)        0 ) goto no_attachment;
    if ( p_value       == (void *)        0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto value_is_wrong_type;

    // Initialized data
    attachment  _attachment = { 0 },
              *p_attachment = (void *) 0;

    // Parse the json value into an attachment
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name_value        = dict_get(p_dict, "name"),
                         *p_description_value = dict_get(p_dict, "description"),
                         *p_format_value      = dict_get(p_dict, "format");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [renderer] Consider adding a \"$schema\" property to the attachment\n");

        // Error check
        if ( p_name_value        == (void *) 0 ) goto no_name_property;
        if ( p_description_value == (void *) 0 ) goto no_description_property;
        if ( p_format_value      == (void *) 0 ) goto no_format_property;

        // Type check
        if ( p_name_value->type        != JSON_VALUE_STRING ) goto name_property_is_wrong_type;
        if ( p_description_value->type != JSON_VALUE_STRING ) goto description_property_is_wrong_type;
        if ( p_format_value->type      != JSON_VALUE_STRING ) goto format_property_is_wrong_type;

        // Store the name
        {

            // Initialized data
            size_t len = strlen(p_name_value->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the attachment name
            strncpy(_attachment._name, p_name_value->string, 255);

            // Store a null terminator
            _attachment._name[len] = '\0';
        }

        // Store the description
        {

            // Initialized data
            size_t len = strlen(p_description_value->string);

            // Error check
            if ( len == 0   ) goto description_property_is_too_short;
            if ( len  > 255 ) goto description_property_is_too_long;

            // Copy the attachment description
            strncpy(_attachment._description, p_description_value->string, 255);

            // Store a null terminator
            _attachment._name[len] = '\0';
        }
    }

    // Allocate memory on the heap
    if ( attachment_create(&p_attachment) == 0 ) goto failed_to_allocate_attachment;

    // Copy the attachment to the heap
    memcpy(p_attachment, &_attachment, sizeof(attachment));

    // Return a pointer to the caller
    *pp_attachment = p_attachment;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_attachment:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"pp_attachment\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_allocate_attachment:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Failed to allocate attachment in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"name\" property of renderer object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
            
            description_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"description\" property of renderer object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
            
            format_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"format\" property of renderer object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"name\" property of renderer object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"name\" property of renderer object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            description_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"description\" property of renderer object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            description_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] \"description\" property of renderer object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            no_description_property:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" is missing required property \"description\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;

            no_format_property:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Parameter \"p_value\" is missing required property \"format\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/renderer.json\n");
                #endif

                // Error
                return 0;
        }
    }
}

int renderer_render ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Initialized data
    renderer *p_renderer = p_instance->context.p_renderer;
    size_t i = 0;

    // Cull objects
    // cull_scene(&p_result, RENDERER_OBJECT_MAX, p_instance->context.p_scene);

    // Sort objects
    // [ ??? ]

    // Clear the framebuffer
    glClearColor(
        p_instance->context.p_renderer->clear_color.x,
        p_instance->context.p_renderer->clear_color.y,
        p_instance->context.p_renderer->clear_color.z,
        1.f
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Iterate through each render pass
    for ( i = 0; i < p_renderer->render_pass_quantity; i++ )
        
        // Render the render pass
        if ( renderer_pass_render(p_renderer, p_renderer->_p_render_passes[i]) == 0 ) goto failed_to_render_pass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Rendering errors
        {
            failed_to_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Failed to render render pass \"%s\" in call to function \"%s\"\n", &p_instance->context.p_renderer->_p_render_passes[i]->_name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int renderer_present ( g_instance *p_instance )
{
    
    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // Present the window
    SDL_GL_SwapWindow(p_instance->window.sdl2.window);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int renderer_pass_render ( renderer *p_renderer, render_pass *p_render_pass )
{

    // Argument check
    if ( p_renderer    == (void *) 0 ) goto no_renderer;
    if ( p_render_pass == (void *) 0 ) goto no_render_pass;

    // Initialized data
    size_t i = 0;

    // Iterate through each shader
    for ( i = 0; i < p_render_pass->shader_quantity; i++ )
    {
        
        // Initialized data
        shader *p_shader = p_render_pass->_p_shaders[i];

        // Bind the shader
        //
        
        // Bind each material
        //
        
            // Draw each object
            //
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Rendering errors
        {
            failed_to_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Failed to render render pass \"%s\" in call to function \"%s\"\n", &p_renderer->_p_render_passes[i]->_name, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
