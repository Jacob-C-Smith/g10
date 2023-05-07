#include <G10/G10.h>

// Uninitialized data
FILE* log_file;

// Initialized data
static GXInstance_t *active_instance = 0;

// Forward declared functions
void pick_physical_device   ( char **required_extension_names );
void create_logical_device  ( char **required_extension_names );
void create_swap_chain      ( void );
void cleanup_swap_chain     ( void );
void create_image_views     ( void );
void create_command_pool    ( void );
void create_command_buffers ( void );
void create_sync_objects    ( void );

void create_buffer          ( VkDeviceSize  size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory );
u32  find_memory_type       ( u32 type_filter, VkMemoryPropertyFlags properties );
int  check_vulkan_device    ( GXInstance_t *p_instance , VkPhysicalDevice physical_device, char **required_extension_names);

VKAPI_ATTR 
VkBool32
VKAPI_CALL debug_callback ( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

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
void setup_debug_messenger(VkDebugUtilsMessengerCreateInfoEXT** debug_messenger_create_info);
VkResult CreateDebugUtilsMessengerEXT ( VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != 0) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

int g_init ( GXInstance_t **pp_instance, const char *path )
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
    GXInstance_t  *p_instance                      = 0;
    size_t         text_len                        = g_load_file(path, 0, false),
                   part_cache_count                = 128,
                   material_cache_count            = 128,
                   shader_cache_count              = 32,
                   ai_cache_count                  = 16,
                   loading_thread_count            = 4;
    signed         window_width                    = 1280,
                   window_height                   = 720;
    JSONValue_t   *p_value                         = 0;
    bool           fullscreen                      = false;
    char          *text                            = calloc(text_len, sizeof(u8)),
                  *name                            = "Instance",
                  *window_title                    = "G10",
                  *requested_physical_device       = 0,
                  *max_buffered_frames             = 2,
                  *initial_scene                   = 0,
                  *log_file_i                      = 0,
                  *input                           = 0,
                  *audio                           = 0,
                  *server                          = 0,
                  *renderer                        = 0;
    array         *p_requested_validation_layers   = 0,
                  *p_requested_instance_extensions = 0,
                  *p_device_extensions             = 0,
                  *p_schedules                     = 0;

    // Load the file
    if ( g_load_file(path, text, false) == 0 )
        goto no_file;

    // Parse the instance JSON
    if ( parse_json_value(text, 0, &p_value) == 0 )
        goto failed_to_parse_json;

    // Is the JSONValue the right type?
    if (p_value->type == JSONobject)
    {
	
        // Initialized data
		dict *p_dict = p_value->object;

        // Parse the name of the game
        name = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "name")), JSONstring);

        // Window 
        window_width  = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "window width")) , JSONinteger);
        window_height = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "window height")), JSONinteger);
        window_title  = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "window title")) , JSONstring);
        fullscreen    = (bool)   JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "fullscreen"))   , JSONboolean);

        // Input
        input = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "input")), JSONstring);

        // Log file
        log_file_i = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "log file")), JSONstring);

        // Path to initial scene 
        initial_scene = (char *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "initial scene")), JSONstring);

        // Caches
        part_cache_count     = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "cache part count"))    , JSONinteger)+1;
        material_cache_count = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "cache material count")), JSONinteger)+1;
        shader_cache_count   = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "cache shader count"))  , JSONinteger)+1;
        ai_cache_count       = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "cache ai count"))      , JSONinteger)+1;

        // Loading thread count
        loading_thread_count = (signed) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "loading thread count"))      , JSONinteger);

        // Vulkan
        p_requested_validation_layers   = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "vulkan validation layers"))  , JSONarray);
        p_requested_instance_extensions = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "vulkan instance extensions")), JSONarray);
        p_device_extensions             = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "vulkan device extensions"))  , JSONarray);
        requested_physical_device       = (char  *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "vulkan physical device"))    , JSONstring);
        max_buffered_frames             = (signed)  JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "max buffered frames"))       , JSONinteger);
        //renderer                      = (char  *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "renderer")                   , JSONstring);
        server                          = (dict  *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "server"))                    , JSONobject);
        p_schedules                     = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "schedules"))                 , JSONarray);

    }

    // Allocate memory for the instance
    p_instance = calloc(1, sizeof(GXInstance_t));
        
    // Set the active instance
    active_instance = p_instance;

    // Global initialization
    {

        // Set the log file before doing anything else
        log_file = (log_file_i) ?  fopen(log_file_i, "w") : stdout;

        // Window initialization
        {
            p_instance->window.width = window_width;
            p_instance->window.height = window_height;
        }
        
        // SDL initialization
        {

            // Initialize SDL
            if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_TIMER) )
                goto noSDL;

            // Initialize SDL Image
            // if ( IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG ) )
                
                // TODO: this needs to be more specific
                //goto noSDL;

            // Initialize SDL Networking
            if ( SDLNet_Init() )

                // TODO: this needs to be more specific
                goto noSDL;

            // Create the window
            p_instance->sdl2.window = SDL_CreateWindow(window_title,
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                (int)window_width, (int)window_height,
                SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | ((fullscreen) ? SDL_WINDOW_FULLSCREEN : 0));

            // Check the window
            if (!p_instance->sdl2.window)
                goto noWindow;
            
            // Display the window
            SDL_ShowWindow(p_instance->sdl2.window);
            
            p_instance->time.clock_div = SDL_GetPerformanceFrequency();

        }

        // Vulkan initialization
        {

            // Initialized data
            char   **device_extensions      = 0;
            size_t   device_extension_count = 0;

            // Create an instance
            {

                // Initialized data
                VkApplicationInfo      application_info            = { 0 };
                VkInstanceCreateInfo   instance_create_info        = { 0 };
                char                 **required_extensions         = 0,
                                     **requested_extensions        = 0,
                                     **requested_validation_layers = 0;                                     
                dict                  *extensions                  = 0;
                size_t                 required_extension_count    = 0,
                                       requested_extension_count   = 0,
                                       requested_layers_count      = 0;
                VkResult               result                      = 0;

                // Get the extensions
                if(p_requested_instance_extensions) {

                    // Get the number of extensions    
                    array_get(p_requested_instance_extensions, 0, &requested_extension_count);

                    // Allocate for each extension
                    requested_extensions = calloc(requested_extension_count, sizeof(char *));

                    // Get each extension
                    array_get(p_requested_instance_extensions, requested_extensions, 0);
                }

                // Populate application info struct
                application_info = (VkApplicationInfo) { 
                    .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName   = "G10",
                    .pEngineName        = "G10",
                    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
                    .engineVersion      = VK_MAKE_VERSION(0, 0, 1),
                    .apiVersion         = VK_API_VERSION_1_3
                };
                
                // Populate instance create info struct
                {

                    // Initialized data
                    VkInstanceCreateInfo *t = 0;

                    // Get a list of the required instance extensions from SDL
                    {
                        
                        // Get the quantity of required instance extensions
                        SDL_Vulkan_GetInstanceExtensions(p_instance->sdl2.window, &required_extension_count, (void*)0);

                        // Allocate memory for the extensions
                        required_extensions = calloc(required_extension_count + 1, sizeof(char*));

                        // Get the required instance extensions
                        SDL_Vulkan_GetInstanceExtensions(p_instance->sdl2.window, &required_extension_count, required_extensions);
                    }
        
                    // Construct a dictionary from the required extensions
                    dict_from_keys(&extensions, required_extensions, required_extension_count);

                    if(p_requested_validation_layers)
                    {
                        
                        // Get the number requested validation layers
                        array_get(p_requested_validation_layers, 0, &requested_layers_count);

                        // Allocate for each extension
                        requested_validation_layers = calloc(requested_extension_count, sizeof(JSONValue_t *));

                        // Get each extension
                        array_get(p_requested_validation_layers, requested_validation_layers, 0);

                        for (size_t i = 0; i < requested_layers_count; i++)
                            requested_validation_layers[i] = JSON_VALUE(((JSONValue_t *)requested_validation_layers[i]), JSONstring);
                        
                    }

                    // TODO: Add support for user defined extensions
                    instance_create_info  = (VkInstanceCreateInfo) { 
                        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                        .pApplicationInfo        = &application_info,
                        .enabledExtensionCount   = required_extension_count,
                        .ppEnabledExtensionNames = required_extensions,
                        .enabledLayerCount       = requested_layers_count,
                        .ppEnabledLayerNames     = requested_validation_layers
                    };
                    
                    // Create the debug messenger
                    t = &instance_create_info;
                    setup_debug_messenger(&t->pNext);
                }
    
                // Create the vulkan instance
                if ( vkCreateInstance(&instance_create_info, (void*)0, &p_instance->vulkan.instance) != VK_SUCCESS )
                    goto failed_to_create_vulkan_instance;
    
            }
            
            // Set the maximum number of buffered frames
            p_instance->vulkan.max_buffered_frames = max_buffered_frames;

            // Create a surface with SDL2
            if ( SDL_Vulkan_CreateSurface(p_instance->sdl2.window, p_instance->vulkan.instance, &p_instance->vulkan.surface) == SDL_FALSE )
                goto failed_to_create_sdl2_surface;

            array_get(p_device_extensions, 0, &device_extension_count);
            device_extensions = calloc(device_extension_count+1, sizeof(char *));
            array_get(p_device_extensions, device_extensions, 0);
            
            // Iterate over each vulkan device extension
            for (size_t i = 0; i < device_extension_count; i++)

                // JSONValue_t * ---> char *
                device_extensions[i] = JSON_VALUE(((JSONValue_t *)device_extensions[i]), JSONstring);

            // Get a physical device to use
            pick_physical_device(device_extensions);

            // Create a logical device from the physical device
            create_logical_device(device_extensions);

            // Create a swap chain
            create_swap_chain();
            create_image_views();

            // Create other stuff
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
                p_instance->name = calloc(name_len+1, sizeof(u8));

                // Copy the name to the instance
                strncpy(p_instance->name, name, name_len);
            }

            // Set the loading thread limit
            p_instance->loading_thread_count = loading_thread_count;

            // Initialize mutexes
            // TODO: Create mutexes in subsystem initializers
            p_instance->mutexes.part_cache = SDL_CreateMutex();

            // Subsystem initialization
            {
                
                /*
                // Renderer initialization
                {
                    extern void init_renderer ( void );

                    init_renderer();
                }

                // Shader initialization
                {
                    extern void init_shader ( void );

                    // Run the shader system initializer
                    init_shader();
                }

                // Texture initialization
                {

                    // Get an external function
                    extern int init_texture(void);

                    // Run the texture system initializer
                    init_texture();
                }
                */

                // Input initialization
                {

                    // Get an external function
                    extern int init_input(void);

                    // Run the input system initializer
                    init_input();
                }
                
                /*
                // Audio initialization
                {

                    // Get an external function
                    extern int init_audio(void);

                    // Run the audio system initializer
                    init_audio();
                }
                */

                // Part initialization
                {

                    // Get an external function
                    extern int init_part(void);

                    // Run the part system initializer
                    init_part();
                }
                
                // Material initialization
                {

                    // Get an external function
                    extern int init_material(void);

                    // Run the material system initializer
                    init_material();
                }

                //Scheduler initialization
                {
                    
                    // Get an external function
                    extern void init_scheduler( void );
                        
                    // Run the scheduler system initializer
                    init_scheduler();
                }

                /*
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
                */
               
                // AI initialization
                {
                    extern void init_ai(void);

                    init_ai();
                }
            }

            // Construct dictionaries to cache materials, parts, and shaders.
            {

                // If no count is specified by the JSON object. Default to 128.
                dict_construct(&p_instance->cache.materials, material_cache_count);

                // Default to 128 cached parts.
                dict_construct(&p_instance->cache.parts    , part_cache_count);

                // Default to 32 cached shaders.
                dict_construct(&p_instance->cache.shaders  , shader_cache_count);

                // Default to 16 cached ais
                // TODO: Fix
                dict_construct(&p_instance->cache.ais      , 16 /*ai_cache_count*/);

            }

            // Load a renderer
            if ( renderer )
            {

                // Initialized data
                GXRenderer_t *p_renderer = 0;

                // TODO: Fix
                // Differerntiate objects from paths
                //if   ( *renderer == '{' ) 
                //    load_renderer_as_json(&p_renderer, renderer, strlen(renderer));
                //else
                //    load_renderer(&p_renderer, renderer);

                // Set the active renderer
                p_instance->context.renderer = p_renderer;
            }

            

            // Load an input set
            if ( input )
            {
                load_input(&p_instance->input, input);
            }

            //Load audio
            if (audio) {} // Coming soon...
                
            // Load schedules
            if(p_schedules)
            {
  
		        // Initialized data
		        size_t        schedule_count = 0;
                JSONValue_t **pp_elements    = 0;

		        // Get the quantity of elements
		        array_get(p_schedules, 0, &schedule_count );

                // Construct a dictionary for schedules
                dict_construct(&p_instance->data.schedules, 8);
  
		        // Allocate an array for the elements
		        pp_elements = calloc(schedule_count+1, sizeof(JSONValue_t *));

		        // Error check
		        if ( pp_elements == (void *) 0 )
		        	goto no_mem;

		        // Populate the elements of the threads
		        array_get(p_schedules, pp_elements, 0 );			

		        // Set up the threads
		        for (size_t i = 0; i < schedule_count; i++)
		        {

                    // Initialized data
                    JSONValue_t  *p_element = pp_elements[i];
		        	GXSchedule_t *i_schedule = 0;

                    // Parse the schedule as an object
                    if (p_element->type == JSONobject)
                    {                        
                        if ( load_schedule_as_json_value(&i_schedule, p_element) == 0 )
                            goto failed_to_load_schedule;
                    }

                    // Parse the schedule as a file
                    else if (p_element->type == JSONstring)
                    {
                        if ( load_schedule(&i_schedule, p_element->string) == 0 )
                            goto failed_to_load_schedule;
                    }

		        	// Add the schedule into the schedule dictionary
                    dict_add(p_instance->data.schedules, i_schedule->name, i_schedule);
                }
            }

            // Scene dictionary
            dict_construct(&p_instance->data.scenes, 16);

            // Load the initial scene
            if (initial_scene) {

                // Scene pointer
                //GXScene_t* p_scene = 0;

                // Load the scene from the path
                //if ( load_scene(&p_scene, initial_scene) == 0 )
                //    goto failed_to_load_scene;

                // Did the scene load?
                //if (p_scene)

                    // Add the scene to the instance
                //    dict_add(p_instance->data.scenes, p_scene->name, p_scene);

                // Error
                //else

                //    goto no_initial_scene;

                // Set the active scene
                //p_instance->context.scene = p_scene;
            }

            // Set up the server
            //if (server)
            //{
            //    // Load the server as a JSON object
            //    if (server[0] == '{')
            //        load_server_as_json(&p_instance->networking.server, server, strlen(server));
            //    
            //    // Load the server from the filesystem
            //    else
            //        load_server(&p_instance->networking.server, server);
            //}

            // This prevents divide by zero errors when the game loop starts
            p_instance->time.delta_time = 0.001;
        }
        
        // 3rd party subsystem integration
        
        // Discord
        #ifdef BUILD_G10_WITH_DISCORD
            extern void init_discord_integration(void);
            init_discord_integration();
        #endif
        
    }

    // Focus the game window
    SDL_SetWindowInputFocus(p_instance->sdl2.window);
    
    // The instance is running
    p_instance->running = true;
    
    // Return an instance pointer to the caller
    *pp_instance = p_instance;
    
    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // SDL Errors
        {

            // SDL failed to initialize
            noSDL:
                #ifndef NDEBUG
                    g_print_error("[SDL2] Failed to initialize SDL\n");
                #endif

                // Error 
                return 0;

            // SDL Image failed to initialize
            noSDLImage:
                #ifndef NDEBUG
                    g_print_error("[SDL Image] Failed to initialize SDL Image\n");
                #endif

                // Error 
                return 0;

            // SDL Networking failed to initialize
            noSDLNetwork:
                #ifndef NDEBUG
                    g_print_error("[SDL Networking] Failed to initialize SDL Networking\n");
                #endif

                // Error 
                return 0;

            // The SDL window failed to initialize
            noWindow:
                #ifndef NDEBUG
                    g_print_error("[SDL2] Failed to create a window\nSDL Says: %s\n", SDL_GetError());
                #endif
                
                // Error 
                return 0;
        
        }

        // Vulkan errors
        {
            
            failed_to_create_vulkan_instance:
                #ifndef NDEBUG
                    g_print_error("[Vulkan] Failed to create a Vulkan instance\n");
                #endif 

                // Error 
                return 0;
        }

        // User errors
        {
            no_file:
                #ifndef NDEBUG
                    g_print_error("[G10] Failed to open file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
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

        // TOOD: Categorize
        failed_to_parse_json:
            return 0;
        no_instance:
            #ifndef NDEBUG
                g_print_error("[G10] Failed to allocate an instance in call to function \"%s\"\n", __FUNCTION__);
            #endif 

            // Error 
            return 0;

        failed_to_load_scene:
            #ifndef NDEBUG
                g_print_error("[G10] Failed to open initial scene in call to function \"%s\"\n", __FUNCTION__);
            #endif 

            // Error 
            return 0;

        no_device:
        no_surface:
        failed_to_load_schedule:
        failed_to_create_sdl2_surface:
            return 0;
        no_initial_scene:
            g_print_error("[G10] in call to function \"%s\"\n", initial_scene, __FUNCTION__);
            return 0;
    }
}

