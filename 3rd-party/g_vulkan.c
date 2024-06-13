// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

// Vulkan
#include <vulkan/vulkan.h>

// Preprocessor definitions
#define G_VULKAN_INSTANCE_EXTENSION_MAX_QUANTITY 64
#define G_VULKAN_DEVICE_EXTENSION_MAX_QUANTITY   256
#define G_VULKAN_PHYSICAL_DEVICE_MAX_QUANTITY    16
#define G_VULKAN_QUEUE_FAMILY_MAX_QUANTITY       8
#define G_VULKAN_QUEUE_MAX_QUANTITY              256

// Constant
static const float _always_one_point_f = 1.f;

// Function declarations
/** !
 * Get a string from a Vulkan result
 * 
 * @param result the Vulkan result
 * 
 * @return The result string
 */
const char *const g_vulkan_get_result_string ( VkResult result );

/** !
 * Get a string from a Vulkan debug utils message type
 * 
 * @param message_type the Vulkan debug utils message type
 * 
 * @return The result string
 */
const char *const g_vulkan_get_debug_utils_message_type_string ( VkDebugUtilsMessageTypeFlagsEXT message_type );

/** !
 * Validation layer callback 
 * 
 * @param message_severity < VERBOSE | INFO | WARNING | ERROR >
 * @param message_type     < GENERAL | VALIDATION | PERFORMANCE | DEVICE ADDRESS BINDING >
 * @param p_callback_data  pointer to callback data struct
 * @param p_user_data      the active g10 instance
 * 
 * @return VK_FALSE 
 */
static VKAPI_ATTR VkBool32 VKAPI_CALL g_vulkan_debug_callback ( 
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
    VkDebugUtilsMessageTypeFlagsEXT message_type, 
    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, 
    void* p_user_data
);

/** !
 * Initialize the Vulkan API from a json value
 * 
 * @param p_instance pointer to instance
 * @param p_value    pointer 
 * 
 * @return 1 on success, 0 on error
 */
int g_vulkan_initialize ( g_instance *p_instance, json_value *p_value );

/** !
 * Select a physical device
 * 
 * @param p_instance the active instance
 * @param p_value    the json value
 * 
 * @return 1 on success, 0 on error
 */
int g_vulkan_physical_device_select ( g_instance *p_instance, json_value *p_value );

/** !
 * Construct a logical device from the selected physical device
 * 
 * @param p_instance pointer to the active instance
 * @param p_value    a json object
 * 
 * @return 1 on success, 0 on error
 */
int g_vulkan_logical_device_initialize ( g_instance *p_instance, json_value *p_value );

/** !
 * Wait for the device to stop computing
 * 
 * @param p_instance the active instance
 * 
 * @return 1 on success, 0 on error
 */
int g_vulkan_sync_wait_device ( g_instance *p_instance );

/** !
 * Construct a Vulkan semaphore
 * 
 * @param p_instance the active g10 instance
 * @param p_value    the json value
 * 
 * @return 1 on success, 0 on error 
 */
int g_vulkan_sync_semaphore_create ( g_instance *p_instance, json_value *p_value );

// Function definitions
const char *const g_vulkan_get_result_string ( VkResult result )
{

    // Switch on the result
    switch (result)
    {
        case VK_SUCCESS:
            return "Success";
        case VK_NOT_READY:
            return "Not ready";
        case VK_TIMEOUT:
            return "Timeout";
        case VK_EVENT_SET:
            return "Event set";
        case VK_EVENT_RESET:
            return "Event reset";
        case VK_INCOMPLETE:
            return "Incomplete";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "Out of host memory";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "Out of device memory";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "Initialization failed";
        case VK_ERROR_DEVICE_LOST:
            return "Device lost";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "Memory map failed";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "Layers not present";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "Extension not present";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "Feature not present";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "Incompatible driver";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "Too many objects";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "Format not supported";
        case VK_ERROR_FRAGMENTED_POOL:
            return "Fragmented pool";
        case VK_ERROR_UNKNOWN:
            return "Unknown";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "Out of pool memory";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "Invalid external handle";
        case VK_ERROR_FRAGMENTATION:
            return "Fragmentation";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "Invalid opaque capture address";
        case VK_PIPELINE_COMPILE_REQUIRED:
            return "Pipeline compile required";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "Surface lost (Khronos)";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "Native window in use (Khronos)";
        case VK_SUBOPTIMAL_KHR:
            return "Suboptimal (Khronos)";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "Out of date (Khronos)";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "Incompatible display (Khronos)";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "Validation failed";
        case VK_ERROR_INVALID_SHADER_NV:
            return "Invalid shader (Nvidia)";
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            return "Image usage not supported (Khronos)";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            return "Video picture layout not supported (Khronos)";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            return "Video profile operation not supported (Khronos)";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            return "Video profile format not supported (Khronos)";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            return "Video profile codec not supported (Khronos)";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            return "Video standard version not supported (Khronos)";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "Digital rights management format modifier plane layout";
        case VK_ERROR_NOT_PERMITTED_KHR:
            return "Not permitted (Khronos)";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "Full screen exclusive mode lost";
        case VK_THREAD_IDLE_KHR:
            return "Thread idle (Khronos)";
        case VK_THREAD_DONE_KHR:
            return "Thread done (Khronos)";
        case VK_OPERATION_DEFERRED_KHR:
            return "Operation deferred (Khronos)";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "Operation not deferred (Khronos)";
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
            return "Invalid video standard parameters (Khronos)";
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
            return "Compression exhausted";
        case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
            return "Incompatible shader binary";
        default:
            return "This Vulkan error was not recognized";
    }

    // Done
    return (void *) 0;
}

