#include <G10/G10.h>

// Uninitialized data
FILE* log_file;

// Initialized data
static GXInstance_t *active_instance = 0;

// Forward declared functions
void pick_physical_device   ( char        **required_extension_names );
void create_logical_device  ( char        **required_extension_names );
void create_swap_chain      ( void );
void cleanup_swap_chain     ( void );
void create_image_views     ( void );
void create_command_pool    ( void );
void create_command_buffers ( void );
void create_sync_objects    ( void );

void create_buffer          ( VkDeviceSize  size       , VkBufferUsageFlags    usage     , VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory );
u32  find_memory_type       ( u32           type_filter, VkMemoryPropertyFlags properties );
int  check_vulkan_device    ( GXInstance_t *instance, VkPhysicalDevice physical_device, char** required_extension_names);

VKAPI_ATTR 
VkBool32
VKAPI_CALL    debug_callback               ( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

    switch (messageSeverity)
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            //printf("[Vulkan] [Validation layer] %s\n\n", pCallbackData->pMessage);
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
void          setup_debug_messenger(VkDebugUtilsMessengerCreateInfoEXT** debug_messenger_create_info);
VkResult      CreateDebugUtilsMessengerEXT ( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != 0) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

int           g_init                       ( GXInstance_t      **pp_instance, const char   *path          )
{

    // Argument Check
    {
        #ifndef NDEBUG
            if ( pp_instance == (void *) 0 )
                goto no_instance;
            if ( path        == (void *) 0 )
                goto no_path;
        #endif
    }

    // Initialized data
    GXInstance_t  *ret                           = 0;
    size_t         token_text_len                = g_load_file(path, 0, false),
                   token_count                   = 0,
                   part_cache_count              = 0,
                   material_cache_count          = 0,
                   shader_cache_count            = 0,
                   ai_cache_count                = 0,
                   loading_thread_count          = 4,
                   window_width                  = 0,
                   window_height                 = 0;
    dict          *instance_json_object          = 0;
    bool           fullscreen                    = false;
    char          *token_text                    = calloc(token_text_len, sizeof(u8)),
                  *name                          = "Instance",
                  *window_title                  = "G10",
                 **requested_validation_layers   = 0,
                 **requested_instance_extensions = 0,
                 **device_extensions             = 0,
                  *requested_physical_device     = 0,
                  *max_buffered_frames           = 2,
                  *initial_scene                 = 0,
                  *log_file_i                    = 0,
                  *input                         = 0,
                  *audio                         = 0,
                  *server                        = 0,
                  *renderer                      = 0,
                 **schedules                     = 0;


    // Load the file
    if ( g_load_file(path, token_text, false) == 0 )
        goto no_file;

    // Parse the instance JSON into a dictionary
    parse_json(token_text, token_text_len, &instance_json_object);

    // Parse the dictionary into instance construction parameters
    {
        
        // Initialized data
        JSONToken_t* token = 0;
        
        // Name. Default to "Instance"
        token                 = dict_get(instance_json_object, "name");
        name                  = JSON_VALUE(token, JSONstring);

        // Window 
        {
            // Set the window width. Default to 800
            token         = dict_get(instance_json_object, "window width");
            window_width  = JSON_VALUE(token, JSONprimative);
            window_width  = (long) (window_width) ? atol((char*)window_width) : 1280;

            // Set the window height. Default to 600
            token         = dict_get(instance_json_object, "window height");
            window_height = JSON_VALUE(token, JSONprimative);
            window_height = (long) (window_height) ? atol(window_height) : 720;

            // Set the window title. Default to "G10"
            token         = dict_get(instance_json_object, "window title");
            window_title  = JSON_VALUE(token, JSONstring);
            window_title  = (window_title) ? window_title : "G10";

            // Fullscreen? Default to false
            token         = dict_get(instance_json_object, "fullscreen");
            fullscreen    = JSON_VALUE(token, JSONprimative);

        }
        
        // Input
        {
            token = dict_get(instance_json_object, "input");
            input = (token) ? token->value.n_where : 0;
        }

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

            // Set the shader cache limit
            token                = dict_get(instance_json_object, "cache ai count");
            ai_cache_count       = JSON_VALUE(token, JSONprimative);
            ai_cache_count       = (ai_cache_count) ? atol(ai_cache_count) : 16;

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

        // Renderer
        {
            token    = dict_get(instance_json_object, "renderer");
            renderer = JSON_VALUE(token, JSONstring | JSONobject);
        }

        // Server
        {
            token  = dict_get(instance_json_object, "server");
            server = JSON_VALUE(token, JSONobject);
        }

        // Schedules
        {
            token     = dict_get(instance_json_object, "schedules");
            schedules = JSON_VALUE(token, JSONarray);
        }

    }

    // Allocate the instance
    {

        // Allocate memory for the instance
        *pp_instance    = calloc(1, sizeof(GXInstance_t));
        
        // Get a pointer to the instance
        ret             = *pp_instance;

        // Set the active instance
        active_instance = ret;
    }

    // Global initialization
    {

        // Set the log file before doing anything else
        {
            if   (log_file_i) { log_file = fopen(log_file_i, "w"); }
            else              { log_file = stdout; }
        }

        // Window initialization
        {
            ret->window.width        = window_width;
            ret->window.height       = window_height;
            ret->vulkan.max_buffered_frames = (long)atoi(max_buffered_frames);
        }

        // FMOD initialization
        

        // SDL initialization
        {

            // Initialize SDL
            if (SDL_Init(SDL_INIT_EVERYTHING))
                goto noSDL;

            // Initialize SDL Image
            //if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG ))
                
                // TODO: this needs to be more specific
                //goto noSDL;

            // Initialize SDL Networking
            if (SDLNet_Init())

                // TODO: this needs to be more specific
                goto noSDL;

            // Create the window
            ret->sdl2.window = SDL_CreateWindow(window_title,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                (int)window_width, (int)window_height,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | ((fullscreen) ? SDL_WINDOW_FULLSCREEN : 0));

            // Check the window
            if (!ret->sdl2.window)
                goto noWindow;
            
            // Display the window
            SDL_ShowWindow(ret->sdl2.window);
            
            ret->time.clock_div = SDL_GetPerformanceFrequency();

        }

        // Vulkan initialization
        {   

            // Create an instance
            {

                // Initialized data
                GXInstance_t                        *instance                  = ret;
                VkApplicationInfo                    application_info          = { 0 };
                VkInstanceCreateInfo                 instance_create_info      = { 0 };
                char                               **required_extensions       = 0,
                                                   **requested_extensions      = 0;
                dict                                *extensions                = 0;
                u32                                  required_extension_count  = 0,
                                                     requested_extension_count = 0,
                                                     requested_layers_count    = 0;
    
                VkResult                             result                    = 0;
    
                if(requested_instance_extensions)
                    while (requested_instance_extensions[++requested_extension_count]);

                // Populate application info struct
                {
                    application_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                    application_info.pApplicationName   = "G10";
                    application_info.pEngineName        = "G10";
                    application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                    application_info.engineVersion      = VK_MAKE_VERSION(0, 0, 1);
                    application_info.apiVersion         = VK_API_VERSION_1_3;
                }

                // Populate instance create info struct
                {

                    // Get a list of the required instance extensions from SDL
                    {
                        SDL_Vulkan_GetInstanceExtensions(instance->sdl2.window, &required_extension_count, (void*)0);
                        required_extensions = calloc(required_extension_count + 1, sizeof(char*));
                        SDL_Vulkan_GetInstanceExtensions(instance->sdl2.window, &required_extension_count, required_extensions);
                    }
        
                    // Construct a dictionary from the required extensions
                    dict_from_keys(&extensions, required_extensions, required_extension_count);

                    // Count requested validation layers
                    if(requested_validation_layers)
                        while (requested_validation_layers[++requested_layers_count]);

                    // TODO: Add support for user defined extensions
                    {
                        instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                        instance_create_info.pApplicationInfo        = &application_info;
                        instance_create_info.enabledExtensionCount   = required_extension_count;
                        instance_create_info.ppEnabledExtensionNames = required_extensions;
                        instance_create_info.enabledLayerCount       = requested_layers_count;
                        instance_create_info.ppEnabledLayerNames     = requested_validation_layers;
                    }
                    
                    // Create the debug messenger
                    VkInstanceCreateInfo *t = &instance_create_info;
                    setup_debug_messenger(&t->pNext);
                }
    
                if ( vkCreateInstance(&instance_create_info, (void*)0, &instance->vulkan.instance) != VK_SUCCESS )
                    goto failed_to_create_vulkan_instance;
    
            }

            // Create a surface with SDL2
            if ( SDL_Vulkan_CreateSurface(ret->sdl2.window, ret->vulkan.instance, &ret->vulkan.surface) == SDL_FALSE )
                goto failed_to_create_sdl2_surface;

            // Get a physical device to use
            pick_physical_device(device_extensions);
            create_logical_device(device_extensions);

            create_swap_chain();
            create_image_views();

            create_command_pool();
            create_command_buffers();
            create_sync_objects();
        }

        // G10 Initialization
        {

            // Set the name
            {

                // Initialized data
                size_t name_len = strlen(name);

                // Allocate memory for the name
                ret->name = calloc(name_len+1, sizeof(u8));

                // Copy the name to the instance
                strncpy(ret->name, name, name_len);
            }

            // Set the loading thread limit
            ret->loading_thread_count = loading_thread_count;

            // Initialize mutexes
            {

                // TODO: Create mutexes in subsystem initializers
                ret->mutexes.part_cache        = SDL_CreateMutex();
                ret->mutexes.material_cache    = SDL_CreateMutex();
            }

            // Subsystem initialization
            {

                // Renderer initialization
                {
                    extern void init_renderer ( void );

                    init_renderer();
                }

                // Shader initialization
                {
                    extern void init_shader ( void );

                    init_shader();
                }

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

                // Collider initialization
                {
                    extern void init_collider( void );

                    init_collider();
                }

                // Scene initialization
                {
                    extern void init_scene(void);

                    init_scene();
                }

                // Physics initialization
                {
                    extern void init_physics(void);

                    init_physics();
                }

                // AI initialization
                {
                    extern void init_ai(void);

                    init_ai();
                }
            }

            // Load a renderer
            if (renderer)
            {

                // Initialized data
                GXRenderer_t *p_renderer = 0;

                // Differerntiate objects from paths
                if   ( *renderer == '{' ) 
                    load_renderer_as_json(&p_renderer, renderer, strlen(renderer));
                else
                    load_renderer(&p_renderer, renderer);

                // Set the active renderer
                ret->context.renderer = p_renderer;
            }

            // Construct dictionaries to cache materials, parts, and shaders.
            {

                // If no count is specified by the JSON object. Default to 128.
                dict_construct(&ret->cache.materials, ((material_cache_count) ? (material_cache_count) : 128));

                // Default to 128 cached parts.
                dict_construct(&ret->cache.parts    , ((material_cache_count) ? (material_cache_count) : 128));

                // Default to 32 cached shaders.
                dict_construct(&ret->cache.shaders  , ((shader_cache_count)   ? (shader_cache_count)   : 32));

                // Default to 16 cached ais
                dict_construct(&ret->cache.ais      , ((ai_cache_count)       ? (ai_cache_count)       : 16));

            }

            // Load an input set
            if (input)
                load_input(&ret->input, input);

            //Load audio
            if (audio) {} //Coming soon...
                

            // Load schedules
            if(schedules) {

                // Construct a dictionary for schedules
                dict_construct(&ret->data.schedules, 8);

                // Iterate over each schedule in the schedules array
                for (size_t i = 0; schedules[i]; i++)
                {
                    
                    // Initialized data
                    GXSchedule_t *schedule = 0;

                    // Parse the schedule as an object
                    if (schedules[i][0] == '{')
                        load_schedule_as_json(&schedule, schedules[i], strlen(schedules[i]));

                    // Load the schedule as a path
                    else
                        load_schedule(&schedule, schedules[i]);

                    // Add the schedule into the schedule dictionary
                    dict_add(ret->data.schedules, schedule->name, schedule);
                }
                
            }

            // Scene dictionary
            dict_construct(&ret->data.scenes, 16);

            // Load the initial scene
            if (initial_scene) {

                // Scene pointer
                GXScene_t* scene = 0;

                // Load the scene from the path
                if ( load_scene(&scene, initial_scene) == 0 )
                    goto failed_to_load_scene;

                // Did the scene load?
                if (scene)

                    // Add the scene to the instance
                    dict_add(ret->data.scenes, scene->name, scene);

                // Error
                else

                    goto no_initial_scene;

                // Set the active scene
                ret->context.scene = scene;
            }

            // Set up the server
            if (server)
            {

                // Load the server as a JSON object
                if (server[0] == '{')
                    load_server_as_json(&ret->networking.server, server, strlen(server));
                
                // Load the server from the filesystem
                else
                    load_server(&ret->networking.server, server);

            }

            // This prevents divide by zero errors when the game loop starts
            ret->time.delta_time = 0.001;
        }
    }

    SDL_SetWindowInputFocus(ret->sdl2.window);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
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
            
            failed_to_create_vulkan_instance:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create a Vulkan instance\n");
                #endif 
                return 0;
        }

        // User errors
        {
            no_file:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to open file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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
        failed_to_load_scene:
            #ifndef NDEBUG
                g_print_error("[G10] Failed to open initial scene in call to function \"%s\"\n", __FUNCTION__);
            #endif 
            return 0;

        no_device:
        no_surface:
        failed_to_create_sdl2_surface:
            return 0;
        no_initial_scene:
            g_print_error("[G10] in call to function \"%s\"\n", initial_scene, __FUNCTION__);
            return 0;
    }

}

