#include <G10/GXShader.h>

#define FORMATS_COUNT                 17
#define DESCRIPTOR_TYPE_COUNT         11
#define PUSH_CONSTANT_GETTERS_COUNT   2
#define RASTERIZER_POLYGON_MODE_COUNT 3
#define BLEND_OPERATIONS_COUNT        5
#define BLEND_FACTORS_COUNT           19
#define SHADER_STAGES_COUNT           14
#define SHADER_PIPELINE_CONSTRUCTORS  3

// Forward declarations
int load_graphics_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value );
int load_compute_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value );
int load_ray_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value );

char *shader_pipeline_names [SHADER_PIPELINE_CONSTRUCTORS] =
{
    "graphics",
    "compute",
    "ray"
};
char *format_type_names [FORMATS_COUNT] =
{
    "float",
    "vec2",
    "vec3",
    "vec4",
    "int",
    "ivec2",
    "ivec3",
    "ivec4",
    "uint",
    "uvec2",
    "uvec3",
    "uvec4",
    "double",
    "mat4",
    "mat3",
    "u8vec4",
    "u8vec4 sRGB"
};
char *descriptor_type_names [DESCRIPTOR_TYPE_COUNT] =
{
    "sampler",
    "combined image sampler",
    "sampled image",
    "storage image",
    "uniform texel buffer",
    "storage texel buffer",
    "uniform buffer",
    "storage buffer",
    "uniform buffer dynamic",
    "storage buffer dynamic",
    "input attachment"
};
char *push_constant_getter_names [PUSH_CONSTANT_GETTERS_COUNT] =
{
    "camera position",
    "model matrix"
};
char *rasterizer_polygon_mode_names [RASTERIZER_POLYGON_MODE_COUNT] =
{
    "fill",
    "line",
    "point"
};
char *blend_operation_names [BLEND_OPERATIONS_COUNT] =
{
    "add",
    "subtract",
    "reverse subtract",
    "minimum",
    "maximum"
};
char *blend_factor_names [BLEND_FACTORS_COUNT] =
{
    "zero",
    "one",
    "source color",
    "one minus source color",
    "destination color",
    "one minus destination color",
    "source alpha",
    "one minus source alpha",
    "destination alpha",
    "one minus destination alpha",
    "constant color",
    "one minus constant color",
    "constant alpha",
    "one minus constant alpha",
    "source alpha saturate",
    "source one color",
    "one minus source one color",
    "source one alpha",
    "one minus source one alpha"
};
char *shader_stages_names [SHADER_STAGES_COUNT] =
{
    "vertex",
    "tessellation control",
    "tessellation evaluation",
    "geometry",
    "fragment",
    "compute",
    "task",
    "mesh",
    "ray generation",
    "miss",
    "intersection",
    "closest hit",
    "any hit",
    "callable"
};
void *graphics_pipeline_type_constructors [SHADER_PIPELINE_CONSTRUCTORS] =
{
    &load_graphics_shader_as_json_value,
    &load_compute_shader_as_json_value,
    &load_ray_shader_as_json_value
};
VkDescriptorType  descriptor_type_enums [DESCRIPTOR_TYPE_COUNT] =
{
    VK_DESCRIPTOR_TYPE_SAMPLER,
    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
    VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
    VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
};
VkFormat format_type_enums [FORMATS_COUNT] =
{
    VK_FORMAT_R32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT,
    VK_FORMAT_R32G32B32_SFLOAT,
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R32_SINT,
    VK_FORMAT_R32G32_SINT,
    VK_FORMAT_R32G32B32_SINT,
    VK_FORMAT_R32G32B32A32_SINT,
    VK_FORMAT_R32_UINT,
    VK_FORMAT_R32G32_UINT,
    VK_FORMAT_R32G32B32_UINT,
    VK_FORMAT_R32G32B32A32_UINT,
    VK_FORMAT_R64_SFLOAT,
    // TODO:
    0,
    0,
    VK_FORMAT_R8G8B8A8_UINT,
    VK_FORMAT_R8G8B8A8_SRGB
};
size_t format_type_sizes [FORMATS_COUNT] =
{
    4,
    8,
    12,
    16,
    4,
    8,
    12,
    16,
    4,
    8,
    12,
    16,
    8,
    64,
    36,
    32,
    32
};
void *push_constant_getter_functions [PUSH_CONSTANT_GETTERS_COUNT] =
{
    &get_camera_position,
    &get_model_matrix
};
VkPolygonMode rasterizer_polygon_mode_enum [RASTERIZER_POLYGON_MODE_COUNT] =
{
    VK_POLYGON_MODE_FILL,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_POINT
};
VkBlendOp blend_operation_enums [BLEND_OPERATIONS_COUNT] =
{
    VK_BLEND_OP_ADD,
    VK_BLEND_OP_SUBTRACT,
    VK_BLEND_OP_REVERSE_SUBTRACT,
    VK_BLEND_OP_MIN,
    VK_BLEND_OP_MAX
};
VkBlendFactor blend_factor_enums [BLEND_FACTORS_COUNT] =
{
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    VK_BLEND_FACTOR_DST_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_CONSTANT_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    VK_BLEND_FACTOR_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_SRC_ALPHA_SATURATE,
    VK_BLEND_FACTOR_SRC1_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
    VK_BLEND_FACTOR_SRC1_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
};
VkShaderStageFlagBits shader_stages_enum [SHADER_STAGES_COUNT] =
{
    VK_SHADER_STAGE_VERTEX_BIT,
    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    VK_SHADER_STAGE_GEOMETRY_BIT,
    VK_SHADER_STAGE_FRAGMENT_BIT,
    VK_SHADER_STAGE_COMPUTE_BIT,
    VK_SHADER_STAGE_TASK_BIT_EXT,
    VK_SHADER_STAGE_MESH_BIT_EXT,
    VK_SHADER_STAGE_RAYGEN_BIT_KHR,
    VK_SHADER_STAGE_MISS_BIT_KHR,
    VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
    VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
    VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
    VK_SHADER_STAGE_CALLABLE_BIT_KHR
};

dict *format_types                  = 0,
     *format_sizes                  = 0,
     *descriptor_types              = 0,
     *push_constant_getters         = 0,
     *polygon_modes                 = 0,
     *blend_operations              = 0,
     *blend_factors                 = 0,
     *shader_stagesD                = 0,
     *shader_pipeline_constructors  = 0,
     *pipeline_loader_lookup_tables = 0;

typedef struct {
    char                         *name;
    VkDescriptorType              descriptor_type;
    VkDescriptorSetLayoutBinding  descriptor_set_layout_binding;
    union
    {
        struct {
            size_t i;
        } sampler;

        struct {
            size_t i;
        } combined_image_sampler;

        struct {
            size_t i;
        } sampled_image;

        struct {
            size_t i;
        } storage_image;

        struct {
            size_t i;
        } uniform_texel_buffer;

        struct {
            size_t i;
        } storage_texel_buffer;

        struct {
            size_t i;
        } uniform_buffer_dynamic;

        struct {
            size_t i;
        } storage_buffer_dynamic;

        struct {
            VkDeviceSize    size;
            VkBuffer       *buffers;
            VkDeviceMemory *memories;
        } uniform;

        struct {
            size_t i;
        } input_attachment;

    };
    size_t                        index;
} GXDescriptor_t;

typedef struct {
    char            *name;
    dict            *descriptors;
    GXDescriptor_t **descriptor_data;
    size_t           len,
                     index,
                     descriptor_len;
} GXSet_t;

void init_shader ( void )
{

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();

    // Construct lookup tables
    dict_construct(&format_types                 , FORMATS_COUNT);
    dict_construct(&format_sizes                 , FORMATS_COUNT);
    dict_construct(&descriptor_types             , DESCRIPTOR_TYPE_COUNT);
    dict_construct(&push_constant_getters        , PUSH_CONSTANT_GETTERS_COUNT);
    dict_construct(&polygon_modes                , RASTERIZER_POLYGON_MODE_COUNT);
    dict_construct(&blend_operations             , BLEND_OPERATIONS_COUNT);
    dict_construct(&blend_factors                , BLEND_FACTORS_COUNT);
    dict_construct(&shader_stagesD               , SHADER_STAGES_COUNT);
    dict_construct(&pipeline_loader_lookup_tables, SHADER_PIPELINE_CONSTRUCTORS);

    // Populate lookup tables
    for (size_t i = 0; i < FORMATS_COUNT; i++)
    {
        dict_add(format_types , format_type_names[i]    , (void *)format_type_enums[i]);
        dict_add(format_sizes , format_type_names[i]    , (void *)format_type_sizes[i]);
    }

    for (size_t i = 0; i < DESCRIPTOR_TYPE_COUNT; i++)
        dict_add(descriptor_types, descriptor_type_names[i], (void*)descriptor_type_enums[i]);

    // Initialize push constant getters
    for (size_t i = 0; i < PUSH_CONSTANT_GETTERS_COUNT; i++)
        dict_add(push_constant_getters, push_constant_getter_names[i], (void*)push_constant_getter_functions[i]);

    for (size_t i = 0; i < RASTERIZER_POLYGON_MODE_COUNT; i++)
        dict_add(polygon_modes, rasterizer_polygon_mode_names[i], (void*)rasterizer_polygon_mode_enum[i]);

    for (size_t i = 0; i < BLEND_OPERATIONS_COUNT; i++)
        dict_add(blend_operations, blend_operation_names[i], (void*)blend_operation_enums[i]);

    for (size_t i = 0; i < SHADER_STAGES_COUNT; i++)
        dict_add(shader_stagesD, shader_stages_names[i], (void*)shader_stages_enum[i]);

    for (size_t i = 0; i < BLEND_FACTORS_COUNT; i++)
        dict_add(blend_factors, blend_factor_names[i], (void*)blend_factor_enums[i]);

    for (size_t i = 0; i < SHADER_PIPELINE_CONSTRUCTORS; i++)
        dict_add(pipeline_loader_lookup_tables, shader_pipeline_names[i], graphics_pipeline_type_constructors[i]);


    // Create a shader cache mutex
    p_instance->mutexes.shader_cache = SDL_CreateMutex();

}

