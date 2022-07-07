#include <G10\G10.h>

static GXInstance_t *active_instance = 0;
FILE                *log_file;

void create_vulkan_instance(void);
void create_surface(void);
void pick_physical_device(void);
void create_logical_device(void);
void create_swap_chain(void);
int check_vulkan_device(GXInstance_t* instance, VkPhysicalDevice physical_device, char** required_extension_names);

int           g_init                 ( GXInstance_t      **instance, const char   *path)
{
    // Argument Check
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
                 *name                        = "Instance",
                 *window_title                = "G10",
                **requested_validation_layers = 0,
                **instance_extensions         = 0,
                **device_extensions           = 0,
                 *requested_physical_device   = 0,
                 *initial_scene               = 0,
                 *log_file_i                  = 0,
                 *input                       = 0;
    size_t        window_width                = 0,
                  window_height               = 0;
    bool          fullscreen                  = false;


    // Instance initialization
    {
        // Set the instance
        *instance = ret;

        // Set the active instance
        active_instance = ret;
    }

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
        name                  = JSON_VALUE(token, JSONstring);

        // Window 
        {
            // Set the window width. Default to 800
            token         = dict_get(instance_json_object, "window width");
            window_width  = (size_t) JSON_VALUE(token, JSONprimative);
            window_width  = (window_width) ? atol((char*)window_width) : 1280;

            // Set the window height. Default to 600
            token         = dict_get(instance_json_object, "window height");
            window_height = JSON_VALUE(token, JSONprimative);
            window_height = (window_height) ? atol(window_height) : 720;

            // Set the window title. Default to "G10"
            token         = dict_get(instance_json_object, "window title");
            window_title  = JSON_VALUE(token, JSONstring);
            window_title  = (window_title) ? window_title : "G10";

            // Fullscreen? Default to false
            token         = dict_get(instance_json_object, "fullscreen");
            fullscreen    = JSON_VALUE(token, JSONprimative);

        }
        
        // Input
        token                = dict_get(instance_json_object, "input");
        input                = (token) ? token->value.n_where : 0;

        // Log file
        token                = dict_get(instance_json_object, "log file");
        log_file_i           = JSON_VALUE(token, JSONstring);

        // Initial scene path
        token                = dict_get(instance_json_object, "initial scene");
        initial_scene        = JSON_VALUE(token, JSONstring);

        // Caches
        {

            // Set the part cache limit
            token                = dict_get(instance_json_object, "cache part count");
            part_cache_count     = JSON_VALUE(token, JSONprimative);
            part_cache_count     = (part_cache_count) ? atol(part_cache_count) : 128;

            // Set the material cache limit
            token                = dict_get(instance_json_object, "cache material count");
            material_cache_count = JSON_VALUE(token, JSONprimative);
            material_cache_count = (material_cache_count) ? atol(material_cache_count) : 128;

            // Set the shader cache limit
            token                = dict_get(instance_json_object, "cache shader count");
            shader_cache_count   = JSON_VALUE(token, JSONprimative);
            shader_cache_count   = (shader_cache_count) ? atol(shader_cache_count) : 32;

        }

        // Loading thread count
        token                = dict_get(instance_json_object, "loading thread count");
        loading_thread_count = JSON_VALUE(token, JSONprimative);
        loading_thread_count = (loading_thread_count) ? atol(loading_thread_count) : 4;

        // Vulkan
        {

            // Validation layers
            {
                token                       = dict_get(instance_json_object, "vulkan validation layers");
                requested_validation_layers = JSON_VALUE(token, JSONarray);
            }

            // Requested instance extensions
            {
                token                       = dict_get(instance_json_object, "vulkan instance extensions");
                instance_extensions         = JSON_VALUE(token, JSONarray);
            }

            // Requested device extensions
            {
                token = dict_get(instance_json_object, "vulkan device extensions");
                device_extensions = JSON_VALUE(token, JSONarray);
            }

            // Physical device
            {
                token                     = dict_get(instance_json_object, "vulkan physical device");
                requested_physical_device = JSON_VALUE(token, JSONstring);
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
            (int)window_width, (int)window_height,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | ((fullscreen) ? SDL_WINDOW_FULLSCREEN : 0) ); 

        // Check the window
        if (!ret->window)
            goto noWindow;


    }

    // Vulkan initialization
    {
        create_vulkan_instance();
        create_surface();
        pick_physical_device(device_extensions);
        create_logical_device(device_extensions);
    }

    // G10 Initialization
    {

        // Set the name
        ret->name = name;
        ret->loading_thread_count = loading_thread_count;

        // Subsystem initialization
        {

            // Texture initialization
            {

                // Get an external function
                extern int init_texture ( void );

                // Run the texture system initializer
                init_texture();
            }

            // Input initialization
            {
                
                // Get an external function
                extern int init_input(void);

                // Run the input system initializer
                init_input();
            }

            // Material initialization
            {
                GXMaterial_t* missing_material = 0;

                //load_material(&missing_material, "G10/missing material.json");
            }

        }

        // Construct dictionaries to cache materials, parts, and shaders.
        {
            // If no count is specified by the JSON object. Default to 128.
            dict_construct(&ret->cached_materials, ((material_cache_count) ? (material_cache_count) : 128));

            // Default to 128 cached parts.
            dict_construct(&ret->cached_parts, ((material_cache_count) ? (material_cache_count) : 128));

            // Default to 32 cached shaders.
            dict_construct(&ret->cached_materials, ((material_cache_count) ? (material_cache_count) : 32));
        }

        // Load an input set
        {
            // Load user defined input
            if (input)
                load_input(&ret->input, input);
        }

        // Scene dictionary
        dict_construct(&ret->scenes, 16);

        // Load the initial scene
        if(initial_scene) {
            GXScene_t* scene = 0;
            load_scene(&scene, initial_scene);

            if (scene)
                dict_add(ret->scenes, scene->name, scene);
            else
                goto no_initial_scene;
        }

    }

    // Display the window
    SDL_ShowWindow(ret->window);

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
        no_swap_chain:
        no_instance:
        missing_layer:
        no_debug_messenger:
        no_device:
        no_surface:
            return 0;
        no_initial_scene:
            g_print_error("[G10] Failed to load scene \"%s\" in call to function \"%s\"\n", initial_scene, __FUNCSIG__);
            return 0;
    }

}

