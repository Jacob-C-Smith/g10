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

void init_texture ( void )
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

int create_texture ( GXTexture_t **pp_texture )
{

    // Argument check
	{
		#ifndef NDEBUG
			if ( pp_texture == (void *) 0 ) goto no_texture;
		#endif
	}

	// Initialized data
	GXTexture_t *p_texture = calloc(1, sizeof(GXTexture_t));

	// Error checking
    if ( p_texture == (void *) 0 )
	    goto no_mem;

    // Return a pointer to the caller
	*pp_texture = p_texture;

    // Success
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

int load_texture ( GXTexture_t **pp_texture, char *path )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_texture == (void *) 0 ) goto no_texture;
            if ( path       == (void *) 0 ) goto no_path;
        #endif
    }

    // Initialized data
    GXInstance_t  *p_instance  = g_get_active_instance();
    size_t         len         = g_load_file(path, 0, true);
    char          *text        = calloc(len+1, sizeof(char));
    
    // Memory checking
    if ( text == (void *) 0 )
        goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 ) 
        goto failed_to_load_file;

    // Load the texture 
    if ( load_texture_as_json_text(pp_texture, text) == 0 )
        goto failed_to_load_texture_as_json_text;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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

        // G10 Errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_texture_as_json_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to load texture from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int load_texture_as_json_text ( GXTexture_t **pp_texture, char *text )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_texture == (void *) 0 ) goto no_texture;
            if ( text       == (void *) 0 ) goto no_text;
        #endif
    }

    // Initialized data
    JSONValue_t *p_value = 0;

    // Parse the JSON text into a JSON value
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto failed_to_parse_json;
    
    // Construct a texture
    if ( load_texture_as_json_value(pp_texture, p_value) == 0 )
        goto failed_to_load_texture;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 Errors
        {
            failed_to_parse_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to parse JSON in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_texture:
                #ifndef NDEBUG
                    g_print_error("[G10] [Texture] Failed to load texture in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_texture_as_json_value ( GXTexture_t **pp_texture, JSONValue_t *p_value )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_texture == (void *) 0 ) goto no_texture;
            if ( p_value    == (void *) 0 ) goto no_value;
        #endif
    }

    // External functions
    extern u32 find_memory_type(u32 type_filter, VkMemoryPropertyFlags properties);

    // Initialized data
    dict                 *json_data                = 0;
    char                 *name                     = 0;
    VkSamplerAddressMode  sampler_address_mode     = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    VkFilter              filter                   = VK_FILTER_LINEAR;
    GXInstance_t         *p_instance               = g_get_active_instance();
    GXTexture_t          *p_texture                = 0;
    char                 *file_extension           = 0;
    size_t                width                    = 0,
                          height                   = 0,
                          depth                    = 0;
    i8                   *image_data               = 0;
    SDL_Surface          *image                    = 0;
    VkDeviceSize          image_size               = 0;
    VkBuffer              buffer                   = 0;
    VkDeviceMemory        staging_buffer_memory    = 0;
    VkBuffer              staging_buffer           = 0;
    VkImageCreateInfo     image_create_info        = { 0 };
    void                 *data                     = 0;
    JSONValue_t          *p_image_json_object      = 0,
                         *p_image_view_json_object = 0,
                         *p_sampler_json_object    = 0,
                         *p_path                   = 0;

    // Parse the JSON
    if ( p_value->type == JSONobject ) 
    {

        p_image_json_object      = ((JSONValue_t *)dict_get(json_data, "image"))->object;
        p_image_view_json_object = ((JSONValue_t *)dict_get(json_data, "image view"))->object;
        p_sampler_json_object    = ((JSONValue_t *)dict_get(json_data, "sampler"))->object;
        p_path                   = ((JSONValue_t *)dict_get(json_data, "path"))->string;

        // Check properties
        if ( p_image_json_object && p_image_view_json_object && p_sampler_json_object && p_path )
            goto missing_properties;
    }
    else
        goto wrong_type;
    
    // Construct a texture
    {

        // Allocate a texture
        if ( create_texture(&p_texture) == 0 )
            goto no_texture;
        
        // Load the texture
        if ( p_path )
        {

            if ( p_path->type == JSONstring )
            {
                // Get the file extension
                file_extension = 1 + strrchr(p_path->string, '.');

                // Load an HDR
                if (strcmp(file_extension, "hdr") == 0 || strcmp(file_extension, "HDR") == 0)
                    ; // TODO
                else
                {
                    image      = IMG_Load(p_path->string);
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
            else
                goto failed_to_load_texture;
        }
        else if ( p_path == 0 && p_image_json_object )
        {

            // Parse the image as a JSON object
            if ( p_image_json_object->type == JSONobject)
            {

                // External data
                extern dict *format_types;

                // Initialized data
                JSONValue_t        *p_flags           = 0,
                                   *p_format          = 0,
                                   *p_view_type       = 0,
                                   *p_extent          = 0,
                                   *p_mip_levels      = 0,
                                   *p_array_layers    = 0,
                                   *p_samples         = 0,
                                   *p_tiling          = 0,
                                   *p_usage           = 0,
                                   *p_atomic_sharing  = 0;
                VkImageUsageFlags   usage_flag        = 0;
                size_t              usage_flags       = 0,
                                    extent_count      = 0;
                JSONValue_t       **pp_extents        = 0;

                // Parse the JSON value
                {

                    p_flags          = ((JSONValue_t *) dict_get(p_image_json_object->object, "flags"));
                    p_format         = ((JSONValue_t *) dict_get(p_image_json_object->object, "format"));
                    p_view_type      = ((JSONValue_t *) dict_get(p_image_json_object->object, "view type"));
                    p_extent         = ((JSONValue_t *) dict_get(p_image_json_object->object, "extent"));
                    p_mip_levels     = ((JSONValue_t *) dict_get(p_image_json_object->object, "mip levels"));
                    p_array_layers   = ((JSONValue_t *) dict_get(p_image_json_object->object, "array layers"));
                    p_samples        = ((JSONValue_t *) dict_get(p_image_json_object->object, "samples"));
                    p_tiling         = ((JSONValue_t *) dict_get(p_image_json_object->object, "tiling"));
                    p_usage          = ((JSONValue_t *) dict_get(p_image_json_object->object, "usage"));
                    p_atomic_sharing = ((JSONValue_t *) dict_get(p_image_json_object->object, "atomic sharing"));

                    // Check properties
                    if (
                        p_flags      && p_format       && 
                        p_view_type  && p_extent       &&
                        p_mip_levels && p_array_layers &&
                        p_samples    && p_tiling       &&
                        p_usage      && p_atomic_sharing
                        )
                        goto missing_properties;
                }

                // Construct the image

                // Construct the usage flags
                if ( p_usage->type == JSONarray ){

                    // Initialzied data
                    size_t       *usage_string_count = 0;
                    JSONValue_t **pp_usage_strings   = 0;

                    // Get the array contents
                    {

                        array_get(p_usage->list, 0, &usage_string_count);

                        pp_usage_strings = calloc(usage_string_count+1, sizeof(JSONValue_t *));

                        if ( pp_usage_strings == (void *) 0 )
                            goto no_mem;

                        array_get(p_usage->list, 0, &usage_string_count);
                    }

                    for (size_t i = 0; i < usage_string_count; i++)
                        usage_flag |= (VkImageUsageFlags)(size_t)dict_get(usage_lut, pp_usage_strings[i]->string);
                    
                }

                // Get the extents
                {

                    array_get(p_extent->list, 0, &extent_count);

                    pp_extents = calloc(extent_count+1, sizeof(JSONValue_t *));

                    if ( pp_extents == (void *) 0 )
                        goto no_mem;

                    array_get(p_extent->list, pp_extents, 0);
                }

                construct_image(
                    p_texture, 
                    0, 
                    (p_view_type->string)    ? (VkImageType)(size_t)dict_get(view_type_lut, p_view_type->string) : VK_IMAGE_TYPE_2D,
                    (p_format->string)       ? (VkFormat)(size_t)dict_get(format_types, p_format->string) : VK_FORMAT_B8G8R8A8_UNORM,
                    (pp_extents[0]->integer) ? pp_extents[0]->integer  : 1, 
                    (pp_extents[1]->integer) ? pp_extents[1]->integer  : 1, 
                    (pp_extents[2]->integer) ? pp_extents[2]->integer  : 1,
                    (p_mip_levels->integer)  ? p_mip_levels->integer   : 1,
                    (p_array_layers)         ? p_array_layers->integer : 1,
                    (p_samples)              ? p_samples->integer      : 1,
                    (p_tiling->string)       ? (VkImageTiling)(size_t)dict_get(tiling_lut, p_tiling->string) : VK_IMAGE_TILING_OPTIMAL,
                    usage_flag,
                    VK_SHARING_MODE_EXCLUSIVE,
                    VK_IMAGE_LAYOUT_UNDEFINED
                );
            }
        }

        // Default
        else
        {
            
        }

        
        /*
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
        */
       
        // Return a pointer to the caller
        *pp_texture = p_texture;
    }   
    
    // Success
    return 1;

    no_texture:
    no_value:
    failed_to_load_texture:
    missing_properties:
    wrong_type:
    return 0;
    failed_to_construct_image_view:
    return 0;
    not_enough_swizzle:
    g_print_error("[G10] [Texture] Failed to create image view in call to function \"%s\"\n", __FUNCTION__);
    return 0;
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

int construct_image ( GXTexture_t *p_texture, VkImageCreateFlags flags, VkImageType image_type, VkFormat format, int width, int height, int depth, size_t mip_levels, size_t array_layers, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharing_mode, VkImageLayout initial_layout )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_texture == (void *) 0 ) goto no_texture;
        #endif 
    }

    // Uninitialized data 
    VkMemoryRequirements memory_requirements;

    // Initialized data
    GXInstance_t          *p_instance        = g_get_active_instance();
    VkMemoryAllocateInfo   allocate_info     = { 0 };
    VkImageCreateInfo      image_create_info =
    {
        .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .flags         = 0,
        .imageType     = image_type,
        .format        = format,
        .extent.width  = (u32)width,
        .extent.height = (u32)height,
        .extent.depth  = (u32)depth,
        .mipLevels     = (u32)mip_levels,
        .arrayLayers   = (u32)array_layers,
        .tiling        = tiling,
        .usage         = usage,
        .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .samples       = samples
    };
       
    // Create the image
    if ( vkCreateImage(p_instance->vulkan.device, &image_create_info, 0, &p_texture->texture_image) != VK_SUCCESS )
        goto failed_to_create_image;

    // Figure out how much memory the image will use
    vkGetImageMemoryRequirements(p_instance->vulkan.device, p_texture->texture_image, &memory_requirements);

    // Popultate the allocate info struct
    allocate_info = (VkMemoryAllocateInfo) {
        .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize  = memory_requirements.size,
        .memoryTypeIndex = find_memory_type(memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
    };

    // Allocate memory for the image
    if ( vkAllocateMemory(p_instance->vulkan.device, &allocate_info, 0, &p_texture->texture_image_memory) != VK_SUCCESS)
        goto failed_to_allocate_image_memory;

    // Bind the image to the image memory
    vkBindImageMemory(p_instance->vulkan.device, p_texture->texture_image, p_texture->texture_image_memory, 0);

    // Syccess
    return 1;
    
    failed_to_allocate_image_memory:
    no_texture:
        return 0;

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
                
                // Error
                return 0;
        }
    }
}

int construct_image_view ( GXTexture_t *p_texture, VkImageViewType view_type, VkFormat format, VkComponentMapping swizzle, VkImageAspectFlags aspect_mask )
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
