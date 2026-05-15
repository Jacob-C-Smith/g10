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
#include <transform.h>
#include <attachment.h>
#include <pipeline.h>
#include <renderer.h>
#include <camera.h>
#include <scene.h>

#define G10_BUILD_WITH_SDL3

#ifdef G10_BUILD_WITH_SDL3
#include <SDL3/SDL.h>
#endif

// structure definitions
struct g_instance_s
{

    // name
    char _name[255+1];

    // running?
    bool running; 

    // schedule
    // schedule *p_schedule;

    // context
    struct
    {
        fn_user_code *pfn_user_code;
        renderer *p_renderer;
        scene *p_scene;
        input *p_input;
        u16 fixed_tick_rate;
    } context;

    // graphics
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

    // version
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
        dict *p_pipeline,
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

int poll_input ( g_instance *p_instance );
int program_pipeline ( const char _name[], fn_pipeline_bind_once *pfn_once, fn_pipeline_cull *pfn_cull, fn_pipeline_bind_each *pfn_each, fn_pipeline_draw *pfn_draw );

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

int instance_info ( g_instance *p_instance );

void logger_push();
void logger_pop();
void logger_pad();