void setup_debug_messenger ( VkDebugUtilsMessengerCreateInfoEXT **debug_messenger_create_info )
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

void pick_physical_device ( char **required_extension_names )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(required_extension_names == (void *)0)
                goto no_required_extension_names;
        #endif
    }

    // Initialized data
    GXInstance_t     *p_instance   = g_get_active_instance();
    u32               device_count = 0;
    VkPhysicalDevice *devices      = 0;

    vkEnumeratePhysicalDevices(p_instance->vulkan.instance, &device_count, (void*)0);

    // Error check
    if (device_count == 0)
        goto no_devices;

    devices = calloc(device_count + 1, sizeof(VkPhysicalDevice));

    vkEnumeratePhysicalDevices(p_instance->vulkan.instance, &device_count, devices);

    for (size_t i = 0; i < device_count; i++)
    {
        if (check_vulkan_device(p_instance, devices[i], required_extension_names))
            p_instance->vulkan.physical_device = devices[i];
    }

    no_devices:;
    no_required_extension_names:;
}

void create_logical_device ( char **required_extension_names )
{
    GXInstance_t             *p_instance                 = g_get_active_instance();
    VkDeviceQueueCreateInfo  *queue_create_infos       = calloc(2, sizeof(VkDeviceQueueCreateInfo));
    VkPhysicalDeviceFeatures  device_features          = { 0 };
    VkDeviceCreateInfo        device_create_info       = { 0 };
    VkBool32                  result                   = 0;
    u32                       unique_queue_families[2] = { p_instance->vulkan.queue_family_indices.g, p_instance->vulkan.queue_family_indices.p };

    p_instance->vulkan.priority = 1.f;

    for (size_t i = 0; i < 2; i++)
    {
        queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_infos[i].queueFamilyIndex = i;
        queue_create_infos[i].queueCount = 1;
        queue_create_infos[i].pQueuePriorities = &p_instance->vulkan.priority;
    }

    device_create_info = (VkDeviceCreateInfo) {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount    = 2,
        .pQueueCreateInfos       = queue_create_infos,
        .enabledExtensionCount   = 1,
        .pEnabledFeatures        = &device_features,
        .ppEnabledExtensionNames = required_extension_names
    };

    result = vkCreateDevice(p_instance->vulkan.physical_device, &device_create_info, 0, &p_instance->vulkan.device);

    if (result != VK_SUCCESS) {
        printf("failed to create logical device!");
    }

    vkGetDeviceQueue(p_instance->vulkan.device, unique_queue_families[0], 0, &p_instance->vulkan.graphics_queue);
    vkGetDeviceQueue(p_instance->vulkan.device, unique_queue_families[1], 0, &p_instance->vulkan.present_queue);

} 

