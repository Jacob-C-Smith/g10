/** !
 * Include header for G10.
 * 
 * @file g10/g10.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Build version
#define G10_VERSION_MAJOR 1
#define G10_VERSION_MINOR 0
#define G10_VERSION_PATCH 0

// Preprocessor error checking
#if defined G10_BUILD_WITH_VULKAN && defined G10_BUILD_WITH_OPENGL
    #error "[g10] [preprocessor] g10 builds can only use one graphics API at a time!"
#endif

#if defined G10_BUILD_WITH_SDL2 && defined G10_BUILD_WITH_GLFW
    #error "[g10] [preprocessor] g10 builds can only use one WSI at a time!"
#endif

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// log module
#include <log/log.h>

// sync module
#include <sync/sync.h>

// hash cache module
#include <hash_cache/hash.h>

// array module
#include <array/array.h>

// queue module
#include <queue/queue.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// circular buffer module
#include <circular_buffer/circular_buffer.h>

// parallel module
#include <parallel/parallel.h>
#include <parallel/schedule.h>

// g10
#include <g10/gtypedef.h>
#include <g10/scene.h>
#include <g10/server.h>
#include <g10/renderer.h>
#include <g10/user_code.h>
#include <g10/ai.h>
#include <g10/shell.h>
#include <g10/input.h>

// Platform dependent macros
#ifdef _WIN64
    #define DLLEXPORT extern __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

// Set reallocator macros
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef STACK_REALLOC
    #undef STACK_REALLOC
    #define STACK_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef QUEUE_REALLOC
    #undef QUEUE_REALLOC
    #define QUEUE_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef CIRCULAR_BUFFER_REALLOC
    #undef CIRCULAR_BUFFER_REALLOC
    #define CIRCULAR_BUFFER_REALLOC(p, sz) realloc(p, sz)
#endif

#ifndef G10_REALLOC
    #define G10_REALLOC(p, sz) realloc(p,sz)
#endif

// 3rd party

// Discord
#ifdef BUILD_G10_WITH_DISCORD
    #include <discord_game_sdk.h>
#endif

// FMOD
#ifdef BUILD_G10_WITH_FMOD
    #include <FMOD-core/fmod.h>
#endif

// SDL2
#ifdef G10_BUILD_WITH_SDL2
    #include <SDL2/SDL.h>
#endif

// Vulkan
#ifdef G10_BUILD_WITH_VULKAN
    #include <vulkan/vulkan.h>
#endif

#ifdef G10_BUILD_WITH_OPENGL
    #include <glad/glad.h>
#endif

// Structures
struct g_instance_s
{

    // Name
    char _name[255+1];

    // Running?
    bool running; 

    // Schedule
    schedule *p_schedule;

    // Context
    struct
    {
        fn_user_code_callback  pfn_user_code_callback;
        renderer              *p_renderer;
        server                *p_server;
        scene                 *p_scene;
        input                 *p_input;
        u16                    fixed_tick_rate;
    } context;

    // Graphics
    union
    {
        #ifdef G10_BUILD_WITH_VULKAN
            struct
            {
                VkInstance instance;
                VkPhysicalDevice physical_device;
                VkDevice device;
                
                struct 
                {
                    u32 graphics_index,
                        compute_index,
                        transfer_index,
                        sparse_index;
                } queue_families;

                struct
                {
                    VkQueue graphics,
                            compute,
                            transfer;
                } queue;
            } vulkan;
        #elif defined G10_BUILD_WITH_OPENGL
            struct
            {
                int openGL;
            } opengl;
        #endif
    } graphics;

    // Version
    struct
    {
        u16 major,
            minor,
            patch;
    } version;
    
    // Window
    struct 
    {
        u32 width;
        u32 height;
        char *title;
        union
        {
            #ifdef G10_BUILD_WITH_SDL2
                struct
                {
                    SDL_Window *window;
                    SDL_Event event;
                    #ifdef G10_BUILD_WITH_VULKAN
                    #elif defined G10_BUILD_WITH_OPENGL
                        SDL_GLContext gl_context;
                    #endif
                } sdl2;
            #endif
        };
    } window;

    // Locks
    //struct
    //{
    //    
    //} locks;

    // Cache
    struct 
    {
        dict *p_materials,
             *p_shaders;
    } cache;
    
    // Time
    struct
    {
        timestamp init;
        timestamp exit;
    } time;  

    // Debug
    shell *p_shell;
    circular_buffer *debug;
};

// Initializers
/** !
 * 
 * 
 * 
 * 
 */
u0 g_init_early ( void ) __attribute__((constructor));

// Allocators
/** !
 *  Construct a g10 instance
 *
 * @param pp_instance return
 * @param path        path to instance json file
 *
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_init ( g_instance **pp_instance, const char *p_path );

// Accessors
/** !
 *  Get a pointer to the instance singleton
 * 
 * @param void
 * 
 * @return pointer to g10 instance singleton on success, null pointer on error
 */
DLLEXPORT g_instance *g_get_active_instance ( void );

// Global state
/** !
 *  Start the active g10 instance
 * 
 * @param void
 * 
 * @sa g_stop
 * 
 * @return void
 */
DLLEXPORT void g_start ( void );

/** !
 *  Stop the active g10 instance
 *
 * @param void
 *
 * @sa g_start
 *
 * @return void
 */
DLLEXPORT void g_stop ( void );

/** ! 
 * Print info about a g10 instance
 * 
 * @param p_instance active instance
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int g_info ( g_instance *p_instance );

// File
/** !
 *  Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path        path to the file
 * @param buffer      buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT size_t g_load_file ( const char *const p_path, void *const p_buffer, bool binary_mode );

// Cleanup
/** !
 *  Destroy a g10 instance
 *
 * @param p_instance pointer to the instance pointer
 *
 * @sa g_init
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_exit ( g_instance **pp_instance );

// Destructors
/** !
 * Clean up
 * 
 * 
 * 
 */
u0 g_exit_late ( void ) __attribute__((destructor));
