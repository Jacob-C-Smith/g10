#include <G10\G10.h>

static GXInstance_t *active_instance = 0;
FILE                *log_file;

void create_vulkan_instance(char **requested_instance_extensions, char **requested_validation_layers);
void create_surface(void);
void setup_debug_messenger(void);
void pick_physical_device(char** required_extension_names);
void create_logical_device(char** required_extension_names);
void create_swap_chain(void);
void create_image_views();
void create_render_pass();
void create_framebuffers();
void create_command_pool();
void create_command_buffer();
void record_command_buffer(VkCommandBuffer command_buffer, uint32_t image_index);
void create_sync_objects();

int  check_vulkan_device(GXInstance_t* instance, VkPhysicalDevice physical_device, char** required_extension_names);

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            printf("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            g_print_log("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            g_print_warning("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            g_print_error("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
            break;
    }

    return VK_FALSE;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != 0) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

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
    GXInstance_t *ret                           = calloc(1, sizeof(GXInstance_t));
    size_t        token_text_len                = g_load_file(path, 0, false),
                  token_count                   = 0,
                  part_cache_count              = 32,
                  material_cache_count          = 32,
                  shader_cache_count            = 8,
                  loading_thread_count          = 4;
    dict         *instance_json_object          = 0;
    char         *token_text                    = calloc(token_text_len, sizeof(u8)),
                 *name                          = "Instance",
                 *window_title                  = "G10",
                **requested_validation_layers   = 0,
                **requested_instance_extensions = 0,
                **device_extensions             = 0,
                 *requested_physical_device     = 0,
                 *max_buffered_frames           = 0,
                 *initial_scene                 = 0,
                 *log_file_i                    = 0,
                 *input                         = 0,
                 *scheduler                     = 0;
    size_t        window_width                  = 0,
                  window_height                 = 0;
    bool          fullscreen                    = false;

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
                token                         = dict_get(instance_json_object, "vulkan instance extensions");
                requested_instance_extensions = JSON_VALUE(token, JSONarray);
            }

            // Requested device extensions
            {
                token             = dict_get(instance_json_object, "vulkan device extensions");
                device_extensions = JSON_VALUE(token, JSONarray);
            }

            // Physical device
            {
                token                     = dict_get(instance_json_object, "vulkan physical device");
                requested_physical_device = JSON_VALUE(token, JSONstring);
            }

            // Max buffered frames
            {
                token                     = dict_get(instance_json_object, "max buffered frames");
                max_buffered_frames       = JSON_VALUE(token, JSONprimative);
            }
        }

        // Scheduler
        {
            token = dict_get(instance_json_object, "scheduler");
            scheduler = JSON_VALUE(token, JSONobject);
        }

    }

    
    // Instance initialization
    {
        // Set the instance
        *instance = ret;

        // Set the active instance
        active_instance = ret;

        ret->window_width = window_width;
        ret->window_height = window_height;

    }


    // Set the log file before doing anything else
    {
        if   (log_file_i) { log_file = fopen(log_file_i, "w"); }
        else              { log_file = stdout; }
    }

    // Global initialization
    {

        // SDL initialization
        {

            // Initialize SDL
            if (SDL_Init(SDL_INIT_EVERYTHING))
                goto noSDL;

            // Create the window
            ret->window = SDL_CreateWindow(window_title,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                (int)window_width, (int)window_height,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | ((fullscreen) ? SDL_WINDOW_FULLSCREEN : 0));

            // Check the window
            if (!ret->window)
                goto noWindow;

            // Display the window
            SDL_ShowWindow(ret->window);

        }

        // Vulkan initialization
        {
            create_vulkan_instance(requested_instance_extensions, requested_validation_layers);
            create_surface();
            pick_physical_device(device_extensions);
            create_logical_device(device_extensions);
            create_swap_chain();
            create_render_pass();
            create_image_views();
            create_render_pass();
            create_framebuffers();
            create_command_pool();
            create_command_buffer();
            create_sync_objects();
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
                    extern int init_texture(void);

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

                // Audio initialization
                {

                    // Get an external function
                    extern int init_audio(void);

                    // Run the audio system initializer
                    init_audio();
                }

                // Material initialization
                {
                    GXMaterial_t* missing_material = 0;

                    //load_material(&missing_material, "G10/missing material.json");
                }

                // Scheduler initialization
                {
                    extern void init_scheduler( void );
                    init_scheduler();
                    
                }

            }

            // Construct dictionaries to cache materials, parts, and shaders.
            {
                // If no count is specified by the JSON object. Default to 128.
                dict_construct(&ret->cached_materials, ((material_cache_count) ? (material_cache_count) : 128));

                // Default to 128 cached parts.
                dict_construct(&ret->cached_parts, ((material_cache_count) ? (material_cache_count) : 128));

                // Default to 32 cached shaders.
                dict_construct(&ret->cached_shaders, ((shader_cache_count) ? (shader_cache_count) : 32));
            }

            // Load an input set
            {
                // Load user defined input
                if (input)
                    load_input(&ret->input, input);
            }

            // Load a schedule
            {
                load_scheduler_as_json(&ret->scheduler, scheduler, strlen(scheduler));
            }

            // Scene dictionary
            dict_construct(&ret->scenes, 16);

            // Load the initial scene
            if (initial_scene) {
                GXScene_t* scene = 0;
                load_scene(&scene, initial_scene);

                if (scene)
                    dict_add(ret->scenes, scene->name, scene);
                else
                    goto no_initial_scene;
            }

        }
    }
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