void create_swap_chain ( void )
{

    // Uninitialized data
    VkSurfaceFormatKHR        surface_format;
    VkExtent2D                extent;

    // Initialized data
    GXInstance_t             *p_instance               = g_get_active_instance();
    VkSwapchainCreateInfoKHR  swapchain_create_info  = { 0 };
    VkPresentModeKHR          present_mode           = VK_PRESENT_MODE_MAILBOX_KHR;

    u32                       qfi[2]                 = { 0, 0 };
    
    qfi[0] = p_instance->vulkan.queue_family_indices.g;
    qfi[1] = p_instance->vulkan.queue_family_indices.p;

    surface_format = p_instance->vulkan.swap_chain_details.formats[0];

    for (size_t i = 0; i < 2; i++)
        if (p_instance->vulkan.swap_chain_details.formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && p_instance->vulkan.swap_chain_details.formats[i].format == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            surface_format = p_instance->vulkan.swap_chain_details.formats[i];
    
    extent = (VkExtent2D) {
        .width  = p_instance->window.width,
        .height = p_instance->window.height
    };

    swapchain_create_info = (VkSwapchainCreateInfoKHR) { 
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = p_instance->vulkan.surface,
        .minImageCount    = 2,
        .imageFormat      = surface_format.format,
        .imageColorSpace  = surface_format.colorSpace,
        .imageExtent      = extent,
        .imageArrayLayers = 1,
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode    = present_mode,
        .clipped        = VK_TRUE,
        .oldSwapchain   = VK_NULL_HANDLE
    };
    
    if (p_instance->vulkan.queue_family_indices.g != p_instance->vulkan.queue_family_indices.p)
    {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = &qfi;
    }
    else
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    swapchain_create_info.preTransform   = p_instance->vulkan.swap_chain_details.capabilities.currentTransform;

    if (vkCreateSwapchainKHR(p_instance->vulkan.device, &swapchain_create_info, 0, &p_instance->vulkan.swap_chain))
        // TODO: Replace with goto
        printf("FAILED TO CREATE SWAPCHAIN");

    vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain, &p_instance->vulkan.image_count, 0);

    p_instance->vulkan.swap_chain_images = calloc(p_instance->vulkan.image_count, sizeof(VkImage));

    vkGetSwapchainImagesKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain, &p_instance->vulkan.image_count, p_instance->vulkan.swap_chain_images);

    p_instance->vulkan.swap_chain_image_format = surface_format.format;
    p_instance->vulkan.swap_chain_extent       = extent;
}

