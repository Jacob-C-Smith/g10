#include <G10/GXShader.h>

char             *format_type_names     [ ] = {
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
    "double"
};
char             *descriptor_type_names [ ] = {
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
VkDescriptorType  descriptor_type_enums [ ] = {
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
VkFormat          format_type_enums     [ ] = {
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
    VK_FORMAT_R64_SFLOAT
};
size_t            format_type_sizes     [ ] = {
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
    8
};

dict             *format_types              = 0;
dict             *format_sizes              = 0;
dict             *descriptor_types          = 0;

typedef struct { char* name; VkDescriptorType descriptor_type; dict* uniforms; VkDescriptorSetLayoutBinding descriptor_set_layout_binding; size_t index; } GXDescriptor_t;
typedef struct { char* name; dict* descriptors; size_t len, index; }                    GXSet_t;

void init_shader                 ( void )
{
    dict_construct(&format_types , 16);
    dict_construct(&format_sizes , 16);
    dict_construct(&descriptor_types, 16);

    for (size_t i = 0; i < 13; i++)
    {
        dict_add(format_types , format_type_names[i]    , (void *)format_type_enums[i]);
        dict_add(format_sizes , format_type_names[i]    , (void *)format_type_sizes[i]);
    }

    for (size_t i = 0; i < 11; i++)
        dict_add(descriptor_types, descriptor_type_names[i], (void*)descriptor_type_enums[i]);
}

int construct_descriptor         ( GXDescriptor_t **ret, dict *binding_json )
{
    GXDescriptor_t *i_descriptor = calloc(1, sizeof(GXDescriptor_t));

    char* name = 0,
        * type = 0;

    *ret = i_descriptor;

    // Parse the dictionary
    {
        JSONToken_t* token = 0;

        token = dict_get(binding_json, "name");
        name  = JSON_VALUE(token, JSONstring);

        token = dict_get(binding_json, "type");
        type  = JSON_VALUE(token, JSONstring);
    }

    // Construct the descriptor
    {

        // Set the name
        {
            size_t name_len    = strlen(name);

            i_descriptor->name = calloc(name_len + 1, sizeof(char));

            strncpy(i_descriptor->name, name, name_len);
        }

        {
            i_descriptor->descriptor_type = (VkDescriptorType) dict_get(descriptor_types, type);

            switch (i_descriptor->descriptor_type){
                case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
                {

                }
            }


        }

        
    }

    return 0;
};

int construct_set                ( GXSet_t        **ret, dict *set_json )
{
    // Initialized data
    GXSet_t *set                 = calloc(1, sizeof(GXSet_t));

    char    *set_name            = 0;
    size_t   set_index           = 0;

    size_t   descriptor_count = 0;
    char   **descriptors      = 0;

    // Parse the set JSON
    {
        // Initialized data
        JSONToken_t* token = 0;

        token       = dict_get(set_json, "descriptors");
        descriptors = JSON_VALUE(token, JSONarray);

        token       = dict_get(set_json, "name");
        set_name    = JSON_VALUE(token, JSONstring);
    }

    // Construct a set
    {

        // Count up descriptors
        while (descriptors[++descriptor_count]);

        // Copy the name
        {
            // Initialized data
            size_t name_len = strlen(set_name);

            // Allocate memory for the name
            set->name = calloc(name_len + 1, sizeof(char));

            // Copy the name
            strncpy(set->name, set_name, name_len);
        }

        // Set the binding count
        set->len = descriptor_count;

        // Construct a dictionary for the uniforms
        dict_construct(&set->descriptors, descriptor_count);

        // Construct each descriptor
        {

            // Iterate over each binding slot
            for (size_t j = 0; j < descriptor_count; j++)
            {

                // Initialized data
                char           *name                       = 0,
                               *descriptor_object_text     = descriptors[j];
                size_t          descriptor_object_text_len = strlen(descriptors[j]),
                                descriptor_index           = j;
                dict           *descriptor_json            = 0;

                GXDescriptor_t *desc                       = 0;

                // Parse the JSON 
                parse_json(descriptor_object_text, descriptor_object_text_len, &descriptor_json);

                // Construct the descriptor from the dictionary
                construct_descriptor(&desc, descriptor_json);

                desc->index = j;

                // Add the descriptor to the set
                dict_add(set->descriptors, desc->name, desc);
            }

        }
    }
    return 0;
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
                  *push_constant             = 0;
    size_t         set_count                 = 0;

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
        push_constant        = JSON_VALUE(token, JSONobject);
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
            VkGraphicsPipelineCreateInfo           *graphics_pipeline_create_info       = calloc(1, sizeof(VkGraphicsPipelineCreateInfo));
            VkVertexInputBindingDescription        *binding_description                 = calloc(1, sizeof(VkVertexInputBindingDescription));
            VkViewport                             *viewport                            = calloc(1, sizeof(VkViewport));
            VkRect2D                               *scissor                             = calloc(1, sizeof(VkRect2D));
            VkVertexInputAttributeDescription      *vertex_input_attribute_descriptions = 0;
            
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
                rasterizer_create_info->frontFace               = VK_FRONT_FACE_CLOCKWISE;
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

            // Set up the dynamic state
            {
                
                dynamic_states[0] = VK_DYNAMIC_STATE_VIEWPORT;
                dynamic_states[1] = VK_DYNAMIC_STATE_SCISSOR;

                dynamic_state_create_info->sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamic_state_create_info->dynamicStateCount = 2;
                dynamic_state_create_info->pDynamicStates    = dynamic_states;
            }

            // Set up the pipeline layout
            {

                // Initialized data
                VkDescriptorSetLayoutCreateInfo* descriptor_set_layout = 0;
                size_t                           set_count             = 0;

                // Construct set layouts
                if (sets)
                {
                    // Count up sets
                    while (sets[++set_count]);

                    // Construct a dictionary for shader sets
                    dict_construct(&i_shader->sets, set_count);

                    // Allocate memory for descriptor sets
                    descriptor_set_layout = calloc(set_count, sizeof(VkDescriptorSetLayoutCreateInfo));

                    // Iterate over each set
                    for (size_t i = 0; i < set_count; i++)
                    {
                        // Initialized data
                        GXSet_t *set      = 0;
                        dict    *set_json = 0;
                        size_t   set_len  = strlen(sets[i]);

                        // Parse the set JSON object into a dictionary
                        parse_json(sets[i], set_len, &set_json);

                        // Construct a set
                        construct_set(&set, set_json);

                        // Add the set to the dictionary
                        dict_add(i_shader->sets, set->name, set);

                    }
                }

                // Populate the pipeline layout create info struct
                {
                    pipeline_layout_create_info->sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                    pipeline_layout_create_info->setLayoutCount = set_count;
                    pipeline_layout_create_info->pSetLayouts    = descriptor_set_layout;
                }

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
    }

    // Cache the shader
    g_cache_shader(instance, i_shader);

    return 1;

    // TODO: Error handling
        
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

int set_shader_camera            ( GXShader_t   *shader )
{
    GXInstance_t   *instance   = g_get_active_instance();
    GXCamera_t     *camera     = instance->active_scene->active_camera;

    GXSet_t        *set        = dict_get(shader->sets, "camera");
    GXDescriptor_t *descriptor = dict_get(set->descriptors, "camera data");


    // TODO: Set shader data
    {
        mat4 *v       = &camera->view,
             *p       = &camera->projection;
        vec3 *cam_pos = &camera->location;
        // TODO: Set V
        // TODO: Set P
        // TODO: Set cam pos

    }

    return 0;
}

int set_shader_camera_parameters ( GXShader_t   *shader )
{
    GXInstance_t   *instance   = g_get_active_instance();
    GXCamera_t     *camera     = instance->active_scene->active_camera;

    GXSet_t        *set        = dict_get(shader->sets, "camera");
    GXDescriptor_t *descriptor = dict_get(set->descriptors, "camera parameters");

    // TODO: Set shader data
    {
        vec4  *lightDir;
        float *exposure;
        float *gamma;
        float *prefilteredCubeMipLevels;
        float *scaleIBLAmbient;
        float *debugViewInputs;
        float *debugViewEquation;
    }

    return 0;
}

int set_shader_skybox            ( GXShader_t   *shader )
{
    GXInstance_t   *instance   = g_get_active_instance();
    //GXSkybox_t     *skybox     = instance->active_scene->skybox;

    GXSet_t        *set        = dict_get(shader->sets, "camera");
    GXDescriptor_t *descriptor = dict_get(set->descriptors, "camera parameters");

    // TODO: Set shader data
    {
        // VkImage* skybox->
    }

    return 0;
}

int destroy_shader               ( GXShader_t   *shader )
{
    GXInstance_t* instance = g_get_active_instance();

    vkDestroyPipeline(instance->device, shader->pipeline, 0);
    vkDestroyPipelineLayout(instance->device, shader->pipeline_layout, 0);

    vkDestroyShaderModule(instance->device, shader->vertex_shader_module, 0);
    vkDestroyShaderModule(instance->device, shader->fragment_shader_module, 0);

    //free(shader->name);
    free(shader);
    return 0;
}
