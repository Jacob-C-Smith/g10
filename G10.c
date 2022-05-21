#include <G10\G10.h>

static GXInstance_t* active_instance = 0;
FILE* log_file;

int           g_init           ( GXInstance_t      **instance, const char   *path)
{
    // DEBUG: Argument Check
    {
        #ifndef NDEBUG
            if (path == (void*)0)
                goto no_path;
        #endif
    }

    // Initialized data
    GXInstance_t *ret                         = calloc(1, sizeof(GXInstance_t));
    size_t        token_text_len              = g_load_file(path, 0, false),
                  token_count                 = 0,
                  part_cache_count            = 32,
                  material_cache_count        = 32,
                  shader_cache_count          = 8,
                  loading_thread_count        = 4;
    dict         *instance_json_object        = 0;
    char         *token_text                  = calloc(token_text_len, sizeof(u8)),
                 *name                        = 0,
                 *window_title                = "G10",
                **requested_validation_layers = 0,
                 *requested_physical_device   = 0,
                 *initial_scene               = 0,
                 *log_file_i                  = 0,
                 *input                       = 0;
    i32           window_width                = 1280,
                  window_height               = 720;
    bool          fullscreen                  = false;


    // Load the file
    g_load_file(path, token_text, false);

    // Parse the JSON
    {
        
        // Turn the JSON text into a dictionary
        parse_json(token_text, token_text_len, &instance_json_object);

        // A JSON token
        JSONToken_t* token = 0;
        
        // Name. Default to "Instance"
        token                 = dict_get(instance_json_object, "name");
        name                  = (token) ? token->value.n_where : "Instance";

        // Window 
        {
            // Set the window width. Default to 800
            token         = dict_get(instance_json_object, "window width");
            window_width  = (token) ? atol(token->value.n_where) : 1280;

            // Set the window height. Default to 600
            token         = dict_get(instance_json_object, "window height");
            window_height = (token) ? atol(token->value.n_where) : 720;

            // Set the window title. Default to "G10"
            token         = dict_get(instance_json_object, "window title");
            window_title  = (token) ? token->value.n_where : "G10";

            // Fullscreen? Default to false
            token         = dict_get(instance_json_object, "fullscreen");
            fullscreen    = (token) ? token->value.n_where : false;
        }
        
        // Input
        token                = dict_get(instance_json_object, "input");
        input                = (token) ? token->value.n_where : 0;

        // Log file
        token                = dict_get(instance_json_object, "log file");
        log_file_i           = (token) ? token->value.n_where : 0;

        // Initial scene path
        token                = dict_get(instance_json_object, "initial scene");
        initial_scene        = (token) ? token->value.n_where : 0;

        // Caches
        {

            // Set the part cache limit
            token                = dict_get(instance_json_object, "cache part count");
            part_cache_count     = (token) ? atol(token->value.n_where) : 128;

            token                = dict_get(instance_json_object, "cache material count");
            material_cache_count = (token) ? atol(token->value.n_where) : 128;

            token                = dict_get(instance_json_object, "cache shader count");
            shader_cache_count   = (token) ? atol(token->value.n_where) : 32;
        }

        // Loading thread count
        token                = dict_get(instance_json_object, "loading thread count");
        loading_thread_count = (token) ? atol(token->value.n_where) : 4;

        // Vulkan
        {
            // Validation layers
            {
                token                       = dict_get(instance_json_object, "vulkan validation layers");
                requested_validation_layers = (token) ? (token->value.a_where) : 0;
            }

            // Physical device
            {
                token                     = dict_get(instance_json_object, "vulkan physical device");
                requested_physical_device = (token) ? (token->value.n_where) : 0;
            }
        }

    }

    // Set the log file before doing anything else
    {
        if (log_file_i)
            log_file = fopen(log_file_i, "w");
        else
            log_file = stdout;
    }

    // SDL Initialization
    {

        // Initialize SDL
        if (SDL_Init(SDL_INIT_EVERYTHING))
            goto noSDL;

        // Create the window
        ret->window = SDL_CreateWindow(window_title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            window_width, window_height,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | ((fullscreen) ? SDL_WINDOW_FULLSCREEN : 0) ); 

        // Check the window
        if (!ret->window)
            goto noWindow;


    }

    // Vulkan initialization
    {

        // Uninitialized data
        VkApplicationInfo          *application_info;
        VkInstanceCreateInfo       *instance_create_info;
        VkExtensionProperties      *extension_properties;
        VkLayerProperties          *validation_layer_properties;
        VkPhysicalDevice           *physical_devices;
        VkPhysicalDeviceProperties *physical_devices_properties;
        VkPhysicalDeviceFeatures   *physical_device_features;
        VkQueueFamilyProperties    *queue_family_properties;
        VkDeviceQueueCreateInfo    *queue_create_info;
        VkPhysicalDeviceFeatures   *device_features;
        VkDeviceCreateInfo         *device_create_info;

        // Initialized data
        u32                         extension_count                   = 0,
                                    required_extension_count          = 0,
                                    validation_layers_count           = 0,
                                    requested_validation_layers_count = 0,
                                    physical_device_count             = 0,
                                    queue_family_count                = 0,
                                    queue_flags                       = 0;
        

        const char                **extensions                        = 0,
                                  **required_extensions               = 0,
                                  **validation_layers                 = 0; 

        ret->physical_device                   = VK_NULL_HANDLE;
        ret->device                            = VK_NULL_HANDLE;
        ret->instance                          = VK_NULL_HANDLE;

        // Allocate memory for Vulkan structs
        {
            application_info     = calloc(1, sizeof(VkApplicationInfo));
            instance_create_info = calloc(1, sizeof(VkInstanceCreateInfo));
        }

        // Construct the application info struct
        {
            application_info->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            application_info->pApplicationName   = "G10";
            application_info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            application_info->pEngineName        = "G10";
            application_info->engineVersion      = VK_MAKE_VERSION(1, 0, 0);
            application_info->apiVersion         = VK_API_VERSION_1_3;
        }

        // Get extensions
        {

            // Get the required extensions from SDL2
            {
                SDL_Vulkan_GetInstanceExtensions(ret->window, &required_extension_count, 0);
                required_extensions = calloc(required_extension_count, sizeof(char*));
                SDL_Vulkan_GetInstanceExtensions(ret->window, &required_extension_count, required_extensions);
            }

            // Get a list of all extensions from Vulkan
            {
                vkEnumerateInstanceExtensionProperties((void *)0, &extension_count, (void*)0);
                extension_properties = calloc(extension_count, sizeof(VkExtensionProperties));
                vkEnumerateInstanceExtensionProperties((void*)0, &extension_count, extension_properties);
            }

            // DEBUG: List available extensions
            {
                #ifndef NDEBUG
                    g_print_log("[G10] [Vulkan] Available extensions: \n");
                    for (size_t i = 0; i < extension_count; i++)
                        g_print_log("\t%s\n", extension_properties[i].extensionName);
                    putchar('\n');
                #endif
            }
        }

        // Get validation layers
        {

            // Get validation layers
            {
                vkEnumerateInstanceLayerProperties(&validation_layers_count, (void*)0);
                validation_layer_properties = calloc(validation_layers_count, sizeof(VkLayerProperties));
                vkEnumerateInstanceLayerProperties(&validation_layers_count, validation_layer_properties);
            }

            // DEBUG: List available extensions
            {
                #ifndef NDEBUG
                    g_print_log("[G10] [Vulkan] Available validation layers: \n");
                    for (size_t i = 0; i < validation_layers_count; i++)
                        g_print_log("\t%s\n", validation_layer_properties[i].layerName);
                    putchar('\n');
                #endif
            }

            // Check if validation layers are available
            if(requested_validation_layers)
            {
                bool has_requested_validation_layers   = true;

                // Count up requested validation layers
                while (requested_validation_layers[++requested_validation_layers_count]);

                for (size_t i = 0; i < requested_validation_layers_count; i++)
                {
                    bool result = false;

                    for (size_t j = 0; j < validation_layers_count; j++)
                    {
                        if (strcmp(requested_validation_layers[i], validation_layer_properties[j].layerName) == 0)
                            result = true;
                    }
                    
                    has_requested_validation_layers &= result;

                }
            }
        }

        // Construct the instance create info struct
        {
            instance_create_info->sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instance_create_info->pApplicationInfo        = application_info;
            instance_create_info->enabledExtensionCount   = required_extension_count;
            instance_create_info->ppEnabledExtensionNames = required_extensions;
            instance_create_info->enabledLayerCount       = 0;
        }

        // Create the vulkan instance
        {
            VkResult result = vkCreateInstance(instance_create_info, (void*)0, &ret->instance);
            if ( result != VK_SUCCESS)
                goto failed_to_create_instance;

        }

        // Assign a physical device
        {

            // Get physical devices
            {
                vkEnumeratePhysicalDevices(ret->instance, &physical_device_count, (void*)0);
                physical_devices            = calloc(physical_device_count, sizeof(VkPhysicalDevice));
                physical_devices_properties = calloc(physical_device_count, sizeof(VkPhysicalDeviceProperties));
                physical_device_features    = calloc(physical_device_count, sizeof(VkPhysicalDeviceFeatures));
                vkEnumeratePhysicalDevices(ret->instance, &physical_device_count, physical_devices);
            }

            // Get physical device properties
            {
                for (size_t i = 0; i < physical_device_count; i++)
                    vkGetPhysicalDeviceProperties(physical_devices[i], &physical_devices_properties[i]);

                for (size_t i = 0; i < physical_device_count; i++)
                    vkGetPhysicalDeviceFeatures(physical_devices[i], &physical_device_features[i]);

            }

            // DEBUG: List devices and features
            {
                g_print_log("[Vulkan] Available physical devices\n");
                for (size_t i = 0; i < physical_device_count; i++)
                    g_print_log("\t%s\n", physical_devices_properties[i].deviceName);
                putchar('\n');
            }

            // Pick a physical device
            {

                // If the user has specified a device, use that device
                if (requested_physical_device)
                {
                    for (size_t i = 0; i < physical_device_count; i++)
                        if (strcmp(requested_physical_device, physical_devices_properties[i].deviceName) == 0)
                            ret->physical_device = physical_devices[i];
                }

                // If there is only one physical device, default to it
                if (physical_device_count == 1)
                    ret->physical_device = physical_devices[0];

                // If there are multiple devices, pick the best one
                // TODO:

            }

        }

        // Construct the queues
        {

            // Construct queue families
            {

                // Get queue families
                {
                    vkGetPhysicalDeviceQueueFamilyProperties(ret->physical_device, &queue_family_count, (void*)0);
                    queue_family_properties = calloc(queue_family_count, sizeof(VkQueueFamilyProperties));
                    vkGetPhysicalDeviceQueueFamilyProperties(ret->physical_device, &queue_family_count, queue_family_properties);
                }

                queue_flags = queue_family_properties->queueFlags;

                // DEBUG: List queue families
                {
                    #ifndef NDEBUG
                    if (queue_flags & VK_QUEUE_GRAPHICS_BIT)
                        g_print_log("[Vulkan] Physical device supports graphics operations\n");
                    if (queue_flags & VK_QUEUE_COMPUTE_BIT)
                        g_print_log("[Vulkan] Physical device supports compute operations\n");
                    if (queue_flags & VK_QUEUE_TRANSFER_BIT)
                        g_print_log("[Vulkan] Physical device supports transfer operations\n");
                    if (queue_flags & VK_QUEUE_SPARSE_BINDING_BIT)
                        g_print_log("[Vulkan] Physical device supports sparse memory management operations\n");
                    #endif
                }

            }

            // Create queueus
            {
                queue_create_info = calloc(1, sizeof(VkDeviceQueueCreateInfo));
                queue_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queue_create_info->queueFamilyIndex = 0;
                queue_create_info->queueCount = 1;

                ret->priority = 1.f;

                queue_create_info->pQueuePriorities = &ret->priority;
            }


        }

        // Create a logical device
        {
            device_features    = calloc(1, sizeof(VkPhysicalDeviceFeatures));
            device_create_info = calloc(1, sizeof(VkDeviceCreateInfo));

            device_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            device_create_info->pQueueCreateInfos = queue_create_info;
            device_create_info->queueCreateInfoCount = 1;
            device_create_info->pEnabledFeatures = device_features;

            VkResult result = vkCreateDevice(ret->physical_device, device_create_info, (void*)0, &ret->device);

            // Error handling
            if (result != VK_SUCCESS)
                goto failed_to_create_device;

            vkGetDeviceQueue(ret->device, 0, 0, &ret->graphics_queue);
        }
        
        // Create a window surface
        {
            SDL_Vulkan_CreateSurface(ret->window, ret->instance, &ret->surface);
        }
        

    }

    // G10 Initialization
    {
        // Set the name
        ret->name = name;
        ret->loading_thread_count = loading_thread_count;
        // Construct hash tables for materials, parts, and shaders.
        {
            // If no count is specified by the JSON object. Default to 128.
            dict_construct(&ret->cached_materials, ((material_cache_count) ? (material_cache_count) : 128));

            // Default to 128 cached parts.
            dict_construct(&ret->cached_parts, ((material_cache_count) ? (material_cache_count) : 128));

            // Default to 32 cached shaders.
            dict_construct(&ret->cached_materials, ((material_cache_count) ? (material_cache_count) : 32));
        }

        // Initialize input 
        {
            // Create a dictionary for input keys
            {
                extern struct kn_t;

                extern dict *key_dict;
                extern kn_t  keys[];

                // Make a hash table of keys
                dict_construct(&key_dict, 512);

                // Construct the hash table
                for (size_t i = 0; keys[i].code; i++)
                    dict_add(key_dict, keys[i].name, (void*)(keys[i].code));
            }

            // Load user defined input
            if (input)
                ;/// load_input(&ret->input, input);
        }

        dict_construct(&ret->scenes, 16);
    }

    // Display the window
    SDL_ShowWindow(ret->window);

    // Set the instance
    *instance = ret;

    // Set the active instance
    active_instance = ret;


    return 1;

    // DEBUG: Error handling
    {
        
        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }

        // SDL Errors
        {

            // SDL failed to initialize
            noSDL:
            #ifndef NDEBUG
                g_print_error("[SDL2] Failed to initialize SDL\n");
            #endif
            return 0;

            // SDL Image failed to initialize
            noSDLImage:
            #ifndef NDEBUG
                g_print_error("[SDL Image] Failed to initialize SDL Image\n");
            #endif
            return 0;

            // SDL Networking failed to initialize
            noSDLNetwork:
            #ifndef NDEBUG
                g_print_error("[SDL Networking] Failed to initialize SDL Networking\n");
            #endif
            return 0;

            // The SDL window failed to initialize
            noWindow:
            #ifndef NDEBUG
                g_print_error("[SDL2] Failed to create a window\nSDL Says: %s\n", SDL_GetError());
            #endif
            return 0;
        
        }

        // Vulkan errors
        {
            
            failed_to_create_instance:

            #ifndef NDEBUG
                g_print_error("[Vulkan] Failed to create a Vulkan instance\n");
            #endif 
            return 0;
        }

        // TOOD: Categorize
        requested_validation_layers_type_error:
        requested_physical_device_type_error:
        failed_to_create_device:
        requested_load_thread_count_type_error:
        requested_material_count_type_error:
        requested_shader_count_type_error:
            return 0;
    }

}

