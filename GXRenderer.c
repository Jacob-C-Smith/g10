#include <G10/GXRenderer.h>

// Constants
#define ATTACHMENT_LOAD_OPERATION_COUNT  3
#define ATTACHMENT_STORE_OPERATION_COUNT 3
#define IMAGE_LAYOUTS_COUNT              18
#define SUBPASS_FUNCTION_COUNT           11
#define FORMAT_ENUM_COUNT                130
#define PIPELINE_STAGE_FLAG_BITS_COUNT   18
#define ACCESS_FLAG_BITS_COUNT           18
#define DEPENDENCY_FLAG_BITS_COUNT       3

VkAttachmentLoadOp attachment_load_operation_enums[ATTACHMENT_LOAD_OPERATION_COUNT] =
{
    VK_ATTACHMENT_LOAD_OP_LOAD,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
    VK_ATTACHMENT_LOAD_OP_DONT_CARE
};
char *attachment_load_operation_names[ATTACHMENT_LOAD_OPERATION_COUNT] =
{
    "load",
    "clear",
    "don't care"
};

VkAttachmentStoreOp  attachment_store_operation_enums[ATTACHMENT_STORE_OPERATION_COUNT] =
{
    VK_ATTACHMENT_STORE_OP_STORE,
    VK_ATTACHMENT_STORE_OP_DONT_CARE,
    VK_ATTACHMENT_STORE_OP_NONE,
};
char *attachment_store_operation_names[ATTACHMENT_STORE_OPERATION_COUNT] =
{
    "store",
    "don't care",
    "none"
};

