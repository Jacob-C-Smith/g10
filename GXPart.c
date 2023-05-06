#include <G10/GXPart.h>

void init_part ( void ) 
{
	// Initialized data
	GXInstance_t *p_instance = g_get_active_instance();

	// Create a material cache mutex
	p_instance->mutexes.part_cache = SDL_CreateMutex();
}

int create_part ( GXPart_t **pp_part )
{

	// Initialized data
	{
		#ifndef NDEBUG
			if (pp_part == (void*)0)
				goto no_part;
		#endif
	}

	// Initialized data
	GXPart_t *p_part = calloc(1, sizeof(GXPart_t));

	// Error checking
	if ( p_part == (void *) 0 )
		goto no_mem;

	// Return a pointer to the caller
	*pp_part = p_part;

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

int load_part ( GXPart_t **pp_part, char* path)
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
	if ( token_text == (void *)0 )
		goto no_mem;

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

				// Error
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
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

		// G10 errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Failed to load part in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}
	}

}

int load_part_as_json ( GXPart_t **pp_part, char* token_text, size_t len )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_part == (void *) 0 )
				goto no_part;
			if ( token_text == (void *) 0 ) 
				goto no_token_text;
			if ( len == 0 )
				goto no_len;
		#endif
	}

	// Initialized data
	GXInstance_t *instance  = g_get_active_instance();
	GXPart_t     *p_part    = 0; 
	char         *name      = 0,
		         *path      = 0;
	dict         *part_json = 0;

	// Allocate for a part
	create_part(pp_part);

	// Get a pointer to the part
	p_part = *pp_part;

	// Parse the JSON
	//parse_json(token_text, len, &part_json);

	// Parse the dictionary into constructor parameters
	{
		
		// Initiailzed data
		//JSONToken_t *token = 0;

 		//token = (JSONToken_t *) dict_get(part_json, "name");
		//name  = JSON_VALUE(token, JSONstring);

 		//token = (JSONToken_t *) dict_get(part_json, "path");
		//path  = JSON_VALUE(token, JSONstring);
	}

	// TODO: FIX CACHING. its busted. 

	// Check the cache 
	{

		// Initialized data
		//GXPart_t *cached_part = g_find_part(instance, name);
 		//// Is there a cached part?
		//if ( cached_part )
		//	
		//	// 
		//	*pp_part = cached_part;
	}

	// Construct the part
	{

		// // Copy the name
		// {
		// 	// Initilaized data
		// 	size_t name_len = strlen(name);
 
		// 	// Allocate memory for the string
		// 	p_part->name = calloc(name_len + 1, sizeof(char));
		// 	
		// 	// Error handling
		// 	{
		// 		#ifndef NDEBUG
		// 			if (p_part->name == (void *) 0 )
		// 				goto no_mem;
		// 		#endif
		// 	}
 
		// 	// Copy the string 
		// 	strncpy(p_part->name, name, name_len);
		// }

		// // Get a pointer to the PLY loader
		// extern GXPart_t* load_ply(GXPart_t * part, const char* path);

		// // Call the ply loader with the part
		// load_ply(p_part, path);
	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"pp_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif 

				// Error 
				return 0;

			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"token_text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif 

				// Error 
				return 0;

			no_len:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"len\" in call to function \"%s\"\n", __FUNCTION__);
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

int draw_part ( GXPart_t *p_part )
{

	// Argument check
	{
		if ( p_part == (void *) 0 )
			goto no_part;
	}

	// Initialized data
	GXInstance_t *instance         = g_get_active_instance();
	VkBuffer      vertex_buffers[] = { p_part->vertex_buffer };
	VkDeviceSize  offsets       [] = { 0 };

	// Bind the vertex buffers for the draw call
	vkCmdBindVertexBuffers(instance->vulkan.command_buffers[instance->vulkan.current_frame], 0, 1, vertex_buffers, offsets);

	// Bind the face buffers for the draw call
	vkCmdBindIndexBuffer(instance->vulkan.command_buffers[instance->vulkan.current_frame], p_part->element_buffer, 0, VK_INDEX_TYPE_UINT32);

	// Draw the part
	vkCmdDrawIndexed(instance->vulkan.command_buffers[instance->vulkan.current_frame], (u32)p_part->index_count*3, 1, 0, 0, 0);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"p_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int part_info ( GXPart_t *p_part )
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

int destroy_part ( GXPart_t *p_part ) 
{
	
	// Argument chack
	{
		#ifndef NDEBUG
			if ( p_part == (void *) 0 )
				goto no_part;
		#endif
	}

	// Initialized data
	GXInstance_t *instance = g_get_active_instance();
	
	// Remove the part from the cache
	if ( (GXPart_t *) dict_get(instance->cache.parts, p_part->name) == p_part )
	{
		if (--p_part->users > 1)
			dict_pop(instance->cache.parts, p_part->name, 0);
	}
	else

		// Success
		return 1;

	// Free the name
	free(p_part->name);

	// Free vulkan resources
	{

		// Free the vertex buffer
		vkDestroyBuffer(instance->vulkan.device, p_part->vertex_buffer, 0);
		vkFreeMemory(instance->vulkan.device, p_part->vertex_buffer_memory, 0);

		// Free the index buffer
		vkDestroyBuffer(instance->vulkan.device, p_part->element_buffer, 0);
		vkFreeMemory(instance->vulkan.device, p_part->element_buffer_memory, 0);
	}

	// Free the part itself
	free(p_part);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for \"p_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}