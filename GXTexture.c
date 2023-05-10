#include <G10/GXTexture.h>

dict *texturing_modes = (void *) 0;
     *filtering_modes = (void *) 0;
     *tiling_lut      = (void *) 0;
     *usage_lut       = (void *) 0;
     *view_type_lut   = (void *) 0;
     *swizzle_lut     = (void *) 0;
     *aspect_lut      = (void *) 0;

char *texturing_addressing_keys [ ] = {
    "repeat",
    "mirror repeat",
    "clamp edge",
    "clamp border",
    "mirror clamp edge"
};
enum VkSamplerAddressMode  texturing_addressing_values [ ] = {
    VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE
};

char *texture_filtering_keys [ ] = {
    "linear",
    "nearest"
};
enum VkFilter  texture_filtering_values [ ] = {
    VK_FILTER_LINEAR,
    VK_FILTER_NEAREST
};

char *tiling_keys [ ] = {
    "optimal",
    "linear"
};
enum VkImageTiling  tiling_enum [ ] = {
    VK_IMAGE_TILING_OPTIMAL,
    VK_IMAGE_TILING_LINEAR
};

char *usage_keys [ ] = {
    "transfer source",
    "transfer destination",
    "sampled",
    "storage",
    "color attachment",
    "depth attachment",
    "transient attachment",
    "input attachment"
};

enum VkImageUsageFlagBits usage_enum [ ] = {
    VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
    VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    VK_IMAGE_USAGE_SAMPLED_BIT,
    VK_IMAGE_USAGE_STORAGE_BIT,
    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
    VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT
};

char *view_type_names [ ] = {
    "1D",
    "2D",
    "3D",
    "cubemap",
    "1D array",
    "2D array",
    "cube array"
};

enum VkImageViewType view_type_enums [ ] = {
    VK_IMAGE_VIEW_TYPE_1D,
    VK_IMAGE_VIEW_TYPE_2D,
    VK_IMAGE_VIEW_TYPE_3D,
    VK_IMAGE_VIEW_TYPE_CUBE,
    VK_IMAGE_VIEW_TYPE_1D_ARRAY,
    VK_IMAGE_VIEW_TYPE_2D_ARRAY,
    VK_IMAGE_VIEW_TYPE_CUBE_ARRAY
};

char *swizzle_names [ ] = {
    "identity",
    "zero",
    "one",
    "r",
    "g",
    "b",
    "a"
};

enum VkComponentSwizzle swizzle_enums [ ] = {
    VK_COMPONENT_SWIZZLE_IDENTITY,
    VK_COMPONENT_SWIZZLE_ZERO,
    VK_COMPONENT_SWIZZLE_ONE,
    VK_COMPONENT_SWIZZLE_R,
    VK_COMPONENT_SWIZZLE_G,
    VK_COMPONENT_SWIZZLE_B,
    VK_COMPONENT_SWIZZLE_A
};

char *aspect_names [ ] = {
    "color",
    "depth",
    "stencil",
    "metadata"
};

enum VkImageAspectFlagBits aspect_enums [ ] = {
    VK_IMAGE_ASPECT_COLOR_BIT,
    VK_IMAGE_ASPECT_DEPTH_BIT,
    VK_IMAGE_ASPECT_STENCIL_BIT,
    VK_IMAGE_ASPECT_METADATA_BIT
};