void create_vulkan_instance ( void )
{
    GXInstance_t          *instance                  = g_get_active_instance();

    VkApplicationInfo     *application_info          = calloc(1, sizeof(VkApplicationInfo));
    VkInstanceCreateInfo  *create_info               = calloc(1, sizeof(VkInstanceCreateInfo));
    VkDebugUtilsMessengerCreateInfoEXT *debug_create_info = calloc(1, sizeof(VkDebugUtilsMessengerCreateInfoEXT));;
    char                 **required_extensions       = 0,
                         **requested_extensions      = 0;
    dict                  *extensions                = 0;
    u32                    required_extension_count  = 0,
                           requested_extension_count = 0;
    VkResult               result                    = 0;
    
    // Populate application info struct
    {
        application_info->sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        application_info->pApplicationName   = "G10";
        application_info->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        application_info->pEngineName        = "G10";
        application_info->engineVersion      = VK_MAKE_VERSION(1, 0, 0);
        application_info->apiVersion         = VK_API_VERSION_1_3;
    }

    // Populate instance create info struct
    {
        create_info->sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info->pApplicationInfo = application_info;
        
        // Get a list of the required instance extensions from SDL
        SDL_Vulkan_GetInstanceExtensions(instance->window, &required_extension_count, (void *) 0);
        required_extensions = calloc(required_extension_count + 1, sizeof(char*));
        SDL_Vulkan_GetInstanceExtensions(instance->window, &required_extension_count, required_extensions);

        // Construct a dictionary from the required extensions
        dict_from_keys(&extensions, required_extensions, required_extension_count);

        // TODO: Add support for user defined extensions

        
        

        create_info->enabledExtensionCount  = required_extension_count;
        create_info->ppEnabledExtensionNames = required_extensions;
    }
    
    

    result = vkCreateInstance(create_info, (void*)0, &instance->instance);
    
}