void          setup_debug_messenger        ( VkDebugUtilsMessengerCreateInfoEXT **debug_messenger_create_info )
{

    // Initialized data
    VkDebugUtilsMessengerCreateInfoEXT *i_debug_messenger_create_info = calloc(1, sizeof(VkDebugUtilsMessengerCreateInfoEXT));

    // Populate debug messenger create info struct 
    {
        i_debug_messenger_create_info->sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        i_debug_messenger_create_info->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        i_debug_messenger_create_info->messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        i_debug_messenger_create_info->pfnUserCallback = debug_callback;
    }

    // Return
    *debug_messenger_create_info                   = i_debug_messenger_create_info;
}

void          pick_physical_device         ( char **required_extension_names )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(required_extension_names == (void *)0)
                goto no_required_extension_names;
        #endif
    }

    GXInstance_t* instance = g_get_active_instance();

    u32 device_count = 0;
    VkPhysicalDevice *devices = 0;
    vkEnumeratePhysicalDevices(instance->vulkan.instance, &device_count, (void*)0);

    // Error check
    if (device_count == 0)
        goto no_devices;

    devices = calloc(device_count + 1, sizeof(VkPhysicalDevice));

    vkEnumeratePhysicalDevices(instance->vulkan.instance, &device_count, devices);

    for (size_t i = 0; i < device_count; i++)
    {
        if (check_vulkan_device(instance, devices[i], required_extension_names))
            instance->vulkan.physical_device = devices[i];
    }

    no_devices:;
    no_required_extension_names:;
}

