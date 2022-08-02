#pragma once

#include <stdio.h>
#include <stdbool.h>
#define _USE_MATH_DEFINES // For some (utterly stupid) reason, macros for numerical constants are not automatically defined... 
#include <math.h>

#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <dict/dict.h>

#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/GXScene.h>
#include <G10/GXInput.h>
#include <G10/GXScheduler.h>
#include <G10/GXMaterial.h>
#include <G10/GXScheduler.h>

#ifdef main
#undef main
#endif

struct GXInstance_s
{
	// Name 
	char                     *name;
	
	// Ticks
	size_t                    ticks;

	// SDL2 data
	SDL_Window               *window;
	SDL_Event                 event;
	SDL_AudioDeviceID         audio_device;

	// Vulkan data
	VkInstance                instance;
	VkDebugUtilsMessengerEXT  debug_messenger;
	VkSurfaceKHR              surface;
	VkPhysicalDevice          physical_device;
	VkDevice                  device;
	VkQueue                   graphics_queue;
	VkQueue                   present_queue;
	VkSwapchainKHR            swap_chain;
	VkImage                  *swap_chain_images;
	
	VkFormat                  swap_chain_image_format;
	VkExtent2D                swap_chain_extent;
	VkImageView              *swap_chain_image_views;
	VkFramebuffer            *swap_chain_framebuffers;
	VkRenderPass              render_pass;
	VkCommandPool             command_pool;
	VkCommandBuffer          *command_buffers;
	VkSemaphore              *image_available_semaphores;
	VkSemaphore              *render_finished_semaphores;
	VkFence                  *in_flight_fences;

	i32                       max_buffered_frames;
	u32                       current_frame,
	                          image_count,
		                      image_index;
	float                     priority;

	#pragma pack(push)
	#pragma pack 4
	struct { int g, p; }      queue_family_indices;
	#pragma pack(pop)

	struct { VkSurfaceCapabilitiesKHR capabilities; VkSurfaceFormatKHR* formats; VkPresentModeKHR* present_modes; } swap_chain_details;

	// G10 data
	GXInput_t                *input;

	dict                     *schedules;

	// Loaded scenes
	dict                     *scenes;
	GXScene_t                *active_scene,
		                     *loading_scene;

	// Server 
	GXServer_t               *server;

	// Cached assets
	dict                     *cached_parts;
	dict                     *cached_materials;
	dict                     *cached_shaders;
	
	// G10 queues
	dict                     *load_entity_queue;
	dict                     *actor_queue;

	// Loading mutexes
	SDL_mutex                *load_entity_mutex;

	// Delta time
	u32                       d, 
		                      last_time;
	float                     delta_time;

	// Window parameters
	u32 	                  window_width,
		                      window_height;

	// How many threads should be assigned to load a scene
	size_t                    loading_thread_count;

	// 
    bool                      running;
};

// Allocators
DLLEXPORT int           g_init                ( GXInstance_t     **instance, const char *path);

// File operations
DLLEXPORT size_t        g_load_file           ( const char        *path    , void       *buffer, bool binaryMode);

// Window operations
DLLEXPORT int           g_clear               ( void );
DLLEXPORT int           g_swap                ( GXInstance_t        *instance );
DLLEXPORT int           g_window_resize       ( GXInstance_t        *instance );

DLLEXPORT int           g_delta               ( GXInstance_t        *instance );
DLLEXPORT float         g_time                ( GXInstance_t        *instance );

// Operations
DLLEXPORT int           render_frame          ( GXInstance_t        *instance );

// Debug logging
DLLEXPORT int           g_print_error         ( const char *const  format  , ... );
DLLEXPORT int           g_print_warning       ( const char *const  format  , ... );
DLLEXPORT int           g_print_log           ( const char *const  format  , ... );

// Getters
DLLEXPORT GXInstance_t* g_get_active_instance ( void );

DLLEXPORT void          create_buffer         ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory );

// Cache operations
DLLEXPORT int           g_cache_material      ( GXInstance_t        *instance, GXMaterial_t *material );
DLLEXPORT int           g_cache_part          ( GXInstance_t        *instance, GXPart_t     *part );
DLLEXPORT int           g_cache_shader        ( GXInstance_t        *instance, GXShader_t   *shader );

DLLEXPORT GXMaterial_t *g_find_material       ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXPart_t     *g_find_part           ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXShader_t   *g_find_shader         ( GXInstance_t        *instance, char         *name );

DLLEXPORT void          g_user_exit           ( callback_parameter_t *input, GXInstance_t *instance);

// Conversions
inline float  to_degrees            ( float radians )
{
    return radians * (180.f / (float)M_PI);
}
inline float  to_radians            ( float degrees )
{
    return  degrees * ((float)M_PI / 180.f);
}

DLLEXPORT int           g_exit                ( GXInstance_t      *instance );