size_t        g_load_file      ( const char         *path,     void         *buffer   , bool binaryMode )
{

    // Argument checking 
    {
        #ifndef NDEBUG
            if(path==0)
                goto noPath;
        #endif
    }

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binaryMode) ? "rb" : "r");
    
    // Check if file is valid
    if (f == NULL)
        goto invalidFile;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if(buffer)
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    return ret;

    // Error handling
    {
        // Argument errors
        {
            noPath:
            #ifndef NDEBUG
                g_print_error("[G10] Null path provided to funciton \"%s\\n",__FUNCSIG__);
            #endif
            return 0;
        }

        // File errors
        {
            invalidFile:
            #ifndef NDEBUG
                g_print_error("[G10] Failed to load file \"%s\"\n[Standard library] %s\n",path, strerror(errno));
            #endif
            return 0;
        }
    }
}

int           g_print_error    ( const char *const   format, ... ) 
{

    // Argument check
    {
        if (format == (void*)0)
            goto no_format;
    }

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to red, 
    // print the message, and restore the color.
    printf("\033[91m");

    // TODO: Configure error messages to write to other files?
    vfprintf(log_file, format, aList);
    printf("\033[0m");

    va_end(aList);

    return 0;
    
    // Error handling
    {

        // Argument errors
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }


    }
}