void init_texture         ( void )
{

    // Construct dicts for texture addressing and filtering
    dict_construct(&texturing_modes, 5);
    dict_construct(&filtering_modes, 2);
    dict_construct(&tiling_lut, 2);
    dict_construct(&usage_lut, 8);
    dict_construct(&view_type_lut, 7);
    dict_construct(&swizzle_lut, 7);
    dict_construct(&aspect_lut, 4);

    // Error checking
    {
        #ifndef NDEBUG
            if ( texturing_modes == (void *) 0 )
                goto no_texturing_modes;
            if ( filtering_modes == (void *) 0 )
                goto no_filtering_modes;
            if ( tiling_lut      == (void *) 0 )
                goto no_tiling_lut;
        #endif
    }

    // Add each addressing mode
    for (size_t i = 0; i < 5; i++)
        dict_add(texturing_modes, texturing_addressing_keys[i], (void*)texturing_addressing_values[i]);

    for (size_t i = 0; i < 4; i++)
        dict_add(aspect_lut, aspect_names[i], (void *)aspect_enums[i]);

    // Add each filtering mode and tiling lut
    for (size_t i = 0; i < 2; i++)
    {
        dict_add(filtering_modes, texture_filtering_keys[i], (void*)texture_filtering_values[i]);
        dict_add(tiling_lut     , tiling_keys[i], (void*)tiling_enum[i]);
    }

    for (size_t i = 0; i < 8; i++)
        dict_add(usage_lut, usage_keys[i], (void *)usage_enum[i]);

    for (size_t i = 0; i < 7; i++)
    {
        dict_add(view_type_lut, view_type_names[i], (void*)view_type_enums[i]);
        dict_add(swizzle_lut, swizzle_names[i], (void*)swizzle_enums[i]);
    }
    
    // Success
    return;

    // Error handling
    {

        // Constructor errors
        {
            // TODO
            no_texturing_modes:
            no_filtering_modes:
            no_tiling_lut:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to initialize texture system");
                #endif

                // Error
                return;
        }
    }
}