int create_shader_module ( char *code, size_t code_len, VkShaderModule *shader_module )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( code          == (void *) 0 ) goto no_code;
            if ( code_len      == 0 )          goto no_code_len;
            if ( shader_module == (void *) 0 ) goto no_shader_module;
        #endif
    }

    // Initialized data
    GXInstance_t             *p_instance                = g_get_active_instance();
    VkShaderModuleCreateInfo  shader_module_create_info =
    {
        .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code_len,
        .pCode    = (u32*)code
    };
    VkResult r = vkCreateShaderModule(p_instance->vulkan.device, &shader_module_create_info, (void*)0, shader_module);

    // Error checking
    switch ( r )
    {
        case VK_SUCCESS:
            goto success;
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            goto vulkan_out_of_host_memory;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            goto vulkan_out_of_device_memory;
        case VK_ERROR_INVALID_SHADER_NV:
            goto vulkan_invalid_shader_NV;
        default:
            goto vulkan_unknown;
    }

    success:

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_code:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"code\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_code_len:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"code_len\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for parameter \"shader_module\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // Vulkan errors
        {
            vulkan_out_of_host_memory:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned out of host memory, in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vulkan_out_of_device_memory:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned out of device memory,  in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vulkan_invalid_shader_NV:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned invalid shader NV, in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            vulkan_unknown:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Call to \"vkCreateShaderModule\" returned an unknown error, in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int create_shader ( GXShader_t **pp_shader )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;
        #endif
    }

    // Initialized data
    GXShader_t *p_shader = calloc(1, sizeof(GXShader_t));

    // Check memory
    if ( p_shader == (void *) 0 )
        goto no_mem;

    // Return a pointer to the caller
    *pp_shader = p_shader;

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
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

int load_shader ( GXShader_t **pp_shader, const char *path )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;
            if ( path      == (void *) 0 ) goto no_path;
        #endif
    }

    // Initialized data
    size_t  len  = g_load_file(path, (void *) 0, true);
    char   *text = calloc(len + 1, sizeof(char));

    // Error check
    if ( text == (void *) 0 )
        goto no_mem;

    // Load a file
    if ( g_load_file(path, text, true) == 0 )
        goto failed_to_load_file;

    // Load a shader as JSON text
    if ( load_shader_as_json(pp_shader, text) == 0 )
        goto failed_to_load_shader_as_json_text;

    // Clean the scope
    free(text);

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

            failed_to_load_shader_as_json_text:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to load shader as JSON text in call to function \"%s\"\n", __FUNCTION__);
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

int load_shader_as_json ( GXShader_t **pp_shader, char *text )
{
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;
            if ( text      == (void *) 0 ) goto no_text;
        #endif
    }

    // Initialized data
    JSONValue_t *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto no_mem;

    // Load a shader as JSON text
    if ( load_shader_as_json_value(pp_shader, p_value) == 0 )
        goto failed_to_load_shader_as_json_value;

    // Clean the scope
    // free_json_value(p_value);

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {

            failed_to_load_shader_as_json_value:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to load shader as JSON value in call to function \"%s\"\n", __FUNCTION__);
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

