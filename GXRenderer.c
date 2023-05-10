#include <G10/GXRenderer.h>

// Forward declared functions
int draw_material();

// Constants
#define ATTACHMENT_LOAD_OPERATION_COUNT  2
#define ATTACHMENT_STORE_OPERATION_COUNT 2
#define IMAGE_LAYOUTS_COUNT              18
#define SUBPASS_FUNCTION_COUNT           11

VkAttachmentLoadOp attachment_load_operation_enums[ATTACHMENT_LOAD_OPERATION_COUNT]   = {
    VK_ATTACHMENT_LOAD_OP_LOAD,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
};
char *attachment_load_operation_names[ATTACHMENT_LOAD_OPERATION_COUNT]   = {
    "load",
    "clear",
};

VkAttachmentStoreOp  attachment_store_operation_enums[ATTACHMENT_STORE_OPERATION_COUNT] = {
    VK_ATTACHMENT_STORE_OP_STORE,
    VK_ATTACHMENT_STORE_OP_NONE,
};
char *attachment_store_operation_names[ATTACHMENT_STORE_OPERATION_COUNT] = {
    "store",
    "none",
};

VkImageLayout image_layout_enums[IMAGE_LAYOUTS_COUNT] = {
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    VK_IMAGE_LAYOUT_PREINITIALIZED,
    VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL,
    VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
};
char *image_layout_names[IMAGE_LAYOUTS_COUNT] = {
    "undefined",
    "general",
    "color attachment",
    "depth stencil attachment",
    "depth stencil read only",
    "shader read only",
    "transfer source",
    "transfer destination",
    "pre initialized",
    "depth read only stencil attachment",
    "depth attachment stencil read only",
    "depth attachment",
    "depth read only",
    "stencil attachment",
    "stencil read only",
    "read only",
    "attachment",
    "present source"
};

void *subpass_function_callbacks[SUBPASS_FUNCTION_COUNT] = {
    &draw_scene,
    &draw_material,
    0,//&draw_depth_pass,
    0,//&draw_direct_lighting,
    0,//&draw_shadows,
    0,//&draw_translucency,
    0,//&draw_indirect_lighting,
    0,//&draw_depth_of_field,
    0,//&draw_motion_blur,
    0,//&draw_bloom_blur,
    0,//&draw_tone_map_gamma_correct
};
char *subpass_function_names[SUBPASS_FUNCTION_COUNT] = {
    "draw scene",
    "material output",
    "depth pass",
    "direct lighting",
    "shadows",
    "translucency",
    "indirect lighting",
    "depth of field",
    "motion blur",
    "bloom blur",
    "tone map gamma correct"
};

dict *attachment_load_operations  = 0,
     *image_layouts               = 0,
     *subpass_functions           = 0,
     *attachment_store_operations = 0;

void init_renderer ( void )
{

    // Construct lookup tables
    dict_construct(&attachment_load_operations , ATTACHMENT_LOAD_OPERATION_COUNT);
    dict_construct(&attachment_store_operations, ATTACHMENT_STORE_OPERATION_COUNT);
    dict_construct(&image_layouts              , IMAGE_LAYOUTS_COUNT);
    dict_construct(&subpass_functions          , SUBPASS_FUNCTION_COUNT);

    // Populate lookup tables
    {
        
        // Populate attachment load operations
        for (size_t i = 0; i < ATTACHMENT_LOAD_OPERATION_COUNT; i++)
            dict_add(attachment_load_operations, attachment_load_operation_names[i], (void*)attachment_load_operation_enums[i]);

        // Populate attachment store operations
        for (size_t i = 0; i < ATTACHMENT_STORE_OPERATION_COUNT; i++)
            dict_add(attachment_store_operations, attachment_store_operation_names[i], (void*)attachment_store_operation_enums[i]);

        for (size_t i = 0; i < IMAGE_LAYOUTS_COUNT; i++)
            dict_add(image_layouts, image_layout_names[i], (void *)image_layout_enums[i]);

        for (size_t i = 0; i < SUBPASS_FUNCTION_COUNT; i++)
            dict_add(subpass_functions, subpass_function_names[i], (void *)subpass_function_callbacks[i]);
    }

    return;
}