void          create_logical_device        ( char **required_extension_names )
{
    GXInstance_t             *instance                 = g_get_active_instance();
    VkDeviceQueueCreateInfo  *queue_create_infos       = calloc(2, sizeof(VkDeviceQueueCreateInfo));
    VkPhysicalDeviceFeatures  device_features          = { 0 };
    VkDeviceCreateInfo        device_create_info       = { 0 };
    VkBool32                  result                   = 0;
    u32                       unique_queue_families[2] = { instance->vulkan.queue_family_indices.g, instance->vulkan.queue_family_indices.p };

    instance->vulkan.priority = 1.f;

    for (size_t i = 0; i < 2; i++)
    {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = i;
        queue_create_infos[i].queueCount = 1;
        queue_create_infos[i].pQueuePriorities = &instance->vulkan.priority;
    }

    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.queueCreateInfoCount = 2;
    device_create_info.pQueueCreateInfos = queue_create_infos;
    device_create_info.enabledExtensionCount = 1;
    device_create_info.pEnabledFeatures = &device_features;

    device_create_info.ppEnabledExtensionNames = required_extension_names;
    
    result = vkCreateDevice(instance->vulkan.physical_device, &device_create_info, 0, &instance->vulkan.device);

    if (result != VK_SUCCESS) {
        printf("failed to create logical device!");
    }

    vkGetDeviceQueue(instance->vulkan.device, unique_queue_families[0], 0, &instance->vulkan.graphics_queue);
    vkGetDeviceQueue(instance->vulkan.device, unique_queue_families[1], 0, &instance->vulkan.present_queue);

} 

