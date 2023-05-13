/** !
 * @file G10/GXTransform.h
 * @author Jacob Smith
 * 
 * Textures
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// SDL2
#include <SDL.h>
#include <SDL_image.h>

// Dictionary submodule
#include <dict/dict.h>

// JSON submodule
#include <json/json.h>

// G10
#include <G10/G10.h>
#include <G10/GXtypedef.h>

struct GXTexture_s
{
	char           *name;
	VkImage         texture_image;
	VkDeviceMemory  texture_image_memory;
	VkImageView     texture_image_view;
	VkSampler       texture_sampler;
};

// Allocator
/** !
 *  Allocate memory for a texture
 *
 * @param pp_texture : return
 *
 * @sa destroy_texture
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_texture ( GXTexture_t **pp_texture );

// Constructors
/** !
 *  Load a texture from a file
 *
 * @param pp_texture : return
 * @param path       : The path to a texture JSON file
 *
 * @sa load_texture_as_json
 * @sa load_texture_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_texture ( GXTexture_t **pp_texture, char *path );

/** !
 *  Load a texture from JSON text
 *
 * @param pp_texture : return
 * @param text       : JSON text
 *
 * @sa load_texture
 * @sa load_texture_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_texture_as_json ( GXTexture_t **pp_texture, char *text );

/** !
 *  Load a texture from a JSON value
 *
 * @param pp_texture : return
 * @param p_value    : JSON value
 *
 * @sa load_texture
 * @sa load_texture_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_texture_as_json_value ( GXTexture_t **pp_texture, JSONValue_t *p_value );

// Vulkan Constructors
/** !
 *  Construct a vulkan image view for a texture
 *
 * @param p_texture   : texture
 * @param view_type   : VkImageViewType (See Vulkan manual)
 * @param format      : VkFormat (See Vulkan manual)
 * @param swizzle     : VkComponentMapping (See Vulkan manual)
 * @param aspect_mask : VkImageAspectFlags (See Vulkan manual)
 * 
 * @sa construct_image
 * @sa construct_sampler
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int construct_image_view ( GXTexture_t *p_texture, VkImageViewType view_type,
                                     VkFormat format, VkComponentMapping swizzle,
								     VkImageAspectFlags aspect_mask );

/** !
 *  Construct a vulkan image view for a texture
 *
 * @param p_texture      : texture
 * @param flags          : VkImageCreateFlags
 * @param image_type     : VkImageType (See Vulkan manual)
 * @param format         : VkFormat (See Vulkan manual)
 * @param width          : image width 
 * @param height         : image height
 * @param depth          : image depth
 * @param mip_levels     : MipMap level (See Vulkan manual)
 * @param array_layers   : quantity of array layers. (See Vulkan manual)
 * @param samples        : samples. Must be power of 2 (2,4,8,16,32,etc)
 * @param tiling         : VkImageTiling (See Vulkan manual)
 * @param usage          : VkImageUsageFlags (See Vulkan manual)
 * @param sharing_mode   : VkSharingMode (See Vulkan manual)
 * @param initial_layout : VkImageLayout (See Vulkan manual)
 * 
 * @sa construct_image_view
 * @sa construct_sampler
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int construct_image ( GXTexture_t *p_texture, VkImageCreateFlags flags, 
	                            VkImageType image_type, VkFormat format, 
	                            int width, int height,
	                            int depth, size_t mip_levels,
	                            size_t array_layers, VkSampleCountFlagBits samples, 
	                            VkImageTiling tiling, VkImageUsageFlags usage,
	                            VkSharingMode sharing_mode, VkImageLayout initial_layout );
/** !
 *  Construct a sampler for a texture
 *
 * @param p_texture : texture
 * 
 * @sa construct_image_view
 * @sa construct_image
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int construct_sampler ( GXTexture_t *p_texture );