int create_renderer ( GXRenderer_t   **pp_renderer )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 )
                goto no_ret;
        #endif
    }

    // Initialized data
    GXRenderer_t *p_renderer = calloc(1, sizeof(GXRenderer_t));

    // Error checking
    if ( p_renderer == (void *) 0 )
        goto no_mem;
    
    // Return a pointer to the caller
    *pp_renderer = p_renderer;

    // Successs
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int create_render_pass ( GXRenderPass_t **pp_render_pass )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_render_pass == (void *) 0 )
                goto no_ret;
        #endif
    }

    // Initialized data
    GXRenderPass_t *p_render_pass = calloc(1, sizeof(GXRenderPass_t));

    // Error checking
    if ( p_render_pass == (void *) 0 )
        goto no_mem;

    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int create_subpass ( GXSubpass_t **pp_subpass )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (pp_subpass == (void *) 0)
                goto no_ret;
        #endif
    }

    // Initialized data
    GXSubpass_t *p_subpass = calloc(1, sizeof(GXSubpass_t));

    // Error checking
    if ( p_subpass == (void *) 0 )
        goto no_mem;

    // Return a pointer to the caller
    *pp_subpass = p_subpass;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_renderer ( GXRenderer_t **pp_renderer, char* path )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 )
                goto no_renderer;
            if ( path == (void *) 0 )
                goto no_path;
        #endif
    }

    // Initialized data
    size_t  len    = g_load_file(path, 0, true);
    char   *buffer = calloc(1+len, sizeof(char));

    // Error checking
    if ( buffer == (void *) 0 )
        goto no_mem;

    // Load the file
    if ( g_load_file(path, buffer, true) == 0 )
        goto failed_to_read_file;

    // Construct a renderer
    if ( load_renderer_as_json(pp_renderer, buffer) == 0 )
        goto failed_to_load_renderer;

    // Clean the scope
    free(buffer);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load renderer from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_renderer_as_json ( GXRenderer_t **pp_renderer, char *text )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 )
                goto no_renderer;
            if ( text == (void *) 0 )
                goto no_text;
        #endif
    }

    // Initialized data
    GXRenderer_t *p_renderer = 0;
    JSONValue_t  *p_value    = 0;

    // Parse the JSON text into a JSON value      
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto failed_to_parse_json;

    // Load the renderer from the JSON value
    if ( load_renderer_as_json_value(pp_renderer, p_value) == 0 )
        goto failed_to_construct_renderer_from_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_construct_renderer_from_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct renderer in in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // JSON errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_renderer_as_json_value ( GXRenderer_t **pp_renderer, JSONValue_t *p_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 )
                goto no_return;
            if ( p_value  == (void *) 0 )
                goto no_value;
        #endif
    }

    // Initialized data
    GXRenderer_t *p_renderer    = 0;
    JSONValue_t  *p_name        = 0,
                 *p_passes      = 0,
                 *p_clear_color = 0;

    // Parse the value as an object
    if ( p_value->type == JSONobject )
    {

        p_name        = dict_get(p_value->object, "name");
        p_passes      = dict_get(p_value->object, "passes");
        p_clear_color = dict_get(p_value->object, "clear color");

        if ( !( p_name && p_passes && p_clear_color ) ) 
            goto missing_parameters;
    }

    // Parse the value as a path
    else if ( p_value->type == JSONstring )
    {
        if ( load_renderer(pp_renderer, p_value->string) == 0 )
            goto faild_to_load_renderer;
    }
    
    // Default
    else
        goto wrong_value_type;

    // Construct the renderer
    {

        // Initialized data
        VkClearValue* clear_color = calloc(2, sizeof(VkClearValue));

        // Allocate a renderer
        if ( create_renderer(&p_renderer) == 0 )
            goto failed_to_allocate_renderer;

        // Copy the name
        if ( p_name->type == JSONstring ){

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for the name
            p_renderer->name = calloc(len + 1, sizeof(char));

            // Error checking
            if ( p_renderer->name == (void *) 0 )
                goto no_mem;

            // Copy the name to the renderer
            strncpy(p_renderer->name, p_name->string, len);
        }
        
        // Parse the clear color as an array
        if ( p_clear_color->type == JSONarray )
        {

            // Initialized data
            size_t        array_len         = 0;
            JSONValue_t **pp_array_contents = 0;

            // Get the array contents
            {

                    // Get the array length
                    array_get(p_clear_color->list, 0, &array_len);

                    // Allocate memory for array
                    pp_array_contents = calloc(array_len, sizeof(JSONValue_t *));

                    // Error checking
                    if ( pp_array_contents == (void *) 0 )
                        goto no_mem;

                    // Dump array to memory
                    array_get(p_clear_color->list, pp_array_contents, 0);
                }

            // Iterate over each array element
            for (size_t i = 0; i < array_len; i++)

                // Set the clear color
                clear_color[0].color.float32[i] = (float) pp_array_contents[i]->floating;

            // Set the depth stencil clear color
            clear_color[1].depthStencil.depth = 1.f;
            clear_color[1].depthStencil.stencil = 0;

            // Store the clear color in the renderer
            p_renderer->clear_colors = clear_color;
        }
        else
            goto wrong_renderer_clear_color;
        
        // Parse each render pass
        if ( p_passes->type == JSONarray )
        {

            // Initialized data
            size_t        array_len         = 0;
            JSONValue_t **pp_array_contents = 0;

            // Get the array contents
            {

                // Get the array length
                array_get(p_passes->list, 0, &array_len);

                // Allocate memory for array
                pp_array_contents = calloc(array_len, sizeof(JSONValue_t *));

                // Error checking
                if ( pp_array_contents == (void *) 0 )
                    goto no_mem;

                // Dump array to memory
                array_get(p_passes->list, pp_array_contents, 0);
            }

            p_renderer->render_pass_count = array_len;

            // Allocate a list of render passes
            p_renderer->render_passes_data = calloc(array_len, sizeof(void *));
            
            // Error checking
            if ( p_renderer->render_passes_data == 0 )
                goto no_mem;

            // Iterate over each render pass
            for (size_t i = 0; i < array_len; i++)

                // Load the render pass as a JSON value
                if ( load_render_pass_as_json_value(&p_renderer->render_passes_data[i], pp_array_contents[i]) == 0 )
                    goto failed_to_load_renderer_as_json_value;
        
        }
        else
            goto wrong_passes_type;

        // Return a pointer to the caller
        *pp_renderer = p_renderer;
    }

    // Success
    return 1;

    // TODO:
    missing_parameters:
    wrong_passes_type:
    failed_to_load_renderer_as_json_value:
    wrong_renderer_clear_color:
    wrong_value_type:
    faild_to_load_renderer:
        // Error
        return 0;

    // Error handling
    {

        // Argument errors
        {
            
            no_return:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_allocate_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to allocate renderer in in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_render_pass ( GXRenderPass_t **pp_render_pass, char* path )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_render_pass == (void *) 0 )
                goto no_render_pass;
            if ( path == (void *) 0 )
                goto no_path;
        #endif
    }

    // Initialized data
    size_t  len    = g_load_file(path, 0, false);
    char   *buffer = calloc(1+len, sizeof(char));

    // Error checking
    if (buffer == (void *)0)
        goto no_mem;

    // Load the file
    if ( g_load_file(path, buffer, false) == 0 )
        goto failed_to_read_file;

    // Construct a render pass
    if ( load_render_pass_as_json(pp_render_pass, buffer) == 0 )
        goto failed_to_load_render_pass;

    // Clean the scope
    free(buffer);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load render pass from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_render_pass_as_json ( GXRenderPass_t **pp_render_pass, char* text )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_render_pass == (void *) 0 )
                goto no_render_pass;
            if ( text == (void *) 0 )
                goto no_text;
        #endif
    }

    // Initialized data
    GXInstance_t   *p_instance    = g_get_active_instance();
    GXRenderPass_t *p_render_pass = 0;
    JSONValue_t    *p_value       = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto failed_to_parse_json_as_value;

    // Load the renderer as a JSON value
    if ( load_render_pass_as_json_value(pp_render_pass, p_value) == 0 )
        goto failed_to_load_renderer_as_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // TODO:
    failed_to_parse_json_as_value:
    failed_to_load_renderer_as_json_value:
    
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int load_render_pass_as_json_value ( GXRenderPass_t **pp_render_pass, JSONValue_t *p_value )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_render_pass == (void *) 0 )
                goto no_render_pass;
            if ( p_value == (void *) 0 )
                goto no_value;
        #endif
    }

    // Initialized data
    GXInstance_t   *p_instance    = g_get_active_instance();
    GXRenderPass_t *p_render_pass = 0;
    JSONValue_t    *p_name        = 0, 
                   *p_attachments = 0,
                   *p_subpasses   = 0;

    // Parse the render pass JSON value
    if ( p_value->type == JSONobject ) {

        p_name        = dict_get(p_value->object, "name");
        p_attachments = dict_get(p_value->object, "attachments");
        p_subpasses   = dict_get(p_value->object, "subpasses");

        // Check for missing parameters
        if ( ! ( p_name && p_attachments && p_subpasses ) )
            goto missing_parameters;
    }
    else
        goto wrong_type;

    // Construct the render pass
    {

        // Initialized data
        VkRenderPassCreateInfo   render_pass_create_info = { 0 };
        VkSubpassDependency      dependency              = { 0 };
        size_t                   attachment_count        = 0,
                                 subpass_count           = 0;
        VkAttachmentDescription *attachments             = 0;
        VkAttachmentReference   *attachment_references   = 0;
        VkSubpassDescription    *subpasses               = 0;

        // Allocate memory for a render pass
        if ( create_render_pass(&p_render_pass) == 0 )
            goto failed_to_create_render_pass;

        // Set the name
        if ( p_name->type == JSONstring ) {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for name
            p_render_pass->name = calloc(len + 1, sizeof(char));

            // Error checking
            if ( p_render_pass->name == (void *) 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_render_pass->name, p_name->string, len);

        }
        
        /*
        // Populate the attachments
        if( attachments_text ){

            // Check for zero attachments
            {
                if (*attachments_text == (void*)0)
                    goto no_attachments;
            }

            // Count up each attachment
            while (attachments_text[++attachment_count]);

            // Allocate memory for attachments and subpasses
            attachments           = calloc(attachment_count, sizeof(VkAttachmentDescription));
            subpasses             = calloc(attachment_count, sizeof(VkSubpassDescription));
            attachment_references = calloc(attachment_count, sizeof(VkAttachmentReference));
            p_render_pass->image_attachments = calloc(attachment_count, sizeof(VkImageView));
            dict_construct(&p_render_pass->attachments, attachment_count);

            // Iterate over each attachment JSON object text
            for (size_t i = 0; i < attachment_count; i++)
            {

                // Initialized data
                char   *attachment_text         = attachments_text[i];
                size_t  attachment_text_len     = strlen(attachment_text);
                dict   *attachment_dict         = 0;
                char   *name                    = 0,
                       *samples                 = 0,
                       *load_operation          = 0,
                       *store_operation         = 0,
                       *stencil_load_operation  = 0,
                       *stencil_store_operation = 0,
                       *initial_layout          = 0,
                       *final_layout = 0;

                // Parse the attachment JSON object into a dictioanry
                parse_json(attachment_text, attachment_text_len, &attachment_dict);

                // Get the required information to construct an attachment
                {

                    // Initialized data
                    JSONToken_t* token = 0;

                    // Get the samples for this attachment
                    token = (JSONToken_t *) dict_get(attachment_dict, "name");
                    name = JSON_VALUE(token, JSONstring);

                    // Get the samples for this attachment
                    token = (JSONToken_t *) dict_get(attachment_dict, "samples");
                    samples = JSON_VALUE(token, JSONprimative);

                    // Get the load operation
                    token = (JSONToken_t *) dict_get(attachment_dict, "load operation");
                    load_operation = JSON_VALUE(token, JSONstring);

                    // Get the store operation
                    token = (JSONToken_t *) dict_get(attachment_dict, "store operation");
                    store_operation = JSON_VALUE(token, JSONstring);

                    // Get the stencil load operation
                    token = (JSONToken_t *) dict_get(attachment_dict, "stencil load operation");
                    stencil_load_operation = JSON_VALUE(token, JSONstring);

                    // Get the stencil store operation
                    token = (JSONToken_t *) dict_get(attachment_dict, "stencil store operation");
                    stencil_store_operation = JSON_VALUE(token, JSONstring);

                    // Get the initial layout
                    token = (JSONToken_t *) dict_get(attachment_dict, "initial layout");
                    initial_layout = JSON_VALUE(token, JSONstring);

                    // Get the final layout
                    token = (JSONToken_t *) dict_get(attachment_dict, "final layout");
                    final_layout = JSON_VALUE(token, JSONstring);
                
                }

                // Populate the attachment struct
                {

                    // Set the attachment format
                    attachments[i].format = p_instance->vulkan.swap_chain_image_format;

                    if ( strcmp(name, "depth") == 0 )
                        attachments[i].format = VK_FORMAT_D32_SFLOAT;

                    // Set the attachment samples
                    attachments[i].samples        = atoi(samples);

                    // Set the attachment load operation
                    attachments[i].loadOp         = (load_operation)          ? (VkAttachmentLoadOp)  dict_get(attachment_load_operations, load_operation)           : VK_ATTACHMENT_LOAD_OP_DONT_CARE;

                    // Set the attachment store operation
                    attachments[i].storeOp        = (store_operation)         ? (VkAttachmentStoreOp) dict_get(attachment_store_operations, store_operation)         : VK_ATTACHMENT_STORE_OP_DONT_CARE;

                    // Set the stencil load operation, if there is a stencil load operation; default to VK_ATTACHMENT_LOAD_OP_DONT_CARE
                    attachments[i].stencilLoadOp  = (stencil_load_operation)  ? (VkAttachmentLoadOp)  dict_get(attachment_load_operations, stencil_load_operation)   : VK_ATTACHMENT_LOAD_OP_DONT_CARE;

                    // Set the stencil store operation, if there is a stencil store operation; default to VK_ATTACHMENT_STORE_OP_DONT_CARE
                    attachments[i].stencilStoreOp = (stencil_store_operation) ? (VkAttachmentStoreOp) dict_get(attachment_store_operations, stencil_store_operation) : VK_ATTACHMENT_STORE_OP_DONT_CARE;

                    attachments[i].initialLayout  = (initial_layout)          ? (VkAttachmentStoreOp) dict_get(image_layouts, initial_layout)                        : VK_IMAGE_LAYOUT_UNDEFINED;

                    attachments[i].finalLayout    = (final_layout)            ? (VkAttachmentStoreOp) dict_get(image_layouts, final_layout)                          : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                    // Create the GXAttachment
                    {

                        // Initialized data
                        GXAttachment_t* a = calloc(1, sizeof(GXAttachment_t));

                        // Copy the name
                        {

                            // Initialized data
                            size_t len = strlen(name);

                            // Allocate memory for name string
                            a->name = calloc(len + 1, sizeof(char));

                            // Copy the string
                            strncpy(a->name, name, len);

                        }

                        // Create a texture
                        {

                            // Initialized data
                            GXTexture_t *t = 0;

                            create_texture(&t);

                            if (strcmp(a->name, "color")==0)
                            {
                                t->texture_image = p_instance->vulkan.swap_chain_images[i];
                                construct_image_view(t, VK_IMAGE_VIEW_TYPE_2D, p_instance->vulkan.swap_chain_image_format, (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY }, VK_IMAGE_ASPECT_COLOR_BIT);

                            }
                            else
                            {
                                construct_image(t, 0, VK_IMAGE_TYPE_2D, VK_FORMAT_D32_SFLOAT, p_instance->vulkan.swap_chain_extent.width, p_instance->vulkan.swap_chain_extent.height, 1, 1, 1, 1, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
                                construct_image_view(t, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_D32_SFLOAT, (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY }, VK_IMAGE_ASPECT_DEPTH_BIT);

                            }
                               
                            p_render_pass->image_attachments[i] = t->texture_image_view;
                            if (i == 0)
                                p_render_pass->image_attachments[i] = p_instance->vulkan.swap_chain_image_views[p_instance->vulkan.image_index];
                        }

                        dict_add(p_render_pass->attachments, name, (void *)i);
                    }
                }
            }
        }

        if( subpasses_text ){
            // Count up subpasses
            while (subpasses_text[++subpass_count]);
            p_render_pass->subpasses_count = subpass_count;

            // Allocate subpass list memory
            p_render_pass->subpasses_data = calloc(subpass_count, sizeof(GXSubpass_t));

            // Iterate over each subpass JSON object text
            for (size_t i = 0; i < subpass_count; i++)
            {

                // Initialized data
                char                   *subpass_text                    = subpasses_text[i];
                size_t                  subpass_text_len                = strlen(subpass_text),
                                        input_attachment_count          = 0,
                                        color_attachment_count          = 0,
                                        depth_attachment_count          = 0,
                                        preserved_attachment_count      = 0;
                dict                   *subpass_dict                    = 0;
                char                   *name                            = 0,
                                      **input_attachments               = 0,
                                      **color_attachments               = 0,
                                       *depth_attachment                = 0,
                                      **preserved_attachments           = 0;
                VkAttachmentReference  *input_attachment_references     = calloc(input_attachment_count    , sizeof(VkAttachmentReference)),
                                       *color_attachment_references     = calloc(color_attachment_count    , sizeof(VkAttachmentReference)),
                                       *preserved_attachment_references = calloc(preserved_attachment_count, sizeof(VkAttachmentReference)),
                                       *depth_attachment_reference      = calloc(1                         , sizeof(VkAttachmentReference));

                // Parse the attachment JSON object into a dictioanry
                parse_json(subpass_text, subpass_text_len, &subpass_dict);

                // Get the required information to construct an attachment
                {

                    // Initialized data
                    JSONToken_t* token = 0;

                    // Get the name of this subpass
                    token = (JSONToken_t *) dict_get(subpass_dict, "name");
                    name = JSON_VALUE(token, JSONstring);

                    // Get the input attachments
                    token = (JSONToken_t *) dict_get(subpass_dict, "input attachments");
                    input_attachments = JSON_VALUE(token, JSONarray);

                    // Get the color attachments
                    token = (JSONToken_t *) dict_get(subpass_dict, "color attachments");
                    color_attachments = JSON_VALUE(token, JSONarray);

                    // Get the depth attachments
                    token = (JSONToken_t *) dict_get(subpass_dict, "depth attachments");
                    depth_attachment = JSON_VALUE(token, JSONstring);

                    // Get the preserved attachments
                    token = (JSONToken_t *) dict_get(subpass_dict, "preserved attachments");
                    preserved_attachments = JSON_VALUE(token, JSONarray);

                }

                // Count up attachment types
                {
                    if (input_attachments)
                        if (*input_attachments)
                            while (input_attachments[++input_attachment_count]);

                    if (color_attachments)
                        if (*color_attachments)
                            while (color_attachments[++color_attachment_count]);
        
                    if (preserved_attachments)
                        if (*preserved_attachments)
                            while (preserved_attachments[++preserved_attachment_count]);
                        
                    subpasses[i].inputAttachmentCount = (u32)input_attachment_count;
                    subpasses[i].colorAttachmentCount = (u32)color_attachment_count;
                    subpasses[i].preserveAttachmentCount = (u32)preserved_attachment_count;
                }

                // Set each attachment
                {


                    // Iterate over each input attachment
                    for (size_t j = 0; j < input_attachment_count; j++)
                    {
                        input_attachment_references[j].attachment = (u32)(size_t) dict_get(p_render_pass->attachments, input_attachments[j]);
                        input_attachment_references[j].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                    }
                    
                    // Iterate over each color attachment
                    for (size_t j = 0; j < color_attachment_count; j++)
                    {
                        color_attachment_references[j].attachment = (u32)(size_t)dict_get(p_render_pass->attachments, color_attachments[j]);
                        color_attachment_references[j].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                    }

                    // Iterate over each preserved attachment
                    for (size_t j = 0; j < preserved_attachment_count; j++)
                        preserved_attachment_references[j].attachment = (u32)(size_t)dict_get(p_render_pass->attachments, preserved_attachments[j]);

                    depth_attachment_reference->attachment = (u32)(size_t)dict_get(p_render_pass->attachments, "depth");
                    depth_attachment_reference->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                }

                // Create a GXSubpass_t 
                {
                    GXSubpass_t* s = 0;
                    create_subpass(&s);
                    s->name = calloc(strlen(name) + 1, sizeof(char));
                    strncpy(s->name, name, strlen(name));
                    p_render_pass->subpasses_data[i] = s;
                }

                {
                    // Initialized data
                    subpasses[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

                    subpasses[i].inputAttachmentCount = (u32)input_attachment_count;

                    if (input_attachment_count)
                        subpasses[i].pInputAttachments = input_attachment_references;

                    subpasses[i].colorAttachmentCount = (u32)color_attachment_count;

                    if (color_attachment_count)
                        subpasses[i].pColorAttachments = color_attachment_references;

                    subpasses[i].pDepthStencilAttachment = depth_attachment_reference;

                    subpasses[i].preserveAttachmentCount = (u32)preserved_attachment_count;

                    if (preserved_attachment_count)
                        subpasses[i].pPreserveAttachments = (u32*) preserved_attachments;


                }

            }

        }
        */
        
        dependency = (VkSubpassDependency) { 
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .srcAccessMask = 0,
            .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        };

        // Populate render pass create info struct
        render_pass_create_info = (VkRenderPassCreateInfo) {
            .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .attachmentCount = (u32)attachment_count,
            .pAttachments    = attachments,
            .subpassCount    = (u32)subpass_count,
            .pSubpasses      = subpasses,
            .dependencyCount = 1,
            .pDependencies   = &dependency
        };

        if ( vkCreateRenderPass(p_instance->vulkan.device, &render_pass_create_info, 0, &p_render_pass->render_pass) != VK_SUCCESS ) {
            g_print_error("failed to create render pass!\n");
        }
    }
    
    p_render_pass->framebuffers = calloc(p_instance->vulkan.image_count, sizeof(GXFramebuffer_t));

    for (size_t i = 0; i < p_instance->vulkan.image_count; i++)
    {

        // Initialized data
        VkImageView attachments[2] = {
            p_instance->vulkan.swap_chain_image_views[i],
            p_render_pass->image_attachments[1]
        };

        VkFramebufferCreateInfo framebuffer_create_info = { 
            .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass      = p_render_pass->render_pass,
            .attachmentCount = 2,
            .pAttachments    = attachments,
            .width           = p_instance->vulkan.swap_chain_extent.width,
            .height          = p_instance->vulkan.swap_chain_extent.height,
            .layers          = 1
        };

        if ( vkCreateFramebuffer(p_instance->vulkan.device, &framebuffer_create_info, 0, &p_render_pass->framebuffers[i].framebuffer) != VK_SUCCESS )
            goto failed_to_create_render_pass;

    }
    
    // Return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // Success
    return 1;

    // TODO:
    wrong_type:
    missing_parameters:
    no_mem:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Vulkan errors
        {

            failed_to_create_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create render pass in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int renderer_info ( GXRenderer_t *p_renderer)
{

    // Argument check
    {
        #ifndef NDEBUG
            if (p_renderer == (void *)0)
                goto no_renderer;
        #endif
    }

    // Formatting 
    g_print_log(" - Renderer info - \n");
    
    // Print the name
    g_print_log("name          : \"%s\"\n", p_renderer->name);

    // Formatting 
    g_print_log("render passes :\n");

    // Iterate over each render pass
    for (size_t i = 0; i < p_renderer->render_pass_count; i++)
    {
        
        // Initialized data
        GXRenderPass_t *rp = p_renderer->render_passes_data[i];

        // Print the name of each render pass
        g_print_log("\t[%d] \"%s\"\n", i, rp->name);


        // Iterate over each subpass
        for (size_t j = 0; j < rp->subpasses_count; j++)
        {

            // Initialized data
            GXSubpass_t *s = rp->subpasses_data[j];

            // Print the name of each render pass
            g_print_log("\t\t[%d] \"%s\"\n", j, s->name);

        }

    }

    putchar('\n');

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"p_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }
}

int add_subpass_callback ( char *name, void(*function_pointer)( ) )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( name             == (void *) 0 ) goto no_name;
            if ( function_pointer == (void *) 0 ) goto no_function_pointer;
        #endif
    }

    return 1;

    // Error handling
    {

        // Argument errors 
        {
            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
            no_function_pointer:
                    #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }

    }
}