VkImageLayout image_layout_enums[IMAGE_LAYOUTS_COUNT] =
{
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
char *image_layout_names[IMAGE_LAYOUTS_COUNT] =
{
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

void *subpass_function_callbacks[SUBPASS_FUNCTION_COUNT] = 
{
    &draw_scene,
    0,//&draw_material,
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
char *subpass_function_names[SUBPASS_FUNCTION_COUNT] = 
{
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

VkFormat format_enums[FORMAT_ENUM_COUNT] = 
{
    VK_FORMAT_UNDEFINED,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    VK_FORMAT_R8_UNORM,
    VK_FORMAT_R8_SNORM,
    VK_FORMAT_R8_USCALED,
    VK_FORMAT_R8_SSCALED,
    VK_FORMAT_R8_UINT,
    VK_FORMAT_R8_SINT,
    VK_FORMAT_R8_SRGB,
    VK_FORMAT_R8G8_UNORM,
    VK_FORMAT_R8G8_SNORM,
    VK_FORMAT_R8G8_USCALED,
    VK_FORMAT_R8G8_SSCALED,
    VK_FORMAT_R8G8_UINT,
    VK_FORMAT_R8G8_SINT,
    VK_FORMAT_R8G8_SRGB,
    VK_FORMAT_R8G8B8_UNORM,
    VK_FORMAT_R8G8B8_SNORM,
    VK_FORMAT_R8G8B8_USCALED,
    VK_FORMAT_R8G8B8_SSCALED,
    VK_FORMAT_R8G8B8_UINT,
    VK_FORMAT_R8G8B8_SINT,
    VK_FORMAT_R8G8B8_SRGB,
    VK_FORMAT_B8G8R8_UNORM,
    VK_FORMAT_B8G8R8_SNORM,
    VK_FORMAT_B8G8R8_USCALED,
    VK_FORMAT_B8G8R8_SSCALED,
    VK_FORMAT_B8G8R8_UINT,
    VK_FORMAT_B8G8R8_SINT,
    VK_FORMAT_B8G8R8_SRGB,
    VK_FORMAT_R8G8B8A8_UNORM,
    VK_FORMAT_R8G8B8A8_SNORM,
    VK_FORMAT_R8G8B8A8_USCALED,
    VK_FORMAT_R8G8B8A8_SSCALED,
    VK_FORMAT_R8G8B8A8_UINT,
    VK_FORMAT_R8G8B8A8_SINT,
    VK_FORMAT_R8G8B8A8_SRGB,
    VK_FORMAT_B8G8R8A8_UNORM,
    VK_FORMAT_B8G8R8A8_SNORM,
    VK_FORMAT_B8G8R8A8_USCALED,
    VK_FORMAT_B8G8R8A8_SSCALED,
    VK_FORMAT_B8G8R8A8_UINT,
    VK_FORMAT_B8G8R8A8_SINT,
    VK_FORMAT_B8G8R8A8_SRGB,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    VK_FORMAT_R16_UNORM,
    VK_FORMAT_R16_SNORM,
    VK_FORMAT_R16_USCALED,
    VK_FORMAT_R16_SSCALED,
    VK_FORMAT_R16_UINT,
    VK_FORMAT_R16_SINT,
    VK_FORMAT_R16_SFLOAT,
    VK_FORMAT_R16G16_UNORM,
    VK_FORMAT_R16G16_SNORM,
    VK_FORMAT_R16G16_USCALED,
    VK_FORMAT_R16G16_SSCALED,
    VK_FORMAT_R16G16_UINT,
    VK_FORMAT_R16G16_SINT,
    VK_FORMAT_R16G16_SFLOAT,
    VK_FORMAT_R16G16B16_UNORM,
    VK_FORMAT_R16G16B16_SNORM,
    VK_FORMAT_R16G16B16_USCALED,
    VK_FORMAT_R16G16B16_SSCALED,
    VK_FORMAT_R16G16B16_UINT,
    VK_FORMAT_R16G16B16_SINT,
    VK_FORMAT_R16G16B16_SFLOAT,
    VK_FORMAT_R16G16B16A16_UNORM,
    VK_FORMAT_R16G16B16A16_SNORM,
    VK_FORMAT_R16G16B16A16_USCALED,
    VK_FORMAT_R16G16B16A16_SSCALED,
    VK_FORMAT_R16G16B16A16_UINT,
    VK_FORMAT_R16G16B16A16_SINT,
    VK_FORMAT_R16G16B16A16_SFLOAT,
    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_UINT,
    VK_FORMAT_R32G32B32A32_SINT,
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R64_UINT,
    VK_FORMAT_R64_SINT,
    VK_FORMAT_R64_SFLOAT,
    VK_FORMAT_R64G64_UINT,
    VK_FORMAT_R64G64_SINT,
    VK_FORMAT_R64G64_SFLOAT,
    VK_FORMAT_R64G64B64_UINT,
    VK_FORMAT_R64G64B64_SINT,
    VK_FORMAT_R64G64B64_SFLOAT,
    VK_FORMAT_R64G64B64A64_UINT,
    VK_FORMAT_R64G64B64A64_SINT,
    VK_FORMAT_R64G64B64A64_SFLOAT,
    0,
    0,
    VK_FORMAT_D16_UNORM,
    0,
    VK_FORMAT_D32_SFLOAT,
    VK_FORMAT_S8_UINT,
    VK_FORMAT_D16_UNORM_S8_UINT,
    VK_FORMAT_D24_UNORM_S8_UINT
};
char *format_names[FORMAT_ENUM_COUNT] = 
{
    "undefined",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "unorm r8",
    "snorm r8",
    "uscaled r8",
    "sscaled r8",
    "uint r8",
    "sint r8",
    "sRGB r8",
    "unorm r8g8",
    "snorm r8g8",
    "uscaled r8g8",
    "sscaled r8g8",
    "uint r8g8",
    "sint r8g8",
    "sRGB r8g8",
    "unorm r8g8b8",
    "snorm r8g8b8",
    "uscaled r8g8b8",
    "sscaled r8g8b8",
    "uint r8g8b8",
    "sint r8g8b8",
    "sRGB r8g8b8",
    "unorm b8g8r8",
    "snorm b8g8r8",
    "uscaled b8g8r8",
    "sscaled b8g8r8",
    "uint b8g8r8",
    "sint b8g8r8",
    "sRGB b8g8r8",
    "unorm r8g8b8a8",
    "snorm r8g8b8a8",
    "uscaled r8g8b8a8",
    "sscaled r8g8b8a8",
    "uint r8g8b8a8",
    "sint r8g8b8a8",
    "sRGB r8g8b8a8",
    "unorm b8g8r8a8",
    "snorm b8g8r8a8",
    "uscaled b8g8r8a8",
    "sscaled b8g8r8a8",
    "uint b8g8r8a8",
    "sint b8g8r8a8",
    "sRGB b8g8r8a8",
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "unorm r16",
    "snorm r16",
    "uscaled r16",
    "sscaled r16",
    "uint r16",
    "sint r16",
    "sfloat r16",
    "unorm r16g16",
    "snorm r16g16",
    "uscaled r16g16",
    "sscaled r16g16",
    "uint r16g16",
    "sint r16g16",
    "sfloat r16g16",
    "unorm r16g16b16",
    "snorm r16g16b16",
    "uscaled r16g16b16",
    "sscaled r16g16b16",
    "uint r16g16b16",
    "sint r16g16b16",
    "sfloat r16g16b16",
    "unorm r16g16b16a16",
    "snorm r16g16b16a16",
    "uscaled r16g16b16a16",
    "sscaled r16g16b16a16",
    "uint r16g16b16a16",
    "sint r16g16b16a16",
    "sfloat r16g16b16a16",
    "uint r32",
    "sint r32",
    "sfloat r32",
    "uint r32g32",
    "sint r32g32",
    "sfloat r32g32",
    "uint r32g32b32",
    "sint r32g32b32",
    "sfloat r32g32b32",
    "uint r32g32b32a32",
    "sint r32g32b32a32",
    "sfloat r32g32b32a32",
    "uint r64",
    "sint r64",
    "sfloat r64",
    "uint r64g64",
    "sint r64g64",
    "sfloat r64g64",
    "uint r64g64b64",
    "sint r64g64b64",
    "sfloat r64g64b64",
    "uint r64g64b64a64",
    "sint r64g64b64a64",
    "sfloat r64g64b64a64",
    0,
    0,
    "unorm d16",
    0,
    "sfloat d32",
    "uint s8",
    "unorm d16 uint s8",
    "unorm d24 uint s8"
};

VkDependencyFlagBits pipeline_stage_flag_bits_enum[PIPELINE_STAGE_FLAG_BITS_COUNT] = 
{
    VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
    VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
    VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
    VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
    VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
    VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
    VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
    VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
    VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
    VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
    VK_PIPELINE_STAGE_TRANSFER_BIT,
    VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
    VK_PIPELINE_STAGE_HOST_BIT,
    VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    VK_PIPELINE_STAGE_NONE
};
char *pipeline_stage_flag_bits_names[PIPELINE_STAGE_FLAG_BITS_COUNT] = 
{
    "top of pipe",
    "draw indirect",
    "vertex input",
    "vertex shader",
    "tessellation control shader",
    "tessellation evaluation shader",
    "geometry shader",
    "fragment shader",
    "early fragment tests",
    "late fragment tests",
    "color attachment output",
    "compute shader",
    "transfer",
    "bottom of pipe",
    "host",
    "all graphics",
    "all commands",
    "none"
};

VkAccessFlagBits access_flag_bits_enum[ACCESS_FLAG_BITS_COUNT] = 
{
    VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
    VK_ACCESS_INDEX_READ_BIT,
    VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
    VK_ACCESS_UNIFORM_READ_BIT,
    VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
    VK_ACCESS_SHADER_READ_BIT,
    VK_ACCESS_SHADER_WRITE_BIT,
    VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
    VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
    VK_ACCESS_TRANSFER_READ_BIT,
    VK_ACCESS_TRANSFER_WRITE_BIT,
    VK_ACCESS_HOST_READ_BIT,
    VK_ACCESS_HOST_WRITE_BIT,
    VK_ACCESS_MEMORY_READ_BIT,
    VK_ACCESS_MEMORY_WRITE_BIT,
    VK_ACCESS_NONE
};
char *access_flag_bits_names[ACCESS_FLAG_BITS_COUNT] = 
{
    "indirect command read",
    "index read",
    "vertex attribute read",
    "uniform read",
    "input attachment read",
    "shader read",
    "shader write",
    "color attachment read",
    "color attachment write",
    "depth stencil attachment read",
    "depth stencil attachment write",
    "transfer read",
    "transfer write",
    "host read",
    "host write",
    "memory read",
    "memory write",
    "none"
};

VkDependencyFlagBits dependency_flag_bits_enum[DEPENDENCY_FLAG_BITS_COUNT] = 
{
    VK_DEPENDENCY_BY_REGION_BIT,
    VK_DEPENDENCY_DEVICE_GROUP_BIT,
    VK_DEPENDENCY_VIEW_LOCAL_BIT
};
char *dependency_flag_bits_name[DEPENDENCY_FLAG_BITS_COUNT] = 
{
    "by region",
    "device group",
    "view local"
};

dict *attachment_load_operations  = 0,
     *image_layouts               = 0,
     *subpass_functions           = 0,
     *attachment_store_operations = 0,
     *format_enumeration_lookup   = 0,
     *pipeline_stage_flag_bits    = 0,
     *access_flag_bits            = 0,
     *dependency_flag_bits        = 0;

void init_renderer ( void )
{

    // Construct lookup tables
    dict_construct(&attachment_load_operations , ATTACHMENT_LOAD_OPERATION_COUNT);
    dict_construct(&attachment_store_operations, ATTACHMENT_STORE_OPERATION_COUNT);
    dict_construct(&image_layouts              , IMAGE_LAYOUTS_COUNT);
    dict_construct(&subpass_functions          , SUBPASS_FUNCTION_COUNT);
    dict_construct(&format_enumeration_lookup  , FORMAT_ENUM_COUNT);
    dict_construct(&pipeline_stage_flag_bits   , PIPELINE_STAGE_FLAG_BITS_COUNT);
    dict_construct(&access_flag_bits           , ACCESS_FLAG_BITS_COUNT);
    dict_construct(&dependency_flag_bits       , DEPENDENCY_FLAG_BITS_COUNT);

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

        for (size_t i = 0; i < FORMAT_ENUM_COUNT; i++)
            dict_add(format_enumeration_lookup, format_names[i], (void *)format_enums[i]);

        for (size_t i = 0; i < PIPELINE_STAGE_FLAG_BITS_COUNT; i++)
            dict_add(pipeline_stage_flag_bits, pipeline_stage_flag_bits_names[i], (void *)pipeline_stage_flag_bits_enum[i]);
        
        for (size_t i = 0; i < ACCESS_FLAG_BITS_COUNT; i++)
            dict_add(access_flag_bits, access_flag_bits_names[i], (void *)access_flag_bits_enum[i]);
    }

    return;
}

int create_renderer ( GXRenderer_t **pp_renderer )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 ) goto no_ret;
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
            if ( pp_render_pass == (void *) 0 ) goto no_ret;
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
            if ( pp_subpass == (void *) 0 ) goto no_ret;
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

int create_attachment ( GXAttachment_t **pp_attachment )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_attachment == (void *) 0 ) goto no_attachment;
        #endif
    }

    // Initialized data
    GXAttachment_t *p_attachment = calloc(1, sizeof(GXSubpass_t));

    // Error checking
    if ( p_attachment == (void *) 0 )
        goto no_mem;

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
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_attachment\" in call to function \"%s\"\n", __FUNCTION__);
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
            if ( pp_renderer == (void *) 0 ) goto no_renderer;
            if ( path        == (void *) 0 ) goto no_path;
        #endif
    }

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(1+len, sizeof(char));

    // Error checking
    if ( text == (void *) 0 )
        goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 )
        goto failed_to_read_file;

    // Construct a renderer
    if ( load_renderer_as_json_text(pp_renderer, text) == 0 )
        goto failed_to_load_renderer;

    // Clean the scope
    free(text);

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

