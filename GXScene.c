#include <G10/GXScene.h>

int create_scene(GXScene_t** scene)
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( scene == (void *) 0 )
				goto no_scene;
		#endif
	}

	// Initialized data
	GXScene_t *ret = calloc(1, sizeof(GXScene_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void*)0)
				goto no_mem;
		#endif

	}

	*scene = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("Null pointer provided for \"scene\" in call to function \"%s\"", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_scene(GXScene_t** scene, const char path[])
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( scene == (void *) 0 )
				goto no_scene;
		#endif
	}

	// Initialized data
	char      *token_text     = 0;
	size_t     token_text_len = 0;

	// Load the file from the path
	{
		token_text_len = g_load_file(path, 0, false);
		token_text     = calloc(token_text_len + 1, sizeof(char));

		// Error checking
		{
			#ifndef NDEBUG
				if(token_text == (void*)0)
					goto no_mem;
			#endif
		}
		
		g_load_file(path, token_text, false);
	}

	// Load the scene as a JSON token
	load_scene_as_json_n(scene, token_text, token_text_len); 

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("Null pointer provided for \"scene\" in call to function \"%s\"", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_scene_as_json(GXScene_t** scene, char* token_text)
{
	// Argument checking
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (token_text == (void *)0)
				goto no_token_text;
		#endif
	}

	// Initialized data
	size_t token_text_len = strlen(token_text);
	
	// Load the scene
	load_scene_as_json_n(scene, token_text, token_text_len);

	return 1;

	// Error handling
	{

		// Argument checking
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"no_token_text\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int entity_thread()
{
	size_t j = 0;
	for (size_t i = 0; i < 10000000000; i++)
		j++;
	
	printf("[DONE]\n");
	fflush(stdout);

	return 1;
}

int load_scene_as_json_n(GXScene_t** scene, char* token_text, size_t len)
{
	// Argument checking
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (token_text == (void *)0)
				goto no_token_text;
			if (len == 0)
				goto no_len;
		#endif
	}

	// Initialized data
	size_t        token_text_len    = strlen(token_text);
	GXScene_t    *i_scene           = 0;
	dict         *scene_json_object = 0;

	// JSON array of object text
	char        **entities          = 0,
		        **cameras           = 0,
		        **lights            = 0,
		        **skyboxes          = 0,
		        **light_probes      = 0;

	// Allocate a scene
	create_scene(scene);

	// Get an internal pointer to the scene
	i_scene = *scene;

	// Parse the JSON
	{

		// Initialized data
		JSONToken_t* token = 0;

		// Parse the JSON text into a dictionary
		parse_json(token_text, len, &scene_json_object);

		// Set the name
		token           = dict_get(scene_json_object, "name");
		i_scene->name   = JSON_VALUE(token, JSONstring);

		// Get array of entity objects and paths
		token           = dict_get(scene_json_object, "entities");
		entities        = JSON_VALUE(token, JSONarray);

		// Get an array of camera objects and paths
		token           = dict_get(scene_json_object, "cameras");
		cameras         = JSON_VALUE(token, JSONarray);

		// Get an array of light objects and paths
		token           = dict_get(scene_json_object, "lights");
		lights          = JSON_VALUE(token, JSONarray);

		// Get a skybox for the scene
		token           = dict_get(scene_json_object, "skybox");
		//i_scene->skybox = 0;//TODO: 
	}

	// Load entities
	if(entities) 
	{
		GXInstance_t *instance             = g_get_active_instance();
		char         *entity_text          = entities[0];
		GXEntity_t   *entity               = 0;
		size_t        loading_thread_count = instance->loading_thread_count;
		SDL_Thread  **threads              = 0;
		
		// TODO: Thread
		threads = calloc(loading_thread_count + 1, sizeof(void*));

		// TODO: Spawn some worker threads to load entities
		for (size_t i = 0; i < loading_thread_count; i++)
		{
			//threads[i] = SDL_CreateThread(entity_thread, "", (void *)0);
		}

		for (size_t i = 0; i < loading_thread_count; i++)
		{
			int ret = 0;
			SDL_WaitThread(threads[i], &ret);
		}

		//{
		//	// Differentiate between objects and paths
		//	if (*entity_text == '{')
		//		load_entity_as_json(&entity, entity_text);
		//	else
		//		load_entity(&entity, entity_text);
		//}

		//// TODO: Atomize

		//// Add the entity to the dictionary
		//dict_add(i_scene->entities, entity->name, entity);

	}	

	// Load cameras
	if(cameras) {
		
	}

	// Load Lights
	if(lights) {

	}

	// Load Light Probes
	if(light_probes) {

	}

	return 1;

	// Error handling
	{

		// Argument checking
		{
			no_scene:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"no_token_text\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_len:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"len\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

		}
	}
}

int append_entity(GXScene_t* scene, GXEntity_t* entity)
{

	// Argument check
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (scene->entities == (void*)0)
				goto no_entities;
			if (entity == (void *)0)
				goto no_entity;
		#endif
		if (entity->name == 0)
			goto no_name;
	}

	// Add the entity to the scene
	dict_add(scene->entities, entity->name, entity);

	return 1;

	// Error handling
	{

		// Argument check
		{
			no_scene:
				#ifndef NDEBUG 
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_entities:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] No entity dictionary in scene in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_name:
				g_print_error("[G10] [Scene] \"entity\" has no name in call to function \"%s\"\n", __FUNCSIG__);
				return 0;
		}
	}

}