void cleanup_swap_chain ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();

    for (size_t i = 0; i < 2; i++)
        vkDestroyFramebuffer(p_instance->vulkan.device, p_instance->vulkan.swap_chain_framebuffers[i], 0);

    for (size_t i = 0; i < 2; i++)
        vkDestroyImageView(p_instance->vulkan.device, p_instance->vulkan.swap_chain_image_views[i], 0);

    vkDestroySwapchainKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain, (void *)0);

}

void create_image_views ( void ) {
    GXInstance_t          *p_instance = g_get_active_instance();
    
    p_instance->vulkan.swap_chain_image_views = calloc(p_instance->vulkan.image_count, sizeof(VkImageView));
    
    for (size_t i = 0; i < p_instance->vulkan.image_count; i++)
    {
        VkImageViewCreateInfo  image_view_create_info = { 
            .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image                           = p_instance->vulkan.swap_chain_images[i],
            .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
            .format                          = p_instance->vulkan.swap_chain_image_format,
            .components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY,
            .subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.baseMipLevel   = 0,
            .subresourceRange.levelCount     = 1,
            .subresourceRange.baseArrayLayer = 0,
            .subresourceRange.layerCount     = 1,
            .flags                           = 0,
            .pNext                           = 0
        };

        if ( vkCreateImageView(p_instance->vulkan.device, &image_view_create_info, 0, &p_instance->vulkan.swap_chain_image_views[i]) != VK_SUCCESS ) {
            g_print_error("failed to create image views!\n");
        }
        
    }
}