int load_renderer_as_json_text ( GXRenderer_t **pp_renderer, char *text )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 ) goto no_renderer;
            if ( text        == (void *) 0 ) goto no_text;
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

    // TODO:
    // Clean the scope
    //free_json_value(p_value);

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
            if ( pp_renderer == (void *) 0 ) goto no_return;
            if ( p_value     == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance    = g_get_active_instance();
    GXRenderer_t *p_renderer    = 0;
    JSONValue_t  *p_name        = 0,
                 *p_passes      = 0,
                 *p_attachments = 0,
                 *p_clear_color = 0;

    // Parse the value as an object
    if ( p_value->type == JSONobject )
    {

        p_name        = dict_get(p_value->object, "name");
        p_passes      = dict_get(p_value->object, "passes");
        p_clear_color = dict_get(p_value->object, "clear color");
        p_attachments = dict_get(p_value->object, "attachments");

        if ( !( p_name && p_passes ) ) 
            goto missing_parameters;
    }

    // Parse the value as a path
    else if ( p_value->type == JSONstring )
    {
        if ( load_renderer(pp_renderer, p_value->string) == 0 )
            goto faild_to_load_renderer;

        // Success
        return 1;
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
        if(p_clear_color == 0)
            goto no_p_clear;
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
        no_p_clear:;
        
        // Parse each render pass
        if ( p_passes->type == JSONarray )
        {
            /*
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
            */
        }
        else
            goto wrong_passes_type;

        // Populate the attachments
        if( p_attachments->type == JSONarray )
        {
            
            // Initialized data
            size_t        array_len         = 0;
            JSONValue_t **pp_array_contents = 0;

            // Get the array contents
            {

                // Get the array length
                array_get(p_attachments->list, 0, &array_len);

                // Allocate memory for array
                pp_array_contents = calloc(array_len, sizeof(JSONValue_t *));

                // Error checking
                if ( pp_array_contents == (void *) 0 )
                    goto no_mem;

                // Dump array to memory
                array_get(p_attachments->list, pp_array_contents, 0);
            }

            // Construct a dictionary for the attachments
            dict_construct(&p_renderer->attachments, array_len);

            // Iterate over each attachment JSON value
            for (size_t i = 0; i < array_len; i++)
            {

                // Initialized data
                GXAttachment_t *p_attachment = 0;

                // Load the attachment as a JSON value
                if ( load_attachment_as_json_value(&p_attachment, pp_array_contents[i]) == 0 )
                    goto failed_to_load_attachment_as_json_value;
                
                // Create a texture
                {
                    
                    // Initialized data
                    GXTexture_t *p_texture = 0;

                    // Allocate memory for a texture
                    if ( create_texture(&p_texture) == 0 )
                        goto failed_to_allocate_texture;
                    
                    if ( strcmp(p_attachment->name, "final") == 0 )
                    {
                        p_texture->texture_image = p_instance->vulkan.swap_chain_images[i];
                        construct_image_view(p_texture, VK_IMAGE_VIEW_TYPE_2D, p_instance->vulkan.swap_chain_image_format, (VkComponentMapping) { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY }, VK_IMAGE_ASPECT_COLOR_BIT);
                    }
                    else
                    {
                        construct_image(
                            p_texture,
                            0, 
                            VK_IMAGE_TYPE_2D,
                            VK_FORMAT_D32_SFLOAT,
                            p_instance->vulkan.swap_chain_extent.width,
                            p_instance->vulkan.swap_chain_extent.height,
                            1,
                            1,
                            1,
                            1,
                            VK_IMAGE_TILING_OPTIMAL,
                            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                            VK_SHARING_MODE_EXCLUSIVE,
                            VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL
                        );
                        construct_image_view(
                            p_texture,
                            VK_IMAGE_VIEW_TYPE_2D, 
                            VK_FORMAT_D32_SFLOAT,
                            (VkComponentMapping) {
                                VK_COMPONENT_SWIZZLE_IDENTITY,
                                VK_COMPONENT_SWIZZLE_IDENTITY,
                                VK_COMPONENT_SWIZZLE_IDENTITY,
                                VK_COMPONENT_SWIZZLE_IDENTITY
                            }, 
                            VK_IMAGE_ASPECT_DEPTH_BIT
                        );
                    }
                    
                    /*
                    p_render_pass->image_attachments[i] = p_texture->texture_image_view;

                    if ( i == 0 )
                        p_render_pass->image_attachments[i] = p_instance->vulkan.swap_chain_image_views[p_instance->vulkan.image_index];
                
                */
                }
                
                // Add the attachment
                dict_add(p_renderer->attachments, p_attachment->name, p_attachment);
            }

        }

        // Return a pointer to the caller
        *pp_renderer = p_renderer;
    }

    // Success
    return 1;

    // TODO:
    
    failed_to_allocate_texture:
    wrong_passes_type:
    failed_to_load_renderer_as_json_value:
    wrong_renderer_clear_color:
    
    faild_to_load_renderer:
    failed_to_load_attachment_as_json_value:
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

        // JSON errors
        {
            wrong_value_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Expected JSON [ string | object ] value in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            missing_parameters:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Missing properties in \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

        }

        // G10 errors
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
            if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
            if ( path           == (void *) 0 ) goto no_path;
        #endif
    }

    // Initialized data
    size_t  len    = g_load_file(path, 0, true);
    char   *text = calloc(1+len, sizeof(char));

    // Error checking
    if (text == (void *)0)
        goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 )
        goto failed_to_read_file;

    // Construct a render pass
    if ( load_render_pass_as_json_text(pp_render_pass, text) == 0 )
        goto failed_to_load_render_pass;

    // Clean the scope
    free(text);

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

