#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <dict/dict.h>
#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

struct GXTexture_s
{
	char           *name;
	VkImage         texture_image;
	VkDeviceMemory  texture_image_memory;
	VkImageView     texture_image_view;
};

// Allocator
DLLEXPORT int create_texture    ( GXTexture_t **texture );

// Constructor
DLLEXPORT int load_texture      ( GXTexture_t **texture, char *path );