void create_command_pool ( void )
{

    // Initialized data
    GXInstance_t            *p_instance = g_get_active_instance();
    VkCommandPoolCreateInfo  pool_create_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = p_instance->vulkan.queue_family_indices.g
    };

    if ( vkCreateCommandPool(p_instance->vulkan.device, &pool_create_info, 0, &p_instance->vulkan.command_pool) != VK_SUCCESS )
        g_print_error("Failed to create command pool!\n");
}

void create_command_buffers ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();

    VkCommandBufferAllocateInfo alloc_info = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = p_instance->vulkan.command_pool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = p_instance->vulkan.max_buffered_frames
    };

    p_instance->vulkan.command_buffers = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkCommandBuffer));

    if (vkAllocateCommandBuffers(p_instance->vulkan.device, &alloc_info, p_instance->vulkan.command_buffers) != VK_SUCCESS) {
        g_print_error("failed to allocate command buffers!\n");
    }
}

void create_sync_objects ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();

    VkSemaphoreCreateInfo *semaphore_create_info = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkSemaphoreCreateInfo));
    VkFenceCreateInfo     *fence_create_info     = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkFenceCreateInfo));
    
    semaphore_create_info->sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    fence_create_info->sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info->flags = VK_FENCE_CREATE_SIGNALED_BIT;

    p_instance->vulkan.image_available_semaphores = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkSemaphore));
    p_instance->vulkan.render_finished_semaphores = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkSemaphore));
    p_instance->vulkan.in_flight_fences           = calloc(p_instance->vulkan.max_buffered_frames, sizeof(VkFence));

    for (size_t i = 0; i < p_instance->vulkan.max_buffered_frames; i++)
    {
        if (vkCreateSemaphore(p_instance->vulkan.device, semaphore_create_info, 0, &p_instance->vulkan.image_available_semaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(p_instance->vulkan.device, semaphore_create_info, 0, &p_instance->vulkan.render_finished_semaphores[i]) != VK_SUCCESS ||
            vkCreateFence(p_instance->vulkan.device, fence_create_info, 0, &p_instance->vulkan.in_flight_fences[i]) != VK_SUCCESS) {
            g_print_error("failed to create semaphores!\n");
        }
    }

}

u32 find_memory_type ( u32 type_filter, VkMemoryPropertyFlags properties )
{
    GXInstance_t *p_instance = g_get_active_instance();
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(p_instance->vulkan.physical_device, &mem_properties);

    for (u32 i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    g_print_error("[G10] Failed to find a suitable memory type\n");
    return 0;
}

int check_vulkan_device ( GXInstance_t *p_instance, VkPhysicalDevice physical_device, char **required_extension_names )
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
            
            vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, p_instance->vulkan.surface, &present_support);

            if (present_support)
                indices.p = i;
            else
                passes_queue = false;

            if (passes_queue)
                break;
        }

        p_instance->vulkan.queue_family_indices.g = indices.g;
        p_instance->vulkan.queue_family_indices.p = indices.p;
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

            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, p_instance->vulkan.surface, &p_instance->vulkan.swap_chain_details.capabilities);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, p_instance->vulkan.surface, &format_count, (void*)0);

            p_instance->vulkan.swap_chain_details.formats = calloc(format_count + 1, sizeof(VkSurfaceFormatKHR));

            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, p_instance->vulkan.surface, &format_count, p_instance->vulkan.swap_chain_details.formats);

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, p_instance->vulkan.surface, &present_mode_count, (void *)0);

            p_instance->vulkan.swap_chain_details.present_modes = calloc(present_mode_count + 1, sizeof(VkPresentModeKHR));

            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, p_instance->vulkan.surface, &present_mode_count, p_instance->vulkan.swap_chain_details.present_modes);
            
            if ((bool)(present_mode_count) & (bool)(format_count) == true)
                has_swap_chain = true;
        }
    }

    return passes_queue && has_extensions && has_swap_chain;
}

