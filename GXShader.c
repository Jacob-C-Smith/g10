#include <G10/GXShader.h>

#define FORMATS_COUNT                 17
#define DESCRIPTOR_TYPE_COUNT         11
#define PUSH_CONSTANT_GETTERS_COUNT   2
#define RASTERIZER_POLYGON_MODE_COUNT 3
#define BLEND_OPERATIONS_COUNT        5
#define BLEND_FACTORS_COUNT           19
#define SHADER_STAGES_COUNT           5


char             *format_type_names              [FORMATS_COUNT] = {
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
char             *descriptor_type_names          [DESCRIPTOR_TYPE_COUNT] = {
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
char             *push_constant_getter_names     [PUSH_CONSTANT_GETTERS_COUNT] = {
    "camera position",
    "model matrix"
};
char             *rasterizer_polygon_mode_names  [RASTERIZER_POLYGON_MODE_COUNT] = {
    "fill",
    "line",
    "point"
};
char             *blend_operation_names          [BLEND_OPERATIONS_COUNT] = {
    "add",
    "subtract",
    "reverse subtract",
    "minimum",
    "maximum"
};
char             *blend_factor_names             [BLEND_FACTORS_COUNT] = {
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
char             *shader_stages_names            [SHADER_STAGES_COUNT] = {
    "vertex",
    "tessellation control",
    "tessellation evaluation",
    "geometry",
    "fragment"
};
VkDescriptorType  descriptor_type_enums          [DESCRIPTOR_TYPE_COUNT] = {
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
VkFormat          format_type_enums              [FORMATS_COUNT] = {
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
size_t            format_type_sizes              [FORMATS_COUNT] = {
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
void            **push_constant_getter_functions [PUSH_CONSTANT_GETTERS_COUNT] = {
    &get_camera_position,
    &get_model_matrix
};
VkPolygonMode     rasterizer_polygon_mode_enum   [RASTERIZER_POLYGON_MODE_COUNT] = {
    VK_POLYGON_MODE_FILL,
    VK_POLYGON_MODE_LINE,
    VK_POLYGON_MODE_POINT
};
VkBlendOp         blend_operation_enums          [BLEND_OPERATIONS_COUNT] = {
    VK_BLEND_OP_ADD,
    VK_BLEND_OP_SUBTRACT,
    VK_BLEND_OP_REVERSE_SUBTRACT,
    VK_BLEND_OP_MIN,
    VK_BLEND_OP_MAX
};
VkBlendFactor     blend_factor_enums             [BLEND_FACTORS_COUNT] = {
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
VkShaderStageFlagBits shader_stages_enum         [SHADER_STAGES_COUNT] = {
    VK_SHADER_STAGE_VERTEX_BIT,
    VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
    VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
    VK_SHADER_STAGE_GEOMETRY_BIT,
    VK_SHADER_STAGE_FRAGMENT_BIT
};

dict             *format_types                       = 0;
dict             *format_sizes                       = 0;
dict             *descriptor_types                   = 0;
dict             *push_constant_getters              = 0;
dict             *polygon_modes                      = 0;
dict             *blend_operations                   = 0;
dict             *blend_factors                      = 0;
dict             *shader_stagesD                     = 0;

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

void init_shader                    ( void )
{

    GXInstance_t *instance = g_get_active_instance();

    dict_construct(&format_types         , FORMATS_COUNT);
    dict_construct(&format_sizes         , FORMATS_COUNT);
    dict_construct(&descriptor_types     , DESCRIPTOR_TYPE_COUNT);
    dict_construct(&push_constant_getters, PUSH_CONSTANT_GETTERS_COUNT);
    dict_construct(&polygon_modes        , RASTERIZER_POLYGON_MODE_COUNT);
    dict_construct(&blend_operations     , BLEND_OPERATIONS_COUNT);
    dict_construct(&blend_factors        , BLEND_FACTORS_COUNT);
    dict_construct(&shader_stagesD       , SHADER_STAGES_COUNT);

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

    instance->mutexes.shader_cache = SDL_CreateMutex();

}

int create_shader_module            ( char         *code   , size_t     code_len  , VkShaderModule *shader_module )
{
    GXInstance_t* instance = g_get_active_instance();

    VkShaderModuleCreateInfo shader_module_create_info = { 0 };
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = code_len;
    shader_module_create_info.pCode = code;

    if (vkCreateShaderModule(instance->vulkan.device, &shader_module_create_info, (void*)0, shader_module))

        // TODO: Replace with a goto
        printf("Failed to create a shader module");

    return 0;
}

int create_shader                   ( GXShader_t  **shader )
{
    // TODO: Argument check
    {

    }

    GXShader_t *i_shader = calloc(1, sizeof(GXShader_t));

    // TODO: Check memory

    *shader = i_shader;

    return 1;

    // TODO: Error handling
    {

    }
}

int load_shader                     ( GXShader_t  **shader, const char *path )
{
    // TODO: Argument check

    char   *token_text     = 0;
    size_t  token_text_len = g_load_file(path, (void*)0, false);
        
    token_text = calloc(token_text_len + 1, sizeof(char));

    g_load_file(path, token_text, false);

    load_shader_as_json(shader, token_text, token_text_len);

    return 1;

    // TODO: Error handling
}

int load_shader_as_json             ( GXShader_t  **shader, char       *token_text, size_t          token_text_len )
{
    // TODO: Argument check
    GXInstance_t  *instance                  = g_get_active_instance();
    GXShader_t    *i_shader                  = 0;
    char          *name                      = 0,
                  *vertex_shader_path        = 0,
                  *geometry_shader_path      = 0,
                  *fragment_shader_path      = 0,
                  *compute_shader_path       = 0,
                  *rasterizer_json           = 0;
    dict          *json_data                 = 0,
                  *vertex_shader_json_data   = 0,
                  *geometry_shader_json_data = 0,
                  *fragment_shader_json_data = 0,
                  *compute_shader_json_data  = 0;
    char         **vertex_groups             = 0,
                 **attachments               = 0,
                 **sets                      = 0,
                  *push_constant_text        = 0;
    JSONToken_t   *token                     = 0;
    size_t         set_count                 = 0;

    // Parse JSON data
    {

        parse_json(token_text, token_text_len, &json_data);

        token                = dict_get(json_data, "name");
        name                 = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "vertex shader path");
        vertex_shader_path   = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "geometry shader path");
        geometry_shader_path = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "fragment shader path");
        fragment_shader_path = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "compute shader path");
        compute_shader_path = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "in");
        vertex_groups        = JSON_VALUE(token, JSONarray);

        token                = dict_get(json_data, "attachments");
        attachments          = JSON_VALUE(token, JSONarray);

        token                = dict_get(json_data, "sets");
        sets                 = JSON_VALUE(token, JSONarray);

        token                = dict_get(json_data, "push constant");
        push_constant_text   = JSON_VALUE(token, JSONobject);

        token                = dict_get(json_data, "rasterizer");
        rasterizer_json      = JSON_VALUE(token, JSONobject);
    }
    
    // Check the cache
    {
        GXShader_t *r = g_find_shader(instance, name);
        
        if (r != (void*)0)
        {
            *shader = r;
            goto exit;
        }
    }

    // Construct the shader
    {

        // Allocate shader memory
        create_shader(shader);

        // Get the pointer to the shader memory
        i_shader = *shader;

        // Set the name
        {

            // Initialized data
            size_t name_len = strlen(name);

            // Allocate memory for the name
            i_shader->name = calloc(name_len + 1, sizeof(char));

            // Error check
            {
                #ifndef NDEBUG
                    if (i_shader->name == (void *)0)
                        goto no_mem;
                #endif
            }

            // Copy the name
            strncpy(i_shader->name, name, name_len);
        }

    }

    // Construct the graphics shader
    if      ( fragment_shader_path ){

        // Load the graphics shader binaries
        {

            // Load the vertex shader binary
            if (vertex_shader_path) {

                // Initialized data
                size_t   vertex_shader_data_len = g_load_file(vertex_shader_path, 0, true);
                char* vertex_shader_data = calloc(vertex_shader_data_len, sizeof(char));

                g_load_file(vertex_shader_path, vertex_shader_data, true);


                // Create a shader module
                create_shader_module(vertex_shader_data, vertex_shader_data_len, &i_shader->graphics.vertex_shader_module);

                free(vertex_shader_data);
            }

            // TODO: Load tessellation control shader
            
            // TODO: Load tessellation evaluation shader
             
            // Load the geometry shader binary
            if (geometry_shader_path) {

                // Initialized data
                size_t   geometry_shader_data_len = g_load_file(geometry_shader_path, 0, true);
                char* geometry_shader_data = calloc(geometry_shader_data_len, sizeof(char));

                g_load_file(geometry_shader_path, geometry_shader_data, true);

                // Create a shader module
                create_shader_module(geometry_shader_data, geometry_shader_data_len, &i_shader->graphics.geometry_shader_module);

                free(geometry_shader_data);
            }

            // Load the fragment shader binary
            if (fragment_shader_path) {
                size_t fragment_shader_data_len = g_load_file(fragment_shader_path, 0, true);
                char* fragment_shader_data = calloc(fragment_shader_data_len + 1, sizeof(char));

                g_load_file(fragment_shader_path, fragment_shader_data, true);

                // Create a shader module
                create_shader_module(fragment_shader_data, fragment_shader_data_len, &i_shader->graphics.fragment_shader_module);

                free(fragment_shader_data);

            }

        }

        // Set up the graphics shader pipeline
        {

            // Initialized data
            VkPipelineShaderStageCreateInfo        *shader_stages                       = calloc(5, sizeof(VkPipelineShaderStageCreateInfo));
            size_t                                  shader_stage_iterator               = 0,
                                                    attachment_count                    = 0;
            VkPipelineVertexInputStateCreateInfo    vertex_input_info_create_info       = { 0 };
            VkPipelineInputAssemblyStateCreateInfo  input_assembly_create_info          = { 0 };
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
            VkVertexInputBindingDescription        *binding_description                 = calloc(1, sizeof(VkVertexInputBindingDescription));
            VkViewport                              viewport                            = { 0 };
            VkRect2D                                scissor                             = { 0 };
            VkVertexInputAttributeDescription      *vertex_input_attribute_descriptions = 0;
            VkDescriptorSetLayoutCreateInfo        *descriptor_set_layout               = 0;

            // Set up the shader stages
            {
                
                // Set up the vertex shader
                if (i_shader->graphics.vertex_shader_module) {
                    VkPipelineShaderStageCreateInfo* vertex_shader_stage_create_info = &shader_stages[shader_stage_iterator++];

                    vertex_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    vertex_shader_stage_create_info->stage  = VK_SHADER_STAGE_VERTEX_BIT;
                    vertex_shader_stage_create_info->module = i_shader->graphics.vertex_shader_module;
                    vertex_shader_stage_create_info->pName  = "main";
                }

                // Set up the tessellation control shader
                if (i_shader->graphics.tessellation_control_shader_module) {
                    VkPipelineShaderStageCreateInfo* tessellation_control_shader_stage_create_info = &shader_stages[shader_stage_iterator++];

                    tessellation_control_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    tessellation_control_shader_stage_create_info->stage  = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                    tessellation_control_shader_stage_create_info->module = i_shader->graphics.tessellation_control_shader_module;
                    tessellation_control_shader_stage_create_info->pName  = "main";
                }

                // Set up the tessellation evaluation shader
                if (i_shader->graphics.tessellation_evaluation_shader_module) {
                    VkPipelineShaderStageCreateInfo* tessellation_evaluation_shader_stage_create_info = &shader_stages[shader_stage_iterator++];

                    tessellation_evaluation_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    tessellation_evaluation_shader_stage_create_info->stage  = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
                    tessellation_evaluation_shader_stage_create_info->module = i_shader->graphics.tessellation_evaluation_shader_module;
                    tessellation_evaluation_shader_stage_create_info->pName  = "main";
                }

                // Set up the geometry shader
                if (i_shader->graphics.geometry_shader_module) {
                    VkPipelineShaderStageCreateInfo* geometry_shader_stage_create_info = &shader_stages[shader_stage_iterator++];

                    geometry_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    geometry_shader_stage_create_info->stage  = VK_SHADER_STAGE_GEOMETRY_BIT;
                    geometry_shader_stage_create_info->module = i_shader->graphics.geometry_shader_module;
                    geometry_shader_stage_create_info->pName  = "main";
                }

                // Set up the fragment shader
                if (i_shader->graphics.fragment_shader_module) {

                    VkPipelineShaderStageCreateInfo *fragment_shader_stage_create_info = &shader_stages[shader_stage_iterator++];

                    fragment_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    fragment_shader_stage_create_info->stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
                    fragment_shader_stage_create_info->module = i_shader->graphics.fragment_shader_module;
                    fragment_shader_stage_create_info->pName  = "main";
                }
            }

            // Set up the vertex input state
            {

                // Initialized data
                u32    vertex_group_count        = 0;
                size_t stride                    = 0,
                       binding_description_count = 0;

                // Did the user specify vertex groups?
                if (vertex_groups)
                {

                    // Count vertex groups
                    while (vertex_groups[++vertex_group_count]);
                    
                    // Allocate vertex attributes
                    vertex_input_attribute_descriptions = calloc(vertex_group_count, sizeof(VkVertexInputAttributeDescription));

                    // Iterate over each vertex group
                    for (size_t i = 0; i < vertex_group_count; i++)
                    {

                        // Initialized data
                        dict        *vertex_group   = 0;
                        JSONToken_t *token          = 0;
                        char        *type           = 0;
                        char        *location       = 0;

                        // Parse the JSON objects into a dictionary
                        parse_json(vertex_groups[i], strlen(vertex_groups[i]), &vertex_group);

                        token    = dict_get(vertex_group, "type");
                        type     = JSON_VALUE(token, JSONstring);

                        token    = dict_get(vertex_group, "location");
                        location = JSON_VALUE(token, JSONprimative);

                        // Construct a vertex input attribute
                        {
                            vertex_input_attribute_descriptions[i].binding  = 0;
                            vertex_input_attribute_descriptions[i].location = atoi(location);
                            vertex_input_attribute_descriptions[i].format   = (VkFormat)dict_get(format_types, type);
                            vertex_input_attribute_descriptions[i].offset   = stride;
                        }

                        stride += (size_t) dict_get(format_sizes, type);

                    }
                    
                    // Construct a binding description
                    {
                        binding_description->binding   = 0;
                        binding_description->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                        binding_description->stride    = stride;
                        binding_description_count      = 1;
                    }
                }

                // Set up the vertex input 
                {
                    vertex_input_info_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                    vertex_input_info_create_info.vertexBindingDescriptionCount   = binding_description_count;
                    vertex_input_info_create_info.pVertexBindingDescriptions      = binding_description;
                    vertex_input_info_create_info.vertexAttributeDescriptionCount = vertex_group_count;
                    vertex_input_info_create_info.pVertexAttributeDescriptions    = vertex_input_attribute_descriptions;
                }
            }

            // Set up the input assembly
            {
                input_assembly_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                input_assembly_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                input_assembly_create_info.primitiveRestartEnable = VK_FALSE;
            }

            // Set up the viewport state
            {

                // Set up the viewport
                {
                    viewport.x = 0.f;
                    viewport.y = 0.f;
                    viewport.width = instance->vulkan.swap_chain_extent.width;
                    viewport.height = instance->vulkan.swap_chain_extent.height;
                    viewport.minDepth = 0.0f;
                    viewport.maxDepth = 1.0f;
                }

                // Set up the scissor
                {
                    scissor.offset.x = 0;
                    scissor.offset.y = 0;
                    scissor.extent = instance->vulkan.swap_chain_extent;
                }

                // Populate the viewport state create struct
                {
                    viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                    viewport_state_create_info.viewportCount = 1;
                    viewport_state_create_info.pViewports    = &viewport;
                    viewport_state_create_info.scissorCount  = 1;
                    viewport_state_create_info.pScissors     = &scissor;
                }
            }

            // Set up the rasterizer 
            {

                VkPolygonMode polygon_mode               = VK_POLYGON_MODE_FILL;
                float         line_width                 = 1.f,
                              depth_bias_constant_factor = 0.f,
                              depth_bias_clamp           = 0.f,
                              depth_bias_slope_factor    = 0.f;

                bool          depth_clamp_enable         = false,
                              rasterizer_discard_enable  = false,
                              clockwise                  = false,
                              depth_bias_enable          = false;

                // Parse the rasterizer property
                if (rasterizer_json) {

                    // Initialized data
                    dict *rasterizer_json_dict              = 0;
                    char *polygon_mode_string               = 0,
                         *line_width_string                 = 0,
                         *depth_bias_constant_factor_string = 0,
                         *depth_bias_clamp_string           = 0,
                         *depth_bias_slope_factor_string    = 0;

                    // Parse the rasterizer JSON text into a JSON dictionary
                    parse_json(rasterizer_json, strlen(rasterizer_json), &rasterizer_json_dict);

                    // Parse the JSON dictionary into rasterizer parameters
                    {
                        JSONToken_t *token                = 0;

                        token                             = dict_get(rasterizer_json_dict, "depth clamp enable");
                        depth_clamp_enable                = JSON_VALUE(token, JSONprimative);

                        token                             = dict_get(rasterizer_json_dict, "rasterizer discard enable");
                        rasterizer_discard_enable         = JSON_VALUE(token, JSONprimative);
                        
                        token                             = dict_get(rasterizer_json_dict, "polygon mode");
                        polygon_mode_string               = JSON_VALUE(token, JSONstring);
                        
                        token                             = dict_get(rasterizer_json_dict, "cull mode");
                        rasterizer_discard_enable         = JSON_VALUE(token, JSONarray);
                                                
                        token                             = dict_get(rasterizer_json_dict, "clockwise");
                        clockwise                         = JSON_VALUE(token, JSONprimative);
                                                
                        token                             = dict_get(rasterizer_json_dict, "depth bias enable");
                        depth_bias_enable                 = JSON_VALUE(token, JSONprimative);

                        token                             = dict_get(rasterizer_json_dict, "depth bias constant factor");
                        depth_bias_constant_factor_string = JSON_VALUE(token, JSONprimative);

                        token                             = dict_get(rasterizer_json_dict, "depth bias clamp");
                        depth_bias_clamp_string           = JSON_VALUE(token, JSONprimative);

                        token                             = dict_get(rasterizer_json_dict, "depth bias slope factor");
                        depth_bias_slope_factor_string    = JSON_VALUE(token, JSONprimative);

                        token                             = dict_get(rasterizer_json_dict, "line width");
                        line_width_string                 = JSON_VALUE(token, JSONprimative);

                    }

                    // Get rasterizer parameters
                    {
                        if (polygon_mode_string)
                            polygon_mode = (VkPolygonMode)dict_get(polygon_modes, polygon_mode_string);

                        if(depth_bias_constant_factor_string)
                            depth_bias_constant_factor = atof(depth_bias_constant_factor_string);

                        if (depth_bias_clamp_string)
                            depth_bias_clamp = atof(depth_bias_clamp_string);

                        if (depth_bias_slope_factor_string)
                            depth_bias_slope_factor = atof(depth_bias_slope_factor_string);

                        if (line_width_string)
                            if (polygon_mode == VK_POLYGON_MODE_LINE)
                            line_width = atof(line_width_string);

                    }

                    // Free the dictionary
                    dict_destroy(rasterizer_json_dict);
                }

                // Populate the rasterizer create struct
                {
                    rasterizer_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                    rasterizer_create_info.depthClampEnable        = depth_clamp_enable;
                    rasterizer_create_info.rasterizerDiscardEnable = rasterizer_discard_enable;
                    rasterizer_create_info.polygonMode             = polygon_mode;
                    rasterizer_create_info.cullMode                = VK_CULL_MODE_FRONT_BIT;
                    rasterizer_create_info.frontFace               = clockwise;
                    rasterizer_create_info.depthBiasEnable         = depth_bias_enable;
                    rasterizer_create_info.depthBiasConstantFactor = depth_bias_constant_factor;
                    rasterizer_create_info.depthBiasClamp          = depth_bias_clamp;
                    rasterizer_create_info.depthBiasSlopeFactor    = depth_bias_slope_factor;
                    rasterizer_create_info.lineWidth               = line_width;
                }

            }

            // Set up the multisampler
            {
                multisampling_create_info.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                multisampling_create_info.sampleShadingEnable  = VK_FALSE;
                multisampling_create_info.minSampleShading     = 1.f;
            }

            // Set up the color blend attachment
            if(attachments){

                // Count each attachment
                while (attachments[++attachment_count]);

                // Allocate memory to create each attachment
                color_blend_attachment_create_info = calloc(attachment_count, sizeof(VkPipelineColorBlendAttachmentState));

                // Iterate over each JSON attachment object
                for (size_t i = 0; i < attachment_count; i++)
                {

                    // Initialized data
                    char *name                           = 0,
                         *source_color_blend_factor      = 0,
                         *destination_color_blend_factor = 0,
                         *color_blend_operation          = 0,
                         *source_alpha_blend_factor      = 0,
                         *destination_alpha_blend_factor = 0,
                         *alpha_blend_operation          = 0;
                    dict *a                              = 0;

                    bool  blend_enable                   = true;

                    // Parse JSON 
                    {

                        // Parse the JSON text into a dictionary
                        parse_json(attachments[i], strlen(attachments[i]), &a);

                        // Parse JSON properties
                        {

                            // Initialized data
                            JSONToken_t *token = 0;

                            token                          = dict_get(a, "name");
                            name                           = JSON_VALUE(token, JSONstring);

                            token                          = dict_get(a, "blend enable");
                            blend_enable                   = JSON_VALUE(token, JSONprimative);

                            token                          = dict_get(a, "source color blend factor");
                            source_color_blend_factor      = JSON_VALUE(token, JSONstring);

                            token                          = dict_get(a, "destination color blend factor");
                            destination_color_blend_factor = JSON_VALUE(token, JSONstring);

                            token                          = dict_get(a, "color blend operation");
                            color_blend_operation          = JSON_VALUE(token, JSONstring);

                            token                          = dict_get(a, "source alpha blend factor");
                            source_alpha_blend_factor      = JSON_VALUE(token, JSONstring);

                            token                          = dict_get(a, "destination alpha blend factor");
                            destination_alpha_blend_factor = JSON_VALUE(token, JSONstring);

                            token                          = dict_get(a, "alpha blend operation");
                            alpha_blend_operation          = JSON_VALUE(token, JSONstring);

                        }

                    }

                    // Set up the attachment
                    {
                        color_blend_attachment_create_info[i].colorWriteMask    = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                        color_blend_attachment_create_info[i].blendEnable       = blend_enable;

                        color_blend_attachment_create_info[i].srcColorBlendFactor = (VkBlendFactor) dict_get(blend_factors   , source_color_blend_factor);
                        color_blend_attachment_create_info[i].dstColorBlendFactor = (VkBlendFactor) dict_get(blend_factors   , destination_color_blend_factor);
                        color_blend_attachment_create_info[i].colorBlendOp        = (VkBlendOp)     dict_get(blend_operations, color_blend_operation);
                    
                        color_blend_attachment_create_info[i].srcAlphaBlendFactor = (VkBlendFactor) dict_get(blend_factors   , source_alpha_blend_factor);
                        color_blend_attachment_create_info[i].dstAlphaBlendFactor = (VkBlendFactor) dict_get(blend_factors   , destination_alpha_blend_factor);
                        color_blend_attachment_create_info[i].alphaBlendOp        = (VkBlendOp)     dict_get(blend_operations, alpha_blend_operation);
                    }

                    // Destroy the JSON
                    dict_destroy(a);
                }

            }

            // Set up the depth / stencil state
            {
                depth_stencil_state_create_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depth_stencil_state_create_info.depthTestEnable = VK_TRUE;
                depth_stencil_state_create_info.depthWriteEnable = VK_TRUE;
                depth_stencil_state_create_info.depthCompareOp   = VK_COMPARE_OP_LESS;
                depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
                depth_stencil_state_create_info.minDepthBounds = 0.f;
                depth_stencil_state_create_info.maxDepthBounds = 1.f;
                depth_stencil_state_create_info.stencilTestEnable = VK_FALSE;
            }

            // Set up the color blending state
            {
                color_blend_create_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                color_blend_create_info.logicOpEnable   = VK_FALSE;
                color_blend_create_info.logicOp         = VK_LOGIC_OP_COPY;
                color_blend_create_info.attachmentCount = attachment_count;
                color_blend_create_info.pAttachments    = color_blend_attachment_create_info;
            }

            // Set up descriptor layout
            {

                // Count up sets
                while (sets[++set_count]);

                i_shader->graphics.set_count              = set_count;
                i_shader->graphics.sets_data              = calloc(set_count, sizeof(struct GXSet_s));
                i_shader->graphics.descriptor_set_layouts = calloc(set_count, sizeof(VkDescriptorSetLayout));


                // Iterate over each set JSON object
                for (size_t i = 0; i < set_count; i++)
                {

                    // Initialized data
                    VkDescriptorSetLayoutCreateInfo  *layout_info         = calloc(1, sizeof(VkDescriptorSetLayoutCreateInfo));
                    VkDescriptorSetLayoutBinding     *ubo_layout_bindings = 0;
                    VkDescriptorPoolSize             *pool_size           = 0;

                    char                             *set_name            = 0,
                                                    **descriptors         = 0;
                    size_t                            descriptor_count    = 0;

                    // Parse JSON 
                    {

                        // Initialized data
                        dict *set_json = 0;
                        
                        // Parse JSON text into a dictionary
                        parse_json(sets[i], strlen(sets[i]), &set_json);

                        // Parse JSON properties
                        {

                            // Initialized data
                            JSONToken_t *token = 0;

                            token       = dict_get(set_json, "name");
                            set_name    = JSON_VALUE(token, JSONstring);

                            token       = dict_get(set_json, "descriptors");
                            descriptors = JSON_VALUE(token, JSONarray);

                        }

                        // Free
                        dict_destroy(set_json);
                        
                    }

                    // Count descriptors
                    while (descriptors[++descriptor_count]);

                    // Allocate memory for descriptors
                    ubo_layout_bindings                     = calloc(descriptor_count, sizeof(VkDescriptorSetLayoutBinding));
                    pool_size                               = calloc(descriptor_count, sizeof(VkDescriptorPoolSize));
                    i_shader->graphics.sets_data[i].descriptors_data = calloc(descriptor_count, sizeof(struct GXSet_s));;

                    dict* descriptor_json = 0;

                    // Iterate over each descriptor JSON object
                    for (size_t j = 0; j < descriptor_count; j++)
                    {

                        // Initialized data
                        char   *descriptor_name = 0, 
                               *type            = 0,
                              **stages          = 0; 

                        // Parse JSON 
                        {
                            
                        
                            // Parse JSON text into a dictionary
                            parse_json(descriptors[j], strlen(descriptors[j]), &descriptor_json);

                            // Parse JSON properties
                            {

                                // Initialized data
                                JSONToken_t *token = 0;

                                token           = dict_get(descriptor_json, "name");
                                descriptor_name = JSON_VALUE(token, JSONstring);

                                token           = dict_get(descriptor_json, "type");
                                type            = JSON_VALUE(token, JSONstring);

                                token           = dict_get(descriptor_json, "stages");
                                stages          = JSON_VALUE(token, JSONarray);

                            }

                            // Copy the descriptor name
                            {

                                // Initialized dtaa
                                size_t len = strlen(descriptor_name);
                                
                                // Allocate memory to copy the name
                                i_shader->graphics.sets_data[i].descriptors_data[j].name = calloc(len+1, sizeof(char));

                                // Error checking
                                strncpy(i_shader->graphics.sets_data[i].descriptors_data[j].name, descriptor_name, len);
                            }

                            // Construct each descriptor set layout binding
                            {
                                VkShaderStageFlagBits f = 0;

                                // Set shader stages
                                for (size_t i = 0; stages[i]; i++)
                                    f |= (VkShaderStageFlagBits) dict_get(shader_stagesD, stages[i]);

                                ubo_layout_bindings[j].binding            = j;

                                // > 1 if array
                                ubo_layout_bindings[j].descriptorCount    = 1;
                                ubo_layout_bindings[j].descriptorType     = (VkDescriptorType)dict_get(descriptor_types, type);
                                ubo_layout_bindings[j].pImmutableSamplers = 0;
                                ubo_layout_bindings[j].stageFlags         = f;
                            }

                            // Construct each pool size
                            {
                                pool_size[j].type            = ubo_layout_bindings[j].descriptorType;
                                pool_size[j].descriptorCount = (u32)2;
                            }

                            // Free
                            dict_destroy(descriptor_json);
                        
                        }

                    }

                    // Populate descriptor set layout info struct
                    {
                        layout_info->sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                        layout_info->bindingCount = descriptor_count;
                        layout_info->pBindings    = ubo_layout_bindings;
                    }

                    // Create a descriptor set layout
                    if (vkCreateDescriptorSetLayout(instance->vulkan.device, layout_info, 0, &i_shader->graphics.descriptor_set_layouts[i]) != VK_SUCCESS) {
                        
                        g_print_error("[G10] [Shader] Failed to create descriptor set layout in call to function \"%s\"\n", __FUNCSIG__);
                        return -1;
                    }

                    // Copy the name of the set
                    {
                                
                        // Initialized data
                        size_t len = strlen(set_name);
                            
                        i_shader->graphics.sets_data[i].name = calloc(len + 1, sizeof(char));

                        // Error checking
                        {
                            #ifndef NDEBUG
                                if (i_shader->graphics.sets_data[i].name == (void *) 0)
                                    goto no_mem;
                            #endif
                        }

                        // Copy the name into the buffer
                        strncpy(i_shader->graphics.sets_data[i].name, set_name, len);

                    }

                    // Set the index of the set
                    i_shader->graphics.sets_data[i].index = i;
                    
                    // Create descriptor pool
                    {
                        VkDescriptorPoolCreateInfo *pool_info = calloc(1, sizeof(VkDescriptorPoolCreateInfo));

                        {
                            pool_info->sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                            pool_info->flags         = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
                            pool_info->poolSizeCount = descriptor_count;
                            pool_info->pPoolSizes    = pool_size;

                            // TODO: Figure out what to do here
                            pool_info->maxSets       = (u32)64;
                        }

                        if (vkCreateDescriptorPool(instance->vulkan.device, pool_info, (void*)0, &i_shader->graphics.sets_data[i].descriptor_pool) != VK_SUCCESS) {
                            g_print_error("[G10] [Shader] Failed to create descriptor pool for set \"%s\" in call to function \"%s\"\n", set_name, __FUNCSIG__);
                            return 0;
                        }
                    }

                }
            }

            // Set up the dynamic state
            {
                dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
                dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR;

                dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamic_state_create_info.dynamicStateCount = 2;
                dynamic_state_create_info.pDynamicStates    = &dynamic_states;
            }

            // Set up the push constant
            if(push_constant_text) {
                
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
                    token                = dict_get(push_constant_dict, "struct");
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
                        
                        token = dict_get(property_dict, "name");
                        property_name = JSON_VALUE(token, JSONstring);

                        token = dict_get(property_dict, "type");
                        char* property_type = JSON_VALUE(token, JSONstring);

                        // TODO: Check property_type?
                        property_size = dict_get(format_sizes, property_type);

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
                push_constant->size       = push_constant_size;
                push_constant->stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            }

            // Set up the pipeline layout
            {

                pipeline_layout_create_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pipeline_layout_create_info.setLayoutCount = set_count;

                if (push_constant_text)
                {
                    pipeline_layout_create_info.pPushConstantRanges = push_constant;
                    pipeline_layout_create_info.pushConstantRangeCount = 1;
                }

                pipeline_layout_create_info.pSetLayouts    = i_shader->graphics.descriptor_set_layouts;
                
            }

            // Construct the pipeline layout
            if (vkCreatePipelineLayout(instance->vulkan.device, &pipeline_layout_create_info, 0, &i_shader->graphics.pipeline_layout) != VK_SUCCESS)
                g_print_error("[G10] [Shader] Failed to create pipeline layout");

            // Set up the graphics pipeline
            {
                graphics_pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                graphics_pipeline_create_info.stageCount = shader_stage_iterator;
                graphics_pipeline_create_info.pStages = shader_stages;
                graphics_pipeline_create_info.pVertexInputState = &vertex_input_info_create_info;
                graphics_pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;

                // TODO: 
                graphics_pipeline_create_info.pTessellationState = 0;
                graphics_pipeline_create_info.pViewportState = &viewport_state_create_info;
                graphics_pipeline_create_info.pRasterizationState = &rasterizer_create_info;
                graphics_pipeline_create_info.pMultisampleState = &multisampling_create_info;

                // TODO: 
                graphics_pipeline_create_info.pDepthStencilState = &depth_stencil_state_create_info;
                graphics_pipeline_create_info.pColorBlendState = &color_blend_create_info;
                graphics_pipeline_create_info.pDynamicState = &dynamic_state_create_info;
                graphics_pipeline_create_info.layout = &pipeline_layout_create_info;
                graphics_pipeline_create_info.layout = i_shader->graphics.pipeline_layout;
                graphics_pipeline_create_info.renderPass = instance->context.renderer->render_passes_data[0]->render_pass;
                graphics_pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
                graphics_pipeline_create_info.basePipelineIndex = -1;
            }

            // Construct the graphics pipeline
            if (vkCreateGraphicsPipelines(instance->vulkan.device, VK_NULL_HANDLE, 1, &graphics_pipeline_create_info, 0, &i_shader->graphics.pipeline) != VK_SUCCESS)
                g_print_error("failed to create graphics pipeline!\n");

            // Clean up 
            {
                free(shader_stages);
                free(color_blend_attachment_create_info);
                free(binding_description);
                free(vertex_input_attribute_descriptions);
            }

            // 
            {

                // Create uniform buffers
                {
                    // Iterate over each uniform
                // Iterate over each frame
                // Create a buffer for each uniform for each frame

                    VkDeviceSize buffer_size = 3 * sizeof(mat4) + sizeof(vec3);

                    i_shader->graphics.uniform_buffers = calloc(sizeof(void*), 2);
                    i_shader->graphics.uniform_buffers_memory = calloc(sizeof(void*), 2);

                    for (size_t i = 0; i < 2; i++)
                        create_buffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &i_shader->graphics.uniform_buffers[i], &i_shader->graphics.uniform_buffers_memory[i]);
                }

                // Create descriptor sets
                {
                    VkDescriptorSetLayout* layouts = calloc(2, sizeof(VkDescriptorSetLayout));
                    VkDescriptorSetAllocateInfo* alloc_info = calloc(1, sizeof(VkDescriptorSetAllocateInfo));

                    for (size_t i = 0; i < 2; i++)
                        memcpy(&layouts[i], &i_shader->graphics.descriptor_set_layouts[0], sizeof(VkDescriptorSetLayout));

                    {
                        alloc_info->sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                        alloc_info->descriptorPool = i_shader->graphics.sets_data[0].descriptor_pool;
                        alloc_info->descriptorSetCount = 2;
                        alloc_info->pSetLayouts = layouts;
                    }

                    i_shader->graphics.sets_data[0].descriptor_sets = calloc(2, sizeof(VkDescriptorSet));

                    if (vkAllocateDescriptorSets(instance->vulkan.device, alloc_info, i_shader->graphics.sets_data[0].descriptor_sets) != VK_SUCCESS) {
                        g_print_error("[G10] [Shader] Failed to allocate descriptor sets in call to function \"%s\"\n", __FUNCSIG__);
                    }

                    for (size_t i = 0; i < 2; i++)
                    {
                        VkDescriptorBufferInfo* buffer_info = calloc(1, sizeof(VkDescriptorBufferInfo));
                        VkWriteDescriptorSet* descriptor_write = calloc(1, sizeof(VkWriteDescriptorSet));

                        buffer_info->buffer = i_shader->graphics.uniform_buffers[i];
                        buffer_info->offset = 0;
                        buffer_info->range = 3 * sizeof(mat4) + sizeof(vec3);

                        descriptor_write->sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        descriptor_write->dstSet = i_shader->graphics.sets_data[0].descriptor_sets[i];
                        descriptor_write->dstBinding = 0;
                        descriptor_write->dstArrayElement = 0;
                        descriptor_write->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        descriptor_write->descriptorCount = 1;
                        descriptor_write->pBufferInfo = buffer_info;

                        vkUpdateDescriptorSets(instance->vulkan.device, 1, descriptor_write, 0, 0);
                    }
                }
            }
    }

    }

    // Construct the compute shader
    else if ( compute_shader_path ) 
    {

    }

    // TODO: Construct the raytracing shader
    // else if ( ray_generation_shader ) ...

    // Cache the shader
    g_cache_shader(instance, i_shader);

    exit:
    return 1;

    // TODO: Error handling
    no_mem:
    push_constant_is_too_large:
        return 0;
        
}

int use_shader                      ( GXShader_t   *shader )
{
    GXInstance_t *instance = g_get_active_instance();
    VkViewport   *viewport = calloc(1, sizeof(VkViewport));
    VkRect2D     *scissor  = calloc(1, sizeof(VkRect2D));

    // Use the shader
    vkCmdBindPipeline(instance->vulkan.command_buffers[instance->vulkan.current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->graphics.pipeline);
    
    // Set the viewport
    {
        viewport->x        = 0.f;
        viewport->y        = 0.f;
        viewport->width    = instance->vulkan.swap_chain_extent.width;
        viewport->height   = instance->vulkan.swap_chain_extent.height;
        viewport->minDepth = 0.f;
        viewport->maxDepth = 1.f;

        vkCmdSetViewport(instance->vulkan.command_buffers[instance->vulkan.current_frame], 0, 1, viewport);
    }
    
    // Set the scissor
    {
        scissor->offset.x = 0;
        scissor->offset.y = 0;

        scissor->extent   = instance->vulkan.swap_chain_extent;

        vkCmdSetScissor(instance->vulkan.command_buffers[instance->vulkan.current_frame], 0, 1, scissor);
    }

    // Clean up
    {
        free(viewport);
        free(scissor);
    }

    return 0;
}

int update_shader_push_constant     ( GXShader_t   *shader )
{
    size_t offset = 0;
    for (size_t i = 0; i < shader->graphics.push_constant_properties[i]; i++)
    {
        char   *push_constant_property_name = shader->graphics.push_constant_properties[i];
        int   (*getter) ( void * )          = dict_get(push_constant_getters, push_constant_property_name);

        if (getter)
            offset = getter((u8*)shader->graphics.push_constant_data + offset);

    }
    return 1;
}

int add_shader_push_constant_getter ( char         *getter_name, int(*getter_function)(void*) )
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
                    g_print_error("[G10] [Shader] Null pointer provided for \"getter_name\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
            no_getter_function:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Null pointer provided for \"getter_function\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // User errors
        {
            already_has_getter:
                #ifndef NDEBUG
                    g_print_error("[G10] [Shader] Failed to add getter in call to function \"%s\". There is already a getter called \"%s\"\n", __FUNCSIG__, getter_name);
                #endif
                return 0;
        }
    }
}

int set_shader_camera               ( GXEntity_t   *p_entity )
{
     
    // TODO: Argument check

    // Initialized data
    GXInstance_t  *instance  = g_get_active_instance();
    GXCamera_t    *camera    = instance->context.scene->active_camera;
    GXTransform_t *transform = p_entity->transform;

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

    vkMapMemory(instance->vulkan.device, p_entity->shader->graphics.uniform_buffers_memory[instance->vulkan.current_frame], 0, sizeof(vec3) + 3 * sizeof(mat4), 0, &data);
    memcpy(data, uniform_buffer, sizeof(vec3) + 3*sizeof(mat4));
    vkUnmapMemory(instance->vulkan.device, p_entity->shader->graphics.uniform_buffers_memory[instance->vulkan.current_frame]);

    return 1;

    // Error handling
    {

        // Argument error
        {
            #ifndef NDEBUG
                no_shader:
                    g_print_log("[G10] [Shader] Null pointer provided for \"shader\" in call to function \"%s\"\n", __FUNCSIG__);
                    return 0;
            #endif
        }
    }
}

int destroy_shader                  ( GXShader_t   *shader )
{

    // TODO: Cache destruction
    GXInstance_t* instance = g_get_active_instance();

    vkDestroyPipeline(instance->vulkan.device, shader->graphics.pipeline, 0);
    vkDestroyPipelineLayout(instance->vulkan.device, shader->graphics.pipeline_layout, 0);

    vkDestroyShaderModule(instance->vulkan.device, shader->graphics.vertex_shader_module, 0);
    vkDestroyShaderModule(instance->vulkan.device, shader->graphics.fragment_shader_module, 0);

    //free(shader->name);
    free(shader);
    return 0;
}
