#pragma once

#include <SDL2/SDL.h>

#include <dict/dict.h>
#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

struct GXTexture_s
{
	char           *name;
	VkImage        *texture_image;
	VkDeviceMemory  texture_image_memory;

};

// Allocator
int create_texture    ( GXTexture_t **texture );

// Constructor
int construct_texture ( GXTexture_t **texture, char       *name, char *path_to_image );

int load_texture      ( GXTexture_t **texture, char       *path );

