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
/* !
 *  Allocates memory for a part.
 *
 * @param pp_part : return
 *
 * @sa destroy_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_part       ( GXPart_t   **pp_part );

// Constructors
/* !
 *  Load a part from the filesystem
 *
 * @param pp_part : return
 * @param path    : The path to a part JSON file
 *
 * @sa load_part_as_json
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_part         ( GXPart_t   **pp_part, char *path );

/* !
 *  Load a part from a JSON object
 *
 * @param pp_part    : return
 * @param token_text : The part JSON object text
 * @param len        : The length of the JSON object text
 * 
 * @sa load_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_part_as_json ( GXPart_t   **pp_part, char *token_text, size_t len );

// Drawing

// TODO: Document
DLLEXPORT int draw_part          ( GXPart_t   *p_part );

// Info

// TODO: Document
DLLEXPORT int part_info          ( GXPart_t   *p_part );

// Destructors
/* !
 *  Free a part and all its contents
 *
 * @param p_part : Pointer to Part
 *
 * @sa create_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_part       ( GXPart_t   *p_part );