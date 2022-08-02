#include <G10/GXTexture.h>

dict *texturing_modes                                = (void *) 0;
dict *filtering_modes                                = (void *) 0;

char                     *texturing_addressing_keys   [ ] = {
                                                              "repeat",
                                                              "mirror repeat",
                                                              "clamp edge",
                                                              "clamp border",
                                                              "mirror clamp edge"
                                                            };

enum VkSamplerAddressMode texturing_addressing_values [ ] = {
                                                              VK_SAMPLER_ADDRESS_MODE_REPEAT,
                                                              VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
                                                              VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
                                                              VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
                                                              VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
                                                            };

char                     *texture_filtering_keys      [ ] = {
                                                              "linear",
                                                              "nearest"
                                                            };

enum VkFilter             texture_filtering_values    [ ] = {
                                                              VK_FILTER_LINEAR,
                                                              VK_FILTER_NEAREST
                                                            };

int init_texture ( void )
{

    // Construct dicts for texture addressing and filtering
    dict_construct(&texturing_modes, 5);
    dict_construct(&filtering_modes, 2);

    // Error checking
    {
        #ifndef NDEBUG
            if ( texturing_modes == (void *)0 )
                goto no_texturing_modes;
            if ( filtering_modes == (void *)0 )
                goto no_filtering_modes;

        #endif
    }

    // Add each addressing mode
    for (size_t i = 0; i < 5; i++)
        dict_add(texturing_modes, texturing_addressing_keys[i], (void*)texturing_addressing_values[i]);

    // Add each filtering mode
    for (size_t i = 0; i < 2; i++)
        dict_add(filtering_modes, texture_filtering_keys[i], (void*)texture_filtering_values[i]);

    // Done
    return 1;

    // Error handling
    {

        // Constructor errors
        {
            no_texturing_modes:
            no_filtering_modes:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to initialize texture system");
                #endif
                return 0;
        }
    }
}

