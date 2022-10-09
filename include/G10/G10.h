 #pragma once

#include <stdio.h>
#include <stdbool.h>
#define HAVE_M_PI

#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <dict/dict.h>
#include <queue/queue.h>

#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/GXScene.h>
#include <G10/GXInput.h>
#include <G10/GXScheduler.h>
#include <G10/GXMaterial.h>
#include <G10/GXCollider.h>
#include <G10/GXServer.h>

//#include <UI/UI.h>

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
	#pragma pack (4)
	struct { int g, p; }      queue_family_indices;
	#pragma pack(pop)

	struct { VkSurfaceCapabilitiesKHR capabilities; VkSurfaceFormatKHR* formats; VkPresentModeKHR* present_modes; } swap_chain_details;

	// G10 

	// Context data

	// Input 
	GXInput_t                *input;

	GXSchedule_t             *active_schedule;
	GXScene_t                *active_scene;
	GXServer_t               *active_server;

	// Schedules
	dict                     *schedules;

	// Loaded scenes
	dict                     *scenes;
	GXScene_t                *loading_scene;

	// Server 
	GXServer_t               *server;

	dict                     *cached_parts;
	dict                     *cached_materials;
	dict                     *cached_shaders;
	
	// G10 queues
	queue                    *load_entity_queue;
	queue                    *actor_move_queue;
	queue                    *actor_collision_queue;
	queue                    *actor_force_queue;
	queue                    *ai_preupdate_queue;
	queue                    *ai_update_queue;

	int                     (*user_code_callback)(GXInstance_t* instance);

	// Mutexes
	SDL_mutex                *load_entity_mutex,
	                         *shader_cache_mutex,
		                     *part_cache_mutex,
		                     *material_cache_mutex,
		                     *move_object_mutex,
		                     *update_force_mutex,
		                     *resolve_collision_mutex,
		                     *ai_preupdate_mutex,
		                     *ai_update_mutex;
	
	// UI
	//UIInstance_t             *ui_instance;

	// Delta time
	u32                       d, 
		                      last_time;
	u64                       clock_div;
	float                     delta_time;

	// Window parameters
	u32 	                  window_width,
		                      window_height;

	// How many threads should be assigned to load a scene
	size_t                    loading_thread_count;

    bool                      running;
};

// Allocators

/* !
 *  Construct a G10 instance from a JSON file
 *
 * @param pp_instance : Pointer to pointer to instance
 * @param path        : Path to instance JSON file
 * 
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_init                ( GXInstance_t     **pp_instance, const char *path);

/* !
 *  Create a Vulkan buffer
 *
 * @param pp_instance : Double pointer to instance
 * @param path        : Path to instance JSON file
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT void          create_buffer         ( VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer *buffer, VkDeviceMemory *buffer_memory );

// File operations
/* !
 *  Load a file. If buffer is null pointer, returns size of file
 *
 * @param path       : Path to file
 * @param buffer     : Buffer to read file
 * @param binaryMode : "r" if false else "rb"
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT size_t        g_load_file           ( const char        *path    , void       *buffer, bool binaryMode);

// Window operations
DLLEXPORT int           g_clear               ( void );
DLLEXPORT int           g_swap                ( GXInstance_t        *instance );
DLLEXPORT int           g_window_resize       ( GXInstance_t        *instance );

DLLEXPORT int           g_delta               ( GXInstance_t        *instance );
DLLEXPORT float         g_time                ( GXInstance_t        *instance );

// Debug logging

/* !
 *  printf in red
 *
 * @param format : printf formatted text
 * @param ...    : Additional parameters
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_print_error         ( const char *const  format  , ... );

/* !
 *  printf in yellow
 *
 * @param format : printf formatted text
 * @param ...    : Additional parameters
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_print_warning       ( const char *const  format  , ... );

/* !
 *  printf in blue
 *
 * @param format : printf formatted text
 * @param ...    : Additional parameters
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_print_log           ( const char *const  format  , ... );
DLLEXPORT int           g_assertion_failed    ( const char        *format  );

DLLEXPORT int           g_start_schedule      ( GXInstance_t *instance, char *name );

// State copy
/* !
 *  Copies state from context data into instance
 *
 * @param instance: The active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           copy_state            ( GXInstance_t *instance );

// Getters
/* !
 *  Returns the active instance
 * 
 *  @return pointer to active instance on success, 0 on error
 */
DLLEXPORT GXInstance_t* g_get_active_instance ( void );

// Cache operations
/* !
 * Cache a material. Caching a material adds it to the garbage collector.
 * Cached materials can be accessed, so long as they are in memory 
 * 
 * @param instance: The active instance
 * @param material: A pointer to a material
 *
 * @sa g_find_material
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_cache_material      ( GXInstance_t        *instance, GXMaterial_t *material );

/* !
 * Cache a part. Caching a part adds it to the garbage collector.
 * Cached parts can be accessed, so long as they are in memory
 *
 * @param instance: The active instance
 * @param part    : A pointer to a part
 *
 * @sa g_find_part
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_cache_part          ( GXInstance_t        *instance, GXPart_t     *part );

/* !
 * Cache a shader. Caching a shader adds it to the garbage collector.
 * Cached shaders can be accessed, so long as they are in memory
 *
 * @param instance: The active instance
 * @param shader  : A pointer to a shader
 *
 * @sa g_find_shader
 *
 * `/*-+b
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_cache_shader        ( GXInstance_t        *instance, GXShader_t   *shader );

DLLEXPORT GXMaterial_t *g_find_material       ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXPart_t     *g_find_part           ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXShader_t   *g_find_shader         ( GXInstance_t        *instance, char         *name );

// User opertations
DLLEXPORT void          g_user_exit           ( callback_parameter_t *input, GXInstance_t *instance);
DLLEXPORT void          g_toggle_mouse_lock(callback_parameter_t state, GXInstance_t* instance);

// Conversions
inline float            to_degrees            ( float radians )
{
    return radians * (180.f / (float)3.141593);
}
inline float            to_radians            ( float degrees )
{
    return  degrees * ((float)3.141593 / 180.f);
}

// Destructors
DLLEXPORT int           g_exit                ( GXInstance_t      *instance );