int create_texture       ( GXTexture_t **pp_texture )
{
    // Argument check
	{
		#ifndef NDEBUG
			if (pp_texture == (void *) 0 )
				goto no_texture;
		#endif
	}

	// Initialized data
	GXTexture_t *p_texture = calloc(1, sizeof(GXTexture_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(p_texture == (void*)0)
				goto no_mem;
		#endif
	}

	*pp_texture = p_texture;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_texture:
				#ifndef NDEBUG
					g_print_error("[G10] [Texture] Null pointer provided for \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_texture ( GXTexture_t **pp_texture, char *path)
{
    // Argument check
    {
        #ifndef NDEBUG
            if(pp_texture == (void *)0)
                goto no_texture;
            if (path == (void*)0)
                goto no_path;
        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance  = g_get_active_instance();
    GXTexture_t   *i_texture = 0;
    size_t         json_len  = g_load_file(path, 0, false);
    char          *json_text = calloc(json_len+1, sizeof(char));
    
    // TODO: Memory checking

    // Load the file
    // TODO: Error checking
    g_load_file(path, json_text, false);

    // Load the texture 
    // TODO: Error checking
    load_texture_as_json(texture, json_text, json_len);

    // Success
    return 1;

    // TODO: Error handling
    no_texture:
    no_path:
    return 0;
}

int load_texture_as_json ( GXTexture_t **texture, char *text )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(texture == (void *)0)
                goto no_texture;
            if (text == (void*)0)
                goto no_token;
        #endif
    }

    extern u32 find_memory_type(u32 type_filter, VkMemoryPropertyFlags properties);

    // Initialized data
    dict                 *json_data              = 0;
    char                 *name                   = 0,
                         *path                   = 0;
    VkSamplerAddressMode  sampler_address_mode   = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkFilter              filter                 = VK_FILTER_LINEAR;
    GXInstance_t         *p_instance             = g_get_active_instance();
    GXTexture_t          *p_texture              = 0;
    char                 *file_extension         = 0,
                         *image_json_object      = 0,
                         *image_view_json_object = 0,
                         *sampler_json_object    = 0;
    size_t                width                  = 0,
                          height                 = 0,
                          depth                  = 0;
    i8                   *image_data             = 0;
    SDL_Surface          *image                  = 0;

    VkDeviceSize          image_size             = 0;
    VkBuffer              buffer                 = 0;
    VkDeviceMemory        staging_buffer_memory  = 0;

    VkBuffer              staging_buffer         = 0;
    VkImageCreateInfo     image_create_info      = { 0 };
    void                 *data                   = 0;

    
    // Parse the JSON
    {

        // Initialized data
        JSONToken_t *t = 0;

        // Parse the JSON text into a dictionary
        //parse_json(text, len, &json_data);

        // Parse the dictionary
        {

            // Get the image
            t = (JSONToken_t *) dict_get(json_data, "image");
            image_json_object = JSON_VALUE(t, JSONobject);

            // Get the image
            t = (JSONToken_t *) dict_get(json_data, "image view");
            image_view_json_object = JSON_VALUE(t, JSONobject);

            t = (JSONToken_t *) dict_get(json_data, "sampler");
            sampler_json_object = JSON_VALUE(t, JSONobject);

            // Get the path
            t = (JSONToken_t *) dict_get(json_data, "path");
            path  = JSON_VALUE(t, JSONstring);
    }
    }

    // Allocate a texture
    if ( create_texture(texture) == 0 )
        goto no_texture;

    p_texture = *texture;

    if (path) {

        // Get the file extension
        file_extension = 1 + strrchr(path, '.');

        // Load an HDR
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

        // Calculate the size of the image
        image_size = width * height * depth;

        // Create a buffer for the image
        create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &staging_buffer, &staging_buffer_memory);

        // Copy the image 
        vkMapMemory(p_instance->vulkan.device, staging_buffer_memory, 0, image_size, 0, &data);
        memcpy(data, image_data, image_size);
        vkUnmapMemory(p_instance->vulkan.device, staging_buffer_memory);

        // Popultate image create info struct
        construct_image(p_texture, 0, VK_IMAGE_TYPE_2D, VK_FORMAT_B8G8R8A8_UNORM, (int)width, (int)height, 1, 1, 1, VK_SAMPLE_COUNT_1_BIT,VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_SHARING_MODE_EXCLUSIVE, VK_IMAGE_LAYOUT_UNDEFINED);

    }
    else if (path == 0 && image_json_object)
    {
        
        // Initialized data
        size_t   len             = strlen(image_json_object);
        dict    *image_json      = 0;
        char   **flags           = 0,
                *format          = 0,
                *view_type       = 0,
               **extent          = 0,
                *mip_levels      = (char *)9,
                *array_layers    = 0,
                *samples         = 0,
                *tiling          = 0,
               **usage           = 0,
                *atomic_sharing  = 0;

        // Parse the json text into a dictionary
        parse_json(image_json_object, len, &image_json);

        // Parse the dictionary
        {
            
            // Initialized data
            JSONToken_t *t = 0;

            t               = (JSONToken_t *) dict_get(image_json, "flags");
            flags           = JSON_VALUE(t, JSONarray);

            t               = (JSONToken_t *) dict_get(image_json, "view type");
            view_type       = JSON_VALUE(t, JSONstring);

            t               = (JSONToken_t *) dict_get(image_json, "format");
            format          = JSON_VALUE(t, JSONstring);

            t               = (JSONToken_t *) dict_get(image_json, "extent");
            extent          = JSON_VALUE(t, JSONarray);

            t               = (JSONToken_t *) dict_get(image_json, "mip levels");
            mip_levels      = JSON_VALUE(t, JSONprimative);

            t               = (JSONToken_t *) dict_get(image_json, "array layers");
            array_layers    = JSON_VALUE(t, JSONprimative);

            t               = (JSONToken_t *) dict_get(image_json, "samples");
            samples         = JSON_VALUE(t, JSONprimative);
            
            t               = (JSONToken_t *) dict_get(image_json, "tiling");
            tiling          = JSON_VALUE(t, JSONstring);

            t               = (JSONToken_t *) dict_get(image_json, "usage");
            usage           = JSON_VALUE(t, JSONarray);

            t               = (JSONToken_t *) dict_get(image_json, "atomic sharing");
            atomic_sharing  = JSON_VALUE(t, JSONprimative);

        }

        extern dict *format_types;
        VkImageUsageFlags usage_flag = 0;
        size_t usage_flags = 0;
        while (usage[++usage_flags]);
        
        for (size_t i = 0; i < usage_flags; i++)
            usage_flag |= (VkImageUsageFlags)(size_t)dict_get(usage_lut, usage[i]);

        construct_image(
            p_texture, 
            0, 
            (view_type)    ? (VkImageType)(size_t)dict_get(view_type_lut, view_type) : VK_IMAGE_TYPE_2D,
            (format)       ? (VkFormat)(size_t)dict_get(format_types, format) : VK_FORMAT_B8G8R8A8_UNORM,
            (extent[0])    ? atoi(extent[0])    : 1, 
            (extent[1])    ? atoi(extent[1])    : 1, 
            (extent[2])    ? atoi(extent[2])    : 1,
            (mip_levels)   ? atoi(mip_levels)   : 1,
            (array_layers) ? atoi(array_layers) : 1,
            (samples)      ? atoi(samples)      : 1,
            (tiling)       ? (VkImageTiling)(size_t)dict_get(tiling_lut, tiling) : VK_IMAGE_TILING_OPTIMAL,
            usage_flag,
            VK_SHARING_MODE_EXCLUSIVE,
            VK_IMAGE_LAYOUT_UNDEFINED
        );
    }

    // Construct an image view
    if (image_view_json_object)
    {
        
        // External data
        extern dict         *format_types;

        // Initialized data
        size_t               len               = strlen(image_view_json_object);
        dict                *image_json        = 0;
        char                *view_type         = 0,
                           **swizzle           = 0,
                           **image_aspects     = 0,
                            *generate_mips     = 0,
                            *format            = 0;
        VkComponentMapping   swizzle_mapping   = {
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        };
        VkImageAspectFlags   image_aspect_flag = 0;
        VkImageViewType      image_view_type   = 0;
        VkFormat             image_view_format = 0;

        // Parse the json text into a dictionary
        parse_json(image_view_json_object, len, &image_json);

        // Parse the dictionary
        {
            
            // Initialized data
            JSONToken_t *t = 0;

            t             = (JSONToken_t *) dict_get(image_json, "view type");
            view_type     = JSON_VALUE(t, JSONstring);

            t             = (JSONToken_t *) dict_get(image_json, "format");
            format        = JSON_VALUE(t, JSONstring);

            t             = (JSONToken_t *) dict_get(image_json, "swizzle");
            swizzle       = JSON_VALUE(t, JSONarray);

            t             = (JSONToken_t *) dict_get(image_json, "image aspects");
            image_aspects = JSON_VALUE(t, JSONarray);

            t             = (JSONToken_t *) dict_get(image_json, "generate mips");
            generate_mips = JSON_VALUE(t, JSONprimative);
            
        }

        // Parse constructor parameters
        {
            if (swizzle)
            {

                // Initialized data
                size_t swizzle_count = 0;

                // Count up swizzles
                while (swizzle[++swizzle_count]);

                // Do we have enough?
                if (swizzle_count > 3)
                {
                    swizzle_mapping.r = (VkComponentSwizzle)dict_get(swizzle_lut, swizzle[0]),
                    swizzle_mapping.g = (VkComponentSwizzle)dict_get(swizzle_lut, swizzle[1]),
                    swizzle_mapping.b = (VkComponentSwizzle)dict_get(swizzle_lut, swizzle[2]),
                    swizzle_mapping.a = (VkComponentSwizzle)dict_get(swizzle_lut, swizzle[3]);
                }
                else
                    goto not_enough_swizzle;
            }

            if (image_aspects)
            {

                size_t image_aspect_count = 0;

                while (image_aspects[++image_aspect_count]);

                for (size_t i = 0; i < image_aspect_count; i++)
                    image_aspect_flag |= (VkImageAspectFlags)(size_t)dict_get(aspect_lut, image_aspects[i]);

            }
        }
        image_view_type   = (VkImageViewType)dict_get(view_type_lut, view_type);
        image_view_format = (VkFormat)dict_get(format_types, format);

        // Construct the image view
        if ( construct_image_view(p_texture, image_view_type, image_view_format, swizzle_mapping, image_aspect_flag) == 0)
            goto failed_to_construct_image_view;
    }

    return 1;

    no_texture:
    no_token:
    return 0;
    failed_to_construct_image_view:
    return 0;
    not_enough_swizzle:
    g_print_error("[G10] [Texture] Failed to create image view in call to function \"%s\"\n", __FUNCTION__);
    return 0;


}

int construct_image      ( GXTexture_t  *p_texture, VkImageCreateFlags flags, VkImageType image_type, VkFormat format, int width, int height, int depth, size_t mip_levels, size_t array_layers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharing_mode, VkImageLayout initial_layout )
{
    GXInstance_t      *p_instance          = g_get_active_instance();
    VkImageCreateInfo  image_create_info = { 0 };
    size_t             dim               = 0;
    VkMemoryAllocateInfo  allocate_info = { 0 };
    // Uninitialized data
    VkMemoryRequirements  memory_requirements;

    if ( height > 1 ) dim++;
    if ( depth  > 1 ) dim++;
    
    // Popultate image create info struct
    {
        image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.flags         = 0;
        image_create_info.imageType     = image_type;
        image_create_info.format        = format;
        image_create_info.extent.width  = (u32)width;
        image_create_info.extent.height = (u32)height;
        image_create_info.extent.depth  = (u32)depth;
        image_create_info.mipLevels     = (u32)mip_levels;
        image_create_info.arrayLayers   = (u32)array_layers;
        image_create_info.tiling        = tiling;
        image_create_info.usage         = usage;
        image_create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.samples       = samples;
    }
       
    // Create the image
    if ( vkCreateImage(p_instance->vulkan.device, &image_create_info, 0, &p_texture->texture_image) != VK_SUCCESS )
        goto failed_to_create_image;

    // Figure out how much memory the image will use
    vkGetImageMemoryRequirements(p_instance->vulkan.device, p_texture->texture_image, &memory_requirements);

    // Popultate the allocate info struct
    {
        allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocate_info.allocationSize = memory_requirements.size;
        allocate_info.memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    }

    // Allocate memory for the image
    if (vkAllocateMemory(p_instance->vulkan.device, &allocate_info, 0, &p_texture->texture_image_memory) != VK_SUCCESS) {
        // TODO: GOTO, error handling
        printf("failed to allocate image memory!");
    }

    // Bind the image to the image memory
    vkBindImageMemory(p_instance->vulkan.device, p_texture->texture_image, p_texture->texture_image_memory, 0);

    return 1;

    // Error handling
    {

        // Argument errors
        {

        }

        // Vulkan errors
        {
            failed_to_create_image:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to create image in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }
}

int construct_image_view ( GXTexture_t  *p_texture, VkImageViewType view_type, VkFormat format, VkComponentMapping swizzle, VkImageAspectFlags aspect_mask )
{
    GXInstance_t          *p_instance               = g_get_active_instance();
    VkImageViewCreateInfo  image_view_create_info = { 0 };

    // Popultate image view create info struct
    {

        image_view_create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image                           = p_texture->texture_image;
        image_view_create_info.viewType                        = view_type;
        image_view_create_info.format                          = format;

        image_view_create_info.subresourceRange.aspectMask     = aspect_mask;
        image_view_create_info.subresourceRange.baseMipLevel   = 0;
        image_view_create_info.subresourceRange.levelCount     = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount     = 1;
    }

    if ( vkCreateImageView(p_instance->vulkan.device, &image_view_create_info, 0, &p_texture->texture_image_view) != VK_SUCCESS )
        ;// TODO: Throw an error

    return 1;
}