void          create_swap_chain            ( void )
{

    // Uninitialized data
    VkSurfaceFormatKHR        surface_format;
    VkExtent2D                extent;

    // Initialized data
    GXInstance_t             *instance               = g_get_active_instance();
    VkSwapchainCreateInfoKHR  swapchain_create_info  = { 0 };
    VkPresentModeKHR          present_mode           = VK_PRESENT_MODE_MAILBOX_KHR;

    u32                       qfi[2]                 = { 0, 0 };
    
    qfi[0] = instance->vulkan.queue_family_indices.g;
    qfi[1] = instance->vulkan.queue_family_indices.p;

    surface_format = instance->vulkan.swap_chain_details.formats[0];

    for (size_t i = 0; i < 2; i++)
        if (instance->vulkan.swap_chain_details.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && instance->vulkan.swap_chain_details.formats[i].format == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            surface_format = instance->vulkan.swap_chain_details.formats[i];
    
    extent.width  = instance->window.width;
    extent.height = instance->window.height;

    // TODO: Check memory
    swapchain_create_info.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.surface          = instance->vulkan.surface;
    swapchain_create_info.minImageCount    = 2;
    swapchain_create_info.imageFormat      = surface_format.format;
    swapchain_create_info.imageColorSpace  = surface_format.colorSpace;
    swapchain_create_info.imageExtent      = extent;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    if (instance->vulkan.queue_family_indices.g != instance->vulkan.queue_family_indices.p)
    {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = &qfi;
    }
    else
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;


    swapchain_create_info.preTransform   = instance->vulkan.swap_chain_details.capabilities.currentTransform;
    swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode    = present_mode;
    swapchain_create_info.clipped        = VK_TRUE;

    swapchain_create_info.oldSwapchain   = VK_NULL_HANDLE;


    if (vkCreateSwapchainKHR(instance->vulkan.device, &swapchain_create_info, 0, &instance->vulkan.swap_chain))
        // TODO: Replace with goto
        printf("FAILED TO CREATE SWAPCHAIN");

    vkGetSwapchainImagesKHR(instance->vulkan.device, instance->vulkan.swap_chain, &instance->vulkan.image_count, 0);

    instance->vulkan.swap_chain_images = calloc(instance->vulkan.image_count, sizeof(VkImage));

    vkGetSwapchainImagesKHR(instance->vulkan.device, instance->vulkan.swap_chain, &instance->vulkan.image_count, instance->vulkan.swap_chain_images);

    instance->vulkan.swap_chain_image_format = surface_format.format;
    instance->vulkan.swap_chain_extent       = extent;
}

void          cleanup_swap_chain           ( void )
{
    GXInstance_t *instance = g_get_active_instance();

    for (size_t i = 0; i < 2; i++)
        vkDestroyFramebuffer(instance->vulkan.device, instance->vulkan.swap_chain_framebuffers[i], 0);

    for (size_t i = 0; i < 2; i++)
        vkDestroyImageView(instance->vulkan.device, instance->vulkan.swap_chain_image_views[i], 0);

    vkDestroySwapchainKHR(instance->vulkan.device, instance->vulkan.swap_chain, (void *)0);

}

void          create_image_views           ( void ) {
    GXInstance_t          *instance = g_get_active_instance();
    
    instance->vulkan.swap_chain_image_views = calloc(instance->vulkan.image_count, sizeof(VkImageView));
    
    for (size_t i = 0; i < instance->vulkan.image_count; i++)
    {
        VkImageViewCreateInfo  image_view_create_info = { 0 };

        image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.image = instance->vulkan.swap_chain_images[i];
        image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format = instance->vulkan.swap_chain_image_format;

        image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel = 0;
        image_view_create_info.subresourceRange.levelCount = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount = 1;

        image_view_create_info.flags = 0;

        image_view_create_info.pNext = 0;

        if (vkCreateImageView(instance->vulkan.device, &image_view_create_info, 0, &instance->vulkan.swap_chain_image_views[i]) != VK_SUCCESS) {
            g_print_error("failed to create image views!\n");
        }
        
    }
}

void          create_command_pool          ( void )
{
    GXInstance_t* instance = g_get_active_instance();
    VkCommandPoolCreateInfo  pool_create_info = { 0 };

    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_create_info.queueFamilyIndex = instance->vulkan.queue_family_indices.g;

    if ( vkCreateCommandPool(instance->vulkan.device, &pool_create_info, 0, &instance->vulkan.command_pool) != VK_SUCCESS )
        g_print_error("Failed to create command pool!\n");
}

void          create_command_buffers       ( void )
{
    GXInstance_t *instance = g_get_active_instance();

    VkCommandBufferAllocateInfo* alloc_info = calloc(1, sizeof(VkCommandBufferAllocateInfo));
    instance->vulkan.command_buffers = calloc(instance->vulkan.max_buffered_frames, sizeof(VkCommandBuffer));

    alloc_info->sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info->commandPool        = instance->vulkan.command_pool;
    alloc_info->level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_info->commandBufferCount = instance->vulkan.max_buffered_frames;

    if (vkAllocateCommandBuffers(instance->vulkan.device, alloc_info, instance->vulkan.command_buffers) != VK_SUCCESS) {
        g_print_error("failed to allocate command buffers!\n");
    }
}

void          create_sync_objects          ( void )
{
    GXInstance_t *instance = g_get_active_instance();

    VkSemaphoreCreateInfo *semaphore_create_info = calloc(instance->vulkan.max_buffered_frames, sizeof(VkSemaphoreCreateInfo));
    VkFenceCreateInfo     *fence_create_info     = calloc(instance->vulkan.max_buffered_frames, sizeof(VkFenceCreateInfo));
    
    semaphore_create_info->sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    fence_create_info->sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info->flags = VK_FENCE_CREATE_SIGNALED_BIT;

    instance->vulkan.image_available_semaphores = calloc(instance->vulkan.max_buffered_frames, sizeof(VkSemaphore));
    instance->vulkan.render_finished_semaphores = calloc(instance->vulkan.max_buffered_frames, sizeof(VkSemaphore));
    instance->vulkan.in_flight_fences           = calloc(instance->vulkan.max_buffered_frames, sizeof(VkFence));

    for (size_t i = 0; i < instance->vulkan.max_buffered_frames; i++)
    {
        if (vkCreateSemaphore(instance->vulkan.device, semaphore_create_info, 0, &instance->vulkan.image_available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(instance->vulkan.device, semaphore_create_info, 0, &instance->vulkan.render_finished_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(instance->vulkan.device, fence_create_info, 0, &instance->vulkan.in_flight_fences[i]) != VK_SUCCESS) {
            g_print_error("failed to create semaphores!\n");
        }
    }

}

u32           find_memory_type             ( u32                   type_filter, VkMemoryPropertyFlags properties)
{
    GXInstance_t *instance = g_get_active_instance();
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(instance->vulkan.physical_device, &mem_properties);

    for (u32 i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    g_print_error("[G10] Failed to find a suitable memory type\n");
    return 0;
}

int           check_vulkan_device          ( GXInstance_t         *instance, VkPhysicalDevice physical_device, char **required_extension_names )
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
            
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, instance->vulkan.surface, &present_support);

            if (present_support)
                indices.p = i;
            else
                passes_queue = false;

            if (passes_queue)
                break;
        }

        instance->vulkan.queue_family_indices.g = indices.g;
        instance->vulkan.queue_family_indices.p = indices.p;
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
                    char *t = 0;
                    dict_pop(device_extension_name_dict, required_extension_names[i], &t);
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

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, instance->vulkan.surface, &instance->vulkan.swap_chain_details.capabilities);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, instance->vulkan.surface, &format_count, (void*)0);

            instance->vulkan.swap_chain_details.formats = calloc(format_count + 1, sizeof(VkSurfaceFormatKHR));

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, instance->vulkan.surface, &format_count, instance->vulkan.swap_chain_details.formats);

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, instance->vulkan.surface, &present_mode_count, (void *)0);

            instance->vulkan.swap_chain_details.present_modes = calloc(present_mode_count + 1, sizeof(VkPresentModeKHR));

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, instance->vulkan.surface, &present_mode_count, instance->vulkan.swap_chain_details.present_modes);
            
            if ((bool)(present_mode_count) & (bool)(format_count)==true)
            {
                has_swap_chain = true;
            }

        }
    }

    return passes_queue && has_extensions && has_swap_chain;
}