int append_camera(GXScene_t* scene, GXCamera_t* camera)
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (scene->cameras == (void*)0)
				goto no_cameras;
			if (camera == (void *)0)
				goto no_camera;
		#endif
		if (camera->name == 0)
			goto no_name;
	}

	// Add the camera to the scene
	dict_add(scene->cameras, camera->name, camera);

	return 1;

	// Error handling
	{

		// Argument check
		{
			no_scene:
				#ifndef NDEBUG 
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_cameras:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] No camera dictionary in scene, in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"camera\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_name:
					g_print_error("[G10] [Scene] \"camera\" has no name in call to function \"%s\"\n", __FUNCSIG__);
				return 0;
		}
	}
}

int append_light(GXScene_t* scene, GXLight_t* light)
{
	// Argument check
	{
		#ifndef NDEBUG
			if (scene == (void *)0)
				goto no_scene;
			if (scene->lights == (void*)0)
				goto no_lights;
			if (light == (void *)0)
				goto no_light;
		#endif
		if (light->name == 0)
			goto no_name;
	}

	// Add the entity to the scene
	dict_add(scene->lights, light->name, light);

	return 1;

	// Error handling
	{

		// Argument check
		{
			no_scene:
				#ifndef NDEBUG 
					g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_lights:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] No light dictionary in scene, in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_light:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Null pointer provided for \"light\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_name:
				g_print_error("[G10] [Scene] \"light\" has no name in call to function \"%s\"\n", __FUNCSIG__);
				return 0;
		}
	}
}

/*


	if (vkEndCommandBuffer(instance->command_buffer) != VK_SUCCESS) {
		g_print_error("failed to 
		rd command buffer!\n");
	}

	uint32_t imageIndex;
	vkAcquireNextImageKHR(instance->device, instance->swap_chain, UINT64_MAX, instance->image_available_semaphores, VK_NULL_HANDLE, &imageIndex);
	vkResetCommandBuffer(instance->command_buffer, 0);

	begin_info->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info->flags = 0;
	begin_info->pInheritanceInfo = 0;

	if (vkBeginCommandBuffer(instance->command_buffer, begin_info) != VK_SUCCESS) {
		g_print_error("Failed to begin recording command buffer!\n");
	}

	render_pass_create_info->sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	render_pass_create_info->renderPass = instance->render_pass;
	render_pass_create_info->framebuffer = instance->swap_chain_framebuffers[0];
	render_pass_create_info->renderArea.offset.x = 0;
	render_pass_create_info->renderArea.offset.y = 0;
	render_pass_create_info->renderArea.extent = instance->swap_chain_extent;

	render_pass_create_info->clearValueCount = 1;
	render_pass_create_info->pClearValues = &clear_color;

	vkCmdBeginRenderPass(instance->command_buffer, render_pass_create_info, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(instance->command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->pipeline);


	viewport->x = 0.0f;
	viewport->y = 0.0f;
	viewport->width = (float)instance->swap_chain_extent.width;
	viewport->height = (float)instance->swap_chain_extent.height;
	viewport->minDepth = 0.0f;
	viewport->maxDepth = 1.0f;
	vkCmdSetViewport(instance->command_buffer, 0, 1, viewport);

	scissor->offset.x = 0;
	scissor->offset.y = 0;

	scissor->extent = instance->swap_chain_extent;
	vkCmdSetScissor(instance->command_buffer, 0, 1, scissor);
	vkCmdDraw(instance->command_buffer, 3, 1, 0, 0);
	vkCmdEndRenderPass(instance->command_buffer);

*/

