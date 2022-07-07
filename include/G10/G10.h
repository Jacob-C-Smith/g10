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
#include <G10/GXMaterial.h>


struct GXInstance_s
{
	// Name 
	char                     *name;
	
	// Ticks
	size_t                    ticks;

	// SDL2 data
	SDL_Window               *window;
	SDL_Event                 event;

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
	VkPipelineLayout          pipeline_layout;
	VkPipeline                graphics_pipeline;
	VkCommandPool             command_pool;
	VkCommandBuffer          *command_buffers;
	VkSemaphore              *image_available_semaphores;
	VkSemaphore              *render_finished_semaphores;
	VkFence                   in_flight_fences;

	u32                       current_frame;
	bool                      framebuffer_resized;

	#pragma pack(push)
	#pragma pack 4
	struct { int g, p; } queue_family_indices;
	#pragma pack(pop)

	struct { VkSurfaceCapabilitiesKHR capabilities; VkSurfaceFormatKHR* formats; VkPresentModeKHR* present_modes; } swap_chain_details;

	// G10 data
	GXInput_t        *input;

	// Loaded scenes
	dict             *scenes;

	// Server 
	//GXServer_t    *server;

	// Cached assets
	dict             *cached_parts;
	dict             *cached_materials;
	dict             *cached_shaders;

	// Delta time
	u32               d, 
		              lastTime;
	float             delta_time;

	u32 	          window_width,
		              window_height;

	size_t            loading_thread_count;

    bool              running;
	float             priority;
};

// Allocators
int           g_init                ( GXInstance_t     **instance, const char *path);

// File operations
size_t        g_load_file           ( const char        *path    , void       *buffer, bool binaryMode); 

// Window operations
int           g_clear               ( void );
int           g_swap                ( GXInstance_t        *instance );
int           g_window_resize       ( GXInstance_t        *instance );

int           g_delta               ( GXInstance_t        *instance );
float         g_time                ( GXInstance_t        *instance );
 

// Debug logging
int           g_print_error         ( const char *const  format  , ... );
int           g_print_warning       ( const char *const  format  , ... );
int           g_print_log           ( const char *const  format  , ... );

// Getters
GXInstance_t* g_get_active_instance ( void );

// Cache operations
int           g_cache_material      ( GXInstance_t        *instance, GXMaterial_t *material );
int           g_cache_part          ( GXInstance_t        *instance, GXPart_t     *part );
int           g_cache_shader        ( GXInstance_t        *instance, GXShader_t   *shader );

GXMaterial_t *g_find_material       ( GXInstance_t        *instance, char         *name );
GXPart_t     *g_find_part           ( GXInstance_t        *instance, char         *name );
GXShader_t   *g_find_shader         ( GXInstance_t        *instance, char         *name );

// Conversions
inline float  to_degrees            ( float radians ) 
{
    return radians * (180 / (float)M_PI);
}
inline float  to_radians            ( float degrees ) 
{
    return  degrees * ((float)M_PI / 180);
}

int           g_exit                ( GXInstance_t      *instance );