void          create_buffer                ( VkDeviceSize          size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory )
{
    VkMemoryRequirements  mem_requirements;
    GXInstance_t         *instance         = g_get_active_instance();
    VkBufferCreateInfo    buffer_info      = { 0 };
    VkMemoryAllocateInfo  alloc_info       = { 0 };
    
    // Create a buffer
    {

        // Populate the create info struct
        buffer_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.size        = size;
        buffer_info.usage       = usage;
        buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        // Create a buffer
        if ( vkCreateBuffer( instance->vulkan.device, &buffer_info, 0, buffer ) != VK_SUCCESS )
            g_print_error("[G10] Failed to create buffer in call to function \"\s\"\n", __FUNCTION__);

        vkGetBufferMemoryRequirements(instance->vulkan.device, *buffer, &mem_requirements);
    }
    

    // Allocate memory for the buffer
    {

        // Populate the allocation struct
        alloc_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.allocationSize  = mem_requirements.size;
        alloc_info.memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties);

        // Allocate memory for the buffer
        if (vkAllocateMemory(instance->vulkan.device, &alloc_info, 0, buffer_memory) != VK_SUCCESS)
            g_print_error("[G10] Failed to allocate memory to buffer in call to funciton \"%s\"\n", __FUNCTION__);

        // Bind the buffer to the device
        vkBindBufferMemory(instance->vulkan.device, *buffer, *buffer_memory, 0);
    }
}