int draw_scene(GXScene_t* scene, GXShader_t *shader)
{

	// Uninitialized data
	u32                       image_index;

	// Initialized data
	GXInstance_t             *instance                = g_get_active_instance();
	VkCommandBufferBeginInfo *begin_info              = calloc(1, sizeof(VkCommandBufferBeginInfo));
	VkRenderPassBeginInfo    *render_pass_begin_info  = calloc(1, sizeof(VkRenderPassBeginInfo));
	VkClearValue              clear_color             = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	VkViewport               *viewport                = calloc(1, sizeof(VkViewport));
	VkSemaphore               wait_semaphores[]       = { instance->image_available_semaphores };
	VkSemaphore               signal_semaphores[]     = { instance->render_finished_semaphores };
	VkPipelineStageFlags      wait_stages[]           = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkRect2D                 *scissor                 = calloc(1, sizeof(VkRect2D));
	VkPresentInfoKHR         *present_info            = calloc(1, sizeof(VkPresentInfoKHR));
	VkSubmitInfo             *submit_info             = calloc(1, sizeof(VkSubmitInfo));
	VkSwapchainKHR            swap_chains[]           = { instance->swap_chain };


	vkWaitForFences(instance->device, 1, &instance->in_flight_fences, VK_TRUE, UINT64_MAX);
	vkResetFences(instance->device, 1, &instance->in_flight_fences);
	vkAcquireNextImageKHR(instance->device, instance->swap_chain, UINT64_MAX, instance->image_available_semaphores, VK_NULL_HANDLE, &image_index);
	 
	vkResetCommandBuffer(instance->command_buffer, 0);
	
	//record_command_buffer(instance->command_buffer, image_index);

	{


		begin_info->sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		vkBeginCommandBuffer(instance->command_buffer, begin_info);

		render_pass_begin_info->sType               = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		render_pass_begin_info->renderPass          = instance->render_pass;
		render_pass_begin_info->framebuffer         = instance->swap_chain_framebuffers[image_index];
		render_pass_begin_info->renderArea.offset.x = 0;
		render_pass_begin_info->renderArea.offset.y = 0;
		render_pass_begin_info->renderArea.extent   = instance->swap_chain_extent;
			

		render_pass_begin_info->clearValueCount = 1;
		render_pass_begin_info->pClearValues = &clear_color;

		vkCmdBeginRenderPass(instance->command_buffer, render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(instance->command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shader->pipeline);

		viewport->x = 0.f;
		viewport->y = 0.f;
		viewport->width = instance->swap_chain_extent.width;
		viewport->height = instance->swap_chain_extent.height;
		viewport->minDepth = 0.f;
		viewport->maxDepth = 1.f;

		vkCmdSetViewport(instance->command_buffer, 0, 1, viewport);

		scissor->offset.x = 0;
		scissor->offset.y = 0;

		scissor->extent = instance->swap_chain_extent;


		vkCmdSetScissor(instance->command_buffer, 0, 1, scissor);

		vkCmdDraw(instance->command_buffer, 3, 1, 0, 0);

		vkCmdEndRenderPass(instance->command_buffer);

		vkEndCommandBuffer(instance->command_buffer);
	}


	submit_info->sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info->waitSemaphoreCount = 1;
	submit_info->pWaitSemaphores   = &wait_semaphores;
	submit_info->pWaitDstStageMask = &wait_stages;
	submit_info->commandBufferCount = 1;
	submit_info->pCommandBuffers = &instance->command_buffer;
	submit_info->signalSemaphoreCount = 1;
	submit_info->pSignalSemaphores = &signal_semaphores;

	if (vkQueueSubmit(instance->graphics_queue, 1, submit_info, instance->in_flight_fences))
		g_print_error("Failed to submit draw command buffer!\n");

	present_info->sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info->waitSemaphoreCount = 1;
	present_info->pWaitSemaphores = signal_semaphores;
	present_info->swapchainCount = 1;
	present_info->pSwapchains = swap_chains;
	present_info->pImageIndices = &image_index;

	vkQueuePresentKHR(instance->present_queue, present_info);
	
	free(begin_info);
	free(render_pass_begin_info);
	free(viewport);
	free(scissor);
	free(present_info);
	free(submit_info);

	return 0;
}

GXEntity_t* get_entity(GXScene_t* scene, const char name[])
{

	// Argument check
	{
		#ifndef NDEBUG
			if(scene==(void*)0)
				goto no_scene;
			if (name == (void*)0)
				goto no_name;
		#endif
	}

	return dict_get(scene->entities, name);

	// Error handling
	{
		no_scene:
			#ifndef NDEBUG
				g_print_error("[G10] [Scene] Null pointer provided for \"scene\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		no_name:
			#ifndef NDEBUG
				g_print_error("[G10] [Scene] Null pointer provided for \"name       \" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
	}
}
