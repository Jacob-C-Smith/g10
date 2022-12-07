#include <G10/GXRenderer.h>

// Constants
const unsigned       ATTACHMENT_LOAD_OPERATION_COUNT     = 2,
                     ATTACHMENT_STORE_OPERATION_COUNT    = 2,
                     IMAGE_LAYOUTS_COUNT                 = 18;

VkAttachmentLoadOp   attachment_load_operation_enums[ ]  = {
    VK_ATTACHMENT_LOAD_OP_LOAD,
    VK_ATTACHMENT_LOAD_OP_CLEAR,
};
char                *attachment_load_operation_names[ ]  = {
    "load",
    "clear",
};
dict                *attachment_load_operations          = 0;

VkAttachmentStoreOp  attachment_store_operation_enums[ ] = {
    VK_ATTACHMENT_STORE_OP_STORE,
    VK_ATTACHMENT_STORE_OP_NONE,
};
char                *attachment_store_operation_names[ ] = {
    "store",
    "none",
};
dict                *attachment_store_operations         = 0;

VkImageLayout        image_layout_enums[ ]               = {
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
char                *image_layout_names[ ]               = {
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
dict                *image_layouts                       = 0;

void          init_renderer       ( void )
{

    // Construct lookup tables
    dict_construct(&attachment_load_operations , ATTACHMENT_LOAD_OPERATION_COUNT);
    dict_construct(&attachment_store_operations, ATTACHMENT_STORE_OPERATION_COUNT);
    dict_construct(&image_layouts              , IMAGE_LAYOUTS_COUNT);

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
    }

    return;
}

int           create_renderer     ( GXRenderer_t **pp_renderer)
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

int           create__render_pass ( GXRenderPass_t** pp_render_pass)
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

int           create_subpass      ( GXSubpass_t **pp_subpass)
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

int           load_render_pass_as_json(GXRenderer_t** pp_render_pass, char* token_text, size_t len)
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
    VkAttachmentDescription *attachments             = 0;
    VkAttachmentReference   *attachment_references   = 0;
    VkSubpassDescription    *subpasses               = 0;
    VkRenderPassCreateInfo  *render_pass_create_info = calloc(1, sizeof(VkRenderPassCreateInfo));
    dict                    *render_pass_dict        = 0;
    char                    *name                    = 0,
                           **attachments_text        = 0,
                           **subpass_text;

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
               subpass_count    = 0;

        // Set the name
        {
            // TODO
        }

        // Populate the attachments
        {

            // Count up each attachment
            while (attachments_text[++attachment_count]);
            
            // Allocate memory for attachments and subpasses
            attachments           = calloc(attachment_count, sizeof(VkAttachmentDescription));
            subpasses             = calloc(attachment_count, sizeof(VkSubpassDescription));
            attachment_references = calloc(attachment_count, sizeof(VkAttachmentReference));

            // Iterate over each attachment JSON object text
            for (size_t i = 0; i < attachment_count; i++)
            {

                // Initialized data
                char   *attachment_text         = attachments_text[i];
                size_t  attachment_text_len     = strlen(attachment_text);
                dict   *attachment_dict         = 0;
                char   *samples                 = 0,
                       *load_operation          = 0,
                       *store_operation         = 0,
                       *stencil_load_operation  = 0,
                       *stencil_store_operation = 0,
                       *initial_layout          = 0,
                       *final_layout            = 0;

                // Parse the attachment JSON object into a dictioanry
                parse_json(attachment_text, attachment_text_len, &attachment_dict);

                // Get the required information to construct an attachment
                {

                    // Initialized data
                    JSONToken_t *token = 0;

                    // Get the samples for this attachment
                    token                   = dict_get(attachment_dict, "samples");
                    samples                 = JSON_VALUE(token, JSONprimative);

                    // Get the load operation
                    token                   = dict_get(attachment_dict, "load operation");
                    load_operation          = JSON_VALUE(token, JSONstring);

                    // Get the store operation
                    token                   = dict_get(attachment_dict, "store operation");
                    store_operation         = JSON_VALUE(token, JSONstring);

                    // Get the stencil load operation
                    token                   = dict_get(attachment_dict, "stencil load operation");
                    stencil_load_operation  = JSON_VALUE(token, JSONstring);
                    
                    // Get the stencil store operation
                    token                   = dict_get(attachment_dict, "stencil store operation");
                    stencil_store_operation = JSON_VALUE(token, JSONstring);

                    // Get the initial layout
                    token                   = dict_get(attachment_dict, "initial layout");
                    initial_layout          = JSON_VALUE(token, JSONstring);

                    // Get the final layout
                    token                   = dict_get(attachment_dict, "final layout");
                    final_layout            = JSON_VALUE(token, JSONstring);

                }

                // Error checking
                {
                    #ifndef NDEBUG
                        if (samples == (void *)0)
                            goto no_samples;
                        if (load_operation == (void*)0)
                            goto no_load_operation;
                        if (store_operation == (void*)0)
                            goto no_store_operation;
                    #endif

                }

                // Populate the attachment struct
                {

                    // Set the attachment format
                    attachments[i].format         = instance->swap_chain_image_format;

                    // Set the attachment samples
                    attachments[i].samples        = atoi(samples);

                    // Set the attachment load operation
                    attachments[i].loadOp         = (VkAttachmentLoadOp)  dict_get(attachment_load_operations, load_operation);

                    // Set the attachment store operation
                    attachments[i].storeOp        = (VkAttachmentStoreOp) dict_get(attachment_store_operations, store_operation);
                    
                    // Set the stencil load operation, if there is a stencil load operation; default to VK_ATTACHMENT_LOAD_OP_DONT_CARE
                    attachments[i].stencilLoadOp  = (stencil_load_operation)  ? (VkAttachmentLoadOp)  dict_get(attachment_load_operations, stencil_load_operation)   : VK_ATTACHMENT_LOAD_OP_DONT_CARE;

                    // Set the stencil store operation, if there is a stencil store operation; default to VK_ATTACHMENT_STORE_OP_DONT_CARE
                    attachments[i].stencilStoreOp = (stencil_store_operation) ? (VkAttachmentStoreOp) dict_get(attachment_store_operations, stencil_store_operation) : VK_ATTACHMENT_LOAD_OP_DONT_CARE;


                    attachments[i].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
                    attachments[i].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                }
            }   

            // Iterate over each subpass JSON object text
            for (size_t i = 0; i < subpass_count; i++)
            {
                // Populate the subpass struct
                {

                    // Initialized data
                    subpasses[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                    subpasses[i].inputAttachmentCount = 0;
                    subpasses[i].pInputAttachments = 0;
                    subpasses[i].colorAttachmentCount = 1;
                    subpasses[i].pColorAttachments = 0;
                    subpasses[i].pResolveAttachments = 0;
                    subpasses[i].pDepthStencilAttachment = 0;
                    subpasses[i].preserveAttachmentCount = 0;
                    subpasses[i].pPreserveAttachments = 0;

                    {

                    }
                }
            }
        }

        


    //    // The index of the attachment in this array is directly referenced from the fragment shader with the layout(location = 0) out vec4 outColor directive!
    //    {
    //        {
    //            color_attachment_reference->attachment = 0;
    //            color_attachment_reference->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    //        }

    //        subpass->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    //        subpass->colorAttachmentCount = 1;
    //        subpass->pColorAttachments = color_attachment_reference;
    //    }

    //    render_pass_create_info->sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    //    render_pass_create_info->attachmentCount = 1;
    //    render_pass_create_info->pAttachments = color_attachment;
    //    render_pass_create_info->subpassCount = 1;
    //    render_pass_create_info->pSubpasses = subpass;
    //}
    //if (vkCreateRenderPass(instance->device, render_pass_create_info, 0, &instance->render_pass) != VK_SUCCESS) {
    //    g_print_error("failed to create render pass!\n");
    //}

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

int           render_frame                 ( GXInstance_t        *instance )
{
    // Initialized data
	VkSemaphore               wait_semaphores[]       = { instance->image_available_semaphores[instance->current_frame]};
	VkSemaphore               signal_semaphores[]     = { instance->render_finished_semaphores[instance->current_frame]};
	VkPipelineStageFlags      wait_stages[]           = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkPresentInfoKHR         *present_info            = calloc(1, sizeof(VkPresentInfoKHR));
	VkSubmitInfo             *submit_info             = calloc(1, sizeof(VkSubmitInfo));
	VkSwapchainKHR            swap_chains[]           = { instance->swap_chain };
    VkResult                  result;
    u64                       start                   = 0,
                              end                     = 0;
    start = SDL_GetPerformanceCounter();


    // Get the command buffer ready for drawing
    {

        // Wait for the previous frame to finish rendering
	    vkWaitForFences(instance->device, 1, &instance->in_flight_fences[instance->current_frame], VK_TRUE, UINT64_MAX);
	
        // Grab an image from the swapchain
        result = vkAcquireNextImageKHR(instance->device, instance->swap_chain, UINT64_MAX, instance->image_available_semaphores[instance->current_frame], VK_NULL_HANDLE, &instance->image_index);
	
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
        vkResetFences(instance->device, 1, &instance->in_flight_fences[instance->current_frame]);

        // Clear out the command buffer
    	vkResetCommandBuffer(instance->command_buffers[instance->current_frame], 0);
	}

    // Draw the frame
    {
        if (false)
        {

            // Initialized data
            GXRenderer_t             *active_renderer   = instance->active_renderer;
            size_t                    render_pass_count = 0;
            VkCommandBufferBeginInfo *begin_info        = calloc(1, sizeof(VkCommandBufferBeginInfo));

            // Set up the command buffer begin info struct
            begin_info->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            // Begin the command buffer
            vkBeginCommandBuffer(instance->command_buffers[instance->current_frame], begin_info);

            // Count up each render pass
            // TODO: FIX
            //while((void *)active_renderer->render_passes[++render_pass_count]);

            // Iterate over each render pass
            for (size_t i = 0; i < render_pass_count; i++)
            {

                // Initialized data
                VkCommandBufferBeginInfo *begin_info = calloc(1, sizeof(VkCommandBufferBeginInfo));
                VkClearValue              clear_color = { {{1.f, 1.f, 1.f, 0.0f}} };

                VkRenderPassBeginInfo    *render_pass_begin_info = calloc(1, sizeof(VkRenderPassBeginInfo));
                size_t                    subpass_count = 0;

                // Start the render pass
                {
                    
                    // Set up the render pass
                    {
                        render_pass_begin_info->sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                        render_pass_begin_info->renderPass          = instance->render_pass;
                        render_pass_begin_info->framebuffer         = instance->swap_chain_framebuffers[instance->image_index];
                        render_pass_begin_info->renderArea.offset.x = 0;
                        render_pass_begin_info->renderArea.offset.y = 0;
                        render_pass_begin_info->renderArea.extent   = instance->swap_chain_extent;
                        render_pass_begin_info->clearValueCount     = 1;
                        render_pass_begin_info->pClearValues        = &clear_color;
                    }

                    // Start the render pass
                    vkCmdBeginRenderPass(instance->command_buffers[instance->current_frame], render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

                }

                // Iterate over each subpass
                for (size_t j = 0; j < subpass_count; j++)
                {
                    draw_scene(instance->active_scene);

                    vkCmdNextSubpass(instance->command_buffers[instance->current_frame], VK_SUBPASS_CONTENTS_INLINE);
                }

                // End the render pass
                {

                    // End the render pass
                    vkCmdEndRenderPass(instance->command_buffers[instance->current_frame]);

                    // End the command buffer
                    vkEndCommandBuffer(instance->command_buffers[instance->current_frame]);

                    // Clean up some memory
                    free(begin_info);
                    free(render_pass_begin_info);
                }

                return 0;
            }
        }

        // TODO: Make draw functions for each 
        // draw_scene_shadows(instance->active_scene)
        draw_scene(instance->active_scene);
        // draw_scene_bv()
        // draw_scene_composite(instance->active_scene)
        // draw_scene_bloom(instance->active_scene)
    }

    // Submit the commands and present the last frame
    {

        // Populate the submit info struct
        {
            submit_info->sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submit_info->waitSemaphoreCount   = 1;
            submit_info->pWaitSemaphores      = &wait_semaphores;
            submit_info->pWaitDstStageMask    = &wait_stages;
            submit_info->commandBufferCount   = 1;
            submit_info->pCommandBuffers      = &instance->command_buffers[instance->current_frame];
            submit_info->signalSemaphoreCount = 1;
            submit_info->pSignalSemaphores    = &signal_semaphores;
        }

        // Submit the draw commands
        if (vkQueueSubmit(instance->graphics_queue, 1, submit_info, instance->in_flight_fences[instance->current_frame]))
            g_print_error("Failed to submit draw command buffer!\n");

        // Populate the present info struct
        {
            present_info->sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            present_info->waitSemaphoreCount = 1;
            present_info->pWaitSemaphores    = &signal_semaphores;
            present_info->swapchainCount     = 1;
            present_info->pSwapchains        = swap_chains;
            present_info->pImageIndices      = &instance->image_index;
        }

        // Present the image to the swapchain
        result = vkQueuePresentKHR(instance->present_queue, present_info);

        // Does the window need to be resized?
        if      ( result == VK_ERROR_OUT_OF_DATE_KHR ||
                  result == VK_SUBOPTIMAL_KHR ) {
            g_window_resize(instance);
        }
        else if ( result != VK_SUCCESS ) {
            printf("failed to present swap chain image!");
        }
    }

    // Compute the current frame index
    instance->current_frame = (instance->current_frame + 1) % instance->max_buffered_frames;
    fail:

    // Deallocation
	free(present_info);
	free(submit_info);
    end = SDL_GetPerformanceCounter();
    instance->delta_time = (float)(((double)(end - start)) / (double)(instance->clock_div));

    instance->ticks += 1;

	return 0;
}

int           load_render_pass ( VkRenderPass** pp_render_pass, char* path)
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

//int           load_render_pass_as_json     ( VkRenderPass       **pp_render_pass, char *token_text, size_t len )
//{
//    
//    // Argument check
//    {
//        #ifndef NDEBUG
//            if (pp_render_pass == (void *) 0)
//                goto no_render_pass;
//            if (token_text == (void*)0)
//                goto no_token_text;
//        #endif
//
//        if (len == 0)
//            goto no_len;
//    }
//
//    // Initialized data
//    GXInstance_t            *instance                = g_get_active_instance();
//    VkAttachmentDescription *attachments             = 0;
//    VkAttachmentReference   *attachment_references   = 0;
//    VkSubpassDescription    *subpasses               = 0;
//    VkRenderPassCreateInfo  *render_pass_create_info = calloc(1, sizeof(VkRenderPassCreateInfo));
//    dict                    *render_pass_dict        = 0;
//    char                    *name                    = 0,
//                           **attachments_text        = 0,
//                           **subpass_text;
//
//    // Parse the render pass JSON into a dictionary
//    parse_json(token_text, len, &render_pass_dict);
//
//    // Parse the JSON dictionary
//    {
//
//        // Initialized data
//        JSONToken_t *token = 0;
//
//        // Get the name of the render pass
//        token            = dict_get(render_pass_dict, "name");
//        name             = JSON_VALUE(token, JSONstring);
//
//        // Get the attachments array
//        token            = dict_get(render_pass_dict, "attachments");
//        attachments_text = JSON_VALUE(token, JSONarray);
//    }
//
//    // Error checking
//    {
//        #ifndef NDEBUG 
//            if (name == (void *)0)
//                goto no_name;
//            if (attachments_text == (void *)0)
//                goto no_attachments;
//        #endif
//    }
//
//    // Construct the render pass
//    {
//
//        // Initialized data
//        size_t attachment_count = 0,
//               subpass_count    = 0;
//
//        // Set the name
//        {
//            // TODO
//        }
//
//        // Populate the attachments
//        {
//
//            // Count up each attachment
//            while (attachments_text[++attachment_count]);
//            
//            // Allocate memory for attachments and subpasses
//            attachments           = calloc(attachment_count, sizeof(VkAttachmentDescription));
//            subpasses             = calloc(attachment_count, sizeof(VkSubpassDescription));
//            attachment_references = calloc(attachment_count, sizeof(VkAttachmentReference));
//
//            // Iterate over each attachment JSON object text
//            for (size_t i = 0; i < attachment_count; i++)
//            {
//
//                // Initialized data
//                char   *attachment_text         = attachments_text[i];
//                size_t  attachment_text_len     = strlen(attachment_text);
//                dict   *attachment_dict         = 0;
//                char   *samples                 = 0,
//                       *load_operation          = 0,
//                       *store_operation         = 0,
//                       *stencil_load_operation  = 0,
//                       *stencil_store_operation = 0,
//                       *initial_layout          = 0,
//                       *final_layout            = 0;
//
//                // Parse the attachment JSON object into a dictioanry
//                parse_json(attachment_text, attachment_text_len, &attachment_dict);
//
//                // Get the required information to construct an attachment
//                {
//
//                    // Initialized data
//                    JSONToken_t *token = 0;
//
//                    // Get the samples for this attachment
//                    token                   = dict_get(attachment_dict, "samples");
//                    samples                 = JSON_VALUE(token, JSONprimative);
//
//                    // Get the load operation
//                    token                   = dict_get(attachment_dict, "load operation");
//                    load_operation          = JSON_VALUE(token, JSONstring);
//
//                    // Get the store operation
//                    token                   = dict_get(attachment_dict, "store operation");
//                    store_operation         = JSON_VALUE(token, JSONstring);
//
//                    // Get the stencil load operation
//                    token                   = dict_get(attachment_dict, "stencil load operation");
//                    stencil_load_operation  = JSON_VALUE(token, JSONstring);
//                    
//                    // Get the stencil store operation
//                    token                   = dict_get(attachment_dict, "stencil store operation");
//                    stencil_store_operation = JSON_VALUE(token, JSONstring);
//
//                    // Get the initial layout
//                    token                   = dict_get(attachment_dict, "initial layout");
//                    initial_layout          = JSON_VALUE(token, JSONstring);
//
//                    // Get the final layout
//                    token                   = dict_get(attachment_dict, "final layout");
//                    final_layout            = JSON_VALUE(token, JSONstring);
//
//                }
//
//                // Error checking
//                {
//                    #ifndef NDEBUG
//                        if (samples == (void *)0)
//                            goto no_samples;
//                        if (load_operation == (void*)0)
//                            goto no_load_operation;
//                        if (store_operation == (void*)0)
//                            goto no_store_operation;
//                    #endif
//
//                }
//
//                // Populate the attachment struct
//                {
//
//                    // Set the attachment format
//                    attachments[i].format         = instance->swap_chain_image_format;
//
//                    // Set the attachment samples
//                    attachments[i].samples        = atoi(samples);
//
//                    // Set the attachment load operation
//                    attachments[i].loadOp         = (VkAttachmentLoadOp)  dict_get(attachment_load_operations, load_operation);
//
//                    // Set the attachment store operation
//                    attachments[i].storeOp        = (VkAttachmentStoreOp) dict_get(attachment_store_operations, store_operation);
//                    
//                    // Set the stencil load operation, if there is a stencil load operation; default to VK_ATTACHMENT_LOAD_OP_DONT_CARE
//                    attachments[i].stencilLoadOp  = (stencil_load_operation)  ? (VkAttachmentLoadOp)  dict_get(attachment_load_operations, stencil_load_operation)   : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//
//                    // Set the stencil store operation, if there is a stencil store operation; default to VK_ATTACHMENT_STORE_OP_DONT_CARE
//                    attachments[i].stencilStoreOp = (stencil_store_operation) ? (VkAttachmentStoreOp) dict_get(attachment_store_operations, stencil_store_operation) : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//
//
//                    attachments[i].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
//                    attachments[i].finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
//
//                }
//            }   
//
//            // Iterate over each subpass JSON object text
//            for (size_t i = 0; i < subpass_count; i++)
//            {
//                // Populate the subpass struct
//                {
//
//                    // Initialized data
//                    subpasses[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//                    subpasses[i].inputAttachmentCount = 0;
//                    subpasses[i].pInputAttachments = 0;
//                    subpasses[i].colorAttachmentCount = 1;
//                    subpasses[i].pColorAttachments = 0;
//                    subpasses[i].pResolveAttachments = 0;
//                    subpasses[i].pDepthStencilAttachment = 0;
//                    subpasses[i].preserveAttachmentCount = 0;
//                    subpasses[i].pPreserveAttachments = 0;
//
//                    {
//
//                    }
//                }
//            }
//        }
//
//        
//
//
//    //    // The index of the attachment in this array is directly referenced from the fragment shader with the layout(location = 0) out vec4 outColor directive!
//    //    {
//    //        {
//    //            color_attachment_reference->attachment = 0;
//    //            color_attachment_reference->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
//    //        }
//
//    //        subpass->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//    //        subpass->colorAttachmentCount = 1;
//    //        subpass->pColorAttachments = color_attachment_reference;
//    //    }
//
//    //    render_pass_create_info->sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//    //    render_pass_create_info->attachmentCount = 1;
//    //    render_pass_create_info->pAttachments = color_attachment;
//    //    render_pass_create_info->subpassCount = 1;
//    //    render_pass_create_info->pSubpasses = subpass;
//    //}
//    //if (vkCreateRenderPass(instance->device, render_pass_create_info, 0, &instance->render_pass) != VK_SUCCESS) {
//    //    g_print_error("failed to create render pass!\n");
//    //}
//
//    }
//
//    return 1;
//
//    // Error handling
//    {
//
//        // Argument errors
//        {
//
//            no_render_pass:
//                #ifndef NDEBUG
//                    g_print_error("[G10] [Renderer] Null pointer provided for \"pp_render_pass\" in call to function \"%s\"\n", __FUNCSIG__);
//                #endif
//                return 0;
//            no_token_text:
//                #ifndef NDEBUG
//                    g_print_error("[G10] [Renderer] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCSIG__);
//                #endif
//                return 0;
//            no_len: 
//                #ifndef NDEBUG
//                    g_print_error("[G10] [Renderer] Parameter \"len\" was zero in call to function \"%s\"\n", __FUNCSIG__);
//                #endif
//                return 0;
//        }
//
//        // JSON errors
//        {
//            no_name:
//                #ifndef NDEBUG
//                    g_print_error("[G10] [Renderer] No \"name\" property in render pass JSON, in call to function \"%s\"\n", __FUNCSIG__);
//                #endif
//                return 0;
//
//            no_attachments:
//                #ifndef NDEBUG
//                    g_print_error("[G10] [Renderer] No \"attachments\" property in render pass JSON, in call to function \"%s\"\n", __FUNCSIG__);
//                #endif
//                return 0;
//            
//            // TODO: Categorize
//            no_samples:
//            no_load_operation:
//            no_store_operation:return 0;
//        }
//
//        // Vulkan errors
//        {
//
//            failed_to_create_render_pass:
//                #ifndef NDEBUG
//                    g_print_error("[G10] [Renderer] Failed to create render pass in call to function \"%s\"\n", __FUNCSIG__);
//                #endif
//                return 0;
//        }
//    }
//}