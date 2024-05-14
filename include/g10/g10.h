﻿/** !
 * Include header for G10.
 * 
 * @file g10/g10.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// log module
#include <log/log.h>

// sync module
#include <sync/sync.h>

// crypto module
#include <crypto/crypto.h>

// array module
#include <array/array.h>

// queue module
#include <queue/queue.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// parallel module
#include <parallel/parallel.h>
#include <parallel/schedule.h>

// g10
#include <g10/gtypedef.h>
#include <g10/server.h>
#include <g10/user_code.h>
#include <g10/ai.h>

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

#ifndef G10_REALLOC
    #define G10_REALLOC(p, sz) realloc(p,sz)
#endif

// Build version
#define G10_VERSION_MAJOR 1
#define G10_VERSION_MINOR 0
#define G10_VERSION_PATCH 0

// Build options
#define BUILD_G10_WITH_ANSI_COLOR true
//#define BUILD_G10_WITH_DISCORD
//#define BUILD_G10_WITH_FMOD
#define BUILD_G10_WITH_SDL2 true
#define BUILD_G10_WITH_VULKAN true
//#define BUILD_G10_WITH_OPENGL true
#define BUILD_G10_WITH_NETWORKING true
#define BUILD_G10_WITH_AVX2 true
#define BUILD_G10_WITH_AVX512 true

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
#ifdef BUILD_G10_WITH_SDL2
    #include <SDL2/SDL.h>
#endif

// Vulkan
#ifdef BUILD_G10_WITH_VULKAN
    #include <vulkan/vulkan.h>
#endif

// Structures
struct g_instance_s
{

    // Name
    char _name[255+1];

    // Running?
    bool running; 

    // Schedule
    parallel_schedule *p_schedule;

    // Context
    struct
    {
        fn_user_code_callback  pfn_user_code_callback;
        server                *p_server;
        u16                    fixed_tick_rate;
    } context;

    // Graphics
    union
    {
        #ifdef BUILD_G10_WITH_VULKAN
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
        #elif defined BUILD_G10_WITH_OPENGL
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
            #ifdef BUILD_G10_WITH_SDL2
            struct
            {
                SDL_Window *window;
                SDL_Event event;
            } sdl2;
            #endif
        };
    } window;

    // Locks
    //struct
    //{
    //    
    //} locks;

    // Time
    struct
    {
        timestamp init;
        timestamp exit;
    } time;  
};

// Allocators
/** !
 *  Construct a g10 instance
 *
 * @param pp_instance : return
 * @param path        : Path to instance json file
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

/** ! 
 * Spawn an HTTP server on another thread 
 * 
 * @param p_instance active instance
 * 
 * @return 1 on success, 0 on errpr
*/
// DLLEXPORT int g_start_server ( g_instance *p_instance );

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

// Destructors
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
