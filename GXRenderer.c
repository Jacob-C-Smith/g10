#include <G10/GXRenderer.h>

// Forward declared functions
int draw_material();

// Constants
#define ATTACHMENT_LOAD_OPERATION_COUNT  2
#define ATTACHMENT_STORE_OPERATION_COUNT 2
#define IMAGE_LAYOUTS_COUNT              18
#define SUBPASS_FUNCTION_COUNT           11

VkAttachmentLoadOp   attachment_load_operation_enums[ATTACHMENT_LOAD_OPERATION_COUNT]   = {
    VK_ATTACHMENT_LOAD_OP_LOAD,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
};
char                *attachment_load_operation_names[ATTACHMENT_LOAD_OPERATION_COUNT]   = {
    "load",
    "clear",
};
dict                *attachment_load_operations                                         = 0;

VkAttachmentStoreOp  attachment_store_operation_enums[ATTACHMENT_STORE_OPERATION_COUNT] = {
    VK_ATTACHMENT_STORE_OP_STORE,
    VK_ATTACHMENT_STORE_OP_NONE,
};
char                *attachment_store_operation_names[ATTACHMENT_STORE_OPERATION_COUNT] = {
    "store",
    "none",
};
dict                *attachment_store_operations                                        = 0;

VkImageLayout        image_layout_enums[IMAGE_LAYOUTS_COUNT]                            = {
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
char                *image_layout_names[IMAGE_LAYOUTS_COUNT]                            = {
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
dict                *image_layouts                                                      = 0;

void                *subpass_function_callbacks[SUBPASS_FUNCTION_COUNT]                 = {
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
char                *subpass_function_names[SUBPASS_FUNCTION_COUNT]                     = {
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
dict                *subpass_functions                                                  = 0;

void          init_renderer                ( void )
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

int           create_renderer              ( GXRenderer_t   **pp_renderer )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (pp_renderer == (void *) 0)
                goto no_ret;
        #endif
    }

    // Initialized data
    GXRenderer_t *p_renderer = calloc(1, sizeof(GXRenderer_t));

    // Error checking
    {
        #ifndef NDEBUG
            if (p_renderer == (void*)0)
                goto no_mem;
        #endif
    }
    
    *pp_renderer = p_renderer;

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

    }
}

int           create_render_pass           ( GXRenderPass_t **pp_render_pass )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (pp_render_pass == (void *) 0)
                goto no_ret;
        #endif
    }

    // Initialized data
    GXRenderPass_t *p_render_pass = calloc(1, sizeof(GXRenderPass_t));

    // Error checking
    {
        #ifndef NDEBUG
            if (p_render_pass == (void*)0)
                goto no_mem;
        #endif
    }

    *pp_render_pass = p_render_pass;

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

    }
}

int           create_subpass               ( GXSubpass_t    **pp_subpass )
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
    {
        #ifndef NDEBUG
            if (p_subpass == (void*)0)
                goto no_mem;
        #endif
    }

    *pp_subpass = p_subpass;

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ret:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

    }
}

