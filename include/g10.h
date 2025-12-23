// header guard
#pragma once

// gsdk
/// core
#include <core/log.h>
#include <core/sync.h>

/// data
#include <data/array.h>
#include <data/cache.h>
#include <data/circular_buffer.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// g10
#include <gtypedef.h>
#include <attachment.h>
#include <pipeline.h>

#define G10_BUILD_WITH_SDL3

#ifdef G10_BUILD_WITH_SDL3
#include <SDL3/SDL.h>
#endif

// structure definitions
struct g_instance_s
{

    // Name
    char _name[255+1];

    // Running?
    bool running; 

    // Schedule
    // schedule *p_schedule;

    // Context
    struct
    {
        // fn_user_code_callback  pfn_user_code_callback;
        renderer *p_renderer;
        // server                *p_server;
        // scene                 *p_scene;
        // input                 *p_input;
        u16 fixed_tick_rate;
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
        #elif defined G10_BUILD_WITH_SDL3
            struct
            {
                SDL_GPUDevice* device;
                SDL_GPUTexture *swapchain_texture;
                SDL_GPUCommandBuffer *command_buffer;
            } sdl3;
        #endif
    } graphics;

    // Version
    struct
    {
        u16 major,
            minor,
            patch;
    } version;
    
    // window
    struct 
    {
        u32 width;
        u32 height;
        char *title;
        union
        {
            #ifdef G10_BUILD_WITH_SDL3
                struct
                {
                    SDL_Window *window;
                    SDL_Event event;
                } sdl3;
            #endif
        };
    } window;

    // cache
    struct 
    {
        cache *p_pipeline,
              *p_attachment,
              *p_texture,
              *p_geometry;
    } cache;
    
    // time
    struct
    {
        timestamp init;
        timestamp exit;
    } time;  

    // debug
    circular_buffer *debug;
};

// function declarations
/// constructors
/** !
 *  Construct a g10 instance
 *
 * @param pp_instance return
 * @param path        path to instance json file
 *
 * @return 1 on success, 0 on error
 */
int g_init 
(
    g_instance **pp_instance,

    const char *p_path
);

g_instance *g_active_instance ( void );

/** !
 * Return the size of a file IF buffer == 0 ELSE read a file into buffer
 * 
 * @param path path to the file
 * @param buffer buffer
 * @param binary_mode "wb" IF true ELSE "w"
 * 
 * @return 1 on success, 0 on error
 */
size_t load_file ( const char *path, void *buffer, bool binary_mode );
