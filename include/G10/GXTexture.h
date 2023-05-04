#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <dict/dict.h>
#include <json/json.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

struct GXTexture_s
{
	char           *name;
	VkImage         texture_image;
	VkDeviceMemory  texture_image_memory;
	VkImageView     texture_image_view;
	VkSampler       texture_sampler;
};

// Allocator
/* ! 
 *   
 */
DLLEXPORT int create_texture       ( GXTexture_t **pp_texture );

// Constructors
DLLEXPORT int load_texture         ( GXTexture_t  **texture     , char                 *path );
DLLEXPORT int load_texture_as_json ( GXTexture_t  **texture     , char                 *token_text, size_t   len );

// Vulkan Constructors

DLLEXPORT int construct_image_view ( GXTexture_t        *p_texture      , VkImageViewType    view_type,
	                                 VkFormat            format         , VkComponentMapping swizzle,
	                                 VkImageAspectFlags  aspect_mask );

DLLEXPORT int construct_image      ( GXTexture_t        *p_texture   , VkImageCreateFlags    flags, 
	                                 VkImageType         image_type  , VkFormat              format, 
	                                 int                 width       , int                   height,
	                                 int                 depth       , size_t                mip_levels,
	                                 size_t              array_layers, VkSampleCountFlagBits samples, 
	                                 VkImageTiling       tiling      , VkImageUsageFlags     usage,
	                                 VkSharingMode       sharing_mode, VkImageLayout         initial_layout );

DLLEXPORT int construct_sampler    ( GXTexture_t        *p_texture );