int           g_print_warning  ( const char *const   format, ... ) 
{

    // Argument check
    {
        if (format == (void*)0)
            goto no_format;
    }

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to yellow, 
    // print the message, and restore the color.
    printf("\033[93m");
    vfprintf(log_file, format, aList);
    printf("\033[0m");

    va_end(aList);

    return 0;

    // Error handling
    {

        // Argument check
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }

}

int           g_print_log      ( const char *const   format, ... ) 
{
    // Argument check
    {
        if (format == (void*)0)
            goto no_format;
    }

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    // Uses ANSI terminal escapes to set the color to light blue, 
    // print the message, and restore the color.
    printf("\033[94m");
    vfprintf(log_file, format, aList);
    printf("\033[0m");

    va_end(aList);
    va_end(aList);

    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }

}

GXInstance_t *g_get_active_instance ( void )
{
    return active_instance;
}

int           g_cache_material ( GXInstance_t        *instance, GXMaterial_t *material )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (material == (void*)0)
                goto no_material;
        #endif
    }

    dict_add(instance->cached_materials, material->name, material);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_material:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

    }
}

int           g_cache_part     ( GXInstance_t       *instance, GXPart_t     *part)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (part == (void*)0)
                goto no_part;
        #endif
    }

    //dict_add(instance->cached_parts, part->name, part);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_part:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"part\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

    }
}