void create_buffer ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory )
{
    VkMemoryRequirements  mem_requirements;
    GXInstance_t         *p_instance         = g_get_active_instance();
    VkBufferCreateInfo    buffer_info      = { 0 };
    VkMemoryAllocateInfo  alloc_info       = { 0 };
    
    // Create a buffer
    {

        // Populate the create info struct
        buffer_info = (VkBufferCreateInfo) {
            .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
            .size        = size,
            .usage       = usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE
        };

        // Create a buffer
        if ( vkCreateBuffer( p_instance->vulkan.device, &buffer_info, 0, buffer ) != VK_SUCCESS )
            g_print_error("[G10] Failed to create buffer in call to function \"\s\"\n", __FUNCTION__);

        vkGetBufferMemoryRequirements(p_instance->vulkan.device, *buffer, &mem_requirements);
    }

    // Allocate memory for the buffer
    {

        // Populate the allocation struct
        alloc_info = (VkMemoryAllocateInfo) {
            .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize  = mem_requirements.size,
            .memoryTypeIndex = find_memory_type(mem_requirements.memoryTypeBits, properties)
        };

        // Allocate memory for the buffer
        if (vkAllocateMemory(p_instance->vulkan.device, &alloc_info, 0, buffer_memory) != VK_SUCCESS)
            g_print_error("[G10] Failed to allocate memory to buffer in call to function \"%s\"\n", __FUNCTION__);

        // Bind the buffer to the device
        vkBindBufferMemory(p_instance->vulkan.device, *buffer, *buffer_memory, 0);
    }
}

size_t g_load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument checking 
    {
        #ifndef NDEBUG
            if ( path == 0 )
                goto noPath;
        #endif
    }

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if (f == NULL)
        goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer )
        ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Return 1
    return ret;

    // Error handling
    {

        // Argument errors
        {
            noPath:
            #ifndef NDEBUG
                g_print_error("[G10] Null path provided to function \"%s\n", __FUNCTION__);
            #endif

            // Error
            return 0;
        }

        // File errors
        {
            invalid_file:
            #ifndef NDEBUG
                g_print_error("[Standard library] Failed to load file \"%s\". %s\n",path, strerror(errno));
            #endif

            // Error
            return 0;
        }
    }
}

void clear_swap_chain ( void )
{
    GXInstance_t *p_instance = g_get_active_instance();
    for (size_t i = 0; i < p_instance->vulkan.image_count; i++) {
        vkDestroyImageView(p_instance->vulkan.device, p_instance->vulkan.swap_chain_image_views[i], 0);
    }

    vkDestroySwapchainKHR(p_instance->vulkan.device, p_instance->vulkan.swap_chain, 0);
}

int g_window_resize ( GXInstance_t *p_instance )
{

    // TODO: 
    SDL_GetWindowSize(p_instance->sdl2.window, &p_instance->window.width, &p_instance->window.height);

    return 0;
}

int g_print_error ( const char *const format, ... ) 
{

    // Argument check
    {
        if (format == (void*)0)
            goto no_format;
    }

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    // Uses ANSI terminal escapes to set the color to red, 
    // print the message, and restore the color.
    printf("\033[91m");
    #endif

    // TODO: Configure error messages to write to other files?
    vfprintf(log_file, format, aList);
    
    #ifdef BUILD_G10_WITH_ANSI_COLOR
    printf("\033[0m");
    #endif

    va_end(aList);

    // Success
    return 1;
    
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

int g_print_warning ( const char *const format, ... ) 
{

    // Argument check
    {
        if (format == (void*)0)
            goto no_format;
    }

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    // Uses ANSI terminal escapes to set the color to yellow, 
    // print the message, and restore the color.
    printf("\033[93m");
    #endif

    vfprintf(log_file, format, aList);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    printf("\033[0m");
    #endif

    va_end(aList);

    // Success
    return 1;

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

int g_print_log ( const char *const format, ... ) 
{

    // Argument check
    {
        if (format == (void*)0)
            goto no_format;
    }

    // Use the varadic argument list in vprintf call
    va_list aList;
    va_start(aList, format);

    #ifdef BUILD_G10_WITH_ANSI_COLOR
    // Uses ANSI terminal escapes to set the color to light blue, 
    // print the message, and restore the color.
    printf("\033[94m");
    #endif

    vfprintf(log_file, format, aList);
    
    #ifdef BUILD_G10_WITH_ANSI_COLOR
    printf("\033[0m");
    #endif

    va_end(aList);
    va_end(aList);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_format:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"format\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }

}

int g_start_schedule ( GXInstance_t* p_instance, char* name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if(name == (void *)0)
                goto no_name;
        #endif
    }

    // Initialized data
    GXSchedule_t* p_schedule = (GXSchedule_t *) dict_get(p_instance->data.schedules, (char *)name);

    // Error checking
    if ( p_schedule == (void *) 0 )
        goto no_schedule;

    // Start the schedule
    if ( start_schedule(p_schedule) == 0 )
        goto failed_to_start_schedule;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif  

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for \"name\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif  

                // Error
                return 0;
        }
        
        // G10 Errors
        {
            no_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to find a schedule named \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif  
                
                // Error
                return 0;

            failed_to_start_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to start schedule \"%s\" in call to function \"%s\"\n", name, __FUNCTION__);
                #endif  
            
                // Error
                return 0;
        }
    }
}

int g_stop_schedule ( GXInstance_t *p_instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if( p_instance == (void *) 0 )
                goto no_instance;
        #endif
    }

    // Stop the instance's active schedule
    if ( stop_schedule(p_instance->context.schedule) == 0 )
        goto failed_to_stop_schedule;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif  

                // Error
                return 0;
        }
        
        // G10 Errors
        {
            failed_to_stop_schedule:
                #ifndef NDEBUG
                    g_print_error("[G10] [Scheduler] Failed to stop active schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif  
            
                // Error
                return 0;
        }
    }
}