size_t        g_load_file                  ( const char           *path,     void         *buffer   , bool binaryMode )
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
        goto invalid_file;

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
                g_print_error("[G10] Null path provided to funciton \"%s\n", __FUNCTION__);
            #endif
            return 0;
        }

        // File errors
        {
            invalid_file:
            #ifndef NDEBUG
                g_print_error("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
            #endif
            return 0;
        }
    }
}

void          clear_swap_chain             ( void )
{
    GXInstance_t *instance = g_get_active_instance();
    for (size_t i = 0; i < instance->vulkan.image_count; i++) {
        vkDestroyImageView(instance->vulkan.device, instance->vulkan.swap_chain_image_views[i], 0);
    }

    vkDestroySwapchainKHR(instance->vulkan.device, instance->vulkan.swap_chain, 0);
}

int           g_window_resize              ( GXInstance_t         *instance)
{

    // TODO: 
    SDL_GetWindowSize(instance->sdl2.window, &instance->window.width, &instance->window.height);

    return 0;
}

int           g_print_error                ( const char *const     format, ... ) 
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
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }


    }
}

int           g_print_warning              ( const char *const     format, ... ) 
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
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }

}

int           g_print_log                  ( const char *const     format, ... ) 
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
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }

}

int           g_start_schedule             ( GXInstance_t* instance, char* name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if(name == (void *)0)
                goto no_name;
        #endif
    }

    // Initialized data
    GXSchedule_t* schedule = dict_get(instance->data.schedules, name);

    // Error checking
    {
        #ifndef NDEBUG
            if (schedule == (void *)0)
                goto no_schedule;
        #endif
    }

    // Start the schedule
    start_schedule(schedule);

    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for \"instance\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif  
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for \"name\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif  
                return 0;
        }

        no_schedule:
            #ifndef NDEBUG
                g_print_error("[G10] [Scheduler] Failed to find a schedule named \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
            #endif  
            return 0;
    }
}