int load_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;
            if ( p_value   == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized dataS
    GXInstance_t  *p_instance                  = g_get_active_instance();
    GXShader_t    *p_shader                    = 0;
    JSONValue_t   *p_type                      = 0,
                  *p_name                      = 0,
                  *p_vertex_shader_path        = 0,
                  *p_geometry_shader_path      = 0,
                  *p_fragment_shader_path      = 0,
                  *p_compute_shader_path       = 0,
                  *p_rasterizer_json           = 0,
                  *p_vertex_shader_json_data   = 0,
                  *p_geometry_shader_json_data = 0,
                  *p_fragment_shader_json_data = 0,
                  *p_compute_shader_json_data  = 0,
                  *p_vertex_groups             = 0,
                  *p_attachments               = 0,
                  *p_sets                      = 0,
                  *p_push_constant_text        = 0;
    size_t         set_count                   = 0;

    // Load a shader as a JSON object
    if ( p_value->type == JSONobject )
    {

        // Get the pipeline type
        p_type = dict_get(p_value->object, "type");

        // Was a pipeline type provided?
        if ( p_type )
        {

            // Get a function pointer from the lookup table
            int (*pipeline_load_as_json_value)(GXShader_t **pp_shader, JSONValue_t *p_value) = dict_get(pipeline_loader_lookup_tables, p_type->string);

            // If the function is valid...
            if ( pipeline_load_as_json_value )

                // ... call the function
                if ( pipeline_load_as_json_value(pp_shader, p_value) == 0 )
                    goto failed_to_parse_shader_pipeline;
        }
        else
            goto unrecognized_type;
    }
    // Load the shader as a path
    else if ( p_value->type == JSONstring )
    {

        // Load the shader as a path
        if ( load_shader(pp_shader, p_value->string) == 0 )
            goto failed_to_load_shader_as_path;
    }
    // Type error
    else
        goto wrong_type;

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {

            failed_to_load_shader_as_path:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to load shader from path in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

            // TODO: More specific
            failed_to_parse_shader_pipeline:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to load shader in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
        }

        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to load shader in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

            unrecognized_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Unrecognized type property value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int use_shader ( GXShader_t *p_shader )
{

    // Argument errors
    {
        #ifndef NDEBUG
            if ( p_shader ) goto no_shader;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    VkRect2D scissor =
    {
        .offset.x = 0,
        .offset.y = 0,
        .extent   = p_instance->vulkan.swap_chain_extent
    };
    VkViewport viewport =
    {
        .x        = 0.f,
        .y        = 0.f,
        .width    = (float)p_instance->vulkan.swap_chain_extent.width,
        .height   = (float)p_instance->vulkan.swap_chain_extent.height,
        .minDepth = 0.f,
        .maxDepth = 1.f
    };

    // Use the shader
    vkCmdBindPipeline(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, p_shader->graphics.pipeline);

    // Set the viewport
    vkCmdSetViewport(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0, 1, &viewport);

    // Set the scissors
    vkCmdSetScissor(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0, 1, &scissor);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/*
int update_shader_push_constant ( GXShader_t *shader )
{
    size_t offset = 0;

    for (size_t i = 0; shader->graphics.push_constant_properties[i]; i++)
    {
        char   *push_constant_property_name = shader->graphics.push_constant_properties[i];
        int   (*getter) ( void * )          = (int (*)( void * )) dict_get(push_constant_getters, push_constant_property_name);

        if (getter)
            offset = getter((u8*)shader->graphics.push_constant_data + offset);

    }
    return 1;
}

int add_shader_push_constant_getter ( char *getter_name, int(*getter_function)(void*) )
{

    // Argument errors
    {
        #ifndef NDEBUG
            if (getter_name == (void *)0)
                goto no_getter_name;
            if (getter_function == (void*)0)
                goto no_getter_function;
        #endif
    }

    // Error checking
    {
        #ifndef NDEBUG
            if (dict_get(push_constant_getters, getter_name))
                goto already_has_getter;
        #endif
    }

    dict_add(push_constant_getters, getter_name, getter_function);

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_getter_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"getter_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            no_getter_function:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"getter_function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // User errors
        {
            already_has_getter:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to add getter in call to function \"%s\". There is already a getter called \"%s\"\n", __FUNCTION__, getter_name);
                #endif

                // Error
                return 0;
        }
    }
}

int set_shader_camera               ( GXEntity_t   *p_entity )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_entity == (void *) 0 )
                goto no_shader;
        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance = g_get_active_instance();
    GXCamera_t    *camera     = p_instance->context.scene->active_camera;
    GXTransform_t *transform  = p_entity->transform;

    mat4 uniform_buffer[4];

    {
        // Update the transform
        transform_model_matrix(transform, &uniform_buffer[0]);

        vec3 a;
        add_vec3(&a, camera->target, camera->location);

        // Update the view matrix
        camera->view = look_at(camera->location, a, camera->up);

        // Update the projection matrix
        camera->projection = perspective_matrix(camera->fov, camera->aspect_ratio, camera->near_clip, camera->far_clip);

        uniform_buffer[1] = camera->view;
        uniform_buffer[2] = camera->projection;

        uniform_buffer[3].a = camera->location.x;
        uniform_buffer[3].b = camera->location.y;
        uniform_buffer[3].c = camera->location.z;
    }

    void *data;

    vkMapMemory(p_instance->vulkan.device, p_entity->shader->graphics.uniform_buffers_memory[p_instance->vulkan.current_frame], 0, sizeof(vec3) + 3 * sizeof(mat4), 0, &data);
    memcpy(data, uniform_buffer, sizeof(vec3) + 3*sizeof(mat4));
    vkUnmapMemory(p_instance->vulkan.device, p_entity->shader->graphics.uniform_buffers_memory[p_instance->vulkan.current_frame]);

    return 1;

    // Error handling
    {

        // Argument error
        {
            #ifndef NDEBUG
                no_shader:
                    g_print_log("[G10] [Shader] Null pointer provided for \"shader\" in call to function \"%s\"\n", __FUNCTION__);
                    return 0;
            #endif
        }
    }
}
*/
int destroy_shader ( GXShader_t *shader )
{

    // TODO: Cache destruction
    GXInstance_t* p_instance = g_get_active_instance();

    vkDestroyPipeline(p_instance->vulkan.device, shader->graphics.pipeline, 0);
    vkDestroyPipelineLayout(p_instance->vulkan.device, shader->graphics.pipeline_layout, 0);

    vkDestroyShaderModule(p_instance->vulkan.device, shader->graphics.vertex_shader_module, 0);
    vkDestroyShaderModule(p_instance->vulkan.device, shader->graphics.fragment_shader_module, 0);

    free(shader->name);
    free(shader);
    return 1;
}

int load_graphics_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value )
{
    // Argument notes
    // p_value will always be valid. p_value->type will always be JSONobject

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;

        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance                            = g_get_active_instance();
    GXShader_t    *p_shader                              = 0;
    JSONValue_t   *p_name                                = 0,
                  *p_vertex_shader_path                  = 0,
                  *p_tessellation_control_shader_path    = 0,
                  *p_tessellation_evaluation_shader_path = 0,
                  *p_task_shader_path                    = 0,
                  *p_mesh_shader_path                    = 0,
                  *p_geometry_shader_path                = 0,
                  *p_fragment_shader_path                = 0,
                  *p_multisampler                        = 0,
                  *p_layout                              = 0,
                  *p_in                                  = 0,
                  *p_attachments                         = 0,
                  *p_layout_sets                         = 0,
                  *p_layout_push_constant                = 0,
                  *p_rasterizer                          = 0;

    // Parse the JSON value
    {

        p_name                                = dict_get(p_value->object, "name");
        p_vertex_shader_path                  = dict_get(p_value->object, "vertex shader path");
        p_tessellation_control_shader_path    = dict_get(p_value->object, "tessellation control shader path");
        p_tessellation_evaluation_shader_path = dict_get(p_value->object, "tessellation evaluation shader path");
        p_geometry_shader_path                = dict_get(p_value->object, "geometry shader path");
        p_task_shader_path                    = dict_get(p_value->object, "task shader path");
        p_mesh_shader_path                    = dict_get(p_value->object, "mesh shader path");
        p_fragment_shader_path                = dict_get(p_value->object, "fragment shader path");
        p_multisampler                        = dict_get(p_value->object, "multisampler");
        p_layout                              = dict_get(p_value->object, "layout");
        p_in                                  = dict_get(p_value->object, "in");
        p_attachments                         = dict_get(p_value->object, "attachments");

        if ( p_layout )
        {
            if ( p_layout->type == JSONobject )
            {
                p_layout_sets          = dict_get(p_layout->object, "sets");
                p_layout_push_constant = dict_get(p_layout->object, "push constant");
            }
        }
        p_rasterizer = dict_get(p_value->object, "rasterizer");


        // Check properties
        if ( !(p_name && p_fragment_shader_path ) )
            goto missing_properties;

        if ( p_vertex_shader_path && p_task_shader_path && p_mesh_shader_path )
            goto missing_properties;

        if ( ( (bool)(p_vertex_shader_path) ^ (bool)(p_task_shader_path && p_mesh_shader_path) ) == 0 )
            goto missing_properties;
    }

    // TODO: Cache check

    // Construct the shader
    {

        // Allocate memory for a shader
        if ( create_shader(&p_shader) == 0 )
            goto failed_to_allocate_shader;

        // Set the shader type
        p_shader->type = g10_pipeline_graphics;

        // Set the name
        if ( p_name->type == JSONstring )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_shader->name == (void *) 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_shader->name, p_name->string, name_len);
        }
        else
            goto name_wrong_type;

        // Load the shader binaries
        {

            // Load the vertex shader binary
            if ( p_vertex_shader_path )
            {

                // Parse the vertex shader path as a string
                if ( p_vertex_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t vertex_shader_data_len = g_load_file(p_vertex_shader_path->string, 0, true);
                    char* vertex_shader_data = calloc(vertex_shader_data_len, sizeof(char));

                    // Error check
                    if ( vertex_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the vertex shader binary file
                    if ( g_load_file(p_vertex_shader_path->string, vertex_shader_data, true) == 0 )
                        goto failed_to_load_vertex_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(vertex_shader_data, vertex_shader_data_len, &p_shader->graphics.vertex_shader_module) == 0 )
                        goto failed_to_create_vertex_shader_module;

                    // Clean the scope
                    free(vertex_shader_data);
                }
                else
                    goto wrong_vertex_shader_path_type;
            }

            // Load tessellation control shader
            if ( p_tessellation_control_shader_path )
            {

                // Parse the tessellation control shader path as a string
                if ( p_tessellation_control_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t tessellation_control_shader_data_len = g_load_file(p_tessellation_control_shader_path->string, 0, true);
                    char* tessellation_control_shader_data = calloc(tessellation_control_shader_data_len, sizeof(char));

                    // Error check
                    if ( tessellation_control_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the tessellation_control shader binary file
                    if ( g_load_file(p_tessellation_control_shader_path->string, tessellation_control_shader_data, true) == 0 )
                        goto failed_to_load_tessellation_control_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(tessellation_control_shader_data, tessellation_control_shader_data_len, &p_shader->graphics.tessellation_control_shader_module) == 0 )
                        goto failed_to_create_tessellation_control_shader_module;

                    // Clean the scope
                    free(tessellation_control_shader_data);
                }
                else
                    goto wrong_tessellation_control_shader_path_type;
            }

            // Load tessellation evaluation shader
            if ( p_tessellation_evaluation_shader_path )
            {

                // Parse the tessellation evaluation shader path as a string
                if ( p_tessellation_evaluation_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t tessellation_evaluation_shader_data_len = g_load_file(p_tessellation_evaluation_shader_path->string, 0, true);
                    char* tessellation_evaluation_shader_data = calloc(tessellation_evaluation_shader_data_len, sizeof(char));

                    // Error check
                    if ( tessellation_evaluation_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the tessellation evaluation shader binary file
                    if ( g_load_file(p_tessellation_evaluation_shader_path->string, tessellation_evaluation_shader_data, true) == 0 )
                        goto failed_to_load_tessellation_evaluation_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(tessellation_evaluation_shader_data, tessellation_evaluation_shader_data_len, &p_shader->graphics.tessellation_control_shader_module) == 0 )
                        goto failed_to_create_tessellation_evaluation_shader_module;

                    // Clean the scope
                    free(tessellation_evaluation_shader_data);
                }
                else
                    goto wrong_tessellation_evaluation_shader_path_type;
            }

            // Load the geometry shader binary
            if ( p_geometry_shader_path )
            {

                // Parse the geometry shader path as a string
                if ( p_geometry_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t geometry_shader_data_len = g_load_file(p_geometry_shader_path->string, 0, true);
                    char* geometry_shader_data = calloc(geometry_shader_data_len, sizeof(char));

                    // Error check
                    if ( geometry_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the geometry shader binary file
                    if ( g_load_file(p_geometry_shader_path->string, geometry_shader_data, true) == 0 )
                        goto failed_to_load_geometry_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(geometry_shader_data, geometry_shader_data_len, &p_shader->graphics.geometry_shader_module) == 0 )
                        goto failed_to_create_geometry_shader_module;

                    // Clean the scope
                    free(geometry_shader_data);
                }
                else
                    goto wrong_geometry_shader_path_type;
            }

            // Load the task shader binary
            if ( p_task_shader_path )
            {

                // Parse the task shader path as a string
                if ( p_task_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t task_shader_data_len = g_load_file(p_task_shader_path->string, 0, true);
                    char* task_shader_data = calloc(task_shader_data_len, sizeof(char));

                    // Error check
                    if ( task_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the task shader binary file
                    if ( g_load_file(p_task_shader_path->string, task_shader_data, true) == 0 )
                        goto failed_to_load_task_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(task_shader_data, task_shader_data_len, &p_shader->graphics.task_shader_module) == 0 )
                        goto failed_to_create_task_shader_module;

                    // Clean the scope
                    free(task_shader_data);
                }
                else
                    goto wrong_task_shader_path_type;
            }

            // Load the mesh shader binary
            if ( p_mesh_shader_path )
            {

                // Parse the mesh shader path as a string
                if ( p_mesh_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t mesh_shader_data_len = g_load_file(p_mesh_shader_path->string, 0, true);
                    char* mesh_shader_data = calloc(mesh_shader_data_len, sizeof(char));

                    // Error check
                    if ( mesh_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the mesh shader binary file
                    if ( g_load_file(p_mesh_shader_path->string, mesh_shader_data, true) == 0 )
                        goto failed_to_load_mesh_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(mesh_shader_data, mesh_shader_data_len, &p_shader->graphics.mesh_shader_module) == 0 )
                        goto failed_to_create_mesh_shader_module;

                    // Clean the scope
                    free(mesh_shader_data);
                }
                else
                    goto wrong_mesh_shader_path_type;
            }

            // Load the fragment shader binary
            if ( p_fragment_shader_path )
            {

                // Parse the fragment shader path as a string
                if ( p_fragment_shader_path->type == JSONstring )
                {
                    // Initialized data
                    size_t fragment_shader_data_len = g_load_file(p_fragment_shader_path->string, 0, true);
                    char* fragment_shader_data = calloc(fragment_shader_data_len + 1, sizeof(char));

                    // Error check
                    if ( fragment_shader_data == (void *) 0 )
                        goto no_mem;

                    // Load the fragment shader binary file
                    if ( g_load_file(p_fragment_shader_path->string, fragment_shader_data, true) == 0 )
                        goto failed_to_load_fragment_shader_binary;

                    // Create a shader module
                    if ( create_shader_module(fragment_shader_data, fragment_shader_data_len, &p_shader->graphics.fragment_shader_module) == 0 )
                        goto failed_to_create_fragment_shader_module;

                    // Clean the scope
                    free(fragment_shader_data);
                }
                else
                    goto wrong_fragment_shader_path_type;
            }
        }

        // Construct the graphics shader
        {

            // Set up the shader stages
            {
                // Initialized data
                VkPipelineShaderStageCreateInfo        *shader_stages                       = calloc(8, sizeof(VkPipelineShaderStageCreateInfo));
                size_t                                  shader_stage_iterator               = 0,
                                                        attachment_count                    = 0;
                VkPipelineVertexInputStateCreateInfo    vertex_input_info_create_info       = { 0 };
                VkPipelineInputAssemblyStateCreateInfo  input_assembly_create_info          = { 0 };
                VkPipelineTessellationStateCreateInfo   tessellation_state_create_info      = { 0 };
                VkPipelineViewportStateCreateInfo       viewport_state_create_info          = { 0 };
                VkPipelineRasterizationStateCreateInfo  rasterizer_create_info              = { 0 };
                VkPipelineMultisampleStateCreateInfo    multisampling_create_info           = { 0 };
                VkPipelineColorBlendAttachmentState    *color_blend_attachment_create_info  = 0;
                VkPipelineColorBlendStateCreateInfo     color_blend_create_info             = { 0 };
                VkPipelineDepthStencilStateCreateInfo   depth_stencil_state_create_info     = { 0 };
                VkPipelineDynamicStateCreateInfo        dynamic_state_create_info           = { 0 };
                VkPipelineLayoutCreateInfo              pipeline_layout_create_info         = { 0 };
                VkDynamicState                          dynamic_states[2]                   = { { 0 }, { 0 } };
                VkPushConstantRange                    *push_constant                       = calloc(1, sizeof(VkPushConstantRange));
                VkGraphicsPipelineCreateInfo            graphics_pipeline_create_info       = { 0 };
                VkVertexInputBindingDescription         binding_description                 = { 0 };
                VkViewport                              viewport                            = { 0 };
                VkRect2D                                scissor                             = { 0 };
                VkVertexInputAttributeDescription      *vertex_input_attribute_descriptions = 0;
                VkDescriptorSetLayoutCreateInfo        *descriptor_set_layout               = 0;

                //////////////////////////////
                // Set up the shader stages //
                //////////////////////////////
                {

                    // Set up the vertex shader
                    if (p_shader->graphics.vertex_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_VERTEX_BIT,
                            .module = p_shader->graphics.vertex_shader_module,
                            .pName  = "main"
                        };

                    }

                    // Set up the tessellation control shader
                    if (p_shader->graphics.tessellation_control_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
                            .module = p_shader->graphics.tessellation_control_shader_module,
                            .pName  = "main"
                        };
                    }

                    // Set up the tessellation evaluation shader
                    if (p_shader->graphics.tessellation_evaluation_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
                            .module = p_shader->graphics.tessellation_evaluation_shader_module,
                            .pName  = "main"
                        };
                    }

                    // Set up the geometry shader
                    if (p_shader->graphics.geometry_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_GEOMETRY_BIT,
                            .module = p_shader->graphics.geometry_shader_module,
                            .pName  = "main"
                        };
                    }

                    // Set up the task shader
                    if (p_shader->graphics.task_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_TASK_BIT_EXT,
                            .module = p_shader->graphics.task_shader_module,
                            .pName  = "main"
                        };
                    }

                    // Set up the mesh shader
                    if (p_shader->graphics.mesh_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_MESH_BIT_EXT,
                            .module = p_shader->graphics.mesh_shader_module,
                            .pName  = "main"
                        };
                    }

                    // Set up the fragment shader
                    if (p_shader->graphics.fragment_shader_module)
                    {
                        shader_stages[shader_stage_iterator++] = (VkPipelineShaderStageCreateInfo)
                        {
                            .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                            .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
                            .module = p_shader->graphics.fragment_shader_module,
                            .pName  = "main"
                        };
                    }

                }

                ///////////////////////////////////
                // Set up the vertex input state //
                ///////////////////////////////////
                if ( p_in )
                {

                    // Initialized data
                    size_t vertex_group_count        = 0,
                           stride                    = 0,
                           binding_description_count = 0;

                    // Did the user specify vertex groups?
                    {

                        // Initialized data
                        size_t        vertex_group_count = 0;
                        JSONValue_t **pp_vertex_groups   = 0;

                        // Get the contents of the array
                        {

                            // Get the array size
                            array_get(p_in->list, 0, &vertex_group_count);

                            // Allocate for vertex group JSON values
                            pp_vertex_groups = calloc(vertex_group_count+1, sizeof(JSONValue_t *));

                            // Error checking
                            if ( pp_vertex_groups == (void *) 0 )
                                goto no_mem;

                            // Get the array contents
                            array_get(p_in->list, pp_vertex_groups, 0);
                        }

                        // Allocate vertex attributes
                        vertex_input_attribute_descriptions = calloc(vertex_group_count, sizeof(VkVertexInputAttributeDescription));

                        // Iterate over each vertex group
                        for (size_t i = 0; i < vertex_group_count; i++)
                        {

                            // Initialized data
                            dict        *vertex_group               = 0;
                            char        *input_attribute_type       = 0;
                            signed       input_attribute_location   = 0;
                            JSONValue_t *p_vertex_group             = pp_vertex_groups[i],
                                        *p_input_attribute_type     = 0,
                                        *p_input_attribute_location = 0;

                            // Parse the JSON value
                            if ( p_vertex_group->type == JSONobject )
                            {

                                p_input_attribute_type     = dict_get(p_vertex_group->object, "type");
                                p_input_attribute_location = dict_get(p_vertex_group->object, "location");

                                // Check properties
                                if ( ! ( p_input_attribute_type && p_input_attribute_location ) )
                                    goto wrong_input_attribute_properties;

                            }
                            else
                                goto vertex_group_wrong_type;

                            if ( p_input_attribute_type->type == JSONstring )
                                input_attribute_type = p_input_attribute_type->string;

                            if ( p_input_attribute_type->type == JSONinteger )
                                input_attribute_location = p_input_attribute_location->integer;

                            // Construct a vertex input attribute
                            vertex_input_attribute_descriptions[i] = (VkVertexInputAttributeDescription)
                            {
                                .binding  = 0,
                                .location = input_attribute_location,
                                .format   = (VkFormat)dict_get(format_types, input_attribute_type),
                                .offset   = (u32)stride
                            };

                            // Accumulate stride
                            stride += (size_t) dict_get(format_sizes, input_attribute_type);
                        }

                        // Construct a binding description
                        binding_description = (VkVertexInputBindingDescription)
                        {
                            .binding   = 0,
                            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
                            .stride    = (u32)stride
                        };
                    }

                    // Set up the vertex input
                    vertex_input_info_create_info = (VkPipelineVertexInputStateCreateInfo)
                    {
                        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
                        .pNext                           = 0,
                        .flags                           = 0,
                        .vertexBindingDescriptionCount   = (u32)binding_description_count,
                        .pVertexBindingDescriptions      = &binding_description,
                        .vertexAttributeDescriptionCount = (u32)vertex_group_count,
                        .pVertexAttributeDescriptions    = vertex_input_attribute_descriptions
                    };
                }
                // ^^^ TODO: Default ^^^
                else
                    ;

                ///////////////////////////////
                // Set up the input assembly //
                ///////////////////////////////
                input_assembly_create_info = (VkPipelineInputAssemblyStateCreateInfo)
                {
                    .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                    .pNext                  = 0,
                    .flags                  = 0,
                    .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                    .primitiveRestartEnable = VK_FALSE,
                };

                ///////////////////////////////////
                // Set up the tessellation state //
                ///////////////////////////////////
                tessellation_state_create_info = (VkPipelineTessellationStateCreateInfo) { 0 };

                ///////////////////////////////
                // Set up the viewport state //
                ///////////////////////////////
                {

                    // Set up the viewport
                    viewport = (VkViewport)
                    {
                        .x        = 0.f,
                        .y        = 0.f,
                        .width    = (float)p_instance->vulkan.swap_chain_extent.width,
                        .height   = (float)p_instance->vulkan.swap_chain_extent.height,
                        .minDepth = 0.0f,
                        .maxDepth = 1.0f
                    };

                    // Set up the scissor
                    scissor = (VkRect2D)
                    {
                        .offset.x = 0,
                        .offset.y = 0,
                        .extent   = p_instance->vulkan.swap_chain_extent
                    };

                    // Populate the viewport state create struct
                    viewport_state_create_info = (VkPipelineViewportStateCreateInfo)
                    {
                        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                        .viewportCount = 1,
                        .pViewports    = &viewport,
                        .scissorCount  = 1,
                        .pScissors     = &scissor
                    };
                }

                ///////////////////////////
                // Set up the rasterizer //
                ///////////////////////////
                if ( p_rasterizer )
                {

                    // Parse the multisampler as a JSON object
                    if ( p_rasterizer->type == JSONobject )
                    {

                        // Initialized data
                        VkPolygonMode polygon_mode                 = VK_POLYGON_MODE_FILL;
                        JSONValue_t  *p_line_width                 = 0,
                                     *p_depth_bias_constant_factor = 0,
                                     *p_depth_bias_clamp           = 0,
                                     *p_depth_bias_slope_factor    = 0,
                                     *p_depth_clamp_enable         = 0,
                                     *p_rasterizer_discard_enable  = 0,
                                     *p_clockwise                  = 0,
                                     *p_depth_bias_enable          = 0;
                        float         line_width                   = 1.f,
                                      depth_bias_constant_factor   = 0.f,
                                      depth_bias_clamp             = 0.f,
                                      depth_bias_slope_factor      = 0.f;
                        bool          depth_clamp_enable           = false,
                                      rasterizer_discard_enable    = false,
                                      clockwise                    = false,
                                      depth_bias_enable            = false;

                        // Parse the rasterizer
                        {

                            p_line_width                 = dict_get(p_rasterizer->object, "line width");
                            p_depth_bias_constant_factor = dict_get(p_rasterizer->object, "depth bias constant factor");
                            p_depth_bias_clamp           = dict_get(p_rasterizer->object, "depth bias clamp");
                            p_depth_bias_slope_factor    = dict_get(p_rasterizer->object, "depth bias slope factor");
                            p_depth_clamp_enable         = dict_get(p_rasterizer->object, "depth clamp enable");
                            p_rasterizer_discard_enable  = dict_get(p_rasterizer->object, "rasterizer discard enable");
                            p_clockwise                  = dict_get(p_rasterizer->object, "clockwise");
                            p_depth_bias_enable          = dict_get(p_rasterizer->object, "depth bias enable");

                            if ( p_line_width )
                            {
                                if ( p_line_width->type == JSONfloat )
                                    line_width = (float) p_line_width->floating;
                                else
                                    goto no_rasterizer_line_width;
                            }

                            if ( p_depth_bias_constant_factor )
                            {
                                if ( p_depth_bias_constant_factor->type == JSONfloat )
                                    line_width = (float) p_depth_bias_constant_factor->floating;
                                else
                                    goto no_rasterizer_depth_bias_constant_factor;
                            }

                            if ( p_depth_bias_clamp )
                            {
                                if ( p_depth_bias_clamp->type == JSONfloat )
                                    line_width = (float) p_depth_bias_clamp->floating;
                                else
                                    goto no_rasterizer_depth_bias_clamp;
                            }

                            if ( p_depth_bias_slope_factor )
                            {
                                if ( p_depth_bias_slope_factor->type == JSONfloat )
                                    line_width = (float) p_depth_bias_slope_factor->floating;
                                else
                                    goto no_rasterizer_depth_bias_slope_factor;
                            }

                            if ( p_depth_clamp_enable )
                            {
                                if ( p_depth_clamp_enable->type == JSONboolean )
                                    depth_clamp_enable = p_depth_clamp_enable->boolean;
                                else
                                    goto no_depth_clamp_enable;
                            }

                            if ( p_rasterizer_discard_enable )
                            {
                                if ( p_rasterizer_discard_enable->type == JSONboolean )
                                    rasterizer_discard_enable = p_rasterizer_discard_enable->boolean;
                                else
                                    goto no_rasterizer_rasterizer_discard_enable;
                            }

                            if ( p_clockwise )
                            {
                                if ( p_clockwise->type == JSONboolean )
                                    clockwise = p_clockwise->boolean;
                                else
                                    goto no_rasterizer_clockwise;
                            }

                            if ( p_depth_bias_enable )
                            {
                                if ( p_depth_bias_enable->type == JSONboolean )
                                    depth_bias_enable = p_depth_bias_enable->boolean;
                                else
                                    goto no_rasterizer_depth_bias_enable;
                            }

                        }

                        // Populate the rasterizer create struct
                        rasterizer_create_info = (VkPipelineRasterizationStateCreateInfo)
                        {
                            .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                            .pNext                   = 0,
                            .flags                   = 0,
                            .depthClampEnable        = depth_clamp_enable,
                            .rasterizerDiscardEnable = rasterizer_discard_enable,
                            .polygonMode             = polygon_mode,
                            .cullMode                = VK_CULL_MODE_FRONT_BIT,
                            .frontFace               = clockwise,
                            .depthBiasEnable         = depth_bias_enable,
                            .depthBiasConstantFactor = depth_bias_constant_factor,
                            .depthBiasClamp          = depth_bias_clamp,
                            .depthBiasSlopeFactor    = depth_bias_slope_factor,
                            .lineWidth               = line_width,
                        };
                    }
                    else
                        goto wrong_rasterizer_type;
                }
                // Default
                else
                {
                    // Populate the rasterizer create struct
                    rasterizer_create_info = (VkPipelineRasterizationStateCreateInfo)
                    {
                        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                        .pNext                   = 0,
                        .flags                   = 0,
                        .depthClampEnable        = false,
                        .rasterizerDiscardEnable = false,
                        .polygonMode             = VK_POLYGON_MODE_FILL,
                        .cullMode                = VK_CULL_MODE_FRONT_BIT,
                        .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                        .depthBiasEnable         = true,
                        .depthBiasConstantFactor = 0.0,
                        .depthBiasClamp          = 0.0,
                        .depthBiasSlopeFactor    = 0.0,
                        .lineWidth               = 1.0
                    };
                }

                /////////////////////////////
                // Set up the multisampler //
                /////////////////////////////
                if ( p_multisampler )
                {

                    // Parse the multisampler as a JSON object
                    if ( p_multisampler->type == JSONobject )
                    {

                        // Initialized data
                        JSONValue_t *p_samples                  = dict_get(p_multisampler->object, "samples"),
                                    *p_sample_shading_enable    = dict_get(p_multisampler->object, "sample shading enable"),
                                    *p_minimum_sample_shading   = dict_get(p_multisampler->object, "minimum sample shading"),
                                    *p_alpha_to_coverage_enable = dict_get(p_multisampler->object, "alpha to coverage enable"),
                                    *p_alpha_to_one_enable      = dict_get(p_multisampler->object, "alpha to one enable");
                        bool         sample_shading_enable      = false,
                                     alpha_to_coverage_enable   = false,
                                     alpha_to_one_enable        = false;
                        signed       samples                    = 1;
                        float        minimum_sample_shading     = 0.0;

                        // Parse the multisampler
                        {

                            if ( p_samples )
                            {
                                if ( p_samples->type == JSONinteger )
                                    samples = (signed int) p_samples->integer;
                                else
                                    goto no_multisampler_samples;
                            }

                            if ( p_sample_shading_enable )
                            {
                                if ( p_sample_shading_enable->type == JSONboolean )
                                    sample_shading_enable = p_sample_shading_enable->boolean;
                                else
                                    goto no_multisampler_sample_shading_enable;
                            }

                            if ( p_alpha_to_coverage_enable )
                            {
                                if ( p_alpha_to_coverage_enable->type == JSONboolean )
                                    alpha_to_coverage_enable = p_alpha_to_coverage_enable->boolean;
                                else
                                    goto no_multisampler_alpha_to_coverage_enable;
                            }

                            if ( p_alpha_to_one_enable )
                            {
                                if ( p_alpha_to_one_enable->type == JSONboolean )
                                    alpha_to_one_enable = p_alpha_to_one_enable->boolean;
                                else
                                    goto no_multisampler_alpha_to_one_enable;
                            }

                            if ( p_minimum_sample_shading )
                            {
                                if ( p_minimum_sample_shading->type == JSONfloat )
                                    minimum_sample_shading = (float) p_minimum_sample_shading->floating;
                                else
                                    goto no_minimum_sample_shading;
                            }
                        }

                        multisampling_create_info  = (VkPipelineMultisampleStateCreateInfo)
                        {
                            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                            .pNext                 = 0,
                            .flags                 = 0,
                            .rasterizationSamples  = samples,
                            .sampleShadingEnable   = sample_shading_enable,
                            .minSampleShading      = minimum_sample_shading,
                            .pSampleMask           = 0,
                            .alphaToCoverageEnable = alpha_to_coverage_enable,
                            .alphaToOneEnable      = alpha_to_one_enable
                        };
                    }
                     // Default
                    else
                        goto wrong_multisampler_type;
                }
                // Default
                else
                {
                    multisampling_create_info  = (VkPipelineMultisampleStateCreateInfo)
                    {
                            .sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                            .pNext                 = 0,
                            .flags                 = 0,
                            .rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT,
                            .sampleShadingEnable   = VK_FALSE,
                            .minSampleShading      = 1.f,
                            .pSampleMask           = 0,
                            .alphaToCoverageEnable = 0,
                            .alphaToOneEnable      = 0
                        };
                }

                //////////////////////////////////////
                // Set up the depth / stencil state //
                //////////////////////////////////////
                depth_stencil_state_create_info = (VkPipelineDepthStencilStateCreateInfo)
                {
                    .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                    .depthTestEnable       = VK_TRUE,
                    .depthWriteEnable      = VK_TRUE,
                    .depthCompareOp        = VK_COMPARE_OP_LESS,
                    .depthBoundsTestEnable = VK_FALSE,
                    .minDepthBounds        = 0.f,
                    .maxDepthBounds        = 1.f,
                    .stencilTestEnable     = VK_FALSE
                };

                //////////////////////////////////
                // Set up the color blend state //
                //////////////////////////////////
                color_blend_create_info = (VkPipelineColorBlendStateCreateInfo)
                {
                    .sType           = 0, // VkStructureType
                    .pNext           = 0, // const void*
                    .flags           = 0, // VkPipelineColorBlendStateCreateFlags
                    .logicOpEnable   = 0, // VkBool32
                    .logicOp         = 0, // VkLogicOp
                    .attachmentCount = 0, // uint32_t
                    .pAttachments    = 0, // const VkPipelineColorBlendAttachmentState*
                    .blendConstants  = 0  // float
                };

                //////////////////////////////
                // Set up the dynamic state //
                //////////////////////////////
                {
                    dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
                    dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR;

                    dynamic_state_create_info = (VkPipelineDynamicStateCreateInfo)
                    {
                        .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                        .dynamicStateCount = 2,
                        .pDynamicStates    = &dynamic_states
                    };
                }

                ////////////////////////////////
                // Create the pipeline layout //
                ////////////////////////////////
                {

                    // Initialized data
                    VkPipelineLayoutCreateInfo pipeline_layout_create_info = {
                        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                        .pNext                  = 0,
                        .flags                  = 0,
                        .setLayoutCount         = 0,
                        .pSetLayouts            = 0,
                        .pushConstantRangeCount = 0,
                        .pPushConstantRanges    = 0
                    };

                    // Create a pipeline layout
                    if ( vkCreatePipelineLayout(p_instance->vulkan.device, &pipeline_layout_create_info, 0, &p_shader->graphics.pipeline_layout) != VK_SUCCESS )
                        goto failed_to_create_pipeline_layout;
                }

                //////////////////////////////
                // Set up the push constant //
                //////////////////////////////
                if ( p_layout_push_constant )
                {

                    /*
                    // Initialized data
                    dict    *push_constant_dict           = 0;
                    char   **push_constant_struct         = 0;
                    size_t   push_constant_size           = 0;
                    size_t   push_constant_property_count = 0;

                    // Parse the push constant JSON into a dictionary
                    parse_json(push_constant_text, strlen(push_constant_text), &push_constant_dict);

                    // Parse the push constant dictionary
                    {

                        // Initialized data
                        JSONToken_t  *token                = 0;

                        // Get the push constant struct
                        token                = (JSONToken_t *) dict_get(push_constant_dict, "struct");
                        push_constant_struct = JSON_VALUE(token, JSONarray);
                    }

                    while (push_constant_struct[++push_constant_property_count]);

                    i_shader->graphics.push_constant_properties = calloc(push_constant_property_count + 1, sizeof(char*));

                    for (size_t i = 0; i < push_constant_property_count; i++)
                    {

                        // Initialized data
                        char   *push_constant_property     = push_constant_struct[i];
                        size_t  push_constant_property_len = strlen(push_constant_property);
                        dict   *property_dict              = 0;

                        char   *property_name              = 0;
                        size_t  property_size              = 0;

                        parse_json(push_constant_property, push_constant_property_len, &property_dict);

                        // Parse the property dictionary
                        {

                            // Initialized data
                            JSONToken_t *token = 0;

                            token = (JSONToken_t *) dict_get(property_dict, "name");
                            property_name = JSON_VALUE(token, JSONstring);

                            token = (JSONToken_t *) dict_get(property_dict, "type");
                            char* property_type = JSON_VALUE(token, JSONstring);

                            // TODO: Check property_type?
                            property_size = (size_t) dict_get(format_sizes, property_type);

                        }

                        // Copy the property name
                        {

                            // Initialized data
                            size_t property_name_len              = strlen(property_name);

                            // Allocate the property name
                            i_shader->graphics.push_constant_properties[i] = calloc(property_name_len + 1, sizeof(char));

                            // Copy the propery name
                            strncpy(i_shader->graphics.push_constant_properties[i], property_name, property_name_len);

                        }

                        // Copy the property size
                        push_constant_size += property_size;

                    }

                    if (push_constant_size > 128)
                        goto push_constant_is_too_large;

                    i_shader->graphics.push_constant_size = push_constant_size;

                    i_shader->graphics.push_constant_data = calloc(128, sizeof(u8));

                    push_constant->offset     = 0;
                    push_constant->size       = (u32)push_constant_size;
                    push_constant->stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
                    */
                }

                //////////////////////////////////
                // Set up the graphics pipeline //
                //////////////////////////////////
                graphics_pipeline_create_info = (VkGraphicsPipelineCreateInfo)
                {
                    .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
                    .pNext               = 0,
                    .flags               = 0,
                    .stageCount          = (u32)shader_stage_iterator,      // Done
                    .pStages             = shader_stages,                   // Done
                    .pVertexInputState   = &vertex_input_info_create_info,  // Done
                    .pInputAssemblyState = &input_assembly_create_info,     // Done

                    // TODO:
                    .pTessellationState  = &tessellation_state_create_info, // Done
                    .pViewportState      = &viewport_state_create_info,     // Done
                    .pRasterizationState = &rasterizer_create_info,         // Done
                    .pMultisampleState   = &multisampling_create_info,      // Done

                    // TODO:
                    .pDepthStencilState = &depth_stencil_state_create_info, // Done
                    .pColorBlendState   = &color_blend_create_info,         // Not done
                    .pDynamicState      = &dynamic_state_create_info,       // Dpme
                    .layout             = p_shader->graphics.pipeline_layout,
                    .renderPass         = 0,//p_instance->context.renderer->render_passes_data[0]->render_pass,
                    .subpass            = 0,
                    .basePipelineHandle = VK_NULL_HANDLE,
                    .basePipelineIndex  = -1
                };

                /////////////////////////////////////
                // Construct the graphics pipeline //
                /////////////////////////////////////
                if ( vkCreateGraphicsPipelines(p_instance->vulkan.device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, 0, &p_shader->graphics.pipeline ) != VK_SUCCESS )
                    goto failed_to_create_graphics_pipeline;
            }
        }

        // Return the pointer to the caller
        *pp_shader = p_shader;
    }

    // Success
    return 1;

    // TODO:
    wrong_input_attribute_properties:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_graphics_pipeline:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create graphics pipeline in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_pipeline_layout:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create pipeline layout in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // JSON errors
        {

            missing_properties:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Missing properties in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            name_wrong_type:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse \"name\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            vertex_group_wrong_type:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse \"vertex attributes\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            wrong_multisampler_type:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse \"multisampler\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_multisampler_samples:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"samples\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_sample_shading_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"sample shading enable\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_alpha_to_coverage_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"alpha to coverage enable\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_multisampler_alpha_to_one_enable:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"alpha to one enable\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_minimum_sample_shading:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse multisampler \"minimum sample shading\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_rasterizer_type:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse \"rasterizer\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_rasterizer_line_width:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse rasterizer \"line width\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_rasterizer_depth_bias_constant_factor:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias constant factor\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_rasterizer_depth_bias_clamp:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias clamp\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_rasterizer_depth_bias_slope_factor:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias slope factor\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_depth_clamp_enable:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth clamp enable\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_rasterizer_rasterizer_discard_enable:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"rasterizer discard enable\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_rasterizer_clockwise:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"clockwise\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            no_rasterizer_depth_bias_enable:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse rasterizer \"depth bias enable\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            failed_to_load_vertex_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load vertex shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_vertex_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create vertex shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_vertex_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse vertex shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_tessellation_control_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load tessellation control shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_tessellation_control_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create tessellation control shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_tessellation_control_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse tessellation control shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_tessellation_evaluation_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load tessellation evaluation shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_tessellation_evaluation_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create tessellation evaluation shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_tessellation_evaluation_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse tessellation evaluation shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_geometry_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load geometry shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_geometry_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create geometry shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_geometry_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse geometry shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_task_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load task shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_task_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create task shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_task_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse task shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_mesh_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load mesh shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_mesh_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create mesh shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_mesh_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse vertex shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_fragment_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load fragment shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_fragment_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create fragment shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_fragment_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse fragment shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_shader:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to allocate shader in call to function \"%s\"\n", __FUNCTION__);
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

