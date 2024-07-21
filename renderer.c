// Header
#include <g10/renderer.h>

// Function declarations
int renderer_pass_render ( renderer *p_renderer, render_pass *p_render_pass );
int render_pass_from_json ( render_pass **pp_render_pass, const char *p_name, json_value *p_value );
int render_pass_from_json_2 ( render_pass **pp_render_pass, const char *p_name, json_value *p_value );

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
    g_instance *p_instance = g_get_active_instance();
    size_t    i = 0,
              render_pass_quantity = 0;
    renderer *p_renderer = (void *) 0;
    node_graph *p_node_graph = (void *) 0;

    // Allocate memory on the heap
    if ( renderer_create(&p_renderer) == 0 ) goto failed_to_allocate_renderer;

    // Parse the json value into a renderer
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name_value        = dict_get(p_dict, "name"),
                         *p_clear_color_value = dict_get(p_dict, "clear color"),
                         *p_attachments_value = dict_get(p_dict, "attachments");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) circular_buffer_push(p_instance->debug, "[g10] [renderer] Consider adding a \"$schema\" property to the renderer");

        // Error check
        if ( p_name_value        == (void *) 0 ) goto no_name_property;
        if ( p_attachments_value == (void *) 0 ) goto no_attachments_property;

        // Type check
        if ( p_name_value->type        != JSON_VALUE_STRING ) goto name_property_is_wrong_type;
        if ( p_clear_color_value->type != JSON_VALUE_ARRAY  ) goto clear_color_property_is_wrong_type;
        if ( p_attachments_value->type != JSON_VALUE_OBJECT ) goto attachments_property_is_wrong_type;

        // Store the name
        {

            // Initialized data
            size_t len = strlen(p_name_value->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the instance name
            strncpy(p_renderer->_name, p_name_value->string, 255);

            // Store a null terminator
            p_renderer->_name[len] = '\0';
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
            p_renderer->clear_color = (vec3)
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
    
        // Construct a node graph
        if ( node_graph_construct(&p_node_graph, p_value) == 0 ) goto failed_to_construct_node_graph;
    
        // Grow the allocation
        p_renderer = G10_REALLOC(p_renderer, sizeof(renderer) + 16 * sizeof(render_pass *));

        // Error check
        if ( p_renderer == (void *) 0 ) goto no_mem;

        // Construct a renderer from the node graph
        {
            
            // Initialized data
            node *_p_nodes[8] = { 0 };
            size_t j = 0;

            // Construct the elements of the render graph
            for (size_t i = 0; i < p_node_graph->node_quantity; i++)
            {

                // Initialized data
                node *p_node = p_node_graph->_p_nodes[i];
                json_value *p_value = p_node->value;
                dict *p_dict = (void *) 0;
                const char *type = (void *) 0;

                // Store source nodes
                if ( p_node->in_quantity == 0 ) _p_nodes[j++] = p_node_graph->_p_nodes[i];

                // Error check
                if ( p_value == (void *) 0 ) continue;

                // Type check
                if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_node_type;

                // Store the dictionary
                p_dict = p_value->object;

                // Store the type
                type = ((json_value *) dict_get(p_dict, "type"))->string;

                // Error check
                //
                if ( strcmp(type, "render pass") == 0 ) 
                {
                    render_pass_from_json_2(&p_renderer->_p_render_passes[render_pass_quantity], p_node->_name, p_value);

                    render_pass_quantity++;
                }
            }
            
            // Store the quantity of render passes
            p_renderer->render_pass_quantity = render_pass_quantity;
        }
    }
    
    // Return a pointer to the caller
    *pp_renderer = p_renderer;

    // Success
    return 1;

    too_many_attachments:
    too_many_passes:
    failed_to_construct_node_graph:
    wrong_node_type:
    no_node_value:

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

int attachment_from_json ( attachment **pp_attachment, json_value *p_value )
{

    // Argument check
    if ( pp_attachment == (void *)        0 ) goto no_attachment;
    if ( p_value       == (void *)        0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto value_is_wrong_type;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
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
        if ( dict_get(p_dict, "$schema") == 0 ) circular_buffer_push(p_instance->debug, "[g10] [renderer] Consider adding a \"$schema\" property to the attachment");

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

int render_pass_from_json ( render_pass **pp_render_pass, const char *p_name, json_value *p_value )
{

    // Argument check
    if ( pp_render_pass == (void *)        0 ) goto no_render_pass;
    if ( p_value        == (void *)        0 ) goto no_value;
    if ( p_value->type  != JSON_VALUE_OBJECT ) goto value_is_wrong_type;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    render_pass *p_render_pass = (void *) 0;

    // Allocate memory on the heap
    if ( render_pass_create(&p_render_pass) == 0 ) goto failed_to_allocate_render_pass;

    // Parse the json value into a render pass
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_description_value = dict_get(p_dict, "description"),
                         *p_shaders_value     = dict_get(p_dict, "shaders");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) circular_buffer_push(p_instance->debug, "[g10] [renderer] Consider adding a \"$schema\" property to the render pass");

        // Error check
        if ( p_description_value == (void *) 0 ) goto no_name_property;
        
        // Type check
        if ( p_description_value->type != JSON_VALUE_STRING ) goto description_property_is_wrong_type;

        // Store the description
        {

            // Initialized data
            size_t len = strlen(p_description_value->string);

            // Error check
            if ( len == 0   ) goto description_property_is_too_short;
            if ( len  > 255 ) goto description_property_is_too_long;

            // Copy the attachment description
            strncpy(p_render_pass->_description, p_description_value->string, 255);

            // Store a null terminator
            p_render_pass->_name[len] = '\0';
        }

        // Construct shaders
        {

            // Initialized data
            dict       *p_dict          = p_shaders_value->object;
            const char *p_scratch[32]   = { 0 };
            size_t  shader_quantity = dict_keys(p_dict, 0);

            // Error check
            if ( shader_quantity > 32 ) goto too_many_shaders;

            // Get the name of each shader
            dict_keys(p_dict, &p_scratch);

            // Store the quantity of shaders
            p_render_pass->shader_quantity = shader_quantity;

            // Grow the allocation
            p_render_pass = G10_REALLOC(p_render_pass, sizeof(render_pass) + ( shader_quantity * sizeof(shader *)) );

            // Iterate through each shader
            for (size_t i = 0; i < shader_quantity; i++)
            {
                
                // Initialized data
                shader     *p_shader       = (void *) 0;
                json_value *p_shader_value = dict_get(p_dict, p_scratch[i]);
                
                // Construct a shader
                shader_from_json_2(&p_shader, p_scratch[i], p_shader_value);

                // Store the shader
                p_render_pass->_p_shaders[i] = p_shader;
            }
        }
    }

    // Store the name
    {

        // Initialized data
        size_t len = strlen(p_name);

        // Error check
        if ( len == 0   ) goto name_property_is_too_short;
        if ( len  > 255 ) goto name_property_is_too_long;

        // Copy the render pass name
        strncpy(p_render_pass->_name, p_name, 255);

        // Store a null terminator
        p_render_pass->_name[len] = '\0';
    }

    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    too_many_shaders:

        // Error
        return 0;

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
            failed_to_allocate_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Failed to allocate render pass in call to function \"%s\"\n", __FUNCTION__);
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

int render_pass_from_json_2 ( render_pass **pp_render_pass, const char *p_name, json_value *p_value )
{

    // Argument check
    if ( pp_render_pass == (void *)        0 ) goto no_render_pass;
    if ( p_value        == (void *)        0 ) goto no_value;
    if ( p_value->type  != JSON_VALUE_OBJECT ) goto value_is_wrong_type;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    render_pass *p_render_pass = (void *) 0;
    node_graph  *p_node_graph  = (void *) 0;
    size_t shader_quantity = 0;

    // Allocate memory on the heap
    if ( render_pass_create(&p_render_pass) == 0 ) goto failed_to_allocate_render_pass;

    // Construct a node graph
    if ( node_graph_construct(&p_node_graph, p_value) == 0 ) goto failed_to_construct_node_graph;
    
    // Parse the json value into a render pass
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_description_value = dict_get(p_dict, "description"),
                         *p_shaders_value     = dict_get(p_dict, "shaders");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) circular_buffer_push(p_instance->debug, "[g10] [renderer] Consider adding a \"$schema\" property to the render pass");

        // Error check
        if ( p_description_value == (void *) 0 ) goto no_name_property;
        
        // Type check
        if ( p_description_value->type != JSON_VALUE_STRING ) goto description_property_is_wrong_type;

        // Store the description
        {

            // Initialized data
            size_t len = strlen(p_description_value->string);

            // Error check
            if ( len == 0   ) goto description_property_is_too_short;
            if ( len  > 255 ) goto description_property_is_too_long;

            // Copy the attachment description
            strncpy(p_render_pass->_description, p_description_value->string, 255);

            // Store a null terminator
            p_render_pass->_name[len] = '\0';
        }
    }

    p_render_pass = G10_REALLOC(p_render_pass, sizeof(render_pass) + 8 * sizeof(shader *));

    // Construct a render pass from the node graph
    {
        
        // Initialized data
        node *_p_nodes[8] = { 0 };
        size_t j = 0;

        // Construct the elements of the render graph
        for (size_t i = 0; i < p_node_graph->node_quantity; i++)
        {

            // Initialized data
            node *p_node = p_node_graph->_p_nodes[i];
            json_value *p_value = p_node->value;
            dict *p_dict = (void *) 0;
            const char *type = (void *) 0;

            // Store source nodes
            if ( p_node->in_quantity == 0 ) _p_nodes[j++] = p_node_graph->_p_nodes[i];

            // Error check
            if ( p_value == (void *) 0 ) continue;

            // Type check
            //if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_node_type;

            // Store the dictionary
            p_dict = p_value->object;

            // Store the type
            type = ((json_value *) dict_get(p_dict, "type"))->string;

            // Error check
            //

            if ( strcmp(type, "shader") == 0 )
            {

                shader_from_json_2(&p_render_pass->_p_shaders[shader_quantity], p_node->_name, p_value);

                shader_quantity++;
            }
        }

        // Store the quantity of shaders
        p_render_pass->shader_quantity = shader_quantity;
    }

    // Store the name
    {

        // Initialized data
        size_t len = strlen(p_name);

        // Error check
        if ( len == 0   ) goto name_property_is_too_short;
        if ( len  > 255 ) goto name_property_is_too_long;

        // Copy the render pass name
        strncpy(p_render_pass->_name, p_name, 255);

        // Store a null terminator
        p_render_pass->_name[len] = '\0';
    }

    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    too_many_shaders:
    failed_to_construct_node_graph:

        // Error
        return 0;

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
            failed_to_allocate_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Failed to allocate render pass in call to function \"%s\"\n", __FUNCTION__);
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