int load_render_pass_as_json_text ( GXRenderPass_t **pp_render_pass, char* text )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
            if ( text           == (void *) 0 ) goto no_text;
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

    // TODO:
    // Clean the scope
    //free_json_value(p_value);

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

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_parse_json_as_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to parse JSON text into JSON value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            failed_to_load_renderer_as_json_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to construct renderer from JSON value in call to function \"%s\"\n", __FUNCTION__);
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
            if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
            if ( p_value        == (void *) 0 ) goto no_value;
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
        if ( p_name->type == JSONstring )
        {

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
        
        
        
        // Populate subpasses
        if ( p_subpasses->type == JSONarray )
        {

            // Initialized data
            size_t        array_len         = 0;
            JSONValue_t **pp_array_contents = 0;

            // Get the array contents
            {

                // Get the array length
                array_get(p_subpasses->list, 0, &array_len);

                // Allocate memory for array
                pp_array_contents = calloc(array_len, sizeof(JSONValue_t *));

                // Error checking
                if ( pp_array_contents == (void *) 0 )
                    goto no_mem;

                // Dump array to memory
                array_get(p_subpasses->list, pp_array_contents, 0);
            }


            // Iterate over each subpass JSON object text
            for (size_t i = 0; i < array_len; i++)
            {

                // Initialized data
                JSONValue_t *p_subpass                       = pp_array_contents[i],
                            *p_subpass_name                  = 0,
                            *p_subpass_input_attachments     = 0,
                            *p_subpass_color_attachments     = 0,
                            *p_subpass_preserved_attachments = 0,
                            *p_subpass_depth_attachment      = 0;

                // Get the required information to construct an attachment
                if ( p_subpass->type == JSONobject)
                {

                    p_subpass_name                  = ( (JSONValue_t *) dict_get(p_subpass->object, "name"));
                    p_subpass_input_attachments     = ( (JSONValue_t *) dict_get(p_subpass->object, "input attachments"));
                    p_subpass_color_attachments     = ( (JSONValue_t *) dict_get(p_subpass->object, "color attachments"));
                    p_subpass_preserved_attachments = ( (JSONValue_t *) dict_get(p_subpass->object, "preserved attachments"));
                    p_subpass_depth_attachment      = ( (JSONValue_t *) dict_get(p_subpass->object, "depth attachments"));
                    
                    if  ( p_subpass_name                  &&
                          p_subpass_input_attachments     && 
                          p_subpass_color_attachments     &&
                          p_subpass_preserved_attachments &&
                          p_subpass_depth_attachment )
                        goto missing_properties;
                }

                // Construct the subpass
                {
                    size_t                  input_attachment_count           = 0,
                                            color_attachment_count           = 0,
                                            preserved_attachment_count       = 0;
                    VkAttachmentReference  *input_attachment_references      = calloc(input_attachment_count    , sizeof(VkAttachmentReference)),
                                           *color_attachment_references      = calloc(color_attachment_count    , sizeof(VkAttachmentReference)),
                                           *preserved_attachment_references  = calloc(preserved_attachment_count, sizeof(VkAttachmentReference)),
                                           *depth_attachment_reference       = calloc(1                         , sizeof(VkAttachmentReference));
                    JSONValue_t           **pp_subpass_input_attachments     = 0,
                                          **pp_subpass_color_attachments     = 0,
                                          **pp_subpass_preserved_attachments = 0,
                                          **pp_subpass_depth_attachment     = 0;

                    // Parse the JSON value
                    {

                        // Get the quantity of each type of attachment
                        if ( p_subpass_input_attachments ) 
                        {
                            if ( p_subpass_input_attachments->type == JSONarray )
                                array_get(p_subpass_input_attachments->list, 0, &input_attachment_count);
                            else
                                goto wrong_subpass_input_attachments_type;
                        }

                        // Default
                        else
                        { }

                        if ( p_subpass_color_attachments ) 
                        {
                            if ( p_subpass_color_attachments )
                                array_get(p_subpass_color_attachments->list, 0, &color_attachment_count);
                            else
                                goto wrong_subpass_color_attachments_type;
                        }

                        // Default
                        else
                        { }

                        if ( p_subpass_preserved_attachments ) 
                        {
                            if ( p_subpass_preserved_attachments->type == JSONarray )
                                array_get(p_subpass_preserved_attachments->list, 0, &preserved_attachment_count);
                            else
                                goto wrong_subpass_preserved_attachments_type;
                        }

                        // Default
                        else
                        { }

                        if ( p_subpass_depth_attachment ) 
                        {
                            if ( p_subpass_depth_attachment->type == JSONstring )
                            { }
                            else
                                goto wrong_subpass_depth_attachments_type;
                        }

                        // Default
                        else
                        { }
                    }

                    // Populate a subpass description structure
                    subpasses[i] = (VkSubpassDescription)
                    {
                        .flags                   = 0,
                        .pipelineBindPoint       = 0,
                        .inputAttachmentCount    = (u32) input_attachment_count,
                        .pInputAttachments       = 0,
                        .colorAttachmentCount    = (u32) color_attachment_count,
                        .pColorAttachments       = 0,
                        .pResolveAttachments     = 0,
                        .pDepthStencilAttachment = 0,
                        .preserveAttachmentCount = (u32) preserved_attachment_count,
                        .pPreserveAttachments    = 0
                    };
                }
            }
        }
        
        dependency = (VkSubpassDependency)
        {
            .srcSubpass    = VK_SUBPASS_EXTERNAL,
            .dstSubpass    = 0,
            .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .srcAccessMask = 0,
            .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        };

        // Populate render pass create info struct
        render_pass_create_info = (VkRenderPassCreateInfo)
        {
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
    
    // TODO: Check
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
    missing_properties:
    wrong_subpass_input_attachments_type:
    wrong_subpass_color_attachments_type:
    wrong_subpass_preserved_attachments_type:
    wrong_subpass_depth_attachments_type:
    failed_to_allocate_texture:
    missing_parameters:
    no_mem:
    failed_to_load_attachment_as_json_value:
    wrong_samples_type:
    wrong_load_operation_type:
    wrong_store_operation_type:
    wrong_stencil_load_operation_type:
    wrong_stencil_store_operation_type:
    wrong_initial_layout_type:
    wrong_final_layout_type:
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

int load_subpass ( GXSubpass_t **pp_subpass, char* path )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_subpass == (void *) 0 ) goto no_subpass;
            if ( path       == (void *) 0 ) goto no_path;
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

    // Construct a subpass
    if ( load_subpass_as_json_value(pp_subpass, buffer) == 0 )
        goto failed_to_load_subpass;

    // Clean the scope
    free(buffer);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
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

            failed_to_load_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to load subpass from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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

int load_subpass_as_json_text ( GXSubpass_t **pp_subpass, char* text )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_subpass == (void *) 0 ) goto no_subpass;
            if ( text       == (void *) 0 ) goto no_text;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance    = g_get_active_instance();
    GXSubpass_t  *p_render_pass = 0;
    JSONValue_t  *p_value       = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto failed_to_parse_json_as_value;

    // Load the renderer as a JSON value
    if ( load_subpass_as_json_value(pp_subpass, p_value) == 0 )
        goto failed_to_load_subpass_as_json_value;

    // Clean the scope
    free_json_value(p_value);

    // Success
    return 1;

    // TODO:
    failed_to_parse_json_as_value:
    failed_to_load_subpass_as_json_value:
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_subpass_as_json_value ( GXSubpass_t **pp_subpass, JSONValue_t *p_value )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_subpass == (void *) 0 ) goto no_subpass;
            if ( p_value    == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance    = g_get_active_instance();
    GXSubpass_t  *p_subpass     = 0;
    JSONValue_t  *p_name        = 0, 
                 *p_            = 0;

    // Parse the render pass JSON value
    if ( p_value->type == JSONobject ) {

        //p_name        = dict_get(p_value->object, "name");
        //p_attachments = dict_get(p_value->object, "attachments");
        //p_subpasses   = dict_get(p_value->object, "subpasses");
        //// Check for missing parameters
        //if ( ! ( p_name && p_attachments && p_subpasses ) )
        //    goto missing_parameters;
    }
    else
        goto wrong_type;

    // Construct the render pass
    {

        // Initialized data

        // Allocate memory for a subpass
        if ( create_subpass(&p_subpass) == 0 )
            goto failed_to_create_subpass;

        // Set the name
        if ( p_name->type == JSONstring )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for name
            p_subpass->name = calloc(len + 1, sizeof(char));

            // Error checking
            if ( p_subpass->name == (void *) 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_subpass->name, p_name->string, len);

        }
    }
        
    // Return a pointer to the caller
    *pp_subpass = p_subpass;

    // Success
    return 1;

    // TODO:
    wrong_type:
    no_mem:
    return 0;

    // Error handling
    {

        // Argument errors
        {

            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
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

            failed_to_create_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create render pass in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_attachment_as_json_value ( GXAttachment_t **pp_attachment, JSONValue_t *p_value )
{
 
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_attachment == (void *) 0 ) goto no_attachment;
            if ( p_value       == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXInstance_t   *p_instance        = g_get_active_instance();
    GXAttachment_t *p_attachment      = 0;
    JSONValue_t    *p_name            = 0,
                   *p_samples         = 0,
                   *p_format          = 0,
                   *p_load_operation  = 0,
                   *p_store_operation = 0,
                   *p_initial_layout  = 0,
                   *p_final_layout    = 0;

    // Parse the attachment JSON value
    if ( p_value->type == JSONobject ) {

        p_name            = dict_get(p_value->object, "name");
        p_samples         = dict_get(p_value->object, "samples");
        p_format          = dict_get(p_value->object, "format");
        p_load_operation  = dict_get(p_value->object, "load operation");
        p_store_operation = dict_get(p_value->object, "store operation");
        p_initial_layout  = dict_get(p_value->object, "initial layout");
        p_final_layout    = dict_get(p_value->object, "final layout");

        // Check for missing parameters
        if ( ! ( p_name && p_samples && p_format && p_initial_layout && p_final_layout ) )
            goto missing_parameters;
    }
    else
        goto wrong_type;

    // Construct the attachment
    {

        // Initialized data
        VkFormat                format                 = 0;
        VkSampleCountFlagBits   samples                = 0;
        VkAttachmentLoadOp      loadOp                 = 0;
        VkAttachmentStoreOp     storeOp                = 0;
        VkAttachmentLoadOp      stencilLoadOp          = 0;
        VkAttachmentStoreOp     stencilStoreOp         = 0;
        VkImageLayout           initialLayout          = 0;
        VkImageLayout           finalLayout            = 0;
        VkAttachmentDescription attachment_description = { 0 };
        
        // Allocate memory for an attachment
        if ( create_attachment(&p_attachment) == 0 )
            goto failed_to_create_attachment;

        // Set the name
        if ( p_name->type == JSONstring )
        {

            // Initialized data
            size_t len = strlen(p_name->string);

            // Allocate memory for name
            p_attachment->name = calloc(len + 1, sizeof(char));

            // Error checking
            if ( p_attachment->name == (void *) 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_attachment->name, p_name->string, len);
        }
        // Default
        else
            goto wrong_name_type;

        // Set the format
        if ( p_format->type == JSONstring )
        {
            format = (VkFormat) dict_get(format_enumeration_lookup, p_format->string);

            if ( format == 0 )
                goto wrong_format;
        }
        // Default
        else
            goto wrong_format_type;

        // Set the sample count
        if ( p_samples->type == JSONinteger )
            samples = p_samples->integer;
        // Default
        else
            goto wrong_name_type;

        // Set the load operation
        if ( p_load_operation->type == JSONstring )
            loadOp = (VkAttachmentLoadOp) dict_get(attachment_load_operations, p_load_operation->string);
        // Default
        else
            goto wrong_load_operation_type;
        
        // Set the store operation
        if ( p_store_operation->type == JSONstring )
            storeOp = (VkAttachmentStoreOp) dict_get(attachment_store_operations, p_store_operation->string);
        // Default
        else
            goto wrong_store_operation_type;
        
        // (Maybe) set the stencil load operation

        // (Maybe) set the stencil store operation
        
        // Set the initial layout
        if ( p_initial_layout->type == JSONstring )
        {
            initialLayout = (VkImageLayout) dict_get(image_layouts, p_initial_layout->string);

            if ( initialLayout == 0 )
                goto wrong_initial_layout;
        }
        // Default
        else
            goto wrong_initial_layout_type;

        // Set the final layout 
        if ( p_final_layout->type == JSONstring )
        {
            finalLayout = (VkImageLayout) dict_get(image_layouts, p_final_layout->string);

            if ( finalLayout == 0  )
                goto wrong_final_layout;
        }
        // Default
        else
            goto wrong_final_layout_type;

        // Populate the attachment description
        p_attachment->attachment_description = (VkAttachmentDescription)
        {
            .flags          = 0,
            .format         = format,
            .samples        = samples,
            .loadOp         = loadOp,
            .storeOp        = storeOp,
            .stencilLoadOp  = stencilLoadOp,
            .stencilStoreOp = stencilStoreOp,
            .initialLayout  = initialLayout,
            .finalLayout    = finalLayout 
        };
    }
        
    // Return a pointer to the caller
    *pp_attachment = p_attachment;

    // Success
    return 1;

    // TODO:
    wrong_type:
    wrong_name_type:
    no_mem:
    missing_parameters:
    failed_to_create_attachment:
    wrong_format:
    wrong_format_type:
    wrong_load_operation:
    wrong_load_operation_type:
    wrong_store_operation:
    wrong_store_operation_type:
    wrong_initial_layout:
    wrong_initial_layout_type:
    wrong_final_layout:
    wrong_final_layout_type:
    return 0;

    // Error handling
    {

        // Argument errors
        {

            no_attachment:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
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

            failed_to_create_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Failed to create render pass in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int print_renderer ( GXRenderer_t *p_renderer )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_renderer == (void *) 0 ) goto no_renderer;
        #endif
    }

    // Formatting 
    g_print_log(" - Renderer info - \n");
    
    // Print the name
    g_print_log("name        : \"%s\"\n", p_renderer->name);

    // Formatting 
    g_print_log("attachments :\n");

    // Print each attachment
    dict_foreach(p_renderer->attachments,print_attachment);
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

                // Error
                return 0;
        }
    }
}