int load_compute_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value )
{

    // Argument notes
    // p_value will always be valid. p_value->type will always be JSONobject

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;
        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance            = g_get_active_instance();
    GXShader_t    *p_shader              = 0,
     		      *p_cache_shader        = 0;
    JSONValue_t   *p_name                = 0,
                  *p_compute_shader_path = 0,
                  *p_layout              = 0;

    // Parse the JSON value
    {

        p_name                = dict_get(p_value->object, "name");
        p_compute_shader_path = dict_get(p_value->object, "compute shader path");
        p_layout              = dict_get(p_value->object, "layout");

        // Check properties
        if ( !( p_name && p_compute_shader_path && p_layout ) )
            goto missing_properties;
    }

	// Lock the shader cache mutex
	SDL_LockMutex(p_instance->mutexes.shader_cache);

	// Search the cache for the shader
	p_cache_shader = g_find_ai(p_instance, p_name->string);

	// If the shader is in the cache ...
	if (p_cache_shader)
	{

		// ... make a copy of the cached shader
		copy_ai(pp_shader, p_cache_shader);

		// Write the return
		p_shader = *pp_shader;

		// Set the initial state
		goto exit;
	}

	// ... the AI is not in the cache

    // Construct the shader
    {

        // Initialized data
        VkShaderModule compute_shader_module = 0;

        // Allocate memory for a shader
        if ( create_shader(&p_shader) == 0 )
            goto failed_to_allocate_shader;

        // Set the shader type
        p_shader->type = g10_pipeline_compute;
        p_shader->compute.x_groups = 1;
        p_shader->compute.y_groups = 1;
        p_shader->compute.z_groups = 1;

        // Set the name
        if ( p_name->type == JSONstring )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_shader->name == (void *) 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_shader->name, p_name->string, name_len);
        }
        else
            goto name_wrong_type;

        // Load the compute shader binary
        if ( p_compute_shader_path )
        {

            // Parse the compute shader path as a string
            if ( p_compute_shader_path->type == JSONstring )
            {

                // Initialized data
                size_t compute_shader_data_len = g_load_file(p_compute_shader_path->string, 0, true);
                char* compute_shader_data = calloc(compute_shader_data_len, sizeof(char));

                // Error check
                if ( compute_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the compute shader binary file
                if ( g_load_file(p_compute_shader_path->string, compute_shader_data, true) == 0 )
                    goto failed_to_load_compute_shader_binary;

                // Create a shader module
                if ( create_shader_module(compute_shader_data, compute_shader_data_len, &compute_shader_module) == 0 )
                    goto failed_to_create_compute_shader_module;

                // Clean the scope
                free(compute_shader_data);
            }
            else
                goto wrong_compute_shader_path_type;
        }
        else
            goto no_compute_shader_path;

        // Construct the compute shader
        {

            // Initialized data
            VkPipelineShaderStageCreateInfo shader_stage                 = { 0 };
            VkPipelineLayoutCreateInfo      pipeline_layout_create_info  = { 0 };
            VkComputePipelineCreateInfo     compute_pipeline_create_info = { 0 };

            // Set up the compute shader stage
            if ( compute_shader_module )
            {

                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_COMPUTE_BIT,
                    .module = compute_shader_module,
                    .pName  = "main"
                };
            }
            else
                ;// TODO:

            //  Set up layout
            {

                // Initialized data
                JSONValue_t                     *p_layout_sets                     = 0,
                                                *p_layout_push_constant            = 0;
                size_t                           layout_sets_count                 = 0;
                JSONValue_t                    **pp_sets                           = 0;
                VkPipelineLayoutCreateInfo       pipeline_layout_create_info       = { 0 };
                size_t                           binding_count                     = 0,
                                                 descriptor_set_layout_count       = 0;
                VkDescriptorSetLayoutBinding     p_bindings                        = { 0 };
                VkPushConstantRange             *p_push_constant                   = 0;
                VkDescriptorSetLayout           *p_descriptor_set_layout           = 0;
                VkDescriptorSetLayoutCreateInfo  descriptor_set_layout_create_info = (VkDescriptorSetLayoutCreateInfo)
                {
                    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                    .pNext        = 0,
                    .flags        = 0,
                    .bindingCount = (u32)binding_count,
                    .pBindings    = &p_bindings
                };
                VkDescriptorSetLayoutBinding     descriptor_set_layout_binding     = (VkDescriptorSetLayoutBinding)
                {
                    .binding            = 0,
                    .descriptorType     = 0,
                    .descriptorCount    = 0,
                    .stageFlags         = 0,
                    .pImmutableSamplers = 0
                };

                // Parse the JSON
                {
                    p_layout_sets          = dict_get(p_layout->object, "sets");
                    p_layout_push_constant = dict_get(p_layout->object, "push constant");
                }

                // Get the contents of the sets array
                {

                    // Get the length of the array
                    array_get(p_layout_sets->list, 0, &layout_sets_count);

                    // Allocate memory for JSON tokens
                    pp_sets = calloc(layout_sets_count, sizeof(JSONValue_t *));

                    // Get the contents of the array
                    array_get(p_layout_sets->list, pp_sets, 0);
                }

                // Iterate over each set
                for ( size_t i = 0; i < layout_sets_count; i++ )
                {

                    // Initialized data
                    JSONValue_t *p_set = pp_sets[i];

                    print_json_value(p_set, stdout);
                    printf("\n\n");
                }

                if ( vkCreateDescriptorSetLayout(p_instance->vulkan.device, &descriptor_set_layout_create_info, 0, &p_shader->compute.set_layout) != VK_SUCCESS)
                    goto failed_to_create_descriptor_set_layout;

                pipeline_layout_create_info = (VkPipelineLayoutCreateInfo)
                {
                    .sType               = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                    .pNext               = 0,
                    .flags               = 0,
                    .setLayoutCount      = (u32)descriptor_set_layout_count,
                    .pSetLayouts         = p_descriptor_set_layout,
                    .pPushConstantRanges = p_push_constant  /* pointer to an array of VkPushConstantRange structures
                                                               defining a set of push constant ranges for use in a
                                                               single pipeline layout. In addition to descriptor set
                                                               layouts, a pipeline layout also describes how many push
                                                               constants can be accessed by each stage of the pipeline.*/

                };

                if ( vkCreatePipelineLayout(p_instance->vulkan.device, &pipeline_layout_create_info, 0, &p_shader->compute.pipeline_layout) != VK_SUCCESS )
                    goto failed_to_create_pipeline_layout;
            }

            // Set up the compute pipeline
            compute_pipeline_create_info = (VkComputePipelineCreateInfo)
            {
                .sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
                .pNext              = 0,
                .flags              = 0,
                .stage              = shader_stage,
                .layout             = p_shader->compute.pipeline_layout,
                .basePipelineHandle = 0,
                .basePipelineIndex  = 0,
            };

            // Construct the compute pipeline
            if ( vkCreateComputePipelines(p_instance->vulkan.device, VK_NULL_HANDLE, 1, &compute_pipeline_create_info, 0, &p_shader->compute.pipeline) != VK_SUCCESS )
                g_print_error("failed to create compute pipeline!\n");

        }

        // Cache the shader
        g_cache_shader(p_instance, p_shader);

        // Return the pointer to the caller
        *pp_shader = p_shader;
    }

    exit:
    // Success
    return 1;

    // TODO:
    no_compute_shader_path:
    failed_to_create_descriptor_set_layout:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_pipeline_layout:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create pipeline layout in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // G10 errors
        {
            failed_to_allocate_shader:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to allocate shader in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
        }

        // JSON errors
        {

            missing_properties:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Missing properties in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            name_wrong_type:
                #ifndef NDEBUG
					g_print_error("[G10] [Shader] Failed to parse \"name\" property in call to function \"%s\"\n", __FUNCTION__);
				#endif

                // Error
                return 0;

            wrong_compute_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse \"compute shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_compute_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load compute shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_compute_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create compute shader module in call to function \"%s\"\n", __FUNCTION__);
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