int           load_renderer                ( GXRenderer_t   **pp_renderer   , char* path )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if (pp_renderer == (void *)0)
                goto no_renderer;
            if (path == (void *)0)
                goto no_path;
        #endif
    }

    // Initialized data
    size_t  len    = g_load_file(path, 0, true);
    char   *buffer = calloc(1+len, sizeof(char));

    // Error checking
    {
        #ifndef NDEBUG
            if (buffer == (void *)0)
                goto no_mem;
        #endif
    }

    // Load the file
    if ( g_load_file(path, buffer, true) == 0 )
        goto failed_to_read_file;

    // Construct a renderer
    if ( load_renderer_as_json(pp_renderer, buffer, len) == 0 )
        goto failed_to_load_renderer;

    // Free resources
    free(buffer);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
                #endif
                return 0;

            failed_to_load_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load renderer from file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int           load_renderer_as_json        ( GXRenderer_t   **pp_renderer   , char* token_text, size_t len )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *)0 )
                goto no_return;
            if ( token_text  == (void *)0 )
                goto no_token_text;
            if (len == 0)
                goto no_len;
        #endif
    }

    // Initialized data
    GXRenderer_t  *p_renderer    = 0;
    dict          *renderer_dict = 0;
    char          *name          = 0,
                 **passes        = 0, 
                 **clear_color   = 0;

    // Parse the JSON text into a dictionary         
    parse_json(token_text, len, &renderer_dict);

    // Parse the JSON dictionary 
    {

        // Initialized data
        JSONToken_t *token = 0;

        // Get the name
        token  = dict_get(renderer_dict, "name");
        name   = JSON_VALUE(token, JSONstring);

        // Get the list of render passes
        token  = dict_get(renderer_dict, "passes");
        passes = JSON_VALUE(token, JSONarray);

        // Get the clear color
        token              = dict_get(renderer_dict, "clear color");
        clear_color        = JSON_VALUE(token, JSONarray);

    }

    // Construct the renderer
    {

        // Allocate a renderer
        if ( create_renderer(pp_renderer) == 0 )
            goto failed_to_allocate_renderer;

        p_renderer = *pp_renderer;

        // Copy the name
        {

            // Initialized data
            size_t len = strlen(name);

            // Allocate memory for the name
            p_renderer->name = calloc(len + 1, sizeof(char));

            // Copy the name to the renderer
            strncpy(p_renderer->name, name, len);
        }

        VkClearValue* clear_c = calloc(2, sizeof(VkClearValue));

        // Set the clear color
        if(clear_color){
            clear_c[0].color.float32[0] = atof(clear_color[0]);
            clear_c[0].color.float32[1] = atof(clear_color[1]);
            clear_c[0].color.float32[2] = atof(clear_color[2]);
            clear_c[0].color.float32[3] = atof(clear_color[3]);

            clear_c[1].depthStencil.depth = 1.f;
            clear_c[1].depthStencil.stencil = 0;


            p_renderer->clear_colors = clear_c;
        }
        else
        {
            clear_c->color.float32[0] = 1.f,
            clear_c->color.float32[1] = 1.f,
            clear_c->color.float32[2] = 1.f,
            clear_c->color.float32[3] = 1.f;

            p_renderer->clear_colors = clear_c;
        }

        // Parse each render pass
        {

            // Initialized data
            size_t render_passes_count = 0;

            // Count up the render passes
            while (passes[++render_passes_count]);

            p_renderer->render_pass_count = render_passes_count;

            // Allocate a list of render passes
            p_renderer->render_passes_data = calloc(render_passes_count, sizeof(void *));
            
            // Error checking
            {
                #ifndef NDEBUG
                    if (p_renderer->render_passes_data == 0)
                        goto no_mem;
                #endif
            }

            // Iterate over each render pass
            for (size_t i = 0; i < render_passes_count; i++)
            {
                
                // Parse render pass JSON
                if (*passes[i] == '{')
                    load_render_pass_as_json(&p_renderer->render_passes_data[i], passes[i], strlen(passes[i]));

                // Load a render pass JSON file
                else
                    load_render_pass(&p_renderer->render_passes_data[i], passes[i], strlen(passes[i]));

            }

        }
    }

    return 1;

    // Error handling
    {

        // JSON errors
        {
            failed_to_parse_json:
                    #ifndef NDEBUG
                        g_print_error("[G10] [Renderer] Failed to parse JSON in call to function \"%s\"\n", __FUNCSIG__);
                    #endif  
                return 0;
        }

        // Argument errors
        {
            
            no_return:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_renderer\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_token_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_len:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Zero provided for \"len\" in in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // G10 Errors
        {
            failed_to_allocate_renderer:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to allocate renderer in in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int           load_render_pass             ( GXRenderPass_t **pp_render_pass, char* path )
{

    // Argument check
    {
        #ifndef NDEBUG
            if (pp_render_pass == (void *)0)
                goto no_render_pass;
            if (path == (void *)0)
                goto no_path;
        #endif
    }

    // Initialized data
    size_t  len    = g_load_file(path, 0, false);
    char   *buffer = calloc(1+len, sizeof(char));

    // Error checking
    {
        #ifndef NDEBUG
            if (buffer == (void *)0)
                goto no_mem;
        #endif
    }

    // Load the file
    if ( g_load_file(path, buffer, false) == 0 )
        goto failed_to_read_file;

    // Construct a render pass
    if ( load_render_pass_as_json(pp_render_pass, buffer, len) == 0 )
        goto failed_to_load_render_pass;

    // Free resources
    free(buffer);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // G10 errors
        {
            failed_to_read_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
                #endif
                return 0;

            failed_to_load_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load render pass from file \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
                #endif
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int           load_render_pass_as_json     ( GXRenderPass_t **pp_render_pass, char* token_text, size_t len )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if (pp_render_pass == (void *) 0)
                goto no_render_pass;
            if (token_text == (void*)0)
                goto no_token_text;
        #endif

        if (len == 0)
            goto no_len;
    }

    // Initialized data
    GXInstance_t            *instance                = g_get_active_instance();
    GXRenderPass_t          *p_render_pass           = 0;
    VkAttachmentDescription *attachments             = 0;
    VkAttachmentReference   *attachment_references   = 0;
    VkSubpassDescription    *subpasses               = 0;
    VkRenderPassCreateInfo  *render_pass_create_info = calloc(1, sizeof(VkRenderPassCreateInfo));
    dict                    *render_pass_dict        = 0;
    char                    *name                    = 0,
                           **attachments_text        = 0,
                           **subpasses_text          = 0;

    // Parse the render pass JSON into a dictionary
    parse_json(token_text, len, &render_pass_dict);

    // Parse the JSON dictionary
    {

        // Initialized data
        JSONToken_t *token = 0;

        // Get the name of the render pass
        token            = dict_get(render_pass_dict, "name");
        name             = JSON_VALUE(token, JSONstring);

        // Get the attachments list
        token            = dict_get(render_pass_dict, "attachments");
        attachments_text = JSON_VALUE(token, JSONarray);

        // Get the subpasses list
        token              = dict_get(render_pass_dict, "subpasses");
        subpasses_text     = JSON_VALUE(token, JSONarray);

    }

    // Error checking
    {
        #ifndef NDEBUG 
            if (name == (void *)0)
                goto no_name;
            if (attachments_text == (void *)0)
                goto no_attachments;
        #endif
    }

    // Construct the render pass
    {

        // Initialized data
        size_t attachment_count = 0,
               subpass_count = 0;

        create_render_pass(pp_render_pass);

        p_render_pass = *pp_render_pass;

        // Set the name
        {

            // Initialized data
            size_t len = strlen(name);

            // Allocate memory for name
            p_render_pass->name = calloc(len + 1, sizeof(char));

            // Copy the name
            strncpy(p_render_pass->name, name, len);

        }

        // Populate the attachments
        if(attachments_text){

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
                    token = dict_get(attachment_dict, "name");
                    name = JSON_VALUE(token, JSONstring);

                    // Get the samples for this attachment
                    token = dict_get(attachment_dict, "samples");
                    samples = JSON_VALUE(token, JSONprimative);

                    // Get the load operation
                    token = dict_get(attachment_dict, "load operation");
                    load_operation = JSON_VALUE(token, JSONstring);

                    // Get the store operation
                    token = dict_get(attachment_dict, "store operation");
                    store_operation = JSON_VALUE(token, JSONstring);

                    // Get the stencil load operation
                    token = dict_get(attachment_dict, "stencil load operation");
                    stencil_load_operation = JSON_VALUE(token, JSONstring);

                    // Get the stencil store operation
                    token = dict_get(attachment_dict, "stencil store operation");
                    stencil_store_operation = JSON_VALUE(token, JSONstring);

                    // Get the initial layout
                    token = dict_get(attachment_dict, "initial layout");
                    initial_layout = JSON_VALUE(token, JSONstring);

                    // Get the final layout
                    token = dict_get(attachment_dict, "final layout");
                    final_layout = JSON_VALUE(token, JSONstring);
                
                }

                // Populate the attachment struct
                {

                    // Set the attachment format
                    attachments[i].format = instance->vulkan.swap_chain_image_format;

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
                                t->texture_image = instance->vulkan.swap_chain_images[i];
                                construct_image_view(t, VK_IMAGE_VIEW_TYPE_2D, instance->vulkan.swap_chain_image_format, (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY }, VK_IMAGE_ASPECT_COLOR_BIT);

                            }
                            else
                            {
                                construct_image(t, 0, VK_IMAGE_TYPE_2D, VK_FORMAT_D32_SFLOAT, instance->vulkan.swap_chain_extent.width, instance->vulkan.swap_chain_extent.height, 1, 1, 1, 1, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
                                construct_image_view(t, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_D32_SFLOAT, (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY }, VK_IMAGE_ASPECT_DEPTH_BIT);

                            }
                               
                            p_render_pass->image_attachments[i] = t->texture_image_view;
                            if (i == 0)
                                p_render_pass->image_attachments[i] = instance->vulkan.swap_chain_image_views[instance->vulkan.image_index];
                        }

                        dict_add(p_render_pass->attachments, name, i);
                    }
                }
            }
        }

        if(subpasses_text){
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
                    token = dict_get(subpass_dict, "name");
                    name = JSON_VALUE(token, JSONstring);

                    // Get the input attachments
                    token = dict_get(subpass_dict, "input attachments");
                    input_attachments = JSON_VALUE(token, JSONarray);

                    // Get the color attachments
                    token = dict_get(subpass_dict, "color attachments");
                    color_attachments = JSON_VALUE(token, JSONarray);

                    // Get the depth attachments
                    token = dict_get(subpass_dict, "depth attachments");
                    depth_attachment = JSON_VALUE(token, JSONstring);

                    // Get the preserved attachments
                    token = dict_get(subpass_dict, "preserved attachments");
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
                        
                    subpasses[i].inputAttachmentCount = input_attachment_count;
                    subpasses[i].colorAttachmentCount = color_attachment_count;
                    subpasses[i].preserveAttachmentCount = preserved_attachment_count;
                }

                // Set each attachment
                {


                    // Iterate over each input attachment
                    for (size_t j = 0; j < input_attachment_count; j++)
                    {
                        input_attachment_references[j].attachment = dict_get(p_render_pass->attachments, input_attachments[j]);
                        input_attachment_references[j].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                    }
                    
                    // Iterate over each color attachment
                    for (size_t j = 0; j < color_attachment_count; j++)
                    {
                        color_attachment_references[j].attachment = dict_get(p_render_pass->attachments, color_attachments[j]);
                        color_attachment_references[j].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                    }

                    // Iterate over each preserved attachment
                    for (size_t j = 0; j < preserved_attachment_count; j++)
                        preserved_attachment_references[j].attachment = dict_get(p_render_pass->attachments, preserved_attachments[j]);

                    depth_attachment_reference->attachment = dict_get(p_render_pass->attachments, "depth");
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

                    subpasses[i].inputAttachmentCount = input_attachment_count;

                    if (input_attachment_count)
                        subpasses[i].pInputAttachments = input_attachment_references;

                    subpasses[i].colorAttachmentCount = color_attachment_count;

                    if (color_attachment_count)
                        subpasses[i].pColorAttachments = color_attachment_references;

                    subpasses[i].pDepthStencilAttachment = depth_attachment_reference;

                    subpasses[i].preserveAttachmentCount = preserved_attachment_count;

                    if (preserved_attachment_count)
                        subpasses[i].pPreserveAttachments = preserved_attachments;


                }

            }

        }

        VkSubpassDependency dependency = { 0 };
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        // Populate render pass create info struct
        {
            render_pass_create_info->sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            render_pass_create_info->attachmentCount = attachment_count;
            render_pass_create_info->pAttachments    = attachments;
            render_pass_create_info->subpassCount    = subpass_count;
            render_pass_create_info->pSubpasses      = subpasses;
            render_pass_create_info->dependencyCount = 1;
            render_pass_create_info->pDependencies   = &dependency;
        }

    }
    
    if ( vkCreateRenderPass(instance->vulkan.device, render_pass_create_info, 0, &p_render_pass->render_pass) != VK_SUCCESS ) {
        g_print_error("failed to create render pass!\n");
    }

    p_render_pass->framebuffers = calloc(instance->vulkan.image_count, sizeof(GXFramebuffer_t));

    for (size_t i = 0; i < instance->vulkan.image_count; i++)
    {
        VkImageView attachments[2] = {
            instance->vulkan.swap_chain_image_views[i],
            p_render_pass->image_attachments[1]
        };

        VkFramebufferCreateInfo framebuffer_create_info = { 0 };

        framebuffer_create_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.renderPass      = p_render_pass->render_pass;
        framebuffer_create_info.attachmentCount = 2;
        framebuffer_create_info.pAttachments    = attachments;
        framebuffer_create_info.width           = instance->vulkan.swap_chain_extent.width;
        framebuffer_create_info.height          = instance->vulkan.swap_chain_extent.height;
        framebuffer_create_info.layers          = 1;

        if (vkCreateFramebuffer(instance->vulkan.device, &framebuffer_create_info, 0, &p_render_pass->framebuffers[i].framebuffer) != VK_SUCCESS)
            g_print_error("Failed to create framebuffer!\n");
    }

    return 1;

    // Error handling
    {

        // Argument errors
        {

            no_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_token_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_len: 
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Parameter \"len\" was zero in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // JSON errors
        {
            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] No \"name\" property in render pass JSON, in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;

            no_attachments:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] No \"attachments\" property in render pass JSON, in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            
            // TODO: Categorize
            no_samples:
            no_load_operation:
            no_store_operation:return 0;
        }

        // Vulkan errors
        {

            failed_to_create_render_pass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create render pass in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int           renderer_info                ( GXRenderer_t    *p_renderer)
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
                    g_print_error("[G10] [Renderer] Null pointer provided for \"p_renderer\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

int           add_subpass_callback         ( char            *name          , void(*function_pointer)( ) )
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
                    g_print_error("[G10] [Renderer] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_function_pointer:
                    #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

    }
}

int           render_frame                 ( GXInstance_t    *instance )
{


    // Initialized data
	VkSemaphore               wait_semaphores[]       = { instance->vulkan.image_available_semaphores[instance->vulkan.current_frame]};
	VkSemaphore               signal_semaphores[]     = { instance->vulkan.render_finished_semaphores[instance->vulkan.current_frame]};
	VkPipelineStageFlags      wait_stages[]           = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo              submit_info             = { 0 };
    VkPresentInfoKHR          present_info            = { 0 };
	VkSwapchainKHR            swap_chains[]           = { instance->vulkan.swap_chain };
    VkResult                  result;
    u64                       start                   = 0,
                              end                     = 0;

    start = SDL_GetPerformanceCounter();

    // Get the command buffer ready for drawing
    {

        // Wait for the previous frame to finish rendering
	    vkWaitForFences(instance->vulkan.device, 1, &instance->vulkan.in_flight_fences[instance->vulkan.current_frame], VK_TRUE, UINT64_MAX);
	
        // Grab an image from the swapchain
        result = vkAcquireNextImageKHR(instance->vulkan.device, instance->vulkan.swap_chain, UINT64_MAX, instance->vulkan.image_available_semaphores[instance->vulkan.current_frame], VK_NULL_HANDLE, &instance->vulkan.image_index);
	
        // Make sure the image is usable
        if      ( result == VK_ERROR_OUT_OF_DATE_KHR ) {
            g_window_resize(instance);
            goto fail;
        }
        else if ( result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR ) {
            printf("Failed to acquire swap chain image!\n");
            goto fail;
        }

        // Only reset the fence if we are submitting work
        vkResetFences(instance->vulkan.device, 1, &instance->vulkan.in_flight_fences[instance->vulkan.current_frame]);

        // Clear out the command buffer
    	vkResetCommandBuffer(instance->vulkan.command_buffers[instance->vulkan.current_frame], 0);
	}

    // Draw the frame
    {
        if (1)
        {

            // Initialized data
            GXRenderer_t             *active_renderer   = instance->context.renderer;
            VkCommandBufferBeginInfo  begin_info        = { 0 };

            // Set up the command buffer begin info struct
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            // Begin the command buffer
            vkBeginCommandBuffer(instance->vulkan.command_buffers[instance->vulkan.current_frame], &begin_info);

            // Iterate over each render pass
            for (size_t i = 0; i < active_renderer->render_pass_count; i++)
            {

                // Initialized data
                GXRenderPass_t           *rp                     = active_renderer->render_passes_data[i];
                VkRenderPassBeginInfo     render_pass_begin_info = { 0 };
                size_t                    subpass_count          = 0;

                // Start the render pass
                {
                    
                    // Set up the render pass
                    {
                        render_pass_begin_info.sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                        render_pass_begin_info.renderPass          = rp->render_pass;
                        render_pass_begin_info.framebuffer         = rp->framebuffers[instance->vulkan.current_frame].framebuffer;
                        render_pass_begin_info.renderArea.offset.x = 0;
                        render_pass_begin_info.renderArea.offset.y = 0;
                        render_pass_begin_info.renderArea.extent   = instance->vulkan.swap_chain_extent;
                        render_pass_begin_info.clearValueCount     = 2;
                        render_pass_begin_info.pClearValues        = active_renderer->clear_colors;
                    }

                    // Start the render pass
                    vkCmdBeginRenderPass(instance->vulkan.command_buffers[instance->vulkan.current_frame], &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
                }

                /*// Iterate over each subpass
                for (size_t j = 0; j < subpass_count; j++)
                {

                    // Initialized data
                    GXSubpass_t *s = active_renderer->render_passes_data[i]->subpasses_data[j];
                    int (*subpass_function_callback)(GXSubpass_t *) = dict_get(subpass_functions, s->name);

                    // Call the function
                    subpass_function_callback(s);

                    // Next subpass
                    vkCmdNextSubpass(instance->vulkan.command_buffers[instance->vulkan.current_frame], VK_SUBPASS_CONTENTS_INLINE);
                }
                */

                // Get a list of entities
                size_t       entity_count = dict_values(instance->context.scene->entities, 0);
                GXEntity_t** entities = calloc(entity_count, sizeof(void*));

                dict_values(instance->context.scene->entities, entities);

                // Draw each entity
                for (size_t i = 0; i < entity_count; i++)
                    draw_entity(entities[i]);

                free(entities);



                // End the render pass
                {

                    // End the render pass
                    vkCmdEndRenderPass(instance->vulkan.command_buffers[instance->vulkan.current_frame]);

                    // End the command buffer
                    vkEndCommandBuffer(instance->vulkan.command_buffers[instance->vulkan.current_frame]);

                }

            }
        }
        else {
            // TODO: Make draw functions for each 
            // draw_scene_shadows(instance->active_scene)
            draw_scene(instance->context.scene);
            // draw_scene_bv()
            // draw_scene_composite(instance->active_scene)
            // draw_scene_bloom(instance->active_scene)
        }
    }

    // Submit the commands
    {

        // Populate the submit info struct
        {
            submit_info.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info.waitSemaphoreCount   = 1;
            submit_info.pWaitSemaphores      = &wait_semaphores;
            submit_info.pWaitDstStageMask    = &wait_stages;
            submit_info.commandBufferCount   = 1;
            submit_info.pCommandBuffers      = &instance->vulkan.command_buffers[instance->vulkan.current_frame];
            submit_info.signalSemaphoreCount = 1;
            submit_info.pSignalSemaphores    = &signal_semaphores;
        }

        // Submit the draw commands
        if (vkQueueSubmit(instance->vulkan.graphics_queue, 1, &submit_info, instance->vulkan.in_flight_fences[instance->vulkan.current_frame]))
            g_print_error("Failed to submit draw command buffer!\n");

        // Populate the present info struct
        {
            present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores    = &signal_semaphores;
            present_info.swapchainCount     = 1;
            present_info.pSwapchains        = swap_chains;
            present_info.pImageIndices      = &instance->vulkan.image_index;
        }

        // Present the image to the swapchain
        result = vkQueuePresentKHR(instance->vulkan.present_queue, &present_info);

        // Does the window need to be resized?
        if      ( result == VK_ERROR_OUT_OF_DATE_KHR ||
                  result == VK_SUBOPTIMAL_KHR ) {
            g_window_resize(instance);
        }
        else if ( result != VK_SUCCESS ) {
            printf("failed to present swap chain image!");
        }
    }

    instance->vulkan.current_frame = (instance->vulkan.current_frame + 1) % instance->vulkan.max_buffered_frames;

    fail:

    end = SDL_GetPerformanceCounter();
    instance->time.delta_time = (float)(((double)(end - start)) / (double)(instance->time.clock_div));
    if(instance->time.ticks%144 == 1)
        printf("Render time: %dms                           \r", (int)( instance->time.delta_time * 1000.f ));
    instance->time.ticks += 1;


	return 1;
}

int           present_frame                ( GXInstance_t    *instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( instance == (void *) 0 )
                goto no_instance;
        #endif
    }


    return 1;
    no_instance:
    return 0;
}

int           draw_material                ( GXSubpass_t    **pp_subpass )
{

    return 1;
}