void create_vulkan_instance ( char **requested_instance_extensions, char **requested_validation_layers )
{
    GXInstance_t          *instance                  = g_get_active_instance();

    VkApplicationInfo     *application_info          = calloc(1, sizeof(VkApplicationInfo));
    VkInstanceCreateInfo  *instance_create_info               = calloc(1, sizeof(VkInstanceCreateInfo));
    VkDebugUtilsMessengerCreateInfoEXT *debug_create_info = calloc(1, sizeof(VkDebugUtilsMessengerCreateInfoEXT));;
    char                 **required_extensions       = 0,
                         **requested_extensions      = 0;
    dict                  *extensions                = 0;
    u32                    required_extension_count  = 0,
                           requested_extension_count = 0,
                           requested_layers_count    = 0;
    
    VkResult               result                    = 0;
    
    while (requested_instance_extensions[++requested_extension_count]);

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
        instance_create_info->sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_create_info->pApplicationInfo = application_info;
        
        // Get a list of the required instance extensions from SDL
        SDL_Vulkan_GetInstanceExtensions(instance->window, &required_extension_count, (void *) 0);
        required_extensions = calloc(required_extension_count + 1, sizeof(char*));
        SDL_Vulkan_GetInstanceExtensions(instance->window, &required_extension_count, required_extensions);

        // Construct a dictionary from the required extensions
        dict_from_keys(&extensions, required_extensions, required_extension_count);

        // TODO: Add support for user defined extensions
        instance_create_info->enabledExtensionCount   = required_extension_count;
        instance_create_info->ppEnabledExtensionNames = required_extensions;

        while (requested_validation_layers[++requested_layers_count]);

        instance_create_info->enabledLayerCount   = requested_layers_count;
        instance_create_info->ppEnabledLayerNames = requested_validation_layers;

        setup_debug_messenger(&instance_create_info->pNext);
    }
    
    

    result = vkCreateInstance(instance_create_info, (void*)0, &instance->instance);
    
}