int           g_cache_shader   ( GXInstance_t       *instance, GXShader_t   *shader)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (shader == (void*)0)
                goto no_shader;
        #endif
    }

    //dict_add(instance->cached_shaders, shader->name, shader);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_shader:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"shader\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

    }
}

GXMaterial_t *g_find_material  ( GXInstance_t       *instance, char         *name )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    return dict_get(instance->cached_materials, name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

    }
}

GXPart_t     *g_find_part      ( GXInstance_t       *instance, char         *name)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    return dict_get(instance->cached_parts, name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

    }
}

GXShader_t   *g_find_shader    ( GXInstance_t       *instance, char         *name)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    return dict_get(instance->cached_shaders, name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return 0;
        }

    }
}

int           g_exit           ( GXInstance_t       *instance )
{

    // Argument check
    {
        if (instance == (void*)0)
            goto no_instance;
    }

    // G10 Cleanup
    {
        free(instance->name);
    }

    // Vulkan cleanup
    {
        vkDestroySurfaceKHR(instance->instance, instance->surface, (void*)0);
        vkDestroyInstance(instance->instance, (void *)0);
        vkDestroyDevice(instance->device, (void *)0);
    }

    // SDL Cleanup
    {
        SDL_DestroyWindow(instance->window);
        SDL_Quit();
    }
    
    free(instance);
    
    return 0;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
                #endif
                return 0;
        }
    }
}

