#include <G10/GXShader.h>

int create_shader_module(char* code, size_t code_len, VkShaderModule* shader_module)
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

int create_shader(GXShader_t** shader)
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

int load_shader(GXShader_t** shader, const char* path)
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

int load_shader_as_json(GXShader_t** shader, char* token_text, size_t token_text_len)
{
    // TODO: Argument check
    GXInstance_t *instance                  = g_get_active_instance();
    GXShader_t   *i_shader                  = 0;
    char         *name                      = 0,
                 *vertex_shader_path        = 0,
                 *fragment_shader_path      = 0;
    dict         *json_data                 = 0,
                 *vertex_shader_json_data   = 0,
                 *fragment_shader_json_data = 0;
    JSONToken_t  *token                     = 0;

    // Allocate for a shader
    create_shader(shader);

    // Get the pointer to the shader memory
    i_shader = *shader;

    // Parse JSON data
    {

        parse_json(token_text, token_text_len, &json_data);

        token                     = dict_get(json_data, "name");
        name                      = JSON_VALUE(token, JSONstring);

        token                     = dict_get(json_data, "vertex shader");
        vertex_shader_json_data   = JSON_VALUE(token, JSONobject);

        token                     = dict_get(json_data, "fragment shader");
        fragment_shader_json_data = JSON_VALUE(token, JSONobject);
    }

    // Construct the shader
    {

        // Set the name
        {
            size_t name_len = strlen(name);

            i_shader->name = calloc(name_len + 1, sizeof(char));

            // TODO: Check memory

            strncpy(i_shader->name, name, name_len);
        }

        // Load the vertex shader
        {
            // Parse the vertex shader JSON object
            parse_json(vertex_shader_json_data, strlen(vertex_shader_json_data), &vertex_shader_json_data);

            // Load the vertex shader binary
            {

                // Initialized data
                char   *vertex_shader_data     = 0;
                size_t  vertex_shader_data_len = 0;

                // Get the vertex shader path
                token              = dict_get(vertex_shader_json_data, "path");
                vertex_shader_path = JSON_VALUE(token, JSONstring);

                // TODO: Check for a valid path

                // Load the vertex shader
                {
                    vertex_shader_data_len = g_load_file(vertex_shader_path, 0, true);
                    vertex_shader_data     = calloc(vertex_shader_data_len + 1, sizeof(char));

                    // TODO: Check memory

                    g_load_file(vertex_shader_path, vertex_shader_data, true);
                }

                // Create a shader module
                create_shader_module(vertex_shader_data, vertex_shader_data_len, &i_shader->vertex_shader_module);
            }
        }

        // Load the fragment shader
        {

            // Parse the fragment shader JSON object
            parse_json(fragment_shader_json_data, strlen(fragment_shader_json_data), &fragment_shader_json_data);

            // Load the fragment shader binary
            {
                char* fragment_shader_data = 0;
                size_t fragment_shader_data_len = 0;

                token = dict_get(fragment_shader_json_data, "path");
                fragment_shader_path = JSON_VALUE(token, JSONstring);

                // TODO: Check for a valid path

                // Load the fragment shader
                {
                    fragment_shader_data_len = g_load_file(fragment_shader_path, 0, true);
                    fragment_shader_data     = calloc(fragment_shader_data_len + 1, sizeof(char));

                    // TODO: Check memory

                    g_load_file(fragment_shader_path, fragment_shader_data, true);
                }

                create_shader_module(fragment_shader_data, fragment_shader_data_len, &i_shader->fragment_shader_module);

            }



        }

        // Set up the shader pipeline
        {

            // Uninitialized data
            VkPipelineLayout                        pipeline_layout;

            // Initialized data
            VkPipelineShaderStageCreateInfo        *shader_stages                      = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
            VkPipelineVertexInputStateCreateInfo   *vertex_input_info_create_info      = calloc(1, sizeof(VkPipelineVertexInputStateCreateInfo));
            VkPipelineInputAssemblyStateCreateInfo *input_assembly_create_info         = calloc(1, sizeof(VkPipelineInputAssemblyStateCreateInfo));
            VkPipelineViewportStateCreateInfo      *viewport_state_create_info         = calloc(1, sizeof(VkPipelineViewportStateCreateInfo));
            VkPipelineRasterizationStateCreateInfo *rasterizer_create_info             = calloc(1, sizeof(VkPipelineRasterizationStateCreateInfo));
            VkPipelineMultisampleStateCreateInfo   *multisampling_create_info          = calloc(1, sizeof(VkPipelineMultisampleStateCreateInfo));
            VkPipelineColorBlendAttachmentState    *color_blend_attachment_create_info = calloc(1, sizeof(VkPipelineColorBlendAttachmentState));
            VkPipelineColorBlendStateCreateInfo    *color_blend_create_info            = calloc(1, sizeof(VkPipelineColorBlendStateCreateInfo));
            VkPipelineDynamicStateCreateInfo       *dynamic_state_create_info          = calloc(1, sizeof(VkPipelineDynamicStateCreateInfo));
            VkPipelineLayoutCreateInfo             *pipeline_layout_create_info        = calloc(1, sizeof(VkPipelineLayoutCreateInfo));
            VkDynamicState                         *dynamic_states                     = calloc(2, sizeof(VkPipelineDynamicStateCreateInfo));
            VkGraphicsPipelineCreateInfo           *graphics_pipeline_create_info      = calloc(1, sizeof(VkGraphicsPipelineCreateInfo));
            
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
                vertex_input_info_create_info->sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                vertex_input_info_create_info->vertexBindingDescriptionCount   = 0;
                vertex_input_info_create_info->pVertexBindingDescriptions      = 0;
                vertex_input_info_create_info->vertexAttributeDescriptionCount = 0;
                vertex_input_info_create_info->pVertexAttributeDescriptions    = 0;
            }

            // Set up the input assembly
            {
                input_assembly_create_info->sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                input_assembly_create_info->topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                input_assembly_create_info->primitiveRestartEnable = VK_FALSE;

            }

            // Set up the viewport state
            {

                // Initialized data
                VkViewport *viewport = calloc(1,sizeof(VkViewport));
                VkRect2D   *scissor  = calloc(1, sizeof(VkRect2D));

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
                color_blend_create_info->sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                color_blend_create_info->logicOpEnable = VK_FALSE;
                color_blend_create_info->logicOp = VK_LOGIC_OP_COPY;
                color_blend_create_info->attachmentCount = 1;
                color_blend_create_info->pAttachments = color_blend_attachment_create_info;
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
                pipeline_layout_create_info->sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                
                // TODO: Fill out?

            }

            // Construct the pipeline
            if (vkCreatePipelineLayout(instance->device, pipeline_layout_create_info, 0, &pipeline_layout) != VK_SUCCESS)
            {
                g_print_error("[G10] [Shader] Failed to create vulkan pipeline");
            }

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
                graphics_pipeline_create_info->layout              = pipeline_layout;
                graphics_pipeline_create_info->renderPass          = instance->render_pass;
                graphics_pipeline_create_info->basePipelineIndex   = -1;

            }

            if (vkCreateGraphicsPipelines(instance->device, VK_NULL_HANDLE, 1, graphics_pipeline_create_info, 0, &i_shader->pipeline) != VK_SUCCESS) {
                g_print_error("failed to create graphics pipeline!\n");
            }

        }
    }

    return 1;

    // TODO: Error handling
}