void create_surface ( void )
{
    GXInstance_t *instance = g_get_active_instance();
    SDL_Vulkan_CreateSurface(instance->window, instance->instance, &instance->surface);

}

void pick_physical_device ( char **required_extension_names )
{
    GXInstance_t* instance = g_get_active_instance();

    u32 device_count = 0;
    VkPhysicalDevice *devices = 0;
    vkEnumeratePhysicalDevices(instance->instance, &device_count, (void*)0);

    // Error check
    if (device_count == 0)
        goto no_devices;

    devices = calloc(device_count + 1, sizeof(VkPhysicalDevice));

    vkEnumeratePhysicalDevices(instance->instance, &device_count, devices);

    for (size_t i = 0; i < device_count; i++)
    {
        if (check_vulkan_device(instance, devices[i], required_extension_names))
            instance->physical_device = devices[i];
    }

no_devices:;

}

void create_logical_device ( char **required_extension_names )
{
    GXInstance_t             *instance = g_get_active_instance();
    VkDeviceQueueCreateInfo  *queue_create_infos = calloc(2, sizeof(VkDeviceQueueCreateInfo));
    VkPhysicalDeviceFeatures *device_features = calloc(1, sizeof(VkPhysicalDeviceFeatures));
    VkDeviceCreateInfo* device_create_info = calloc(1, sizeof(VkDeviceCreateInfo));
    VkBool32 result = 0;
    u32 unique_queue_families[2] = { instance->queue_family_indices.g, instance->queue_family_indices.p };

    instance->priority = 1.f;

    for (size_t i = 0; i < 2; i++)
    {
        VkDeviceQueueCreateInfo iter = queue_create_infos[i];

        iter.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        iter.queueFamilyIndex = unique_queue_families[i];
        iter.queueCount = 1;
        iter.pQueuePriorities = &instance->priority;
    }

    device_create_info->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info->queueCreateInfoCount = 2;
    device_create_info->pQueueCreateInfos = queue_create_infos;
    device_create_info->enabledExtensionCount = 1;
    device_create_info->pEnabledFeatures = device_features;

    device_create_info->ppEnabledExtensionNames = required_extension_names;
    
    result = vkCreateDevice(instance->physical_device, device_create_info, 0, &instance->device);

    if (result != VK_SUCCESS) {
        printf("failed to create logical device!");
    }


    vkGetDeviceQueue(instance->device, unique_queue_families[0], 0, &instance->graphics_queue);
    vkGetDeviceQueue(instance->device, unique_queue_families[1], 0, &instance->present_queue);

} 

void create_swap_chain ( void )
{

}

