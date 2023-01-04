#include <G10/GXPart.h>

int create_part       ( GXPart_t **pp_part )
{

	// Initialized data
	{
		#ifndef NDEBUG
			if (pp_part == (void*)0)
				goto no_part;
		#endif
	}

	// Initialized data
	GXPart_t *i_part = calloc(1, sizeof(GXPart_t));

	// Error checking
	{
		#ifndef NDEBUG
			if (i_part == (void *)0)
				goto no_mem;
		#endif
	}

	// Write the return
	*pp_part = i_part;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for \"pp_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}

}

int load_part         ( GXPart_t **pp_part, char* path)
{

	// Argument error
	{
		#ifndef NDEBUG
			if (pp_part == (void *) 0 )
				goto no_part;
			if (path == (void *) 0 )
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len + 1, sizeof(char));

	// Error checking
	{
		#ifndef NDEBUG
			if (token_text == (void *)0)
				goto no_mem;
		#endif
	}

	// Load the file into the buffer
	if ( g_load_file(path, token_text, false) == 0 )
		goto failed_to_load_file;

	// Load the part as JSON text
	if ( load_part_as_json(pp_part, token_text, len) == 0 )
		goto failed_to_load_part;

	// Free the token text
	free(token_text);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for \"pp_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// G10 errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif
				return 0;

			failed_to_load_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Failed to load part in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;

		}
	}

}

int load_part_as_json ( GXPart_t **part, char* token_text, size_t len)
{
	GXInstance_t *instance  = g_get_active_instance();
	GXPart_t     *i_part    = 0; 

	char         *name      = 0,
		         *path      = 0;

	dict         *part_json = 0;

	// Allocate for a part
	create_part(part);

	// Get a pointer to the part
	i_part = *part;

	// Parse the JSON
	{

		JSONToken_t *token = 0;

		parse_json(token_text, len, &part_json);

		token = dict_get(part_json, "name");
		name  = JSON_VALUE(token, JSONstring);

		token = dict_get(part_json, "path");
		path  = JSON_VALUE(token, JSONstring);
	}

	// Check the cache 
	{

		// Initialized data
		GXPart_t *cached_part = g_find_part(instance, name);


		if (cached_part)
			*part = cached_part;
	}

	//
	{
		{
			size_t name_len = strlen(name);
			i_part->name = calloc(name_len + 1, sizeof(char));
			
			strncpy(i_part->name, name, name_len);
		}

		extern GXPart_t* load_ply(GXPart_t * part, const char* path);

		load_ply(i_part, path);
	}



	return 1;
}

int draw_part         ( GXPart_t  *part)
{

	GXInstance_t *instance = g_get_active_instance();

	VkBuffer vertex_buffers[] = { part->vertex_buffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(instance->vulkan.command_buffers[instance->vulkan.current_frame], 0, 1, vertex_buffers, offsets);
	vkCmdBindIndexBuffer(instance->vulkan.command_buffers[instance->vulkan.current_frame], part->element_buffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(instance->vulkan.command_buffers[instance->vulkan.current_frame], (u32)part->index_count*3, 1, 0, 0, 0);

	return 1;
}

int part_info         ( GXPart_t *p_part )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_part == (void *)0 )
				goto no_part;
		#endif
	}

	// Formatting 
    g_print_log(" - Part info - \n");
    
    // Print the name
    g_print_log("name            : \"%s\"\n", p_part->name);

	// Print the active material
	if (p_part->active_material)
		g_print_log("active material : \"%s\"\n", p_part->active_material->name);

	// Print the active shader
	if (p_part->active_shader)
		g_print_log("active shader   : \"%s\"\n", p_part->active_shader->name);

	// Print the vertex count
	if (p_part->vertex_count)
		g_print_log("vertices        : %lld\n", p_part->vertex_count);

	// Print the face count
	if (p_part->vertex_count)
		g_print_log("faces           : %lld\n", p_part->index_count);

	putchar('\n');

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for \"p_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int destroy_part      ( GXPart_t  *part)
{
	
	// Initialized data
	GXInstance_t *instance = g_get_active_instance();
	
	// Remove the part from the cache
	if (dict_get(instance->cache.parts, part->name) == part)
	{
		if (--part->users > 1)
			dict_pop(instance->cache.parts, part->name, 0);
	}
	else
		return 1;

	// Free the name
	free(part->name);

	// Free vulkan resources
	{

		// Free the vertex buffer
		vkDestroyBuffer(instance->vulkan.device, part->vertex_buffer, 0);
		vkFreeMemory(instance->vulkan.device, part->vertex_buffer_memory, 0);

		// Free the index buffer
		vkDestroyBuffer(instance->vulkan.device, part->element_buffer, 0);
		vkFreeMemory(instance->vulkan.device, part->element_buffer_memory, 0);
	}

	// Free the part itself
	free(part);

	return 1;
}