int load_ray_shader_as_json_value ( GXShader_t **pp_shader, JSONValue_t *p_value )
{

    // Argument notes
    // p_value will always be valid. p_value->type will always be JSONobject

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_shader == (void *) 0 ) goto no_shader;
            if ( p_value   == (void *) 0 ) goto no_value;
        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance                     = g_get_active_instance();
    GXShader_t    *p_shader                       = 0;
    JSONValue_t   *p_name                         = 0,
                  *p_ray_generation_shader_path   = 0,
                  *p_ray_any_hit_shader_path      = 0,
                  *p_ray_closest_hit_shader_path  = 0,
                  *p_ray_miss_shader_path         = 0,
                  *p_ray_intersection_shader_path = 0,
                  *p_ray_callable_shader_path     = 0;

    // Parse the JSON value
    {

        p_name                         = dict_get(p_value->object, "name");
        p_ray_generation_shader_path   = dict_get(p_value->object, "generation shader path");
        p_ray_any_hit_shader_path      = dict_get(p_value->object, "any hit shader path");
        p_ray_closest_hit_shader_path  = dict_get(p_value->object, "closest_hit shader path");
        p_ray_miss_shader_path         = dict_get(p_value->object, "miss shader path");
        p_ray_intersection_shader_path = dict_get(p_value->object, "intersection shader path");
        p_ray_callable_shader_path     = dict_get(p_value->object, "callable shader path");

        // Check properties
        if ( !( p_name && p_ray_generation_shader_path && p_ray_any_hit_shader_path && p_ray_closest_hit_shader_path && p_ray_miss_shader_path && p_ray_intersection_shader_path && p_ray_callable_shader_path) )
            goto missing_properties;
    }

    // TODO: Cache check

    // Construct the shader
    {

        // Initialized data
        VkShaderModule ray_generation_shader_module   = 0,
                       ray_any_hit_shader_module      = 0,
                       ray_closest_hit_shader_module  = 0,
                       ray_miss_shader_module         = 0,
                       ray_intersection_shader_module = 0,
                       ray_callable_shader_module     = 0;

        // Allocate memory for a shader
        if ( create_shader(&p_shader) == 0 )
            goto failed_to_allocate_shader;

        // Set the shader type
        p_shader->type = g10_pipeline_ray;

        // Set the name
        if ( p_name->type == JSONstring )
        {

            // Initialized data
            size_t name_len = strlen(p_name->string);

            // Allocate memory for the name
            p_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            if ( p_shader->name == (void *) 0 )
                goto no_mem;

            // Copy the name
            strncpy(p_shader->name, p_name->string, name_len);
        }
        else
            goto name_wrong_type;

        // Load the ray generation shader binary
        if ( p_ray_generation_shader_path )
        {

            // Parse the ray generation shader path as a string
            if ( p_ray_generation_shader_path->type == JSONstring )
            {

                // Initialized data
                size_t ray_generation_shader_data_len = g_load_file(p_ray_generation_shader_path->string, 0, true);
                char* ray_generation_shader_data = calloc(ray_generation_shader_data_len, sizeof(char));

                // Error check
                if ( ray_generation_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the ray generation shader binary file
                if ( g_load_file(p_ray_generation_shader_path->string, ray_generation_shader_data, true) == 0 )
                    goto failed_to_load_ray_generation_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_generation_shader_data, ray_generation_shader_data_len, &ray_generation_shader_module) == 0 )
                    goto failed_to_create_ray_generation_shader_module;

                // Clean the scope
                free(ray_generation_shader_data);
            }
            else
                goto wrong_ray_generation_shader_path_type;
        }
        else
            goto no_ray_generation_shader_path;

        // Load the ray any hit shader binary
        if ( p_ray_any_hit_shader_path )
        {

            // Parse the ray any hit shader path as a string
            if ( p_ray_any_hit_shader_path->type == JSONstring )
            {

                // Initialized data
                size_t ray_any_hit_shader_data_len = g_load_file(p_ray_any_hit_shader_path->string, 0, true);
                char* ray_any_hit_shader_data = calloc(ray_any_hit_shader_data_len, sizeof(char));

                // Error check
                if ( ray_any_hit_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the ray any hit shader binary file
                if ( g_load_file(p_ray_any_hit_shader_path->string, ray_any_hit_shader_data, true) == 0 )
                    goto failed_to_load_ray_any_hit_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_any_hit_shader_data, ray_any_hit_shader_data_len, &ray_any_hit_shader_module) == 0 )
                    goto failed_to_create_ray_any_hit_shader_module;

                // Clean the scope
                free(ray_any_hit_shader_data);
            }
            else
                goto wrong_ray_any_hit_shader_path_type;
        }
        else
            goto no_ray_any_hit_shader_path;

        // Load the ray closest hit shader binary
        if ( p_ray_closest_hit_shader_path )
        {

            // Parse the ray closest hit shader path as a string
            if ( p_ray_closest_hit_shader_path->type == JSONstring )
            {

                // Initialized data
                size_t ray_closest_hit_shader_data_len = g_load_file(p_ray_closest_hit_shader_path->string, 0, true);
                char* ray_closest_hit_shader_data = calloc(ray_closest_hit_shader_data_len, sizeof(char));

                // Error check
                if ( ray_closest_hit_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the ray closest hit shader binary file
                if ( g_load_file(p_ray_closest_hit_shader_path->string, ray_closest_hit_shader_data, true) == 0 )
                    goto failed_to_load_ray_closest_hit_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_closest_hit_shader_data, ray_closest_hit_shader_data_len, &ray_closest_hit_shader_module) == 0 )
                    goto failed_to_create_ray_closest_hit_shader_module;

                // Clean the scope
                free(ray_closest_hit_shader_data);
            }
            else
                goto wrong_ray_closest_hit_shader_path_type;
        }
        else
            goto no_ray_closest_hit_shader_path;

        // Load the ray miss shader binary
        if ( p_ray_miss_shader_path )
        {

            // Parse the ray miss shader path as a string
            if ( p_ray_miss_shader_path->type == JSONstring )
            {
                // Initialized data
                size_t ray_miss_shader_data_len = g_load_file(p_ray_miss_shader_path->string, 0, true);
                char* ray_miss_shader_data = calloc(ray_miss_shader_data_len, sizeof(char));

                // Error check
                if ( ray_miss_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the ray miss shader binary file
                if ( g_load_file(p_ray_miss_shader_path->string, ray_miss_shader_data, true) == 0 )
                    goto failed_to_load_ray_miss_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_miss_shader_data, ray_miss_shader_data_len, &ray_miss_shader_module) == 0 )
                    goto failed_to_create_ray_miss_shader_module;

                // Clean the scope
                free(ray_miss_shader_data);
            }
            else
                goto wrong_ray_miss_shader_path_type;
        }
        else
            goto no_ray_miss_shader_path;

        // Load the ray intersection shader binary
        if ( p_ray_intersection_shader_path )
        {

            // Parse the ray intersection shader path as a string
            if ( p_ray_intersection_shader_path->type == JSONstring )
            {
                // Initialized data
                size_t ray_intersection_shader_data_len = g_load_file(p_ray_intersection_shader_path->string, 0, true);
                char* ray_intersection_shader_data = calloc(ray_intersection_shader_data_len, sizeof(char));

                // Error check
                if ( ray_intersection_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the ray intersection shader binary file
                if ( g_load_file(p_ray_intersection_shader_path->string, ray_intersection_shader_data, true) == 0 )
                    goto failed_to_load_ray_intersection_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_intersection_shader_data, ray_intersection_shader_data_len, &ray_intersection_shader_module) == 0 )
                    goto failed_to_create_ray_intersection_shader_module;

                // Clean the scope
                free(ray_intersection_shader_data);
            }
            else
                goto wrong_ray_intersection_shader_path_type;
        }
        else
            goto no_ray_intersection_shader_path;

        // Load the ray callable shader binary
        if ( p_ray_callable_shader_path )
        {

            // Parse the ray callable shader path as a string
            if ( p_ray_callable_shader_path->type == JSONstring )
            {

                // Initialized data
                size_t ray_callable_shader_data_len = g_load_file(p_ray_callable_shader_path->string, 0, true);
                char* ray_callable_shader_data = calloc(ray_callable_shader_data_len, sizeof(char));

                // Error check
                if ( ray_callable_shader_data == (void *) 0 )
                    goto no_mem;

                // Load the ray callable shader binary file
                if ( g_load_file(p_ray_callable_shader_path->string, ray_callable_shader_data, true) == 0 )
                    goto failed_to_load_ray_callable_shader_binary;

                // Create a shader module
                if ( create_shader_module(ray_callable_shader_data, ray_callable_shader_data_len, &ray_callable_shader_module) == 0 )
                    goto failed_to_create_ray_callable_shader_module;

                // Clean the scope
                free(ray_callable_shader_data);
            }
            else
                goto wrong_ray_callable_shader_path_type;
        }
        else
            goto no_ray_callable_shader_path;

        // // TODO: Construct the ray shader
        {

            // Initialized data
            VkPipelineShaderStageCreateInfo   shader_stage                 = { 0 };
            VkPipelineLayoutCreateInfo        pipeline_layout_create_info  = { 0 };
            VkRayTracingPipelineCreateInfoKHR ray_pipeline_create_info     = { 0 };

            // Set up the ray generation shader stage
            if ( ray_generation_shader_module )
            {

                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray any hit shader stage
            if ( ray_any_hit_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray closest hit shader stage
            if ( ray_closest_hit_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray miss shader stage
            if ( ray_miss_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_MISS_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray intersection shader stage
            if ( ray_intersection_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // Set up the ray callable shader stage
            if ( ray_callable_shader_module )
            {
                // Set the shader stage
                shader_stage = (VkPipelineShaderStageCreateInfo)
                {
                    .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .stage  = VK_SHADER_STAGE_CALLABLE_BIT_KHR,
                    .module = ray_generation_shader_module,
                    .pName  = "main"
                };
            }

            // TODO:
            //  Set up layout
            {

                // Initialized data
                VkPipelineLayoutCreateInfo      pipeline_layout_create_info       = { 0 };
                VkDescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = { 0 };
                VkDescriptorSetLayoutBinding    descriptor_set_layout_binding     = { 0 };
                size_t                          binding_count                     = 0;
                VkDescriptorSetLayoutBinding    p_bindings                        = { 0 };

                descriptor_set_layout_binding = (VkDescriptorSetLayoutBinding)
                {
                    .binding            = 0,
                    .descriptorType     = 0,
                    .descriptorCount    = 0,
                    .stageFlags         = 0,
                    .pImmutableSamplers = 0
                };

                descriptor_set_layout_create_info = (VkDescriptorSetLayoutCreateInfo)
                {
                    .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
                    .pNext        = 0,
                    .flags        = 0,
                    .bindingCount = (u32)binding_count,
                    .pBindings    = &p_bindings
                };

                pipeline_layout_create_info = (VkPipelineLayoutCreateInfo)
                {
                    .sType               = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
                    .pNext               = 0,
                    .flags               = 0,
                    .setLayoutCount      = 0,
                    .pSetLayouts         = 0,
                    .pPushConstantRanges = 0
                };
            }

            // Set up the compute pipeline
            ray_pipeline_create_info = (VkRayTracingPipelineCreateInfoKHR)
            {
                .sType                        = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
                .pNext                        = 0,
                .flags                        = 0,
                .stageCount                   = 0,
                .pStages                      = 0,
                .groupCount                   = 0,
                .pGroups                      = 0,
                .maxPipelineRayRecursionDepth = 0,
                .pLibraryInfo                 = 0,
                .pLibraryInterface            = 0,
                .pDynamicState                = 0,
                .layout                       = 0,
                .basePipelineHandle           = 0,
                .basePipelineIndex            = 0
            };

            // Construct the compute pipeline
            if ( vkCreateComputePipelines(p_instance->vulkan.device, VK_NULL_HANDLE, 1, &ray_pipeline_create_info, 0, &p_shader->ray.pipeline) != VK_SUCCESS )
                g_print_error("failed to create ray pipeline!\n");

        }

        // Return the pointer to the caller
        *pp_shader = p_shader;
    }

    // Success
    return 1;

    // TODO:
    missing_properties:
    failed_to_allocate_shader:
    no_mem:
    name_wrong_type:

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }

        // JSON errors
        {
            wrong_ray_generation_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse generation ray shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_any_hit_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse any hit ray shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_closest_hit_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse closest hit ray shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_miss_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse miss ray shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_intersection_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse intersection ray shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_ray_callable_shader_path_type:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to parse callable ray shader path in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_generation_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"generation shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_any_hit_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"any hit shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_closest_hit_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"closest_hit shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_miss_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"miss shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_intersection_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"intersection shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_ray_callable_shader_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] No \"callable shader path\" property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;


            failed_to_load_ray_generation_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load generation ray shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_any_hit_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load any hit ray shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_closest_hit_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load closest hit ray shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_miss_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load miss ray shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_intersection_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load intersection ray shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_ray_callable_shader_binary:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to load callable ray shader binary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_generation_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create generation ray shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_any_hit_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create any hit ray shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_closest_hit_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create closest hit ray shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_miss_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create miss ray shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_intersection_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create intersection ray shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_create_ray_callable_shader_module:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to create callable ray shader module in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}