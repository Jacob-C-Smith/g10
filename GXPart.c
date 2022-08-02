#include <G10/GXPart.h>

int create_part ( GXPart_t **part )
{
	GXPart_t *i_part = calloc(1, sizeof(GXPart_t));

	*part = i_part;

	return 1;

}

int load_part(GXPart_t** part, char* path)
{
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len +1, sizeof(char));

	g_load_file(path, token_text, false);

	load_part_as_json(part, token_text, len);

	return 1;
}

int load_part_as_json(GXPart_t** part, char* token_text, size_t len)
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

int draw_part(GXPart_t* part)
{

	GXInstance_t *instance = g_get_active_instance();

	VkBuffer vertex_buffers[] = { part->vertex_buffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(instance->command_buffers[instance->current_frame], 0, 1, vertex_buffers, offsets);
	vkCmdBindIndexBuffer(instance->command_buffers[instance->current_frame], part->element_buffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdDrawIndexed(instance->command_buffers[instance->current_frame], (u32)part->index_count*3, 1, 0, 0, 0);
	return 0;
}


int destroy_part(GXPart_t* part)
{
	
	// Initialized data
	GXInstance_t *instance = g_get_active_instance;
	


	// Remove the part from the cache
	if (dict_get(instance->cached_parts, part->name) == part)
	{
		if (--part->users > 1)
			dict_pop(instance->cached_parts, part->name, 0);
	}
	else
		return 0;

	// Free the name
	free(part->name);

	// Free vulkan resources
	{

		// Free the vertex buffer
		vkDestroyBuffer(instance->device, part->vertex_buffer, 0);
		vkFreeMemory(instance->device, part->vertex_buffer_memory, 0);

		// Free the index buffer
		vkDestroyBuffer(instance->device, part->element_buffer, 0);
		vkFreeMemory(instance->device, part->element_buffer_memory, 0);
	}

	// Free the part itself
	free(part);

	return 0;
}