int           copy_state                   ( GXInstance_t         *instance )
{
    
    // Initialized data
    size_t       actor_count = 0,
                 ai_count    = 0;

    GXEntity_t **actors      = 0,
               **ais         = 0;

    // Lock the mutexes
    {

        // Physics
        SDL_LockMutex(instance->mutexes.move_object);
        SDL_LockMutex(instance->mutexes.update_force);
        SDL_LockMutex(instance->mutexes.resolve_collision);

        // AI
        SDL_LockMutex(instance->mutexes.ai_preupdate);
        SDL_LockMutex(instance->mutexes.ai_update);
    }

    // Get a list of actors and ais
    {
        actor_count = dict_keys(instance->context.scene->actors, 0);
        ai_count    = dict_keys(instance->context.scene->ais, 0);

        actors      = calloc(actor_count+1, sizeof(void *));
        ais         = calloc(ai_count+1, sizeof(void *));

        dict_values(instance->context.scene->actors, actors);
        dict_values(instance->context.scene->ais, ais);
    }



    // Reconstruct the queues
    {

        // Destroy the old queues
        {

            // Physics
            {
                if (instance->queues.actor_move)
                    queue_destroy(instance->queues.actor_move);

                if (instance->queues.actor_force)
                    queue_destroy(instance->queues.actor_force);

                if (instance->queues.actor_collision)
                    queue_destroy(instance->queues.actor_collision);
            }

            // AI
            {
                if (instance->queues.ai_preupdate)
                    queue_destroy(instance->queues.ai_preupdate);

                if (instance->queues.ai_update)
                    queue_destroy(instance->queues.ai_update);
            }
        }

        // Physics
        queue_construct(&instance->queues.actor_move     , actor_count + 1);
        queue_construct(&instance->queues.actor_force    , actor_count + 1);
        queue_construct(&instance->queues.actor_collision, actor_count + 1);

        // AI
        queue_construct(&instance->queues.ai_preupdate   , ai_count + 1 );
        queue_construct(&instance->queues.ai_update      , ai_count + 1 );
    }

    // Populate the new queues
    {

        // Physics
        for (size_t i = 0; i < actor_count; i++)
        {
            queue_enqueue(instance->queues.actor_move     , actors[i]);
            queue_enqueue(instance->queues.actor_force    , actors[i]);
            queue_enqueue(instance->queues.actor_collision, actors[i]);
        }

        // AI
        for (size_t i = 0; i < ai_count; i++)
        {
            queue_enqueue(instance->queues.ai_preupdate, ais[i]);
            queue_enqueue(instance->queues.ai_update   , ais[i]);
        }
    }

    // Free the lists
    free(actors);
    free(ais);

    // Unlock the mutexes
    {
        
        // Physics
        SDL_UnlockMutex(instance->mutexes.move_object);
        SDL_UnlockMutex(instance->mutexes.update_force);
        SDL_UnlockMutex(instance->mutexes.resolve_collision);

        // AI
        SDL_UnlockMutex(instance->mutexes.ai_preupdate);
        SDL_UnlockMutex(instance->mutexes.ai_update);
    }

    return 1;
}

GXInstance_t *g_get_active_instance        ( void )
{
    return active_instance;
}

int           g_cache_material             ( GXInstance_t         *instance, GXMaterial_t *material )
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

    dict_add(instance->cache.materials, material->name, material);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_material:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

int           g_cache_part                 ( GXInstance_t         *instance, GXPart_t     *part)
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
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_part:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"part\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

int           g_cache_shader               ( GXInstance_t         *instance, GXShader_t   *shader )
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
    
    dict_add(instance->cache.shaders, shader->name, shader);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_shader:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"shader\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