int render_frame ( GXInstance_t *p_instance )
{


    // Initialized data
	VkSemaphore               wait_semaphores[]       = { p_instance->vulkan.image_available_semaphores[p_instance->vulkan.current_frame]};
	VkSemaphore               signal_semaphores[]     = { p_instance->vulkan.render_finished_semaphores[p_instance->vulkan.current_frame]};
	VkPipelineStageFlags      wait_stages[]           = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	
	VkSwapchainKHR            swap_chains[]           = { p_instance->vulkan.swap_chain };
    VkResult                  result;
    u64                       start                   = 0,
                              end                     = 0;

    start = SDL_GetPerformanceCounter();

    // Get the command buffer ready for drawing
    {

        // Wait for the previous frame to finish rendering
	    vkWaitForFences(p_instance->vulkan.device, 1, &p_instance->vulkan.in_flight_fences[p_instance->vulkan.current_frame], VK_TRUE, UINT64_MAX);
	
        // Grab an image from the swapchain
        result = vkAcquireNextImageKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain, UINT64_MAX, p_instance->vulkan.image_available_semaphores[p_instance->vulkan.current_frame], VK_NULL_HANDLE, &p_instance->vulkan.image_index);
	
        // Make sure the image is usable
        if      ( result == VK_ERROR_OUT_OF_DATE_KHR ) {
            g_window_resize(p_instance);
            goto fail;
        }
        else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR ) {
            printf("Failed to acquire swap chain image!\n");
            goto fail;
        }

        // Only reset the fence if we are submitting work
        vkResetFences(p_instance->vulkan.device, 1, &p_instance->vulkan.in_flight_fences[p_instance->vulkan.current_frame]);

        // Clear out the command buffer
    	vkResetCommandBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0);
	}

    // Draw the frame
    {
        // Initialized data
        GXRenderer_t             *active_renderer   = p_instance->context.renderer;
        VkCommandBufferBeginInfo  begin_info        = { 0 };

        // Set up the command buffer begin info struct
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // Begin the command buffer
        vkBeginCommandBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], &begin_info);

        // Iterate over each render pass
        for (size_t i = 0; i < active_renderer->render_pass_count; i++)
        {

            // Initialized data
            GXRenderPass_t           *rp                     = active_renderer->render_passes_data[i];
            size_t                    subpass_count          = 0;
            size_t       entity_count = dict_values(p_instance->context.scene->entities, 0);
            GXEntity_t** entities     = calloc(entity_count, sizeof(void*));

            VkRenderPassBeginInfo     render_pass_begin_info = 
            {
                .sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .renderPass          = rp->render_pass,
                .framebuffer         = rp->framebuffers[p_instance->vulkan.current_frame].framebuffer,
                .renderArea.offset.x = 0,
                .renderArea.offset.y = 0,
                .renderArea.extent   = p_instance->vulkan.swap_chain_extent,
                .clearValueCount     = 2,
                .pClearValues        = active_renderer->clear_colors
            };

            // Start the render pass
            vkCmdBeginRenderPass(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

            dict_values(p_instance->context.scene->entities, entities);

            // Draw each entity
            for (size_t i = 0; i < entity_count; i++)
                draw_entity(entities[i]);

            free(entities);
            
            // End the render pass
            vkCmdEndRenderPass(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame]);
        }
        
        // End the command buffer
        vkEndCommandBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame]);

    }

    // Submit the commands
    {
        
        // Initialized data
        VkSubmitInfo submit_info = 
        {
            .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .waitSemaphoreCount   = 1,
            .pWaitSemaphores      = &wait_semaphores,
            .pWaitDstStageMask    = &wait_stages,
            .commandBufferCount   = 1,
            .pCommandBuffers      = &p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores    = &signal_semaphores
        };
        VkPresentInfoKHR present_info = 
        {
            .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores    = &signal_semaphores,
            .swapchainCount     = 1,
            .pSwapchains        = swap_chains,
            .pImageIndices      = &p_instance->vulkan.image_index
        };

        // Submit the draw commands
        if ( vkQueueSubmit(p_instance->vulkan.graphics_queue, 1, &submit_info, p_instance->vulkan.in_flight_fences[p_instance->vulkan.current_frame]) )
            g_print_error("Failed to submit draw command buffer!\n");

        // Present the image to the swapchain
        result = vkQueuePresentKHR(p_instance->vulkan.present_queue, &present_info);

        // Does the window need to be resized?
        switch ( result )
        {
            case VK_ERROR_OUT_OF_DATE_KHR:
            case VK_SUBOPTIMAL_KHR:
                g_window_resize(p_instance);
                break;
            case VK_SUCCESS:
                break;

            default:
                goto failed_to_queue_presentation;
        }
    }

    p_instance->vulkan.current_frame = (p_instance->vulkan.current_frame + 1) % p_instance->vulkan.max_buffered_frames;

    fail:

    end = SDL_GetPerformanceCounter();
    p_instance->time.delta_time = (float)(((double)(end - start)) / (double)(p_instance->time.clock_div));
    if(p_instance->time.ticks%144 == 1)
        printf("Render time: %dms                           \r", (int)( p_instance->time.delta_time * 1000.f ));
    p_instance->time.ticks += 1;

    // Success
	return 1;

    // TODO:
    failed_to_queue_presentation:
        return 0;
    
    // Error handling
    {

    }
}

int present_frame ( GXInstance_t *p_instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_instance == (void *) 0 )
                goto no_instance;
        #endif
    }


    return 1;
    no_instance:
    return 0;
}

int draw_material ( GXSubpass_t    **pp_subpass )
{

    return 1;
}