int check_vulkan_device  ( GXInstance_t *instance, VkPhysicalDevice physical_device, char **required_extension_names )
{

    bool passes_queue   = false,
         has_extensions = false,
         has_swap_chain = false;
    
    // Check queue indices
    {
        struct { u32 g, p; }     indices = { 0, 0 };
        u32                      queue_family_count = 0;
        VkQueueFamilyProperties* queue_families = 0;

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, (void*)0);

        queue_families = calloc(queue_family_count + 1, sizeof(VkQueueFamilyProperties));

        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families);

        for (size_t i = 0; i < queue_family_count; i++)
        {
            VkBool32 present_support = 0;

            passes_queue = true;

            if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
                indices.g = i;
            else
                passes_queue = false;
            
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, instance->surface, &present_support);

            if (present_support)
                indices.p = i;
            else
                passes_queue = false;

            if (passes_queue)
                break;
        }

        instance->queue_family_indices.g = indices.g;
        instance->queue_family_indices.p = indices.p;
    }

    // Check device extensions
    if(required_extension_names) {
        u32                    device_extension_count = 0;
        VkExtensionProperties *available_device_extensions = 0;
        
        dict                  *device_extension_name_dict = 0;

        vkEnumerateDeviceExtensionProperties(physical_device, (void*)0, &device_extension_count, (void*)0);
        
        available_device_extensions = calloc(device_extension_count + 1, sizeof(VkExtensionProperties));
        
        vkEnumerateDeviceExtensionProperties(physical_device, (void*)0, &device_extension_count, available_device_extensions);

        dict_from_keys(&device_extension_name_dict, required_extension_names, 32);

        char** available_device_extension_names = 0;

        available_device_extension_names = calloc(device_extension_count + 1, sizeof(char*));

        dict_keys(device_extension_name_dict, available_device_extension_names);

        for (size_t i = 0; i < required_extension_names[i]; i++)
        {
            for (size_t j = 0; j < device_extension_count; j++)
            {
                if (strcmp(required_extension_names[i], available_device_extension_names[j]) == 0)
                {
                    dict_pop(device_extension_name_dict, required_extension_names[i], 0);
                    break;
                }
            }
        }

        if (dict_keys(device_extension_name_dict, 0) == 0)
            has_extensions = true;
         
    }
    else
        has_extensions = true;

    // Check for swap chain support
    {
        if (has_extensions)
        {
            u32 format_count       = 0,
                present_mode_count = 0;

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, instance->surface, &instance->swap_chain_details.capabilities);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, instance->surface, &format_count, (void*)0);

            instance->swap_chain_details.formats = calloc(format_count + 1, sizeof(VkSurfaceFormatKHR));

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, instance->surface, &format_count, instance->swap_chain_details.formats);

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, instance->surface, &present_mode_count, (void *)0);

            instance->swap_chain_details.present_modes = calloc(present_mode_count + 1, sizeof(VkPresentModeKHR));

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, instance->surface, &present_mode_count, instance->swap_chain_details.present_modes);
            
            if ((bool)(present_mode_count)& (bool)(format_count)==true)
            {
                has_swap_chain = true;
            }

        }
    }

    return passes_queue && has_extensions && has_swap_chain;
}

size_t        g_load_file            ( const char         *path,     void         *buffer   , bool binaryMode )
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

int           g_print_error          ( const char *const   format, ... ) 
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

int           g_print_warning        ( const char *const   format, ... ) 
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

int           g_print_log            ( const char *const   format, ... ) 
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

GXInstance_t *g_get_active_instance  ( void )
{
    return active_instance;
}

void          g_create_vulkan_buffer ( VkDeviceSize        size    , VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *bufferMemory) {
     
    GXInstance_t         *instance        = g_get_active_instance();
    VkBufferCreateInfo   *buffer_info     = 0;
    VkMemoryRequirements  memRequirements;
    VkMemoryAllocateInfo *alloc_info      = calloc(1, sizeof(VkMemoryAllocateInfo));

    buffer_info->sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info->size        = size;
    buffer_info->usage       = usage;
    buffer_info->sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(instance->device, &buffer_info, (void *)0, &buffer) != VK_SUCCESS) {
    
        // ERROR
    }

//    vkGetBufferMemoryRequirements(instance->device, buffer, &memRequirements);

//    alloc_info->sType                = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//    alloc_info->allocationSize       = memRequirements.size;
//    alloc_info->memoryTypeIndex      = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(instance->device, &alloc_info, (void *)0, &bufferMemory) != VK_SUCCESS) {
        // Error handling
    }

    vkBindBufferMemory(instance->device, buffer, bufferMemory, 0);
}

int           g_cache_material       ( GXInstance_t       *instance, GXMaterial_t *material )
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

int           g_cache_part           ( GXInstance_t       *instance, GXPart_t     *part)
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

int           g_cache_shader         ( GXInstance_t       *instance, GXShader_t   *shader)
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

GXMaterial_t *g_find_material        ( GXInstance_t       *instance, char         *name )
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

GXPart_t     *g_find_part            ( GXInstance_t       *instance, char         *name)
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

GXShader_t   *g_find_shader          ( GXInstance_t       *instance, char         *name)
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

int           g_exit                 ( GXInstance_t       *instance )
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
        
        vkDestroySwapchainKHR(instance->device, instance->swap_chain, (void*)0);
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