int renderer_info ( const renderer *const p_renderer )
{

    // Argument check
    if ( p_renderer == (void *) 0 ) goto no_renderer;

    // Print the renderer
    printf("Renderer:\n");
    printf(" - name : %s\n", p_renderer->_name);
    printf(" - passes : \n");

    // Print each render pass
    for (size_t i = 0; i < p_renderer->render_pass_quantity; i++)
    {

        // Initialized data
        render_pass *p_render_pass = p_renderer->_p_render_passes[i];
        
        // Print the render pass
        printf("    [%d]\n", i);
        printf("     - name    : %s\n", p_render_pass->_name);            
        printf("     - shaders : \n");            

        // Print each shader
        for (size_t i = 0; i < p_render_pass->shader_quantity; i++)
        {

            // Initialized data
            shader *p_shader = p_render_pass->_p_shaders[i];
            
            // Print the shader
            printf("        [%d] : %s\n", i, p_shader->_name);            
        }
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
        }
    }
}

int renderer_pass_render ( renderer *p_renderer, render_pass *p_render_pass )
{

    // Argument check
    if ( p_renderer    == (void *) 0 ) goto no_renderer;
    if ( p_render_pass == (void *) 0 ) goto no_render_pass;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    size_t i = 0;

    // Iterate through each shader
    for ( i = 0; i < p_render_pass->shader_quantity; i++ )
    {
        
        // Initialized data
        shader *p_shader = p_render_pass->_p_shaders[i];
        fn_shader_on_bind pfn_shader_on_bind = p_shader->functions.pfn_shader_on_bind;
        fn_shader_on_draw pfn_shader_on_draw = p_shader->functions.pfn_shader_on_draw;

        // Bind the shader
        shader_bind(p_shader);

        // Bind the camera
        pfn_shader_on_bind(p_shader, (void *)p_instance->context.p_scene->context.p_camera);

        // Draw each object
        for (size_t i = 0; i < p_shader->count; i++)
        {

            // Initialized data
            mat4 _m = { 0 };
            mesh_data *p_mesh_data = p_shader->_p_draw_items[i];
            
            // Store the local matrix
            transform_get_matrix_local(p_mesh_data->p_transform, &_m);
            
            // Bind the model matrix
            glUniformMatrix4fv(glGetUniformLocation(p_shader->opengl.program, "M"), 1, GL_FALSE, &_m);
            
            // Draw the mesh
            pfn_shader_on_draw(p_mesh_data);
        }
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
