#include <G10/GXShader.h>

char             *format_type_names              [ ] = {
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
    "u8vec4"
};
char             *descriptor_type_names          [ ] = {
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
char             *push_constant_getter_names     [ ] = {
    "camera position",
};
VkDescriptorType  descriptor_type_enums          [ ] = {
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
VkFormat          format_type_enums              [ ] = {
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
    VK_FORMAT_R8G8B8A8_UINT
};
size_t            format_type_sizes              [ ] = {
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
    36
};
void            **push_constant_getter_functions [ ] = {
    &get_camera_position
};

dict             *format_types                       = 0;
dict             *format_sizes                       = 0;
dict             *descriptor_types                   = 0;
dict             *push_constant_getters              = 0;

typedef struct { 
    char                         *name; 
    VkDescriptorType              descriptor_type;
    VkDescriptorSetLayoutBinding  descriptor_set_layout_binding;
    union                         
    {
        struct {
            VkDeviceSize    size;
            VkBuffer       *buffers;
            VkDeviceMemory *memories;
        } uniform;

        struct {

            // TODO: 
            size_t i;
        } image;
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

void init_shader                 ( void )
{
    dict_construct(&format_types , 16);
    dict_construct(&format_sizes , 16);
    dict_construct(&descriptor_types, 16);
    dict_construct(&push_constant_getters, 64);

    for (size_t i = 0; i < 16; i++)
    {
        dict_add(format_types , format_type_names[i]    , (void *)format_type_enums[i]);
        dict_add(format_sizes , format_type_names[i]    , (void *)format_type_sizes[i]);
    }

    for (size_t i = 0; i < 11; i++)
        dict_add(descriptor_types, descriptor_type_names[i], (void*)descriptor_type_enums[i]); 

    // Initialize push constant getters 
    for (size_t i = 0; i < 1; i++)
        dict_add(push_constant_getters, push_constant_getter_names[i], (void*)push_constant_getter_functions[i]);

}

int create_shader_module         ( char         *code   , size_t     code_len  , VkShaderModule *shader_module )
{
    GXInstance_t* instance = g_get_active_instance();

    VkShaderModuleCreateInfo* shader_module_create_info = calloc(1, sizeof(VkShaderModuleCreateInfo));
    shader_module_create_info->sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info->codeSize = code_len;
    shader_module_create_info->pCode = code;

    if (vkCreateShaderModule(instance->device, shader_module_create_info, (void*)0, shader_module))

        // TODO: Replace with a goto
        printf("Failed to create a shader module");

    return 0;
}

int create_shader                ( GXShader_t  **shader )
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

int load_shader                  ( GXShader_t  **shader, const char *path )
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

int load_shader_as_json          ( GXShader_t  **shader, char       *token_text, size_t          token_text_len )
{
    // TODO: Argument check
    GXInstance_t  *instance                  = g_get_active_instance();
    GXShader_t    *i_shader                  = 0;
    char          *name                      = 0,
                  *vertex_shader_path        = 0,
                  *fragment_shader_path      = 0;
    dict          *json_data                 = 0,
                  *vertex_shader_json_data   = 0,
                  *fragment_shader_json_data = 0;
    char         **vertex_groups             = 0,
                 **sets                      = 0,
                  *push_constant_text        = 0;
    u32            set_count                 = 0;

    JSONToken_t   *token                     = 0;

    // Parse JSON data
    {

        parse_json(token_text, token_text_len, &json_data);

        token                = dict_get(json_data, "name");
        name                 = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "vertex shader path");
        vertex_shader_path   = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "fragment shader path");
        fragment_shader_path = JSON_VALUE(token, JSONstring);

        token                = dict_get(json_data, "in");
        vertex_groups        = JSON_VALUE(token, JSONarray);

        token                = dict_get(json_data, "sets");
        sets                 = JSON_VALUE(token, JSONarray);

        token                = dict_get(json_data, "push constant");
        push_constant_text        = JSON_VALUE(token, JSONobject);
    }
    
    // Check the cache
    {
        GXShader_t *r = g_find_shader(instance, name);
        
        if (r != (void*)0)
        {
            *shader = r;
            return 1;
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
            size_t name_len = strlen(name);

            i_shader->name = calloc(name_len + 1, sizeof(char));

            // TODO: Check memory

            strncpy(i_shader->name, name, name_len);
        }

        // Load the vertex shader binary
        {

            // Initialized data
            size_t   vertex_shader_data_len = g_load_file(vertex_shader_path, 0, true);
            char    *vertex_shader_data     = calloc(vertex_shader_data_len, sizeof(char));

            g_load_file(vertex_shader_path, vertex_shader_data, true);

            // Create a shader module
            create_shader_module(vertex_shader_data, vertex_shader_data_len, &i_shader->vertex_shader_module);

            free(vertex_shader_data);
        }
        
        // Load the fragment shader binary
        {
            size_t fragment_shader_data_len = g_load_file(fragment_shader_path, 0, true);
            char* fragment_shader_data      = calloc(fragment_shader_data_len + 1, sizeof(char));
            
            g_load_file(fragment_shader_path, fragment_shader_data, true);

            // Create a shader module
            create_shader_module(fragment_shader_data, fragment_shader_data_len, &i_shader->fragment_shader_module);
                
            free(fragment_shader_data);

        }

        // Set up the shader pipeline
        {

            // Initialized data
            VkPipelineShaderStageCreateInfo        *shader_stages                       = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
            VkPipelineVertexInputStateCreateInfo   *vertex_input_info_create_info       = calloc(1, sizeof(VkPipelineVertexInputStateCreateInfo));
            VkPipelineInputAssemblyStateCreateInfo *input_assembly_create_info          = calloc(1, sizeof(VkPipelineInputAssemblyStateCreateInfo));
            VkPipelineViewportStateCreateInfo      *viewport_state_create_info          = calloc(1, sizeof(VkPipelineViewportStateCreateInfo));
            VkPipelineRasterizationStateCreateInfo *rasterizer_create_info              = calloc(1, sizeof(VkPipelineRasterizationStateCreateInfo));
            VkPipelineMultisampleStateCreateInfo   *multisampling_create_info           = calloc(1, sizeof(VkPipelineMultisampleStateCreateInfo));
            VkPipelineColorBlendAttachmentState    *color_blend_attachment_create_info  = calloc(1, sizeof(VkPipelineColorBlendAttachmentState));
            VkPipelineColorBlendStateCreateInfo    *color_blend_create_info             = calloc(1, sizeof(VkPipelineColorBlendStateCreateInfo));
            VkPipelineDynamicStateCreateInfo       *dynamic_state_create_info           = calloc(1, sizeof(VkPipelineDynamicStateCreateInfo));
            VkPipelineLayoutCreateInfo             *pipeline_layout_create_info         = calloc(1, sizeof(VkPipelineLayoutCreateInfo));
            VkDynamicState                         *dynamic_states                      = calloc(2, sizeof(VkPipelineDynamicStateCreateInfo));
            VkPushConstantRange                    *push_constant                       = calloc(1, sizeof(VkPushConstantRange));
            VkGraphicsPipelineCreateInfo           *graphics_pipeline_create_info       = calloc(1, sizeof(VkGraphicsPipelineCreateInfo));
            VkVertexInputBindingDescription        *binding_description                 = calloc(1, sizeof(VkVertexInputBindingDescription));
            VkViewport                             *viewport                            = calloc(1, sizeof(VkViewport));
            VkRect2D                               *scissor                             = calloc(1, sizeof(VkRect2D));
            VkVertexInputAttributeDescription      *vertex_input_attribute_descriptions = 0;
            VkDescriptorSetLayoutCreateInfo        *descriptor_set_layout               = 0;

            // Set up the shader stages
            {

                // Set up the vertex shader
                {
                    VkPipelineShaderStageCreateInfo* vertex_shader_stage_create_info = &shader_stages[0];
                    vertex_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    vertex_shader_stage_create_info->stage  = VK_SHADER_STAGE_VERTEX_BIT;
                    vertex_shader_stage_create_info->module = i_shader->vertex_shader_module;
                    vertex_shader_stage_create_info->pName  = "main";
                }

                // Set up the fragment shader
                {

                    VkPipelineShaderStageCreateInfo *fragment_shader_stage_create_info = &shader_stages[1];

                    fragment_shader_stage_create_info->sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    fragment_shader_stage_create_info->stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
                    fragment_shader_stage_create_info->module = i_shader->fragment_shader_module;
                    fragment_shader_stage_create_info->pName  = "main";
                }
            }

            // Set up the vertex input state
            {
                u32 vertex_group_count = 0;

                size_t stride = 0,
                    binding_description_count = 0;

                if (vertex_groups)
                {

                    while (vertex_groups[++vertex_group_count]);
                    

                    vertex_input_attribute_descriptions = calloc(vertex_group_count, sizeof(VkVertexInputAttributeDescription));

                    for (size_t i = 0; i < vertex_group_count; i++)
                    {
                        dict        *vertex_group   = 0;
                        JSONToken_t *token          = 0;
                        char        *type           = 0;
                        char        *location       = 0;

                        parse_json(vertex_groups[i], strlen(vertex_groups[i]), &vertex_group);

                        token    = dict_get(vertex_group, "type");
                        type     = JSON_VALUE(token, JSONstring);

                        token    = dict_get(vertex_group, "location");
                        location = JSON_VALUE(token, JSONprimative);

                        vertex_input_attribute_descriptions[i].binding  = 0;
                        vertex_input_attribute_descriptions[i].location = atoi(location);
                        vertex_input_attribute_descriptions[i].format   = (VkFormat)dict_get(format_types, type);
                        vertex_input_attribute_descriptions[i].offset   = stride;
                        
                        stride += (size_t) dict_get(format_sizes, type);

                    }

                    binding_description->binding = 0;
                    binding_description->inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                    binding_description->stride = stride;
                    binding_description_count = 1;
                }

                vertex_input_info_create_info->sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertex_input_info_create_info->vertexBindingDescriptionCount   = binding_description_count;
                vertex_input_info_create_info->pVertexBindingDescriptions      = binding_description;
                vertex_input_info_create_info->vertexAttributeDescriptionCount = vertex_group_count;
                vertex_input_info_create_info->pVertexAttributeDescriptions    = vertex_input_attribute_descriptions;
            }

            // Set up the input assembly
            {
                input_assembly_create_info->sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                input_assembly_create_info->topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                input_assembly_create_info->primitiveRestartEnable = VK_FALSE;

            }

            // Set up the viewport state
            {

                // Set up the viewport
                {
                    viewport->x = 0.f;
                    viewport->y = 0.f;
                    viewport->width = instance->swap_chain_extent.width;
                    viewport->height = instance->swap_chain_extent.height;
                    viewport->minDepth = 0.0f;
                    viewport->maxDepth = 1.0f;
                }

                // Set up the scissor
                {
                    scissor->offset.x = 0;
                    scissor->offset.y = 0;
                    scissor->extent = instance->swap_chain_extent;
                }

                viewport_state_create_info->sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewport_state_create_info->viewportCount = 1;
                viewport_state_create_info->pViewports    = viewport;
                viewport_state_create_info->scissorCount  = 1;
                viewport_state_create_info->pScissors     = scissor;
            }

            // Set up the rasterizer 
            {
                rasterizer_create_info->sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizer_create_info->depthClampEnable        = VK_FALSE;
                rasterizer_create_info->depthBiasClamp          = 0.0f;
                rasterizer_create_info->rasterizerDiscardEnable = VK_FALSE;
                rasterizer_create_info->polygonMode             = VK_POLYGON_MODE_FILL;
                rasterizer_create_info->lineWidth               = 1.f;
                rasterizer_create_info->cullMode                = VK_CULL_MODE_BACK_BIT;
                rasterizer_create_info->frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                rasterizer_create_info->depthBiasEnable         = VK_FALSE;
            }

            // Set up the multisampler
            {
                multisampling_create_info->sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisampling_create_info->sampleShadingEnable  = VK_FALSE;
                multisampling_create_info->rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
                multisampling_create_info->minSampleShading     = 1.f;
            }

            // Set up the color blend attachment
            {
                color_blend_attachment_create_info->colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
                color_blend_attachment_create_info->blendEnable         = VK_FALSE;
                color_blend_attachment_create_info->srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
                color_blend_attachment_create_info->dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
                color_blend_attachment_create_info->colorBlendOp        = VK_BLEND_OP_ADD;
                color_blend_attachment_create_info->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                color_blend_attachment_create_info->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                color_blend_attachment_create_info->alphaBlendOp        = VK_BLEND_OP_ADD;
                color_blend_attachment_create_info->blendEnable         = VK_TRUE;
                color_blend_attachment_create_info->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
                color_blend_attachment_create_info->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
                color_blend_attachment_create_info->colorBlendOp        = VK_BLEND_OP_ADD;
                color_blend_attachment_create_info->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
                color_blend_attachment_create_info->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
                color_blend_attachment_create_info->alphaBlendOp        = VK_BLEND_OP_ADD;
            }

            // Set up the color blending state
            {
                color_blend_create_info->sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                color_blend_create_info->logicOpEnable   = VK_FALSE;
                color_blend_create_info->logicOp         = VK_LOGIC_OP_COPY;
                color_blend_create_info->attachmentCount = 1;
                color_blend_create_info->pAttachments    = color_blend_attachment_create_info;
            }

            // Set up descriptor layout
            {
                VkDescriptorSetLayoutBinding    *ubo_layout_binding = calloc(1, sizeof(VkDescriptorSetLayoutBinding));
                VkDescriptorSetLayoutCreateInfo *layout_info        = calloc(1, sizeof(VkDescriptorSetLayoutCreateInfo));

                {
                    ubo_layout_binding->binding            = 0;
                    ubo_layout_binding->descriptorCount    = 1;
                    ubo_layout_binding->descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ubo_layout_binding->pImmutableSamplers = 0;
                    ubo_layout_binding->stageFlags         = VK_SHADER_STAGE_VERTEX_BIT;
                }

                {
                    layout_info->sType                     = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
                    layout_info->bindingCount              = 1;
                    layout_info->pBindings                 = ubo_layout_binding;
                }
                if (vkCreateDescriptorSetLayout(instance->device, layout_info, 0, &i_shader->descriptor_set_layout) != VK_SUCCESS) {
                    return -1;
                }
            }

            // Set up the dynamic state
            {
                
                dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
                dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR;

                dynamic_state_create_info->sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamic_state_create_info->dynamicStateCount = 2;
                dynamic_state_create_info->pDynamicStates    = dynamic_states;
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
                
                i_shader->push_constant_properties = calloc(push_constant_property_count + 1, sizeof(char*));
                
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
                        i_shader->push_constant_properties[i] = calloc(property_name_len + 1, sizeof(char));

                        // Copy the propery name
                        strncpy(i_shader->push_constant_properties[i], property_name, property_name_len);

                    }

                    // Copy the property size
                    push_constant_size += property_size;

                }

                if (push_constant_size > 128)
                    goto push_constant_is_too_large;

                i_shader->push_constant_size = push_constant_size;

                i_shader->push_constant_data = calloc(128, sizeof(u8));

                push_constant->offset     = 0;
                push_constant->size       = push_constant_size;
                push_constant->stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
            }

            // Set up the pipeline layout
            {

                pipeline_layout_create_info->sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                pipeline_layout_create_info->setLayoutCount = 1;
                pipeline_layout_create_info->pPushConstantRanges = push_constant;
                pipeline_layout_create_info->pushConstantRangeCount = 1;

                i_shader->descriptor_set_layouts            = calloc(1, sizeof(VkDescriptorSetLayout));

                pipeline_layout_create_info->pSetLayouts    = &i_shader->descriptor_set_layout;
                
            }

            // Construct the pipeline
            if (vkCreatePipelineLayout(instance->device, pipeline_layout_create_info, 0, &i_shader->pipeline_layout) != VK_SUCCESS)
                g_print_error("[G10] [Shader] Failed to create vulkan pipeline");

            // Set up the graphics pipeline
            {
                graphics_pipeline_create_info->sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                graphics_pipeline_create_info->stageCount          = 2;
                graphics_pipeline_create_info->pStages             = shader_stages;
                graphics_pipeline_create_info->pVertexInputState   = vertex_input_info_create_info;
                graphics_pipeline_create_info->pInputAssemblyState = input_assembly_create_info;
                graphics_pipeline_create_info->pViewportState      = viewport_state_create_info;
                graphics_pipeline_create_info->pRasterizationState = rasterizer_create_info;
                graphics_pipeline_create_info->pMultisampleState   = multisampling_create_info;
                graphics_pipeline_create_info->pColorBlendState    = color_blend_create_info;
                graphics_pipeline_create_info->pDynamicState       = dynamic_state_create_info;
                graphics_pipeline_create_info->layout              = pipeline_layout_create_info;
                graphics_pipeline_create_info->renderPass          = instance->render_pass;
                graphics_pipeline_create_info->subpass             = 0;
                graphics_pipeline_create_info->basePipelineHandle  = VK_NULL_HANDLE;
                graphics_pipeline_create_info->layout              = i_shader->pipeline_layout;
                graphics_pipeline_create_info->renderPass          = instance->render_pass;
                graphics_pipeline_create_info->basePipelineIndex   = -1;
            }

            // Construct the graphics pipeline
            if (vkCreateGraphicsPipelines(instance->device, VK_NULL_HANDLE, 1, graphics_pipeline_create_info, 0, &i_shader->pipeline) != VK_SUCCESS)
                g_print_error("failed to create graphics pipeline!\n");

            // Clean up 
            {
                free(shader_stages);
                free(vertex_input_info_create_info);
                free(input_assembly_create_info);
                free(viewport_state_create_info);
                free(rasterizer_create_info);
                free(multisampling_create_info);
                free(color_blend_attachment_create_info);
                free(color_blend_create_info);
                free(dynamic_state_create_info);
                free(pipeline_layout_create_info);
                free(dynamic_states);
                free(graphics_pipeline_create_info);
                free(binding_description);
                free(viewport);
                free(scissor);
                free(vertex_input_attribute_descriptions);
            }

        }

        // Create uniform buffers
        {
            // Iterate over each uniform
            // Iterate over each frame
            // Create a buffer for each uniform for each frame

            VkDeviceSize buffer_size = 3 * sizeof(mat4) + sizeof(vec3);
    
            i_shader->uniform_buffers        = calloc(sizeof(void *), 2);
            i_shader->uniform_buffers_memory = calloc(sizeof(void *), 2);

            for (size_t i = 0; i < 2; i++)
                create_buffer(buffer_size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &i_shader->uniform_buffers[i], &i_shader->uniform_buffers_memory[i]);
        }

        // Create descriptor pool
        {
            VkDescriptorPoolSize       *pool_size = calloc(1, sizeof(VkDescriptorPoolSize));
            VkDescriptorPoolCreateInfo *pool_info = calloc(1, sizeof(VkDescriptorPoolCreateInfo));

            {
                pool_size->type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                pool_size->descriptorCount = (u32)2;
            }

            {
                pool_info->sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
                pool_info->poolSizeCount = 1;
                pool_info->pPoolSizes    = pool_size;
                pool_info->maxSets       = (u32)2;
            }

            if (vkCreateDescriptorPool(instance->device, pool_info, (void *)0, &i_shader->descriptor_pool) != VK_SUCCESS) {
                return -1;
        }
        }

        // Create descriptor sets
        {
            VkDescriptorSetLayout       *layouts    = calloc(2, sizeof(VkDescriptorSetLayout));
            VkDescriptorSetAllocateInfo *alloc_info = calloc(1, sizeof(VkDescriptorSetAllocateInfo));

            for (size_t i = 0; i < 2; i++)
                memcpy(&layouts[i], &i_shader->descriptor_set_layout, sizeof(VkDescriptorSetLayout));

            {
                alloc_info->sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                alloc_info->descriptorPool     = i_shader->descriptor_pool;
                alloc_info->descriptorSetCount = 2;
                alloc_info->pSetLayouts        = layouts;
            }

            i_shader->descriptor_sets = calloc(2, sizeof(VkDescriptorSet));

            if (vkAllocateDescriptorSets(instance->device, alloc_info, i_shader->descriptor_sets) != VK_SUCCESS) {

            }

            for (size_t i = 0; i < 2; i++) {
                VkDescriptorBufferInfo *buffer_info      = calloc(1, sizeof(VkDescriptorBufferInfo));
                VkWriteDescriptorSet   *descriptor_write = calloc(1, sizeof(VkWriteDescriptorSet));

                buffer_info->buffer = i_shader->uniform_buffers[i];
                buffer_info->offset = 0;
                buffer_info->range  = 3*sizeof(mat4) + sizeof(vec3);

                descriptor_write->sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptor_write->dstSet          = i_shader->descriptor_sets[i];
                descriptor_write->dstBinding      = 0;
                descriptor_write->dstArrayElement = 0;
                descriptor_write->descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptor_write->descriptorCount = 1;
                descriptor_write->pBufferInfo     = buffer_info;
    
                vkUpdateDescriptorSets(instance->device, 1, descriptor_write, 0, 0);
            }
        }
    }

    // Cache the shader
    g_cache_shader(instance, i_shader);

    return 1;

    // TODO: Error handling
push_constant_is_too_large:
    return 0;
        
}

