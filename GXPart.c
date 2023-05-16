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
			if ( pp_part == (void*) 0 ) goto no_part;
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
			if (pp_part == (void *) 0 ) goto no_part;
			if (path    == (void *) 0 ) goto no_path;
		#endif
	}

	// Initialized data
	size_t  len  = g_load_file(path, 0, true);
	char   *text = calloc(len + 1, sizeof(char));

	// Error checking
	if ( text == (void *)0 )
		goto no_mem;

	// Load the file into the buffer
	if ( g_load_file(path, text, true) == 0 )
		goto failed_to_load_file;

	// Load the part as JSON text
	if ( load_part_as_json_text(pp_part, text) == 0 )
		goto failed_to_load_part;

	// Free the token text
	free(text);

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

int load_part_as_json_text ( GXPart_t **pp_part, char* text )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_part == (void *) 0 ) goto no_part;
			if ( text    == (void *) 0 ) goto no_text;
		#endif
	}

	// Initialized data
	GXInstance_t *p_instance  = g_get_active_instance();
	JSONValue_t  *p_value = 0;

	// Parse the JSON text into a JSON value
	if ( parse_json_value(text, 0, &p_value) == 0 )
		goto failed_to_parse_json;

	// Parse the JSON value into a part
	if ( load_part_as_json_value(pp_part, p_value) == 0 )
		goto failed_to_load_part;

	// Success
	return 1;

	// Error handling
	{

		failed_to_parse_json:
		failed_to_load_part:
			return 0;

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"pp_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_part_as_json_value ( GXPart_t **pp_part, JSONValue_t *p_value )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_part == (void *) 0 ) goto no_part;
			if ( p_value == (void *) 0 ) goto no_value;
		#endif
	}

	// Initialized data
	GXInstance_t *p_instance = g_get_active_instance();
	GXPart_t     *p_part     = 0;
	JSONValue_t  *name       = 0,
		         *path       = 0;

	// Parse the JSON as an object
	if ( p_value->type == JSONobject )
	{

		// Initialized data
		dict *part_json = p_value->object;

		name = dict_get(part_json, "name");
		path = dict_get(part_json, "path");

		// Check for required data
		if ( !(name && path) )
			goto not_enough_properties;
	}

	// Parse the JSON as a path
	else if ( p_value->type == JSONstring )
	{

		// Load a part from a JSON file
		if ( load_part(pp_part, p_value->string) == 0 )
			goto failed_to_load_part_from_file;

		// Success
		return 1;
	}

	// Check the cache
	{

		// Initialized data
		GXPart_t* p_cache_part = 0;

		// Lock the part cache mutex
		SDL_LockMutex(p_instance->mutexes.part_cache);

		// Search the cache for the part
		p_cache_part = g_find_part(p_instance, name->string);

		// If the part is in the cache ...
		if (p_cache_part)
		{

			// ... make a copy of the cached part
			//copy_part(pp_part, p_cache_part);

			// Write the return
			p_part = *pp_part;
			p_part->users++;

			// TODO: Fix
			return 1;
			//	goto set_initial_state;
		}

	}

	// Construct the part
	{

		// Allocate a part
		if ( create_part(pp_part) == 0 )
			goto failed_to_create_part;

		p_part = *pp_part;

		// Copy the name
		{

			// Initilaized data
			size_t name_len = strlen(name->string);

			// Allocate memory for the string
			p_part->name = calloc(name_len + 1, sizeof(char));

			// Error handling
			if ( p_part->name == (void *) 0 )
				goto no_mem;

			// Copy the string
			strncpy(p_part->name, name->string, name_len);
		}

		// Get a pointer to the PLY loader
		extern GXPart_t* load_ply(GXPart_t * part, const char* path);

		// Call the ply loader with the part
		load_ply(p_part, path->string);
	}

	// Success
	return 1;

	// Error handling
	{

		// TODO:
		failed_to_load_part_from_file:
		failed_to_create_part:
			return 0;

		// Argument errors
		{
			no_part:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"pp_part\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_value:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
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
			not_enough_properties:
				#ifndef NDEBUG
					g_print_error("[G10] [Part] Missing JSON properties to parse entity in call to function \"%s\". Consult gschema\n", __FUNCTION__);
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
		#ifndef NDEBUG
			if ( p_part == (void *) 0 ) goto no_part;
		#endif
	}

	// Initialized data
	GXInstance_t *p_instance         = g_get_active_instance();
	VkBuffer      vertex_buffers[] = { p_part->vertex_buffer };
	VkDeviceSize  offsets       [] = { 0 };

	// Bind the vertex buffers for the draw call
	vkCmdBindVertexBuffers(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], 0, 1, vertex_buffers, offsets);

	// Bind the face buffers for the draw call
	vkCmdBindIndexBuffer(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], p_part->element_buffer, 0, VK_INDEX_TYPE_UINT32);

	// Draw the part
	vkCmdDrawIndexed(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], (u32)p_part->index_count*3, 1, 0, 0, 0);

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
			if ( p_part == (void *) 0 ) goto no_part;
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

				// Error
				return 0;
		}
	}
}

int destroy_part ( GXPart_t **pp_part )
{

	// Argument chack
	{
		#ifndef NDEBUG
			if ( pp_part == (void *) 0 )
				goto no_part;
		#endif
	}
	
	// Initialized data
	GXInstance_t *p_instance = g_get_active_instance();
	GXPart_t     *p_part = *pp_part;

	*pp_part = 0;

	// Remove the part from the cache
	if ( (GXPart_t *) dict_get(p_instance->cache.parts, p_part->name) == p_part )
	{
		if (--p_part->users > 1)
			dict_pop(p_instance->cache.parts, p_part->name, 0);
	}
	else

		// Success
		return 1;

	// Free the name
	free(p_part->name);

	// Free vulkan resources
	{

		// Free the vertex buffer
		vkDestroyBuffer(p_instance->vulkan.device, p_part->vertex_buffer, 0);
		vkFreeMemory(p_instance->vulkan.device, p_part->vertex_buffer_memory, 0);

		// Free the index buffer
		vkDestroyBuffer(p_instance->vulkan.device, p_part->element_buffer, 0);
		vkFreeMemory(p_instance->vulkan.device, p_part->element_buffer_memory, 0);
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