int           g_cache_ai                   ( GXInstance_t         *instance, GXAI_t       *ai )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(instance == (void *)0)
                goto no_instance;
            if (ai == (void*)0)
                goto no_ai;
        #endif
    }
    
    dict_add(instance->cache.ais, ai->name, ai);

    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_ai:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"ai\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

void          g_user_exit                  ( callback_parameter_t input, GXInstance_t* instance)
{
    instance->running = false;
    if (instance->context.schedule)
    {
        GXThread_t *main_thread = dict_get(instance->context.schedule->threads, "Main Thread");

        main_thread->running    = false;
    }
}
  
void          g_toggle_mouse_lock          ( callback_parameter_t state, GXInstance_t *instance )
{
    if(state.inputs.key.depressed == true)
    {
        SDL_SetRelativeMouseMode(!SDL_GetRelativeMouseMode());

        #ifndef NDEBUG
            g_print_log("[G10] Toggling mouse locking\n");
        #endif
    }

    SDL_Delay(333);
}

void          g_play_sound                 ( callback_parameter_t state, GXInstance_t *instance )
{
    GXSound_t** sampleSound = NULL;
    load_sound(sampleSound, )


}

 
GXMaterial_t *g_find_material              ( GXInstance_t         *instance, char         *name )
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

    return dict_get(instance->cache.materials, name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

GXPart_t     *g_find_part                  ( GXInstance_t         *instance, char         *name)
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

    return dict_get(instance->cache.parts, name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

GXShader_t   *g_find_shader                ( GXInstance_t         *instance, char         *name)
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

    GXShader_t* ret = 0;

    ret = dict_get(instance->cache.shaders, name);

    return ret;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

GXAI_t       *g_find_ai                    ( GXInstance_t         *instance, char         *name)
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

    GXAI_t* ret = 0;

    ret = dict_get(instance->cache.ais, name);

    return ret;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;

            no_name:
            #ifndef NDEBUG
                printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

    }
}

int           g_exit                       ( GXInstance_t         *instance )
{

    // Argument check
    {
        if (instance == (void*)0)
            goto no_instance;
    }

    // Wait for the GPU to finish whatever its doing
    vkDeviceWaitIdle(instance->vulkan.device);

    // Cleanup
    {

        // Free instance data
        {
            
            // Free the instance name
            free(instance->name);
        }


        // Free scenes
        {
            size_t scene_count = dict_keys(instance->data.scenes, 0);
            GXScene_t **scenes = calloc(scene_count, sizeof(void *));

            dict_values(instance->data.scenes, scenes);

            for (size_t i = 0; i < scene_count; i++)
                destroy_scene(scenes[i]);

            free(scenes);

            dict_destroy(instance->data.scenes);
        }

        // Free shaders
        {

            // Initialized data
            size_t        shader_count = dict_values(instance->cache.shaders, 0);
            GXShader_t  **shaders      = calloc(shader_count, sizeof(GXShader_t*));

            // Get a list of orphan shaders
            dict_values(instance->cache.shaders, shaders);

            // Iterate over each shader
            for (size_t i = 0; i < shader_count; i++)
            {

                // Destroy the shader
                shaders[i]->users = 1;
                // TODO: Fix
                //destroy_shader(shaders[i]);

            }

            // Free the list
            free(shaders);
        }

        // Free materials
        {
            ;
        }

        // Free parts
        {
            ;
        }
    }

    // Vulkan cleanup
    {
        clear_swap_chain();

        for (size_t i = 0; i < instance->vulkan.max_buffered_frames; i++)
        {
            vkDestroySemaphore(instance->vulkan.device, instance->vulkan.render_finished_semaphores[i], 0);
            vkDestroySemaphore(instance->vulkan.device, instance->vulkan.image_available_semaphores[i], 0);
            vkDestroyFence(instance->vulkan.device, instance->vulkan.in_flight_fences[i], 0);

        }

        vkDestroyCommandPool(instance->vulkan.device, instance->vulkan.command_pool, 0);

        vkDestroyDevice(instance->vulkan.device, (void*)0);
        vkDestroySurfaceKHR(instance->vulkan.instance, instance->vulkan.surface, (void*)0);
        vkDestroyInstance(instance->vulkan.instance, (void*)0);
    }

    //FMOD Cleanup
    {
        //Replace this with a struct member later?
        //Not thread-safe
        FMOD_RESULT fmodResult = FMOD_System_Release(instance->fmod.system);

    }

    // SDL Cleanup
    {
        SDL_DestroyWindow(instance->sdl2.window);
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
                    g_print_error("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }
}