int use_shader                   ( GXShader_t   *shader )
{
    GXInstance_t *instance = g_get_active_instance();
    VkViewport   *viewport = calloc(1, sizeof(VkViewport));
    VkRect2D     *scissor  = calloc(1, sizeof(VkRect2D));

    // Use the shader
    vkCmdBindPipeline(instance->command_buffers[instance->current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, shader->pipeline);
    
    // Set the viewport
    {
        viewport->x        = 0.f;
        viewport->y        = 0.f;
        viewport->width    = instance->swap_chain_extent.width;
        viewport->height   = instance->swap_chain_extent.height;
        viewport->minDepth = 0.f;
        viewport->maxDepth = 1.f;

        vkCmdSetViewport(instance->command_buffers[instance->current_frame], 0, 1, viewport);
    }
    
    // Set the scissor
    {
        scissor->offset.x = 0;
        scissor->offset.y = 0;

        scissor->extent   = instance->swap_chain_extent;

        vkCmdSetScissor(instance->command_buffers[instance->current_frame], 0, 1, scissor);
    }

    // Clean up
    {
        free(viewport);
        free(scissor);
    }

    return 0;
}

int update_shader_push_constant  ( GXShader_t   *shader )
{
    size_t offset = 0;
    for (size_t i = 0; i < shader->push_constant_properties[i]; i++)
    {
        char   *push_constant_property_name = shader->push_constant_properties[i];
        int   (*getter) ( void * )          = dict_get(push_constant_getters, push_constant_property_name);

        if (getter)
            offset = getter((u8*)shader->push_constant_data + offset);

    }
    return 1;
}

int add_shader_push_constant_getter ( char* getter_name, int(*getter_function)(void*) )
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

int set_shader_camera(GXEntity_t *p_entity)
{
     
    // TODO: Argument check

    // Initialized data
    GXInstance_t  *instance  = g_get_active_instance();
    GXCamera_t    *camera    = instance->active_scene->active_camera;
    GXTransform_t *transform = p_entity->transform;

    // Update the transform
    transform_model_matrix(transform, &transform->model_matrix);

    vec3 a;
    add_vec3(&a, camera->target, camera->location);

    // Update the view matrix
    camera->view       = look_at(camera->location, a, camera->up);

    // Update the projection matrix
    camera->projection = perspective_matrix(camera->fov, camera->aspect_ratio, camera->near_clip, camera->far_clip);

    mat4 uniform_buffer[4];
    uniform_buffer[0]   = transform->model_matrix;
    uniform_buffer[1]   = camera->view;
    uniform_buffer[2]   = camera->projection;

    uniform_buffer[3].a = camera->location.x;
    uniform_buffer[3].b = camera->location.y;
    uniform_buffer[3].c = camera->location.z;

    void *data;

    vkMapMemory(instance->device, p_entity->shader->uniform_buffers_memory[instance->current_frame], 0, sizeof(vec3) + 3 * sizeof(mat4), 0, &data);
    memcpy(data, uniform_buffer, sizeof(vec3) + 3*sizeof(mat4));
    vkUnmapMemory(instance->device, p_entity->shader->uniform_buffers_memory[instance->current_frame]);

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

int destroy_shader               ( GXShader_t   *shader )
{

    // TODO: Cache destruction
    GXInstance_t* instance = g_get_active_instance();

    vkDestroyPipeline(instance->device, shader->pipeline, 0);
    vkDestroyPipelineLayout(instance->device, shader->pipeline_layout, 0);

    vkDestroyShaderModule(instance->device, shader->vertex_shader_module, 0);
    vkDestroyShaderModule(instance->device, shader->fragment_shader_module, 0);

    //free(shader->name);
    free(shader);
    return 0;
}
