#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

#include <dict/dict.h>
#include <JSON/JSON.h>

#include <vulkan/vulkan.h>

struct GXPart_s
{
	char           *name;

	GXMaterial_t   *active_material;
	GXShader_t     *active_shader;


	VkBuffer        vertex_buffer,
	                element_buffer;
	VkDeviceMemory  vertex_buffer_memory,
	                element_buffer_memory;
	 
	size_t          vertex_count,
	                index_count,
		            users;
};

// Allocators
DLLEXPORT int create_part       ( GXPart_t **part );

// Constructors
DLLEXPORT int load_part         ( GXPart_t **part, char     *path );
DLLEXPORT int load_part_as_json ( GXPart_t **part, char     *token_text, size_t len );

// Drawing
DLLEXPORT int draw_part          ( GXPart_t   *part );

// Destructors
DLLEXPORT int destroy_part       ( GXPart_t   *part );