void setup_debug_messenger ( VkDebugUtilsMessengerCreateInfoEXT **debug_messenger_create_info )
{
    VkDebugUtilsMessengerCreateInfoEXT *i_debug_messenger_create_info = calloc(1, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

    i_debug_messenger_create_info->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    i_debug_messenger_create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    i_debug_messenger_create_info->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    i_debug_messenger_create_info->pfnUserCallback = debug_callback;

    *debug_messenger_create_info = i_debug_messenger_create_info;
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
    GXInstance_t             *instance                 = g_get_active_instance();
    VkDeviceQueueCreateInfo  *queue_create_infos       = calloc(2, sizeof(VkDeviceQueueCreateInfo));
    VkPhysicalDeviceFeatures *device_features          = calloc(1, sizeof(VkPhysicalDeviceFeatures));
    VkDeviceCreateInfo       *device_create_info       = calloc(1, sizeof(VkDeviceCreateInfo));
    VkBool32                  result                   = 0;
    u32                       unique_queue_families[2] = { instance->queue_family_indices.g, instance->queue_family_indices.p };

    instance->priority = 1.f;

    for (size_t i = 0; i < 2; i++)
    {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = i;
        queue_create_infos[i].queueCount = 1;
        queue_create_infos[i].pQueuePriorities = &instance->priority;
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

    // Uninitialized data
    VkSurfaceFormatKHR        surface_format;
    VkPresentModeKHR          present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
    VkExtent2D                extent;

    // Initialized data
    GXInstance_t             *instance               = g_get_active_instance();
    VkSwapchainCreateInfoKHR *swapchain_create_info  = calloc(1, sizeof(VkSwapchainCreateInfoKHR));
    u32                       qfi[2]                 = { 0, 0 };
    
    qfi[0] = instance->queue_family_indices.g;
    qfi[1] = instance->queue_family_indices.p;

    surface_format = instance->swap_chain_details.formats[0];

    for (size_t i = 0; i < 2; i++)
        if (instance->swap_chain_details.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && instance->swap_chain_details.formats[i].format == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            surface_format = instance->swap_chain_details.formats[i];
    
    extent.width  = instance->window_width;
    extent.height = instance->window_height;

    // TODO: Check memory
    swapchain_create_info->sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info->surface          = instance->surface;
    swapchain_create_info->minImageCount    = 2;
    swapchain_create_info->imageFormat      = surface_format.format;
    swapchain_create_info->imageColorSpace  = surface_format.colorSpace;
    swapchain_create_info->imageExtent      = extent;
    swapchain_create_info->imageArrayLayers = 1;
    swapchain_create_info->imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    if (instance->queue_family_indices.g != instance->queue_family_indices.p)
    {
        swapchain_create_info->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info->queueFamilyIndexCount = 2;
        swapchain_create_info->pQueueFamilyIndices = &qfi;
    }
    else
        swapchain_create_info->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;


    swapchain_create_info->preTransform   = instance->swap_chain_details.capabilities.currentTransform;
    swapchain_create_info->compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info->presentMode    = present_mode;
    swapchain_create_info->clipped        = VK_TRUE;

    swapchain_create_info->oldSwapchain   = VK_NULL_HANDLE;


    if (vkCreateSwapchainKHR(instance->device, swapchain_create_info, 0, &instance->swap_chain))
        // TODO: Replace with goto
        printf("FAILED TO CREATE SWAPCHAIN");

    vkGetSwapchainImagesKHR(instance->device, instance->swap_chain, &instance->image_count, 0);

    instance->swap_chain_images = calloc(instance->image_count, sizeof(VkImage));

    vkGetSwapchainImagesKHR(instance->device, instance->swap_chain, &instance->image_count, instance->swap_chain_images);

    instance->swap_chain_image_format = surface_format.format;
    instance->swap_chain_extent       = extent;
}

void create_image_views() {
    GXInstance_t          *instance = g_get_active_instance();
    
    instance->swap_chain_image_views = calloc(instance->image_count, sizeof(VkImageView));
    
    for (size_t i = 0; i < instance->image_count; i++)
    {
        VkImageViewCreateInfo* image_view_create_info = calloc(1, sizeof(VkImageViewCreateInfo));

        image_view_create_info->sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info->image = instance->swap_chain_images[i];
        image_view_create_info->viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info->format = instance->swap_chain_image_format;

        image_view_create_info->components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info->components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info->components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info->components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        image_view_create_info->subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info->subresourceRange.baseMipLevel = 0;
        image_view_create_info->subresourceRange.levelCount = 1;
        image_view_create_info->subresourceRange.baseArrayLayer = 0;
        image_view_create_info->subresourceRange.layerCount = 1;

        image_view_create_info->flags = 0;

        image_view_create_info->pNext = 0;

        if (vkCreateImageView(instance->device, image_view_create_info, 0, &instance->swap_chain_image_views[i]) != VK_SUCCESS) {
            g_print_error("failed to create image views!\n");
        }
        
    }
}

void create_render_pass() {
    GXInstance_t *instance = g_get_active_instance();
    VkAttachmentDescription *color_attachment = calloc(1, sizeof(VkAttachmentDescription));
    VkAttachmentReference   *color_attachment_reference = calloc(1, sizeof(VkAttachmentReference));
    VkSubpassDescription    *subpass                    = calloc(1, sizeof(VkSubpassDescription));
    VkRenderPassCreateInfo  *render_pass_create_info    = calloc(1, sizeof(VkRenderPassCreateInfo));

    color_attachment->format = instance->swap_chain_image_format;
    color_attachment->samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment->loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment->storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment->stencilLoadOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment->stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment->initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment->finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    color_attachment_reference->attachment = 0;
    color_attachment_reference->layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    //The index of the attachment in this array is directly referenced from the fragment shader with the layout(location = 0) out vec4 outColor directive!
    subpass->pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass->colorAttachmentCount = 1;
    subpass->pColorAttachments = color_attachment_reference;

    render_pass_create_info->sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info->attachmentCount = 1;
    render_pass_create_info->pAttachments = color_attachment;
    render_pass_create_info->subpassCount = 1;
    render_pass_create_info->pSubpasses = subpass;

    if (vkCreateRenderPass(instance->device, render_pass_create_info, 0, &instance->render_pass) != VK_SUCCESS) {
        g_print_error("failed to create render pass!\n");
    }
}

void create_framebuffers()
{
    GXInstance_t* instance = g_get_active_instance();
    instance->swap_chain_framebuffers = calloc(instance->image_count, sizeof(VkFramebuffer));
    for (size_t i = 0; i < instance->image_count; i++)
    {
        VkImageView attachments[1] = {
            instance->swap_chain_image_views[i]
        };

        VkFramebufferCreateInfo* framebuffer_create_info = calloc(1, sizeof(VkFramebufferCreateInfo));
        framebuffer_create_info->sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info->renderPass = instance->render_pass;
        framebuffer_create_info->attachmentCount = 1;
        framebuffer_create_info->pAttachments = attachments;
        framebuffer_create_info->width = instance->swap_chain_extent.width;
        framebuffer_create_info->height = instance->swap_chain_extent.height;
        framebuffer_create_info->layers = 1;

        if (vkCreateFramebuffer(instance->device, framebuffer_create_info, 0, &instance->swap_chain_framebuffers[i]) != VK_SUCCESS)
            g_print_error("Failed to create framebuffer!\n");
    }
}

void create_command_pool()
{
    GXInstance_t* instance = g_get_active_instance();
    VkCommandPoolCreateInfo* pool_create_info = calloc(1, sizeof(VkCommandPoolCreateInfo));

    pool_create_info->sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info->flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_create_info->queueFamilyIndex = instance->queue_family_indices.g;

    if (vkCreateCommandPool(instance->device, pool_create_info, 0, &instance->command_pool)!=VK_SUCCESS)
        g_print_error("Failed to create command pool!\n");
}

void create_command_buffer()
{
    GXInstance_t *instance = g_get_active_instance();

    VkCommandBufferAllocateInfo* alloc_info = calloc(1, sizeof(VkCommandBufferAllocateInfo));

    alloc_info->sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info->commandPool        = instance->command_pool;
    alloc_info->level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info->commandBufferCount = 1;

    if (vkAllocateCommandBuffers(instance->device, alloc_info, &instance->command_buffer) != VK_SUCCESS) {
        g_print_error("failed to allocate command buffers!\n");
    }
}

void create_sync_objects()
{
    GXInstance_t *instance = g_get_active_instance();

    VkSemaphoreCreateInfo *semaphore_create_info = calloc(1, sizeof(VkSemaphoreCreateInfo));
    VkFenceCreateInfo     *fence_create_info     = calloc(1, sizeof(VkFenceCreateInfo));

    semaphore_create_info->sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    fence_create_info->sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info->flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateSemaphore(instance->device, semaphore_create_info, 0, &instance->image_available_semaphores) != VK_SUCCESS ||
        vkCreateSemaphore(instance->device, semaphore_create_info, 0, &instance->render_finished_semaphores) != VK_SUCCESS ||
        vkCreateFence(instance->device, fence_create_info, 0, &instance->in_flight_fences) != VK_SUCCESS) {
        g_print_error("failed to create semaphores!\n");
    }

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
            
            if ((bool)(present_mode_count) & (bool)(format_count)==true)
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

int           render_frame           ( GXInstance_t        *instance )
{

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

void          g_user_exit(callback_parameter_t* input, GXInstance_t* instance)
{
    instance->running = false;
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
        //free(instance->name);
    }

    // Vulkan cleanup
    {
        
        vkDestroySwapchainKHR(instance->device, instance->swap_chain, (void*)0);
        vkDestroySurfaceKHR(instance->instance, instance->surface, (void*)0);
        vkDestroyDevice(instance->device, (void*)0);
        vkDestroyInstance(instance->instance, (void*)0);
    }

    // SDL Cleanup
    {
        SDL_DestroyWindow(instance->window);
        SDL_Quit();
    }
    
    free(instance);
    
    g_print_log("[G10] Exit successful\n");

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

