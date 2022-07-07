#include <G10/GXTexture.h>
#include <Windows.h>

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

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {


    VkPhysicalDeviceMemoryProperties memProperties;
    GXInstance_t* instance = g_get_active_instance();

    vkGetPhysicalDeviceMemoryProperties(instance->physical_device, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    g_print_error("[Vulkan] Failed to find suitable memory type in call to function \"%s\"\n", __FUNCSIG__);

    return 0;

    
}

void create_buffer ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer buffer, VkDeviceMemory buffer_memory)
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if (size == 0)
                goto no_size;
            if (usage == 0)
                goto no_usage;
            if (properties == 0)
                goto no_properties;

        #endif
    }

    // Uninitialized data
    VkMemoryRequirements  mem_requirements;

    // Initialized data
    GXInstance_t         *instance        = g_get_active_instance();
    VkBufferCreateInfo    buffer_info     = { 0 };
    VkMemoryAllocateInfo  allocation_info = { 0 };
    VkDevice              device          = instance->device;

    // Create a buffer
    {

        // Populate buffer info struct
        buffer_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size        = size;
        buffer_info.usage       = usage;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
        // Create the buffer
        if (vkCreateBuffer(device, &buffer_info, 0, &buffer) != VK_SUCCESS)

            // Error checking
            goto failed_buffer_create;
    }

    // Allocate for the buffer
    {

        // Populate the mem_requirements struct
        vkGetBufferMemoryRequirements(device, buffer, &mem_requirements);

        // Populate the allocation info struct
        allocation_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocation_info.allocationSize  = mem_requirements.size;
        allocation_info.memoryTypeIndex = findMemoryType(mem_requirements.memoryTypeBits, properties);

        // Allocate memory
        if (vkAllocateMemory(device, &allocation_info, 0, &buffer_memory) != VK_SUCCESS)

            // Error checking
            goto failed_buffer_allocate;
    }

    // Bind the buffer to the device
    vkBindBufferMemory(device, buffer, buffer_memory, 0);

    return 0;

    // Error handling
    {
        
        // Argument errors
        {
            no_size:
                #ifndef NDEBUG
                
                #endif
                return 0;
            no_usage:
            no_properties:
            no_buffer:
            no_buffer_memory:
                return 0;
        }

        // Vulkan errors
        {
            failed_buffer_create:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create buffer in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;

            failed_buffer_allocate:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to allocate buffer memory in call to function \"%s\"\n",__FUNCSIG__);
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

int load_texture ( GXTexture_t **texture, const char *path, char *addressing, char *filtering )
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
    char          *file_extension        = 1 + strrchr(path, '.');
    size_t         width                 = 0,
                   height                = 0,
                   depth                 = 0;
    i8            *image_data            = 0;
    SDL_Surface   *image                 = 0;

    VkDevice       device                = instance->device;
    VkDeviceSize   image_size            = 0;
    VkBuffer       buffer                = 0;
    VkDeviceMemory staging_buffer_memory = 0;
    VkBuffer       staging_buffer        = 0;
    void          *data;

    *texture = i_texture;
    // Get the file extension
    
    // Load as a png
    if (strcmp(file_extension, "hdr") == 0 || strcmp(file_extension, "HDR") == 0)
        ; // TODO
    else
    {
        //image      = IMG_Load(path);
        image_data = image->pixels;
        width      = image->w;
        height     = image->h;
        depth      = image->format->BytesPerPixel;
    }

    image_size = width * height * depth;

    create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buffer, staging_buffer_memory);

    VkResult i = vkMapMemory(device, staging_buffer_memory, 0, image_size, 0, &data);

    memcpy(data, image_data, image_size);

    vkUnmapMemory(device, staging_buffer_memory);

    SDL_FreeSurface(image);

    return 1;

    no_texture:
    no_path:
    return 0;
}