int copy_state ( GXInstance_t *p_instance )
{
    
    // Argument check
    {
        #ifndef NDEBUG
            if ( p_instance == (void *) 0 )
                goto no_instance;
            if ( p_instance->context.scene == (void *) 0 )
                goto no_context_scene;
        #endif
    }

    // Initialized data
    size_t       actor_count = 0,
                 ai_count    = 0;

    GXEntity_t **actors      = 0,
               **ais         = 0;

    // Lock the mutexes
    
    // Physics
    SDL_LockMutex(p_instance->mutexes.move_object);
    SDL_LockMutex(p_instance->mutexes.update_force);
    SDL_LockMutex(p_instance->mutexes.resolve_collision);

    // AI
    SDL_LockMutex(p_instance->mutexes.ai_preupdate);
    SDL_LockMutex(p_instance->mutexes.ai_update);

    // Get a list of actors and ais
    {
        actor_count = dict_keys(p_instance->context.scene->actors, 0);
        ai_count    = dict_keys(p_instance->context.scene->ais, 0);

        actors      = calloc(actor_count+1, sizeof(void *));
        ais         = calloc(ai_count+1, sizeof(void *));

        dict_values(p_instance->context.scene->actors, actors);
        dict_values(p_instance->context.scene->ais, ais);
    }

    // Reconstruct the queues
    {

        // Destroy the old queues
        {

            // Physics
            {
                if (p_instance->queues.actor_move)
                    queue_destroy(&(queue *)p_instance->queues.actor_move);

                if (p_instance->queues.actor_force)
                    queue_destroy(&(queue *)p_instance->queues.actor_force);

                if (p_instance->queues.actor_collision)
                    queue_destroy(&(queue *)p_instance->queues.actor_collision);
            }

            // AI
            {
                if (p_instance->queues.ai_preupdate)
                    queue_destroy(&(queue *)p_instance->queues.ai_preupdate);

                if (p_instance->queues.ai_update)
                    queue_destroy(&(queue *)p_instance->queues.ai_update);
            }
        }

        // Physics
        queue_construct(&p_instance->queues.actor_move);
        queue_construct(&p_instance->queues.actor_force);
        queue_construct(&p_instance->queues.actor_collision);

        // AI
        queue_construct(&p_instance->queues.ai_preupdate);
        queue_construct(&p_instance->queues.ai_update);
    }

    // Populate the new queues
    {

        // Physics
        for (size_t i = 0; i < actor_count; i++)
        {
            queue_enqueue(p_instance->queues.actor_move     , actors[i]);
            queue_enqueue(p_instance->queues.actor_force    , actors[i]);
            queue_enqueue(p_instance->queues.actor_collision, actors[i]);
        }

        // AI
        for (size_t i = 0; i < ai_count; i++)
        {
            queue_enqueue(p_instance->queues.ai_preupdate, ais[i]);
            queue_enqueue(p_instance->queues.ai_update   , ais[i]);
        }
    }

    // Clean the scope
    free(actors);
    free(ais);

    // Unlock the mutexes
        
    // Physics
    SDL_UnlockMutex(p_instance->mutexes.move_object);
    SDL_UnlockMutex(p_instance->mutexes.update_force);
    SDL_UnlockMutex(p_instance->mutexes.resolve_collision);

    // AI
    SDL_UnlockMutex(p_instance->mutexes.ai_preupdate);
    SDL_UnlockMutex(p_instance->mutexes.ai_update);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_context_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] No active scene in instance in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

GXInstance_t *g_get_active_instance ( void )
{
    return active_instance;
}