int print_attachment ( GXAttachment_t *p_attachment )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if ( p_attachment == (void *) 0 )
				goto no_attachment;
		#endif
	}

	// Formatting 
    g_print_log(" - Attachment info - \n");
    
    // Print the name
    g_print_log("name            : \"%s\"\n", p_attachment->name);
    
    // Print the attachment description
    g_print_log("format          : \"%s\"\n", format_names[p_attachment->attachment_description.format]);

    // Print the sanple count
    g_print_log("samples         : %d\n", p_attachment->attachment_description.samples);

    // Print the load operation
    g_print_log("load operation  : \"%s\"\n", attachment_load_operation_names[p_attachment->attachment_description.loadOp]);
    
    // Print the store operation
    g_print_log("store operation : \"");
    if ( p_attachment->attachment_description.storeOp == VK_ATTACHMENT_STORE_OP_NONE )
        g_print_log("none\"\n");
    else
        g_print_log("%s\"\n", attachment_store_operation_names[p_attachment->attachment_description.storeOp]);

    // Print the initial layout
    g_print_log("initial layout  : \"");
    if ( p_attachment->attachment_description.initialLayout > 8 )
        g_print_log("none\"\n");
    else
        g_print_log("%s\"\n", image_layout_names[p_attachment->attachment_description.initialLayout]);

    g_print_log("final layout    : \"");
    if ( p_attachment->attachment_description.finalLayout > 8 )
        g_print_log("none\"\n");
    else
        g_print_log("%s\"\n", image_layout_names[p_attachment->attachment_description.finalLayout]);

	putchar('\n');

	// Success
    return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_attachment:
				#ifndef NDEBUG
					g_print_error("[G10] [Renderer] Null pointer provided for \"p_attachment\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				
				// Error
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
        if ( result == VK_ERROR_OUT_OF_DATE_KHR ) {
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
        GXRenderer_t             *active_renderer = p_instance->context.renderer;
        VkCommandBufferBeginInfo  begin_info      = { 0 };

        // Set up the command buffer begin info struct
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        // Begin the command buffer
        vkBeginCommandBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], &begin_info);

        // Iterate over each render pass
        for (size_t i = 0; i < active_renderer->render_pass_count; i++)
        {

            // Initialized data
            GXRenderPass_t  *rp            = active_renderer->render_passes_data[i];
            size_t           subpass_count = 0;
            size_t           entity_count  = dict_values(p_instance->context.scene->entities, 0);
            GXEntity_t     **entities      = calloc(entity_count, sizeof(void*));

            VkRenderPassBeginInfo render_pass_begin_info = 
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

            
            // TODO: Iterate over each subpass
            {

                // Initialized data
                GXSubpass_t *p_subpass = 0;

                // TODO: Check memory

                // TODO: Wait for sync primatives
                
                // Bind the subpasses shader pipeline
                if ( p_subpass->shader->type == g10_pipeline_graphics )
                {
                    vkCmdBindPipeline(
                        p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame],
                        VK_PIPELINE_BIND_POINT_GRAPHICS,
                        p_subpass->shader->graphics.pipeline
                    );
                }
                else if ( p_subpass->shader->type == g10_pipeline_compute )
                {
                    vkCmdBindPipeline(
                        p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame],
                        VK_PIPELINE_BIND_POINT_COMPUTE, 
                        p_subpass->shader->compute.pipeline
                    );
                }
                else if ( p_subpass->shader->type == g10_pipeline_ray )
                {

                    vkCmdBindPipeline(
                        p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame],
                        VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
                        p_subpass->shader->graphics.pipeline
                    );
                }
                else
                    goto failed_to_bind_pipeline_type;

                
                // TODO: Bind descriptor sets
                // TODO: Push constants
                // TODO: call vkCmdDrawIndexed()
                // TODO: Wait for sync primatives

                vkCmdNextSubpass(
                    p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame],
                    VK_SUBPASS_CONTENTS_INLINE
                );
            }
            
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
    if(p_instance->time.ticks%1000 == 1)
        printf("Render time: %dms                           \r", (int)( p_instance->time.delta_time * 1000.f ));
    p_instance->time.ticks += 1;

    // Success
	return 1;

    // TODO:
    failed_to_queue_presentation:
    failed_to_bind_pipeline_type:
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
            if ( p_instance == (void *) 0 ) goto no_instance;
        #endif
    }


    return 1;
    no_instance:
    return 0;
}

int destroy_renderer ( GXRenderer_t **pp_renderer )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_renderer == (void *) 0 ) goto no_renderer;
        #endif
    }

    // TODO:
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
        }
    }
}

int destroy_render_pass ( GXRenderPass_t **pp_render_pass )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_render_pass == (void *) 0 ) goto no_render_pass;
        #endif
    }

    // TODO:
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
        }
    }
}

int destroy_subpass ( GXSubpass_t **pp_subpass )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_subpass == (void *) 0 ) goto no_subpass;
        #endif
    }

    // TODO:
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_subpass:
                #ifndef NDEBUG
                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_subpass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}