int create_texture ( GXTexture_t **texture )
{
    // Argument check
	{
		#ifndef NDEBUG
			if ( texture == (void *) 0 )
				goto no_texture;
		#endif
	}

	// Initialized data
	GXTexture_t *ret = calloc(1, sizeof(GXTexture_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void*)0)
				goto no_mem;
		#endif
	}

	*texture = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_texture:
				#ifndef NDEBUG
					g_print_error("[G10] [Texture] Null pointer provided for \"texture\" in call to function \"%s\"", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_texture(GXTexture_t** texture, char* path)
{
    // Argument check
    {
        #ifndef NDEBUG
            if(texture == (void *)0)
                goto no_texture;
            if (path == (void*)0)
                goto no_path;
        #endif
    }

    // Initialized data
    GXInstance_t  *instance              = g_get_active_instance();
    GXTexture_t   *i_texture             = 0;
    size_t         json_len              = g_load_file(path, 0, false);
    char          *json_text             = calloc(json_len+1, sizeof(char));
    
    // Load the file
    g_load_file(path, json_text, false);

    // Load the texture 
    load_texture_as_json(texture, json_text, json_len);

    return 1;

    no_texture:
    no_path:
    return 0;
}

int load_texture_as_json (GXTexture_t **texture, char *token_text, size_t token_len )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(texture == (void *)0)
                goto no_texture;
            if (token_text == (void*)0)
                goto no_token;
        #endif
    }

    extern u32 find_memory_type(u32 type_filter, VkMemoryPropertyFlags properties);

    // Uninitialized data
    VkMemoryRequirements  memory_requirements;

    // Initialized data
    dict                 *json_data            = 0;
    char                 *name                 = 0,
                         *path                 = 0;
    VkSamplerAddressMode  sampler_address_mode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkFilter              filter               = VK_FILTER_LINEAR;
    GXInstance_t         *instance              = g_get_active_instance();
    GXTexture_t          *i_texture             = 0;
    char                 *file_extension        = 0;
    size_t                width                 = 0,
                          height                = 0,
                          depth                 = 0;
    i8                   *image_data            = 0;
    SDL_Surface          *image                 = 0;

    VkDeviceSize          image_size            = 0;
    VkBuffer              buffer                = 0;
    VkDeviceMemory        staging_buffer_memory = 0;
    VkMemoryAllocateInfo *allocate_info         = calloc(1, sizeof(VkMemoryAllocateInfo));

    VkBuffer              staging_buffer        = 0;
    VkImageCreateInfo    *image_create_info     = calloc(1, sizeof(VkImageCreateInfo));
    void                 *data                  = 0;

    
    // Parse the JSON
    {

        // Initialized data
        JSONToken_t *token = 0;

        // Parse the JSON text into a dictionary
        parse_json(token_text, token_len, &json_data);

        // Get the name
        token = dict_get(json_data, "name");
        name  = JSON_VALUE(token, JSONstring);

        // Get the addressing mode
        {
            token                = dict_get(json_data, "addressing");

            // Recycle the token pointer 
            token                = JSON_VALUE(token, JSONstring);

            // Get the corresponding enum value
            sampler_address_mode = (VkSamplerAddressMode)dict_get(filtering_modes, token);
        }

        // Get the filter mode
        {
            token  = dict_get(json_data, "filter");

            // Recycle the token pointer
            token  = JSON_VALUE(token, JSONstring);

            // Get the corresponding enum value
            filter = (VkFilter)dict_get(filtering_modes, token);
        }

        // Get the path
        token = dict_get(json_data, "path");
        path  = JSON_VALUE(token, JSONstring);
    }

    create_texture(texture);
    i_texture = *texture;

    // Get the file extension
    file_extension = 1 + strrchr(path, '.');

    // Load as a png
    if (strcmp(file_extension, "hdr") == 0 || strcmp(file_extension, "HDR") == 0)
        ; // TODO
    else
    {
        image      = IMG_Load(path);
        image_data = image->pixels;
        width      = image->w;
        height     = image->h;
        depth      = image->format->BytesPerPixel;
    }

    image_size = width * height * depth;

    create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &staging_buffer, &staging_buffer_memory);

    vkMapMemory(instance->device, staging_buffer_memory, 0, image_size, 0, &data);
    memcpy(data, image_data, image_size);
    vkUnmapMemory(instance->device, staging_buffer_memory);

    {
        image_create_info->sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info->imageType = VK_IMAGE_TYPE_2D;
        image_create_info->extent.width = width;
        image_create_info->extent.height = height;
        
        image_create_info->extent.depth = 1;
        image_create_info->mipLevels = 1;
        image_create_info->arrayLayers = 1;


        if (depth == 3)
        {
            g_print_error("[G10] [Texture] No 24 bit images allowed!");
            return 0;
        }
        else if (depth == 4)
            image_create_info->format = VK_FORMAT_R8G8B8A8_SRGB;

        image_create_info->tiling = VK_IMAGE_TILING_OPTIMAL;
        image_create_info->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info->usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        image_create_info->sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_create_info->samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info->flags = 0;
    }

    if (vkCreateImage(instance->device, image_create_info, 0, &i_texture->texture_image) != VK_SUCCESS)
        g_print_error("[G10] [Texture] Failed to create image in call to function \"%s\"\n", __FUNCSIG__);

    vkGetImageMemoryRequirements(instance->device, i_texture->texture_image, &memory_requirements);

    allocate_info->sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocate_info->allocationSize = memory_requirements.size;
    allocate_info->memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(instance->device, allocate_info, 0, &i_texture->texture_image_memory) != VK_SUCCESS) {
        printf("failed to allocate image memory!");
    }

    vkBindImageMemory(instance->device, i_texture->texture_image, i_texture->texture_image_memory, 0);



    return 1;

    no_texture:
    no_token:
    return 0;


}
