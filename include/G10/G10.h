 #pragma once
#define BUILD_G10_WITH_DISCORD

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
#include <G10/GXRenderer.h>
#include <G10/GXInput.h>
#include <G10/GXScheduler.h>
#include <G10/GXMaterial.h>
#include <G10/GXCollider.h>
#include <G10/GXServer.h>

#ifdef BUILD_G10_WITH_DISCORD
#include <G10/GXDiscordIntegration.h>
#include <discord_game_sdk.h>
#endif

//#include <UI/UI.h>

#ifdef main
#undef main
#endif

struct GXInstance_s
{

    // Name 
    char                     *name;
    
    // SDL2 
    struct {
        SDL_Window               *window;
        SDL_Event                 event;
        SDL_AudioDeviceID         audio_device;
    }           sdl2;

    // Vulkan 
    struct {
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
    }           vulkan;

    // Window parameters
    struct {
        u32 width,
            height;
    }           window;

    // Context
    struct {
        GXSchedule_t  *schedule;
        GXScene_t     *scene,
                      *loading_scene;
        GXRenderer_t  *renderer;
        int          (*user_code_callback) (GXInstance_t *instance);
    }           context;

    // Data
    struct {
        dict *schedules,
             *scenes;
    }           data;

    // Networking
    struct {
        GXServer_t *server;
        GXClient_t *client;
    }           networking;

    // Cache
    struct {
        dict *parts,
             *materials,
             *shaders,
             *ais;
    }           cache;

    // Queues
    struct {
        queue *load_entity,
              *actor_move,
              *actor_collision,
              *actor_force,
              *ai_preupdate,
              *ai_update;
    }           queues;
    
    // Mutexes
    struct {
        SDL_mutex *load_entity,
                  *shader_cache,
                  *part_cache,
                  *ai_cache,
                  *material_cache,
                  *move_object,
                  *update_force,
                  *resolve_collision,
                  *ai_preupdate,
                  *ai_update;
    }           mutexes;

    // Time
    struct {
        size_t ticks;
        u32    d, 
               last_time;
        u64    clock_div;
        float  delta_time;
    }           time;
    
    // Discord integration
    #ifdef BUILD_G10_WITH_DISCORD
    
    struct {
        struct IDiscordCore                *core;
        struct IDiscordUserManager         *users;
        struct IDiscordAchievementManager  *achievements;
        struct IDiscordActivityManager     *activities;
        struct IDiscordRelationshipManager *relationships;
        struct IDiscordApplicationManager  *application;
        struct IDiscordLobbyManager        *lobbies;
        struct DiscordUser                  user;
        DiscordUserId                       user_id;
    }           discord_integration;

    #endif

    // Input 
    GXInput_t                *input;

    // How many threads should be used to load a scene
    size_t                    loading_thread_count;

    bool                      running;
};

// Allocators

/* !
 *  Construct a G10 instance from a JSON file
 *
 * @param pp_instance : return
 * @param path        : Path to instance JSON file
 * 
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_init                ( GXInstance_t     **pp_instance, const char *path );

/* !
 * TODO: DOCUMENT
 *  Create a Vulkan buffer
 *
 * @param size          : Double pointer to instance
 * @param usage         : Path to instance JSON 
 * @param properties    :
 * @param buffer        :
 * @param buffer_memory :
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
DLLEXPORT size_t        g_load_file           ( const char        *path    , void       *buffer, bool binaryMode );

// Window operations
/* !
 *  Resize the window using instance->window_width and instance->window_height
 *
 * @param instance : The active instance
 *
 * @return 1 on success, 0 on error
 */
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

/* !
 *  Set an instances active schedule, and start running said schedule
 * 
 *  @param instance : the instance
 *  @param name     : the name of the schedule to start running
 * 
 *  @sa g_stop_schedule
 *  @sa g_user_exit
 * 
 *  @return 1 on success, 0 on error
 */
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
 *
 * @param instance: The active instance
 * @param shader  : A pointer to a shader
 *
 * @sa g_find_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_cache_shader        ( GXInstance_t        *instance, GXShader_t   *shader );

/* !
 * Cache an ai. Caching an ai adds it to the garbage collector.
 *
 * @param instance : The active instance
 * @param ai       : A pointer to an ai
 *
 * @sa g_find_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int           g_cache_ai            ( GXInstance_t        *instance, GXAI_t       *ai );

DLLEXPORT GXMaterial_t *g_find_material       ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXPart_t     *g_find_part           ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXShader_t   *g_find_shader         ( GXInstance_t        *instance, char         *name );
DLLEXPORT GXAI_t       *g_find_ai             ( GXInstance_t        *instance, char         *name );

// User opertations
DLLEXPORT void          g_user_exit           ( callback_parameter_t input, GXInstance_t    *instance );
DLLEXPORT void          g_toggle_mouse_lock   ( callback_parameter_t state, GXInstance_t    *instance );

// Conversions

/* !
 * Convert radians to degrees
 *
 * @param radians : Radians
 *
 * @sa to_radians
 *
 * @return degrees
 */
inline float            to_degrees            ( float radians )
{
    return radians * (180.f / (float)3.141593);
}

/* !
 * Convert degrees to radians
 *
 * @param degrees : Degrees
 *
 * @sa to_degrees
 *
 * @return Radians
 */
inline float            to_radians            ( float degrees )
{
    return  degrees * ((float)3.141593 / 180.f);
}

// Destructors
DLLEXPORT int           g_exit                ( GXInstance_t      *instance );