const char *const g_vulkan_get_debug_utils_message_type_string ( VkDebugUtilsMessageTypeFlagsEXT message_type )
{

    // Switch on the result
    switch (message_type)
    {

        // General
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
            return "general";

        // Validation
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
            return "validation";
        
        // Performance
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
            return "performance";

        // Device address binding
        case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
            return "device address binding";

        // Default
        default:
            return "This Vulkan utils message type was not recognized";
    }

    // Done
    return (void *) 0;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL g_vulkan_debug_callback
( 
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity, 
    VkDebugUtilsMessageTypeFlagsEXT message_type, 
    const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, 
    void* p_user_data
)
{

    // Switch on the message severity
    switch (message_severity)
    {

        // Verbose
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:

            // Log the message
            printf("[g10] [vulkan] [validation] [%s] %s\n", g_vulkan_get_debug_utils_message_type_string(message_type), p_callback_data->pMessage);
            
            // Done
            break;

        // Info
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:

            // Log the message
            log_info("[g10] [vulkan] [validation] [%s] %s\n", g_vulkan_get_debug_utils_message_type_string(message_type), p_callback_data->pMessage);
            
            // Done
            break;
        
        // Warning
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:

            // Log the message
            log_warning("[g10] [vulkan] [validation] [%s] %s\n", g_vulkan_get_debug_utils_message_type_string(message_type), p_callback_data->pMessage);
            
            // Done
            break;

        // Error
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:

            // Log the message
            log_error("[g10] [vulkan] [validation] [%s] %s\n", g_vulkan_get_debug_utils_message_type_string(message_type), p_callback_data->pMessage);
            
            // Done
            break;
        
        // Default
        default:

            // Log the message
            printf("[g10] [vulkan] [validation] Unrecognized message severity\n");
            
            // Done
            break;
    }

    // Done
    return VK_FALSE;
}

int g_vulkan_initialize ( g_instance *p_instance, json_value *p_value )
{

    // TODO: Argument check
    //

    // pNext chain: 
    // ❌ VkDebugReportCallbackCreateInfoEXT
    // ✔️ VkDebugUtilsMessengerCreateInfoEXT
    // ❌ VkDirectDriverLoadingListLUNARG
    // ❌ VkExportMetalObjectCreateInfoEXT
    // ❌ VkLayerSettingsCreateInfoEXT
    // ❌ VkValidationFeaturesEXT
    // ❌ VkValidationFlagsEXT

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    if ( p_value    == (void *) 0 ) goto no_value;

    // Initialized data
    VkInstanceCreateInfo _instance_create_info = { 0 };
    VkApplicationInfo _app_info = { 0 };
    VkResult result = VK_ERROR_UNKNOWN;
    unsigned int extension_count = 0;
    void  *p_next = 0;
    void **pp_next = &p_next;
    const char *extension_names[G_VULKAN_INSTANCE_EXTENSION_MAX_QUANTITY] = { 0 };
    json_value *p_validation = (void *) 0,
               *p_device     = (void *) 0;

    #ifdef BUILD_G10_WITH_SDL2
        
        // Get the quantity of required instance extensions
        if ( SDL_Vulkan_GetInstanceExtensions(p_instance->window.sdl2.window, &extension_count, (void *) 0) == SDL_FALSE ) goto failed_to_get_instance_extensions;

        // Error check
        if ( extension_count > G_VULKAN_INSTANCE_EXTENSION_MAX_QUANTITY ) goto too_many_instance_extensions;

        // Get the required instance extensions
        if ( SDL_Vulkan_GetInstanceExtensions(p_instance->window.sdl2.window, &extension_count, extension_names) == SDL_FALSE ) goto failed_to_get_instance_extensions;

    #endif

    // Error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Parse the json object into a Vulkan instance
    {

        // Initialized data
        dict *p_dict = p_value->object;
        VkDebugUtilsMessengerCreateInfoEXT _debug_utils_messenger_create_info = { 0 };
        
        // Store the validation layers array
        p_validation = dict_get(p_dict, "validation");

        // Store the device object
        p_device = dict_get(p_dict, "device");

        // Error check
        if ( ! ( p_validation ) ) goto no_validation;
        if ( p_validation->type != JSON_VALUE_OBJECT ) goto wrong_validation_type;

        // Parse the json object into validation layers
        {

            // Initialized data
            dict *p_dict = p_validation->object;
            json_value *p_enabled = dict_get(p_dict, "enabled"),
                       *p_layers  = dict_get(p_dict, "layers");
            
            // Missing properties
            if ( ! ( p_enabled && p_layers) ) goto missing_validation_properties;

            // Populate the debug utils messenger 
            _debug_utils_messenger_create_info = (VkDebugUtilsMessengerCreateInfoEXT)
            {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                .pNext = *pp_next,
                .flags = 0,
                .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                .pfnUserCallback = g_vulkan_debug_callback,
                .pUserData = p_instance
            };
            
            // Build the pNext chain
            p_next = &_debug_utils_messenger_create_info;

            // Update the pNext state
            *pp_next = p_next;
        }

        // No validation layers
        no_validation:;
    }

    // Populate the application info struct
    _app_info = (VkApplicationInfo)
    {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = p_instance->_name,
        .applicationVersion = VK_MAKE_VERSION(p_instance->version.major, p_instance->version.minor, p_instance->version.patch),
        .pEngineName        = "g10",
        .engineVersion      = VK_MAKE_VERSION(G10_VERSION_MAJOR, G10_VERSION_MINOR, G10_VERSION_PATCH),
        .apiVersion         = VK_API_VERSION_1_3
    };

    // Populate the instance create info struct
    _instance_create_info = (VkInstanceCreateInfo)
    {
        .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext                   = *pp_next,
        .flags                   = 0,
        .pApplicationInfo        = &_app_info,
        .enabledLayerCount       = 0,
        .ppEnabledLayerNames     = 0,
        .enabledExtensionCount   = extension_count,
        .ppEnabledExtensionNames = extension_names
    };

    // Create the Vulkan instance
    result = vkCreateInstance(&_instance_create_info, (void *)0, &p_instance->graphics.vulkan.instance);

    // Error check
    if ( result != VK_SUCCESS ) goto failed_to_create_vulkan_instance;

    // Select a physical device
    if ( g_vulkan_physical_device_select(p_instance, (void *) 0) == 0 ) goto failed_to_select_a_physical_device;

    // Create a logical device
    if ( g_vulkan_logical_device_initialize(p_instance, p_device) == 0 ) goto failed_to_create_a_logical_device;

    // Success
    return 1;

    wrong_validation_type:
    missing_validation_properties:
    failed_to_create_a_logical_device:
    
        // Error
        return 0;

    // Error handling
    {

        // Argument errors
        {

            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_select_a_physical_device:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Failed to select a physical device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        // JSON errors
        {
            wrong_type:
                #ifndef NDEBUG
                    log_error("Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_create_vulkan_instance:
                #ifndef NDEBUG
                    log_error("[vulkan] Call to \"vkCreateInstance\" returned a(n) \"%s\" error in call to function \"%s\"\n", g_vulkan_get_result_string(result), __FUNCTION__);
                #endif

                // More granular error reporting
                switch (result)
                {
                    case VK_ERROR_OUT_OF_HOST_MEMORY:

                        // Write a message to standard out
                        log_info("[g10] A host memory allocation has failed.\n");

                        // Done
                        break;

                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:

                        // Write a message to standard out
                        log_info("[g10] A device memory allocation has failed.\n");

                        // Done
                        break;

                    case VK_ERROR_INITIALIZATION_FAILED:

                        // Write a message to standard out
                        log_info("[g10] Initialization of an object could not be completed for implementation-specific reasons.\n");

                        // Done
                        break;

                    case VK_ERROR_LAYER_NOT_PRESENT:
                                          
                        // Write a message to standard out
                        log_info("[g10] A requested layer is not present or could not be loaded.\n");

                        // Done
                        break;

                    case VK_ERROR_EXTENSION_NOT_PRESENT:
                        
                        // Write a message to standard out
                        log_info("[g10] A requested extension is not supported.\n");

                        // Done
                        break;

                    case VK_ERROR_INCOMPATIBLE_DRIVER:

                        // Write a message to standard out
                        log_info("[g10] The requested version of Vulkan is not supported by the driver or is otherwise incompatible for implementation-specific reasons.\n");

                        // Done
                        break;
                } 

                // Error
                return 0;
        }
    
        // SDL2 errors
        {
            failed_to_get_instance_extensions:
                #ifndef NDEBUG
                    log_error("[sdl2] Call to \"SDL_Vulkan_GetInstanceExtensions\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            too_many_instance_extensions:
                #ifndef NDEBUG
                    log_error("[g10] The maximum quantity of instance extensions has been exceeded in call to function \"%s\"\n", __FUNCTION__);
                    log_info("NOTE: This is a hard coded limit. Now would be a good time to reassess the way this code is implemented\n", __FUNCTION__);                    
                #endif

                // Error
                return 0;
        }
    }
}

int g_vulkan_physical_device_select ( g_instance *p_instance, json_value *p_value )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    // if ( p_value    == (void *) 0 ) goto no_value;

    // Initialized data
    u32 device_count = 0;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkResult result = VK_ERROR_UNKNOWN;
    VkPhysicalDevice _physical_devices[G_VULKAN_PHYSICAL_DEVICE_MAX_QUANTITY] = { 0 };

    // Get the quantity of physical devices
    result = vkEnumeratePhysicalDevices(p_instance->graphics.vulkan.instance, &device_count, (void *) 0);

    // Error checking
    if ( result       == VK_INCOMPLETE                       ) goto failed_to_enumerate_all_physical_devices;
    if ( result       != VK_SUCCESS                          ) goto failed_to_enumerate_physical_devices;
    if ( device_count == 0                                   ) goto failed_to_find_suitable_gpu;
    if ( device_count >  G_VULKAN_PHYSICAL_DEVICE_MAX_QUANTITY ) goto too_many_physical_devices;

    done_enumerating_physical_devices:;

    // Get the quantity of physical devices
    result = vkEnumeratePhysicalDevices(p_instance->graphics.vulkan.instance, &device_count, _physical_devices);

    // Error checking
    if ( result == VK_INCOMPLETE ) goto failed_to_enumerate_all_physical_devices;
    if ( result != VK_SUCCESS    ) goto failed_to_enumerate_physical_devices;
    
    // Store the first physical device
    physical_device = _physical_devices[0];

    // Fast exit
    if ( device_count == 1 ) goto found_a_device;

    found_a_device:

    // Store the physical device
    p_instance->graphics.vulkan.physical_device = physical_device;

    // Success
    return 1;

    // Warnings
    {
        
        // Vulkan
        {
            failed_to_enumerate_all_physical_devices:
                #ifndef NDEBUG
                    log_warning("[g10] [vulkan] Not all physical devices were enumerated in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Continue...
                goto done_enumerating_physical_devices;

            too_many_physical_devices:
                #ifndef NDEBUG
                    log_error("[g10] The maximum quantity of physical devices has been exceeded in call to function \"%s\"\n", __FUNCTION__);
                    log_info("NOTE: This is a hard coded limit. Now would be a good time to reassess the way this code is implemented\n", __FUNCTION__);                    
                #endif

                // Error
                return 0;
        }
    }

    // Error handling
    {

        // Argument errors
        {

            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_enumerate_physical_devices:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Failed to enumerate physical devices in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_find_suitable_gpu:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Failed to find a suitable physical device in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_vulkan_logical_device_initialize ( g_instance *p_instance, json_value *p_value )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    if ( p_value    == (void *) 0 ) goto no_value;

    // pNext chain
    // ❌ VkDeviceDeviceMemoryReportCreateInfoEXT
    // ❌ VkDeviceDiagnosticsConfigCreateInfoNV
    // ❌ VkDeviceGroupDeviceCreateInfo
    // ❌ VkDeviceMemoryOverallocationCreateInfoAMD
    // ❌ VkDevicePrivateDataCreateInfo
    // ❌ VkDeviceQueueShaderCoreControlCreateInfoARM
    // ❌ VkPhysicalDevice16BitStorageFeatures
    // ❌ VkPhysicalDevice4444FormatsFeaturesEXT
    // ❌ VkPhysicalDevice8BitStorageFeatures
    // ❌ VkPhysicalDeviceASTCDecodeFeaturesEXT
    // ❌ VkPhysicalDeviceAccelerationStructureFeaturesKHR
    // ❌ VkPhysicalDeviceAddressBindingReportFeaturesEXT
    // ❌ VkPhysicalDeviceAmigoProfilingFeaturesSEC
    // ❌ VkPhysicalDeviceAttachmentFeedbackLoopDynamicStateFeaturesEXT
    // ❌ VkPhysicalDeviceAttachmentFeedbackLoopLayoutFeaturesEXT
    // ❌ VkPhysicalDeviceBlendOperationAdvancedFeaturesEXT
    // ❌ VkPhysicalDeviceBorderColorSwizzleFeaturesEXT
    // ❌ VkPhysicalDeviceBufferDeviceAddressFeatures
    // ❌ VkPhysicalDeviceBufferDeviceAddressFeaturesEXT
    // ❌ VkPhysicalDeviceClusterCullingShaderFeaturesHUAWEI
    // ❌ VkPhysicalDeviceCoherentMemoryFeaturesAMD
    // ❌ VkPhysicalDeviceColorWriteEnableFeaturesEXT
    // ❌ VkPhysicalDeviceComputeShaderDerivativesFeaturesNV
    // ❌ VkPhysicalDeviceConditionalRenderingFeaturesEXT
    // ❌ VkPhysicalDeviceCooperativeMatrixFeaturesKHR
    // ❌ VkPhysicalDeviceCooperativeMatrixFeaturesNV
    // ❌ VkPhysicalDeviceCopyMemoryIndirectFeaturesNV
    // ❌ VkPhysicalDeviceCornerSampledImageFeaturesNV
    // ❌ VkPhysicalDeviceCoverageReductionModeFeaturesNV
    // ❌ VkPhysicalDeviceCubicClampFeaturesQCOM
    // ❌ VkPhysicalDeviceCubicWeightsFeaturesQCOM
    // ❌ VkPhysicalDeviceCudaKernelLaunchFeaturesNV
    // ❌ VkPhysicalDeviceCustomBorderColorFeaturesEXT
    // ❌ VkPhysicalDeviceDedicatedAllocationImageAliasingFeaturesNV
    // ❌ VkPhysicalDeviceDepthBiasControlFeaturesEXT
    // ❌ VkPhysicalDeviceDepthClampZeroOneFeaturesEXT
    // ❌ VkPhysicalDeviceDepthClipControlFeaturesEXT
    // ❌ VkPhysicalDeviceDepthClipEnableFeaturesEXT
    // ❌ VkPhysicalDeviceDescriptorBufferFeaturesEXT
    // ❌ VkPhysicalDeviceDescriptorIndexingFeatures
    // ❌ VkPhysicalDeviceDescriptorPoolOverallocationFeaturesNV
    // ❌ VkPhysicalDeviceDescriptorSetHostMappingFeaturesVALVE
    // ❌ VkPhysicalDeviceDeviceGeneratedCommandsComputeFeaturesNV
    // ❌ VkPhysicalDeviceDeviceGeneratedCommandsFeaturesNV
    // ❌ VkPhysicalDeviceDeviceMemoryReportFeaturesEXT
    // ❌ VkPhysicalDeviceDiagnosticsConfigFeaturesNV
    // ❌ VkPhysicalDeviceDisplacementMicromapFeaturesNV
    // ❌ VkPhysicalDeviceDynamicRenderingFeatures
    // ❌ VkPhysicalDeviceDynamicRenderingUnusedAttachmentsFeaturesEXT
    // ❌ VkPhysicalDeviceExclusiveScissorFeaturesNV
    // ❌ VkPhysicalDeviceExtendedDynamicState2FeaturesEXT
    // ❌ VkPhysicalDeviceExtendedDynamicState3FeaturesEXT
    // ❌ VkPhysicalDeviceExtendedDynamicStateFeaturesEXT
    // ❌ VkPhysicalDeviceExtendedSparseAddressSpaceFeaturesNV
    // ❌ VkPhysicalDeviceExternalFormatResolveFeaturesANDROID
    // ❌ VkPhysicalDeviceExternalMemoryRDMAFeaturesNV
    // ❌ VkPhysicalDeviceExternalMemoryScreenBufferFeaturesQNX
    // ❌ VkPhysicalDeviceFaultFeaturesEXT
    // ❌ VkPhysicalDeviceFeatures2
    // ❌ VkPhysicalDeviceFragmentDensityMap2FeaturesEXT
    // ❌ VkPhysicalDeviceFragmentDensityMapFeaturesEXT
    // ❌ VkPhysicalDeviceFragmentDensityMapOffsetFeaturesQCOM
    // ❌ VkPhysicalDeviceFragmentShaderBarycentricFeaturesKHR
    // ❌ VkPhysicalDeviceFragmentShaderInterlockFeaturesEXT
    // ❌ VkPhysicalDeviceFragmentShadingRateEnumsFeaturesNV
    // ❌ VkPhysicalDeviceFragmentShadingRateFeaturesKHR
    // ❌ VkPhysicalDeviceFrameBoundaryFeaturesEXT
    // ❌ VkPhysicalDeviceGlobalPriorityQueryFeaturesKHR
    // ❌ VkPhysicalDeviceGraphicsPipelineLibraryFeaturesEXT
    // ❌ VkPhysicalDeviceHostImageCopyFeaturesEXT
    // ❌ VkPhysicalDeviceHostQueryResetFeatures
    // ❌ VkPhysicalDeviceImage2DViewOf3DFeaturesEXT
    // ❌ VkPhysicalDeviceImageCompressionControlFeaturesEXT
    // ❌ VkPhysicalDeviceImageCompressionControlSwapchainFeaturesEXT
    // ❌ VkPhysicalDeviceImageProcessing2FeaturesQCOM
    // ❌ VkPhysicalDeviceImageProcessingFeaturesQCOM
    // ❌ VkPhysicalDeviceImageRobustnessFeatures
    // ❌ VkPhysicalDeviceImageSlicedViewOf3DFeaturesEXT
    // ❌ VkPhysicalDeviceImageViewMinLodFeaturesEXT
    // ❌ VkPhysicalDeviceImagelessFramebufferFeatures
    // ❌ VkPhysicalDeviceIndexTypeUint8FeaturesEXT
    // ❌ VkPhysicalDeviceInheritedViewportScissorFeaturesNV
    // ❌ VkPhysicalDeviceInlineUniformBlockFeatures
    // ❌ VkPhysicalDeviceInvocationMaskFeaturesHUAWEI
    // ❌ VkPhysicalDeviceLegacyDitheringFeaturesEXT
    // ❌ VkPhysicalDeviceLineRasterizationFeaturesEXT
    // ❌ VkPhysicalDeviceLinearColorAttachmentFeaturesNV
    // ❌ VkPhysicalDeviceMaintenance4Features
    // ❌ VkPhysicalDeviceMaintenance5FeaturesKHR
    // ❌ VkPhysicalDeviceMaintenance6FeaturesKHR
    // ❌ VkPhysicalDeviceMemoryDecompressionFeaturesNV
    // ❌ VkPhysicalDeviceMemoryPriorityFeaturesEXT
    // ❌ VkPhysicalDeviceMeshShaderFeaturesEXT
    // ❌ VkPhysicalDeviceMeshShaderFeaturesNV
    // ❌ VkPhysicalDeviceMultiDrawFeaturesEXT
    // ❌ VkPhysicalDeviceMultisampledRenderToSingleSampledFeaturesEXT
    // ❌ VkPhysicalDeviceMultiviewFeatures
    // ❌ VkPhysicalDeviceMultiviewPerViewRenderAreasFeaturesQCOM
    // ❌ VkPhysicalDeviceMultiviewPerViewViewportsFeaturesQCOM
    // ❌ VkPhysicalDeviceMutableDescriptorTypeFeaturesEXT
    // ❌ VkPhysicalDeviceNestedCommandBufferFeaturesEXT
    // ❌ VkPhysicalDeviceNonSeamlessCubeMapFeaturesEXT
    // ❌ VkPhysicalDeviceOpacityMicromapFeaturesEXT
    // ❌ VkPhysicalDeviceOpticalFlowFeaturesNV
    // ❌ VkPhysicalDevicePageableDeviceLocalMemoryFeaturesEXT
    // ❌ VkPhysicalDevicePerStageDescriptorSetFeaturesNV
    // ❌ VkPhysicalDevicePerformanceQueryFeaturesKHR
    // ❌ VkPhysicalDevicePipelineCreationCacheControlFeatures
    // ❌ VkPhysicalDevicePipelineExecutablePropertiesFeaturesKHR
    // ❌ VkPhysicalDevicePipelineLibraryGroupHandlesFeaturesEXT
    // ❌ VkPhysicalDevicePipelinePropertiesFeaturesEXT
    // ❌ VkPhysicalDevicePipelineProtectedAccessFeaturesEXT
    // ❌ VkPhysicalDevicePipelineRobustnessFeaturesEXT
    // ❌ VkPhysicalDevicePortabilitySubsetFeaturesKHR
    // ❌ VkPhysicalDevicePresentBarrierFeaturesNV
    // ❌ VkPhysicalDevicePresentIdFeaturesKHR
    // ❌ VkPhysicalDevicePresentWaitFeaturesKHR
    // ❌ VkPhysicalDevicePrimitiveTopologyListRestartFeaturesEXT
    // ❌ VkPhysicalDevicePrimitivesGeneratedQueryFeaturesEXT
    // ❌ VkPhysicalDevicePrivateDataFeatures
    // ❌ VkPhysicalDeviceProtectedMemoryFeatures
    // ❌ VkPhysicalDeviceProvokingVertexFeaturesEXT
    // ❌ VkPhysicalDeviceRGBA10X6FormatsFeaturesEXT
    // ❌ VkPhysicalDeviceRasterizationOrderAttachmentAccessFeaturesEXT
    // ❌ VkPhysicalDeviceRayQueryFeaturesKHR
    // ❌ VkPhysicalDeviceRayTracingInvocationReorderFeaturesNV
    // ❌ VkPhysicalDeviceRayTracingMaintenance1FeaturesKHR
    // ❌ VkPhysicalDeviceRayTracingMotionBlurFeaturesNV
    // ❌ VkPhysicalDeviceRayTracingPipelineFeaturesKHR
    // ❌ VkPhysicalDeviceRayTracingPositionFetchFeaturesKHR
    // ❌ VkPhysicalDeviceRelaxedLineRasterizationFeaturesIMG
    // ❌ VkPhysicalDeviceRenderPassStripedFeaturesARM
    // ❌ VkPhysicalDeviceRepresentativeFragmentTestFeaturesNV
    // ❌ VkPhysicalDeviceRobustness2FeaturesEXT
    // ❌ VkPhysicalDeviceSamplerYcbcrConversionFeatures
    // ❌ VkPhysicalDeviceScalarBlockLayoutFeatures
    // ❌ VkPhysicalDeviceSchedulingControlsFeaturesARM
    // ❌ VkPhysicalDeviceSeparateDepthStencilLayoutsFeatures
    // ❌ VkPhysicalDeviceShaderAtomicFloat2FeaturesEXT
    // ❌ VkPhysicalDeviceShaderAtomicFloatFeaturesEXT
    // ❌ VkPhysicalDeviceShaderAtomicInt64Features
    // ❌ VkPhysicalDeviceShaderClockFeaturesKHR
    // ❌ VkPhysicalDeviceShaderCoreBuiltinsFeaturesARM
    // ❌ VkPhysicalDeviceShaderDemoteToHelperInvocationFeatures
    // ❌ VkPhysicalDeviceShaderDrawParametersFeatures
    // ❌ VkPhysicalDeviceShaderEarlyAndLateFragmentTestsFeaturesAMD
    // ❌ VkPhysicalDeviceShaderEnqueueFeaturesAMDX
    // ❌ VkPhysicalDeviceShaderFloat16Int8Features
    // ❌ VkPhysicalDeviceShaderImageAtomicInt64FeaturesEXT
    // ❌ VkPhysicalDeviceShaderImageFootprintFeaturesNV
    // ❌ VkPhysicalDeviceShaderIntegerDotProductFeatures
    // ❌ VkPhysicalDeviceShaderIntegerFunctions2FeaturesINTEL
    // ❌ VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT
    // ❌ VkPhysicalDeviceShaderObjectFeaturesEXT
    // ❌ VkPhysicalDeviceShaderSMBuiltinsFeaturesNV
    // ❌ VkPhysicalDeviceShaderSubgroupExtendedTypesFeatures
    // ❌ VkPhysicalDeviceShaderSubgroupUniformControlFlowFeaturesKHR
    // ❌ VkPhysicalDeviceShaderTerminateInvocationFeatures
    // ❌ VkPhysicalDeviceShaderTileImageFeaturesEXT
    // ❌ VkPhysicalDeviceShadingRateImageFeaturesNV
    // ❌ VkPhysicalDeviceSubgroupSizeControlFeatures
    // ❌ VkPhysicalDeviceSubpassMergeFeedbackFeaturesEXT
    // ❌ VkPhysicalDeviceSubpassShadingFeaturesHUAWEI
    // ❌ VkPhysicalDeviceSwapchainMaintenance1FeaturesEXT
    // ❌ VkPhysicalDeviceSynchronization2Features
    // ❌ VkPhysicalDeviceTexelBufferAlignmentFeaturesEXT
    // ❌ VkPhysicalDeviceTextureCompressionASTCHDRFeatures
    // ❌ VkPhysicalDeviceTilePropertiesFeaturesQCOM
    // ❌ VkPhysicalDeviceTimelineSemaphoreFeatures
    // ❌ VkPhysicalDeviceTransformFeedbackFeaturesEXT
    // ❌ VkPhysicalDeviceUniformBufferStandardLayoutFeatures
    // ❌ VkPhysicalDeviceVariablePointersFeatures
    // ❌ VkPhysicalDeviceVertexAttributeDivisorFeaturesKHR
    // ❌ VkPhysicalDeviceVertexInputDynamicStateFeaturesEXT
    // ❌ VkPhysicalDeviceVideoMaintenance1FeaturesKHR
    // ❌ VkPhysicalDeviceVulkan11Features
    // ❌ VkPhysicalDeviceVulkan12Features
    // ❌ VkPhysicalDeviceVulkan13Features
    // ❌ VkPhysicalDeviceVulkanMemoryModelFeatures
    // ❌ VkPhysicalDeviceWorkgroupMemoryExplicitLayoutFeaturesKHR
    // ❌ VkPhysicalDeviceYcbcr2Plane444FormatsFeaturesEXT
    // ❌ VkPhysicalDeviceYcbcrDegammaFeaturesQCOM
    // ❌ VkPhysicalDeviceYcbcrImageArraysFeaturesEXT
    // ❌ VkPhysicalDeviceZeroInitializeWorkgroupMemoryFeatures

    // Initialized data
    VkDeviceCreateInfo _device_create_info = { 0 };
    VkPhysicalDeviceFeatures _physical_device_features = { 0 };
    u32 queue_quantity        = 0,
        queue_family_quantity = 0,
        extension_quantity    = 0;
    VkDeviceQueueCreateInfo _device_queue_create_info = { 0 };
    VkQueueFamilyProperties _queue_family_properties[G_VULKAN_QUEUE_FAMILY_MAX_QUANTITY] = { 0 };
    const char *_extensions[G_VULKAN_DEVICE_EXTENSION_MAX_QUANTITY] = { 0 };
    VkDeviceQueueCreateInfo _queue_create_info[G_VULKAN_QUEUE_MAX_QUANTITY] = { 0 };
    VkResult result = VK_ERROR_UNKNOWN;

    // Error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Parse the json object into a device create info struct
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_features   = dict_get(p_dict, "features"),
                   *p_queues     = dict_get(p_dict, "queues"),
                   *p_extensions = dict_get(p_dict, "extensions");

        // Check for missing properties
        if ( ! ( p_features ) ) goto missing_features_property;

        // Error check
        if ( p_features->type != JSON_VALUE_ARRAY ) goto wrong_features_type;

        // Parse the queue families
        {

            // pNext chain
            // ❌ VkDeviceQueueGlobalPriorityCreateInfoKHR 

            // Initialized data
            bool has_graphics = false,
                 has_compute  = false,
                 has_transfer = false,
                 has_sparse   = false,
                 has_required = false;
            u32 graphics_quantity = 0,
                compute_quantity  = 0,
                transfer_quantity = 0,
                sparse_quantity   = 0,
                total_quantity    = 0;

            // Get the quantity of queue families
            vkGetPhysicalDeviceQueueFamilyProperties(p_instance->graphics.vulkan.physical_device, &queue_family_quantity, (void *) 0);

            // Error check
            if ( queue_family_quantity > G_VULKAN_QUEUE_FAMILY_MAX_QUANTITY ) goto too_many_queue_families;

            // Get the queue families
            vkGetPhysicalDeviceQueueFamilyProperties(p_instance->graphics.vulkan.physical_device, &queue_family_quantity, _queue_family_properties);

            // Roughly categorize each queue family
            for (int i = 0; i < queue_family_quantity; i++)
            {

                // Initialized data
                VkQueueFamilyProperties queue_family_properties = _queue_family_properties[i];
                
                // Graphics
                if ( queue_family_properties.queueFlags & VK_QUEUE_GRAPHICS_BIT )
                {

                    // Set the graphics flag
                    has_graphics = true;

                    // Store the index
                    p_instance->graphics.vulkan.queue_families.graphics_index = i;

                    // Accumulate
                    graphics_quantity++;
                }

                // Compute
                if ( queue_family_properties.queueFlags & VK_QUEUE_COMPUTE_BIT )
                {
                    
                    // Set the compute flag
                    has_compute = true;

                    // Store the index
                    p_instance->graphics.vulkan.queue_families.compute_index = i;

                    // Accumulate
                    compute_quantity++;
                }

                // Transfer
                if ( queue_family_properties.queueFlags & VK_QUEUE_TRANSFER_BIT )
                {
                    
                    // Set the transfer flag
                    has_transfer = true;

                    // Store the index
                    p_instance->graphics.vulkan.queue_families.transfer_index = i;

                    // Accumulate
                    transfer_quantity++;
                }
            
                // Accumulate
                total_quantity += queue_family_properties.queueCount;
            }
            
            // TODO: More checks and more heuristics
            //

            // TODO: Construct more than one queue
            _queue_create_info[0] = (VkDeviceQueueCreateInfo)
            {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext = (void *) 0,
                .flags = 0,
                .queueFamilyIndex = p_instance->graphics.vulkan.queue_families.graphics_index,
                .queueCount = 1,
                .pQueuePriorities = &_always_one_point_f,
            };

            out_of_queues:
        }

        // TODO: Parse the device extensions
        {

            // Initialized data
            array *p_array = p_extensions->list;

            // Store the quantity of extensions
            extension_quantity = array_size(p_array);

            // Iterate through the array
            for (size_t i = 0; i < extension_quantity; i++)
            {
                
                // Initialized data
                json_value *p_extension = (void *) 0;

                // Store the extension json
                array_index(p_array, i, &p_extension);

                // Error check
                if ( p_extension->type != JSON_VALUE_STRING ) goto wrong_feature_type;

                // Store the extension string
                _extensions[i] = p_extension->string;
            }
        }

        // Parse the device features
        {

            // Initialized data
            array *p_array = p_features->list;

            // Iterate through the array
            for (size_t i = 0; i < array_size(p_array); i++)
            {
                
                // Initialized data
                json_value *p_feature = (void *) 0;

                // Store the feature
                array_index(p_array, i, &p_feature);

                // Error check
                if ( p_feature->type != JSON_VALUE_STRING ) goto wrong_feature_type;
            }
        }
    
        // Populate the device create info struct
        _device_create_info = (VkDeviceCreateInfo)
        {
            .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext                   = 0,          // TODO: Build pNext chain
            .flags                   = 0,          // Reserved
            .queueCreateInfoCount    = 1,
            .pQueueCreateInfos       = _queue_create_info,
            .enabledLayerCount       = 0,          // Deprecated since 1.3
            .ppEnabledLayerNames     = (void *) 0, // Deprecated since 1.3
            .enabledExtensionCount   = extension_quantity,
            .ppEnabledExtensionNames = _extensions,
            .pEnabledFeatures        = &_physical_device_features
        };
    }

    // Create the logical device
    result = vkCreateDevice(p_instance->graphics.vulkan.physical_device, &_device_create_info, (void *) 0, &p_instance->graphics.vulkan.device);

    // Error check
    if ( result != VK_SUCCESS ) goto failed_to_create_a_logical_device;

    // Store queues
    vkGetDeviceQueue(p_instance->graphics.vulkan.device, p_instance->graphics.vulkan.queue_families.graphics_index, 0, &p_instance->graphics.vulkan.queue.graphics);
    vkGetDeviceQueue(p_instance->graphics.vulkan.device, p_instance->graphics.vulkan.queue_families.compute_index, 0, &p_instance->graphics.vulkan.queue.compute);
    vkGetDeviceQueue(p_instance->graphics.vulkan.device, p_instance->graphics.vulkan.queue_families.transfer_index, 0, &p_instance->graphics.vulkan.queue.transfer);

    // Success
    return 1;

    // TODO: 
    no_instance:
    no_value:
    wrong_type:
    missing_features_property:
    wrong_features_type:
    wrong_feature_type:
    failed_to_create_a_logical_device:

        // Error
        return 0;

    // Error handling
    {
        
        // Vulkan errors
        {
            too_many_queue_families:
                #ifndef NDEBUG
                    log_error("[g10] The maximum quantity of queue families has been exceeded in call to function \"%s\"\n", __FUNCTION__);
                    log_info("NOTE: This is a hard coded limit. Now would be a good time to reassess the way this code is implemented\n", __FUNCTION__);                    
                #endif

                // Error
                return 0;
        }
    }
}

int g_vulkan_sync_wait_device ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Initialized data
    VkResult result = VK_ERROR_UNKNOWN;

    // Wait for the device to idle
    result = vkDeviceWaitIdle(p_instance->graphics.vulkan.device);

    // Error check
    if ( result != VK_SUCCESS ) goto failed_to_device_wait_idle;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [vulkan] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Vulkan errors
        {
            failed_to_device_wait_idle:
                #ifndef NDEBUG
                    log_error("[vulkan] Call to \"vkDeviceWaitIdle\" returned a(n) \"%s\" error in call to function \"%s\"\n", g_vulkan_get_result_string(result), __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_vulkan_sync_semaphore_create ( g_instance *p_instance, json_value *p_value )
{

    // TODO: Argument check
    //

    // Initialized data
    VkSemaphore semaphore = VK_NULL_HANDLE;
    VkResult result = VK_ERROR_UNKNOWN;

    // Create a semaphore

    // Success
    return 1;

    // TODO: Error handling
    //
}
