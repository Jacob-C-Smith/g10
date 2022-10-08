#include <G10/GXRenderer.h>

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
    u64 start = 0,
        end = 0;
    start = SDL_GetPerformanceCounter();


    // Get the command buffer ready for drawing
    {

        // Wait for the previous frame to finish rendering
	    vkWaitForFences(instance->device, 1, &instance->in_flight_fences[instance->current_frame], VK_TRUE, UINT64_MAX);
	
        // Grab an image from the swapchain
        result = vkAcquireNextImageKHR(instance->device, instance->swap_chain, UINT64_MAX, instance->image_available_semaphores[instance->current_frame], VK_NULL_HANDLE, &instance->image_index);
	
        // Make sure the image is usable
        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            g_window_resize(instance);
            goto fail;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
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
        // TODO: Make draw functions for each 
        // draw_scene_shadows(instance->active_scene)
        draw_scene(instance->active_scene);
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
        if      (    result == VK_ERROR_OUT_OF_DATE_KHR
                  || result == VK_SUBOPTIMAL_KHR ) {
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