int g_cache_material ( GXInstance_t *p_instance, GXMaterial_t *material )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (material == (void*)0)
                goto no_material;
        #endif
    }

    dict_add(p_instance->cache.materials, material->name, material);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_material:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int g_cache_part ( GXInstance_t *p_instance, GXPart_t *p_part)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (p_part == (void*)0)
                goto no_part;
        #endif
    }

    //dict_add(p_instance->cached_parts, p_part->name, p_part);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_part:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_part\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int g_cache_shader ( GXInstance_t *p_instance, GXShader_t *p_shader )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (p_shader == (void*)0)
                goto no_shader;
        #endif
    }
    
    dict_add(p_instance->cache.shaders, p_shader->name, p_shader);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_shader:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int g_cache_ai ( GXInstance_t *p_instance, GXAI_t *p_ai )
{
    // Argument check
    {
        #ifndef NDEBUG
            if ( p_instance == (void *) 0 )
                goto no_instance;
            if ( p_ai == (void *) 0 )
                goto no_ai;
        #endif
    }
    
    dict_add(p_instance->cache.ais, p_ai->name, p_ai);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            no_ai:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_ai\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

void g_user_exit ( callback_parameter_t input, GXInstance_t* p_instance )
{
    p_instance->running = false;
    if (p_instance->context.schedule)
    {
        GXThread_t *main_thread = (GXThread_t *)dict_get(p_instance->context.schedule->threads, "Main Thread");

        main_thread->running    = false;
    }
}
  
void g_toggle_mouse_lock ( callback_parameter_t state, GXInstance_t *p_instance )
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

void g_play_sound ( callback_parameter_t state, GXInstance_t *p_instance )
{
    // GXSound_t* sampleSound = NULL;
    // static bool played = false;
    // if (played == false)
    // {
    //     load_sound(&sampleSound, "G10/ding.mp3");
    //     play_sound(sampleSound, false);
    //     played = true;
    // }



}

GXMaterial_t *g_find_material ( GXInstance_t *p_instance, char *name )
{
    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Success
    return (GXMaterial_t *) dict_get(p_instance->cache.materials, (char *)name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

GXPart_t *g_find_part ( GXInstance_t *p_instance, char *name)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Success
    return (GXPart_t *) dict_get(p_instance->cache.parts, (char *)name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

GXShader_t *g_find_shader ( GXInstance_t *p_instance, char *name)
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Success
    return (GXShader_t *) dict_get(p_instance->cache.shaders, (char *)name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

GXAI_t *g_find_ai ( GXInstance_t *p_instance, char *name )
{

    // Argument check
    {
        #ifndef NDEBUG
            if(p_instance == (void *)0)
                goto no_instance;
            if (name == (void*)0)
                goto no_name;
        #endif
    }

    // Success
    return (GXAI_t *) dict_get(p_instance->cache.ais, (char *)name);

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    printf("[G10] Null pointer provided for \"name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

    }
}

int g_exit ( GXInstance_t **pp_instance )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_instance == (void *) 0 )
                goto no_instance;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance = *pp_instance;

    // No more instance for caller
    *pp_instance = 0;

    // Wait for the GPU to finish whatever its doing
    vkDeviceWaitIdle(p_instance->vulkan.device);

    // Cleanup the instance
    {
        
        // Free the instance name
        free(p_instance->name);

        // TODO: Free scenes
        //dict_free_clear(p_instance->data.scenes, &destroy_scene);

        // Cleanup the context
        {

            // Clear the active schedule...
            p_instance->context.schedule = 0;

            // ... the active scene ...
            p_instance->context.scene = 0;

            // ... any loading scene ...
            p_instance->context.loading_scene = 0;

            // ... and the active renderer ...
            p_instance->context.renderer = 0;

            // ... and the user code callback 
            p_instance->context.user_code_callback = 0;
        }

        // Cleanup the data
        {

            // TODO: Cleanup the scenes...
            //dict_free_clear(p_instance->data.scenes, destroy_scene);

            // TODO: ... and the schedules
            //dict_free_clear(p_instance->data.schedules, destroy_schedule);
        }

        // Cleanup the cache
        {        
            
            // TODO: Free the shaders...
            //dict_free_clear(p_instance->cache.shaders, &destroy_shader);
        
            // TODO: Free materials
            //dict_free_clear(p_instance->cache.materials, &destroy_material);

            // ... and the parts...
            dict_free_clear(p_instance->cache.parts, &destroy_part);
        }

        // Cleanup mutexes
        {
            
            // Destroy the entity loading mutex...
            SDL_DestroyMutex(p_instance->mutexes.load_entity);

            // ... and the cache mutexes...
            SDL_DestroyMutex(p_instance->mutexes.shader_cache);
            SDL_DestroyMutex(p_instance->mutexes.part_cache);
            SDL_DestroyMutex(p_instance->mutexes.ai_cache);
            SDL_DestroyMutex(p_instance->mutexes.material_cache);

            // ... and the physics mutexes...
            SDL_DestroyMutex(p_instance->mutexes.move_object);
            SDL_DestroyMutex(p_instance->mutexes.update_force);
            SDL_DestroyMutex(p_instance->mutexes.resolve_collision);

            // ... and the AI mutexes
            SDL_DestroyMutex(p_instance->mutexes.ai_preupdate);
            SDL_DestroyMutex(p_instance->mutexes.ai_update);
        }

        // Cleanup queues
        {

            // Destroy the entity loading queue
            queue_destroy(&p_instance->queues.load_entity);

            // Destroy the light probe loading queue
            //queue_destroy(&p_instance->queues.load_light_probe);

            // Destroy the physics queues
            queue_destroy(&p_instance->queues.actor_move);
            queue_destroy(&p_instance->queues.actor_collision);
            queue_destroy(&p_instance->queues.actor_force);

            // Destroy the AI queues
            queue_destroy(&p_instance->queues.ai_preupdate);
            queue_destroy(&p_instance->queues.ai_update);
        }

        // Cleanup input
        destroy_input(p_instance->input);
    }

    // Cleanup vulkan
    {

        // Clear the swapchain
        clear_swap_chain();

        // Destroy synchronization primatives
        for (size_t i = 0; i < p_instance->vulkan.max_buffered_frames; i++)
        {

            // Destroy semaphores
            vkDestroySemaphore(p_instance->vulkan.device, p_instance->vulkan.render_finished_semaphores[i], 0);
            vkDestroySemaphore(p_instance->vulkan.device, p_instance->vulkan.image_available_semaphores[i], 0);

            // Destroy fences
            vkDestroyFence(p_instance->vulkan.device, p_instance->vulkan.in_flight_fences[i], 0);
        }

        // Destroy the command pool
        vkDestroyCommandPool(p_instance->vulkan.device, p_instance->vulkan.command_pool, 0);

        // Destroy the logical device
        vkDestroyDevice(p_instance->vulkan.device, (void*)0);

        // Destroy the surface
        vkDestroySurfaceKHR(p_instance->vulkan.instance, p_instance->vulkan.surface, (void*) 0);

        // Destroy the instance
        vkDestroyInstance(p_instance->vulkan.instance, (void*) 0) ;
    }

    // Clean up FMOD
    #ifdef BUILD_G10_WITH_FMOD
        
        //FMOD Cleanup

        //Replace this with a struct member later?
        //Not thread-safe
        FMOD_RESULT fmodResult = FMOD_System_Release(instance->fmod.system);

    #endif

    // Clean up discord
    #ifdef BUILD_G10_WITH_DISCORD
    #endif

    // SDL Cleanup
    SDL_DestroyWindow(p_instance->sdl2.window);
    SDL_Quit();
    
    // Free the instance data
    free(p_instance);
    
    g_print_log("[G10] Exit successful\n");

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] Null pointer provided for \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
