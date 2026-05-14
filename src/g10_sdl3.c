// standard library
#include <stdio.h>
#include <stdlib.h>

// gsdk
/// data
#include <data/array.h>
#include <data/dict.h>

/// reflection
#include <reflection/json.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <renderer.h>
#include <attachment.h>
#include <framebuffer.h>
#include <render_pass.h>
#include <pipeline.h>
#include <aabb.h>
#include <geometry.h>
#include <sampler.h>
#include <uniform.h>
#include <texture.h>

// sdl3
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

// forward declarations
/// sdl3
int g_sdl3_init ( g_instance *p_instance );
int g_sdl3_window_from_json ( g_instance *p_instance, const json_value *p_value );

/// renderer
int g_sdl3_renderer_from_json ( renderer **pp_renderer, const json_value *p_value );
int g_sdl3_render_early ( g_instance *p_instance );
int g_sdl3_render_draw ( g_instance *p_instance );
int g_sdl3_render_late ( g_instance *p_instance );

/// render pass
int g_sdl3_render_pass_from_json ( render_pass **pp_render_pass, const json_value *p_value );
int g_sdl3_render_pass_draw ( g_instance *p_instance, render_pass *p_render_pass );

/// attachment
int g_sdl3_attachment_from_json ( attachment **pp_attachment, const json_value *p_value );

/// pipeline
int g_sdl3_pipeline_from_json ( pipeline **pp_pipeline, const json_value *p_value );
int g_sdl3_pipeline_bind ( render_pass *p_render_pass, pipeline *p_pipeline );
int g_sdl3_pipeline_draw ( render_pass *p_render_pass, pipeline *p_pipeline );

/// framebuffer
int g_sdl3_framebuffer_from_json ( framebuffer **pp_framebuffer, const json_value *p_value );

/// geometry
int g_sdl3_geometry_from_json ( geometry **pp_geometry, const json_value *p_value );
int g_sdl3_geometry_bind ( render_pass *p_render_pass, geometry *p_geometry );

/// texture
int g_sdl3_texture_from_data ( texture **pp_texture, u32 width, u32 height, u32 channels, const void *p_data );
int g_sdl3_texture_construct ( texture **pp_texture, u32 width, u32 height, u32 channels, const void *p_data );
int g_sdl3_texture_from_color ( texture **pp_texture, f32 r, f32 g, f32 b, f32 a );
int g_sdl3_texture_load ( texture **pp_texture, const char *p_path );

/// sampler
int g_sdl3_sampler_from_json ( sampler **pp_sampler, const json_value *p_value );

/// uniform
int g_sdl3_uniform_from_json ( uniform **pp_uniform, const json_value *p_value );

// static data
dict *p_sdl2_key_lookup = (void *) 0;
dict *p_sdl2_key_scancode = (void *) 0;

struct 
{
    char _name[64];
    SDL_Scancode _scancode;
    bool _active;
} _key_lookup[SDL_SCANCODE_COUNT] = 
{

    // invalid
    [SDL_SCANCODE_UNKNOWN] = { ._name = "INVALID", ._active = false, ._scancode = SDL_SCANCODE_UNKNOWN },

    // letters
    [SDL_SCANCODE_A] = { ._name = "A", ._active = false, ._scancode = SDL_SCANCODE_A },
    [SDL_SCANCODE_B] = { ._name = "B", ._active = false, ._scancode = SDL_SCANCODE_B },
    [SDL_SCANCODE_C] = { ._name = "C", ._active = false, ._scancode = SDL_SCANCODE_C },
    [SDL_SCANCODE_D] = { ._name = "D", ._active = false, ._scancode = SDL_SCANCODE_D },
    [SDL_SCANCODE_E] = { ._name = "E", ._active = false, ._scancode = SDL_SCANCODE_E },
    [SDL_SCANCODE_F] = { ._name = "F", ._active = false, ._scancode = SDL_SCANCODE_F },
    [SDL_SCANCODE_G] = { ._name = "G", ._active = false, ._scancode = SDL_SCANCODE_G },
    [SDL_SCANCODE_H] = { ._name = "H", ._active = false, ._scancode = SDL_SCANCODE_H },
    [SDL_SCANCODE_I] = { ._name = "I", ._active = false, ._scancode = SDL_SCANCODE_I },
    [SDL_SCANCODE_J] = { ._name = "J", ._active = false, ._scancode = SDL_SCANCODE_J },
    [SDL_SCANCODE_K] = { ._name = "K", ._active = false, ._scancode = SDL_SCANCODE_K },
    [SDL_SCANCODE_L] = { ._name = "L", ._active = false, ._scancode = SDL_SCANCODE_L },
    [SDL_SCANCODE_M] = { ._name = "M", ._active = false, ._scancode = SDL_SCANCODE_M },
    [SDL_SCANCODE_N] = { ._name = "N", ._active = false, ._scancode = SDL_SCANCODE_N },
    [SDL_SCANCODE_O] = { ._name = "O", ._active = false, ._scancode = SDL_SCANCODE_O },
    [SDL_SCANCODE_P] = { ._name = "P", ._active = false, ._scancode = SDL_SCANCODE_P },
    [SDL_SCANCODE_Q] = { ._name = "Q", ._active = false, ._scancode = SDL_SCANCODE_Q },
    [SDL_SCANCODE_R] = { ._name = "R", ._active = false, ._scancode = SDL_SCANCODE_R },
    [SDL_SCANCODE_S] = { ._name = "S", ._active = false, ._scancode = SDL_SCANCODE_S },
    [SDL_SCANCODE_T] = { ._name = "T", ._active = false, ._scancode = SDL_SCANCODE_T },
    [SDL_SCANCODE_U] = { ._name = "U", ._active = false, ._scancode = SDL_SCANCODE_U },
    [SDL_SCANCODE_V] = { ._name = "V", ._active = false, ._scancode = SDL_SCANCODE_V },
    [SDL_SCANCODE_W] = { ._name = "W", ._active = false, ._scancode = SDL_SCANCODE_W },
    [SDL_SCANCODE_X] = { ._name = "X", ._active = false, ._scancode = SDL_SCANCODE_X },
    [SDL_SCANCODE_Y] = { ._name = "Y", ._active = false, ._scancode = SDL_SCANCODE_Y },
    [SDL_SCANCODE_Z] = { ._name = "Z", ._active = false, ._scancode = SDL_SCANCODE_Z },

    [SDL_SCANCODE_1] = { ._name = "1", ._active = false, ._scancode = SDL_SCANCODE_1 },
    [SDL_SCANCODE_2] = { ._name = "2", ._active = false, ._scancode = SDL_SCANCODE_2 },
    [SDL_SCANCODE_3] = { ._name = "3", ._active = false, ._scancode = SDL_SCANCODE_3 },
    [SDL_SCANCODE_4] = { ._name = "4", ._active = false, ._scancode = SDL_SCANCODE_4 },
    [SDL_SCANCODE_5] = { ._name = "5", ._active = false, ._scancode = SDL_SCANCODE_5 },
    [SDL_SCANCODE_6] = { ._name = "6", ._active = false, ._scancode = SDL_SCANCODE_6 },
    [SDL_SCANCODE_7] = { ._name = "7", ._active = false, ._scancode = SDL_SCANCODE_7 },
    [SDL_SCANCODE_8] = { ._name = "8", ._active = false, ._scancode = SDL_SCANCODE_8 },
    [SDL_SCANCODE_9] = { ._name = "9", ._active = false, ._scancode = SDL_SCANCODE_9 },
    [SDL_SCANCODE_0] = { ._name = "0", ._active = false, ._scancode = SDL_SCANCODE_0 },

    // white space
    [SDL_SCANCODE_RETURN]    = { ._name = "RETURN"   , ._active = false, ._scancode = SDL_SCANCODE_RETURN },
    [SDL_SCANCODE_ESCAPE]    = { ._name = "ESCAPE"   , ._active = false, ._scancode = SDL_SCANCODE_ESCAPE },
    [SDL_SCANCODE_BACKSPACE] = { ._name = "BACKSPACE", ._active = false, ._scancode = SDL_SCANCODE_BACKSPACE },
    [SDL_SCANCODE_TAB]       = { ._name = "TAB"      , ._active = false, ._scancode = SDL_SCANCODE_TAB },
    [SDL_SCANCODE_SPACE]     = { ._name = "SPACE"    , ._active = false, ._scancode = SDL_SCANCODE_SPACE },

    // symbols
    [SDL_SCANCODE_MINUS]        = { ._name = "MINUS"        , ._active = false, ._scancode = SDL_SCANCODE_MINUS },
    [SDL_SCANCODE_EQUALS]       = { ._name = "EQUALS"       , ._active = false, ._scancode = SDL_SCANCODE_EQUALS },
    [SDL_SCANCODE_LEFTBRACKET]  = { ._name = "LEFT BRACKET" , ._active = false, ._scancode = SDL_SCANCODE_LEFTBRACKET },
    [SDL_SCANCODE_RIGHTBRACKET] = { ._name = "RIGHT BRACKET", ._active = false, ._scancode = SDL_SCANCODE_RIGHTBRACKET },
    [SDL_SCANCODE_BACKSLASH]    = { ._name = "BACKSLASH"    , ._active = false, ._scancode = SDL_SCANCODE_BACKSLASH }, 
    [SDL_SCANCODE_NONUSHASH]    = { ._name = "NONUSHASH"    , ._active = false, ._scancode = SDL_SCANCODE_NONUSHASH }, 
    [SDL_SCANCODE_SEMICOLON]    = { ._name = "SEMICOLON"    , ._active = false, ._scancode = SDL_SCANCODE_SEMICOLON },
    [SDL_SCANCODE_APOSTROPHE]   = { ._name = "APOSTROPHE"   , ._active = false, ._scancode = SDL_SCANCODE_APOSTROPHE },
    [SDL_SCANCODE_GRAVE]        = { ._name = "GRAVE"        , ._active = false, ._scancode = SDL_SCANCODE_GRAVE }, 
    [SDL_SCANCODE_COMMA]        = { ._name = "COMMA"        , ._active = false, ._scancode = SDL_SCANCODE_COMMA },
    [SDL_SCANCODE_PERIOD]       = { ._name = "PERIOD"       , ._active = false, ._scancode = SDL_SCANCODE_PERIOD },
    [SDL_SCANCODE_SLASH]        = { ._name = "SLASH"        , ._active = false, ._scancode = SDL_SCANCODE_SLASH },

    // f keys
    [SDL_SCANCODE_F1]   = { ._name = "F1", ._active = false, ._scancode = SDL_SCANCODE_F1},
    [SDL_SCANCODE_F2]   = { ._name = "F2", ._active = false, ._scancode = SDL_SCANCODE_F2},
    [SDL_SCANCODE_F3]   = { ._name = "F3", ._active = false, ._scancode = SDL_SCANCODE_F3},
    [SDL_SCANCODE_F4]   = { ._name = "F4", ._active = false, ._scancode = SDL_SCANCODE_F4},
    [SDL_SCANCODE_F5]   = { ._name = "F5", ._active = false, ._scancode = SDL_SCANCODE_F5},
    [SDL_SCANCODE_F6]   = { ._name = "F6", ._active = false, ._scancode = SDL_SCANCODE_F6},
    [SDL_SCANCODE_F7]   = { ._name = "F7", ._active = false, ._scancode = SDL_SCANCODE_F7},
    [SDL_SCANCODE_F8]   = { ._name = "F8", ._active = false, ._scancode = SDL_SCANCODE_F8},
    [SDL_SCANCODE_F9]   = { ._name = "F9", ._active = false, ._scancode = SDL_SCANCODE_F9},
    [SDL_SCANCODE_F10]  = { ._name = "F10", ._active = false, ._scancode = SDL_SCANCODE_F10},
    [SDL_SCANCODE_F11]  = { ._name = "F11", ._active = false, ._scancode = SDL_SCANCODE_F11},
    [SDL_SCANCODE_F12]  = { ._name = "F12", ._active = false, ._scancode = SDL_SCANCODE_F12},

    // special
    [SDL_SCANCODE_CAPSLOCK]    = { ._name = "CAPS LOCK"   , ._active = false, ._scancode = SDL_SCANCODE_SLASH },
    [SDL_SCANCODE_PRINTSCREEN] = { ._name = "PRINT SCREEN", ._active = false, ._scancode = SDL_SCANCODE_PRINTSCREEN },
    [SDL_SCANCODE_SCROLLLOCK]  = { ._name = "SCROLL LOCK" , ._active = false, ._scancode = SDL_SCANCODE_SCROLLLOCK },
    [SDL_SCANCODE_PAUSE]       = { ._name = "PAUSE"       , ._active = false, ._scancode = SDL_SCANCODE_PAUSE },
    [SDL_SCANCODE_INSERT]      = { ._name = "INSERT"      , ._active = false, ._scancode = SDL_SCANCODE_INSERT },
    
    // navigation
    [SDL_SCANCODE_HOME]         = { ._name = "HOME"     , ._active = false, ._scancode = SDL_SCANCODE_HOME },
    [SDL_SCANCODE_PAGEUP]       = { ._name = "PAGE UP"  , ._active = false, ._scancode = SDL_SCANCODE_PAGEUP },
    [SDL_SCANCODE_DELETE]       = { ._name = "DELETE"   , ._active = false, ._scancode = SDL_SCANCODE_DELETE },
    [SDL_SCANCODE_END]          = { ._name = "END"      , ._active = false, ._scancode = SDL_SCANCODE_END },
    [SDL_SCANCODE_PAGEDOWN]     = { ._name = "PAGE DOWN", ._active = false, ._scancode = SDL_SCANCODE_PAGEDOWN },
    [SDL_SCANCODE_RIGHT]        = { ._name = "RIGHT"    , ._active = false, ._scancode = SDL_SCANCODE_RIGHT },
    [SDL_SCANCODE_LEFT]         = { ._name = "LEFT"     , ._active = false, ._scancode = SDL_SCANCODE_LEFT },
    [SDL_SCANCODE_DOWN]         = { ._name = "DOWN"     , ._active = false, ._scancode = SDL_SCANCODE_DOWN },
    [SDL_SCANCODE_UP]           = { ._name = "UP"       , ._active = false, ._scancode = SDL_SCANCODE_UP },
    [SDL_SCANCODE_NUMLOCKCLEAR] = { ._name = "NUM LOCK" , ._active = false, ._scancode = SDL_SCANCODE_NUMLOCKCLEAR },
    
    // keypad
    [SDL_SCANCODE_KP_DIVIDE]   = { ._name = "KEYPAD DIVIDE"  , ._active = false, ._scancode = SDL_SCANCODE_KP_DIVIDE },
    [SDL_SCANCODE_KP_MULTIPLY] = { ._name = "KEYPAD MULTIPLY", ._active = false, ._scancode = SDL_SCANCODE_KP_MULTIPLY },
    [SDL_SCANCODE_KP_MINUS]    = { ._name = "KEYPAD MINUS"   , ._active = false, ._scancode = SDL_SCANCODE_KP_MINUS },
    [SDL_SCANCODE_KP_PLUS]     = { ._name = "KEYPAD PLUS"    , ._active = false, ._scancode = SDL_SCANCODE_KP_PLUS },
    [SDL_SCANCODE_KP_ENTER]    = { ._name = "KEYPAD ENTER"   , ._active = false, ._scancode = SDL_SCANCODE_KP_ENTER },
    [SDL_SCANCODE_KP_1]        = { ._name = "KEYPAD 1"       , ._active = false, ._scancode = SDL_SCANCODE_KP_1 },
    [SDL_SCANCODE_KP_2]        = { ._name = "KEYPAD 2"       , ._active = false, ._scancode = SDL_SCANCODE_KP_2 },
    [SDL_SCANCODE_KP_3]        = { ._name = "KEYPAD 3"       , ._active = false, ._scancode = SDL_SCANCODE_KP_3 },
    [SDL_SCANCODE_KP_4]        = { ._name = "KEYPAD 4"       , ._active = false, ._scancode = SDL_SCANCODE_KP_4 },
    [SDL_SCANCODE_KP_5]        = { ._name = "KEYPAD 5"       , ._active = false, ._scancode = SDL_SCANCODE_KP_5 },
    [SDL_SCANCODE_KP_6]        = { ._name = "KEYPAD 6"       , ._active = false, ._scancode = SDL_SCANCODE_KP_6 },
    [SDL_SCANCODE_KP_7]        = { ._name = "KEYPAD 7"       , ._active = false, ._scancode = SDL_SCANCODE_KP_7 },
    [SDL_SCANCODE_KP_8]        = { ._name = "KEYPAD 8"       , ._active = false, ._scancode = SDL_SCANCODE_KP_8 },
    [SDL_SCANCODE_KP_9]        = { ._name = "KEYPAD 9"       , ._active = false, ._scancode = SDL_SCANCODE_KP_9 },
    [SDL_SCANCODE_KP_0]        = { ._name = "KEYPAD 0"       , ._active = false, ._scancode = SDL_SCANCODE_KP_0 },
    [SDL_SCANCODE_KP_PERIOD]   = { ._name = "KEYPAD PERIOD"  , ._active = false, ._scancode = SDL_SCANCODE_KP_PERIOD },

    // modifier
    [SDL_SCANCODE_LCTRL]  = { ._name = "LEFT CONTROL" , ._active = false, ._scancode = SDL_SCANCODE_LCTRL },
    [SDL_SCANCODE_LSHIFT] = { ._name = "LEFT SHIFT"   , ._active = false, ._scancode = SDL_SCANCODE_LSHIFT },
    [SDL_SCANCODE_LALT]   = { ._name = "LEFT ALT"     , ._active = false, ._scancode = SDL_SCANCODE_LALT },
    [SDL_SCANCODE_RCTRL]  = { ._name = "RIGHT CONTROL", ._active = false, ._scancode = SDL_SCANCODE_RCTRL },
    [SDL_SCANCODE_RSHIFT] = { ._name = "RIGHT SHIFT"  , ._active = false, ._scancode = SDL_SCANCODE_RSHIFT },
    [SDL_SCANCODE_RALT]   = { ._name = "RIGHT ALT"    , ._active = false, ._scancode = SDL_SCANCODE_RALT }
};

// function definitions
const char *key_lookup_key_accessor ( const void *const p_value )
{
    struct {
        char _name[64];
        SDL_Scancode _scancode;
        bool _active;
    } *p_key_lookup = (void *)p_value;

    return p_key_lookup->_name;
}

int g_sdl3_init ( g_instance *p_instance )
{

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // initialized data 
    bool ok = true;

    // initialize sdl3
    ok = SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    if ( !ok ) goto failed_to_initialize_sdl3;

    // construct the key lookups
    if ( dict_construct(&p_sdl2_key_lookup  , SDL_SCANCODE_COUNT, NULL, (fn_key_accessor *)key_lookup_key_accessor, NULL) == 0 ) goto failed_to_construct_dict;
    if ( dict_construct(&p_sdl2_key_scancode, SDL_SCANCODE_COUNT, NULL, (fn_key_accessor *)key_lookup_key_accessor, NULL) == 0 ) goto failed_to_construct_dict;

    // populate the key lookup
    for (size_t i = 0; i < SDL_SCANCODE_COUNT; i++)
    {

        // Add the key
        dict_add(p_sdl2_key_lookup  , &_key_lookup[i]);
        dict_add(p_sdl2_key_scancode, &_key_lookup[i]);
    }

    // success
    return 1;
    
    // error handling
    {

        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // dict errors
        {
            failed_to_construct_dict:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Failed to construct key lookup in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // sdl3 errors
        {
            failed_to_initialize_sdl3:
                #ifndef NDEBUG
                    log_error("[sdl3] Failed to initialize sdl3 in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_render_early ( g_instance *p_instance )
{

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // initialized data
    u32 width  = 0,
        height = 0;
        
    // acquire the command buffer
    p_instance->graphics.sdl3.command_buffer = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.device);

    // get the swapchain texture
    SDL_WaitAndAcquireGPUSwapchainTexture
    (
        p_instance->graphics.sdl3.command_buffer, 
        p_instance->window.sdl3.window, 

        &p_instance->graphics.sdl3.swapchain_texture, 
        &width, 
        &height
    );

    // swapchain texture unavailable
    if ( p_instance->graphics.sdl3.swapchain_texture == NULL )
    {
        
        // submit the empty command buffer
        SDL_SubmitGPUCommandBuffer(p_instance->graphics.sdl3.command_buffer);
        
        // unavailable
        return 0;
    }

    // success 
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_render_draw ( g_instance *p_instance ) 
{

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // initialized data
    renderer *p_renderer = p_instance->context.p_renderer;
    array *p_passes = p_renderer->p_passes;
    size_t len = array_size(p_passes);

    // iterate through each render pass
    for (size_t i = 0; i < 1; i++)
    {
            
        // initialized data
        render_pass *p_render_pass = NULL;
        
        // get the i'th render pass
        array_index(p_passes, i, (void **)&p_render_pass);

        // draw the render pass 
        g_sdl3_render_pass_draw(p_instance, p_render_pass);
    }

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_render_late ( g_instance *p_instance )
{

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // submit the command buffer
    SDL_SubmitGPUCommandBuffer(p_instance->graphics.sdl3.command_buffer);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_render_pass_draw ( g_instance *p_instance, render_pass *p_render_pass )
{

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // initialized data
    framebuffer *p_framebuffer = p_render_pass->p_framebuffer;
    size_t len = array_size(p_render_pass->p_pipelines);
    size_t attachment_len = array_size(p_framebuffer->p_attachments);
    SDL_GPUColorTargetInfo _color_target_ci[8] = { 0 };
    SDL_GPUDepthStencilTargetInfo _depth_target_ci = { 0 };

    // iterate through each attachment
    for (size_t i = 0; i < attachment_len; i++)
    {

        // initialized data
        attachment *p_attachment = NULL;

        // get the i'th attachment
        array_index(p_framebuffer->p_attachments, i, (void **)&p_attachment);

        // setup the color target info
        _color_target_ci[i] = (SDL_GPUColorTargetInfo)
        {
            .clear_color = (SDL_FColor)
            {
                p_framebuffer->clear[0], 
                p_framebuffer->clear[1], 
                p_framebuffer->clear[2], 
                p_framebuffer->clear[3]
            },
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_STORE,
            .texture = p_attachment->p_handle
        };

        // use the swapchain texture for the output attachment
        if ( 0 == strncmp(p_attachment->_name, "OUTPUT", 6) )
            _color_target_ci[i].texture = p_instance->graphics.sdl3.swapchain_texture;
    }
    
    // populate depth target
    if ( p_framebuffer->p_depth )
    {
        _depth_target_ci = (SDL_GPUDepthStencilTargetInfo)
        {
            .texture = p_framebuffer->p_depth->p_handle,
            .clear_depth = 1.0f,
            .load_op = SDL_GPU_LOADOP_CLEAR,
            .store_op = SDL_GPU_STOREOP_DONT_CARE,
            .stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
            .stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
            .cycle = false,
            .clear_stencil = 0
        };
    }

    // begin the render pass
    p_render_pass->p_handle = SDL_BeginGPURenderPass(
        p_instance->graphics.sdl3.command_buffer, 
        
        (const SDL_GPUColorTargetInfo *)&_color_target_ci, 
        attachment_len, 
        ( p_framebuffer->p_depth ) ? &_depth_target_ci : (void *) 0
    );
    
    // viewport and scissor
    {
        SDL_SetGPUViewport
        (
            p_render_pass->p_handle, 
            &(SDL_GPUViewport)
            {
                .x = 0, 
                .y = 0, 
                .w = (float)p_instance->window.width, 
                .h = (float)p_instance->window.height,
                .min_depth = 0.0f,
                .max_depth = 1.0f
            }
        );

        SDL_SetGPUScissor
        (
            p_render_pass->p_handle, 
            &(SDL_Rect)
            {
                0, 
                0, 
                (int)p_instance->window.width, 
                (int)p_instance->window.height
            }
        );
    }

    // iterate through each pipeline
    for (size_t i = 0; i < len; i++)
    {
        
        // initialized data
        pipeline *p_pipeline = NULL;

        // store the i'th pipeline
        array_index(p_render_pass->p_pipelines, i, (void **)&p_pipeline);

        // bind the pipeline
        g_sdl3_pipeline_bind(p_render_pass, p_pipeline);
        
        // bind once
        if ( p_pipeline->pfn_bind_once )
            p_pipeline->pfn_bind_once(p_render_pass, p_pipeline);

        // pipeline draw
        g_sdl3_pipeline_draw(p_render_pass, p_pipeline);
    } 

    // end the render pass
    SDL_EndGPURenderPass(p_render_pass->p_handle);

    // success
    return 1;

    // error handling
    {
        
        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_window_from_json ( g_instance *p_instance, const json_value *p_value )
{

    // argument check
    if ( p_instance    ==        (void *) 0 ) goto no_instance;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    SDL_GPUDevice* device = NULL;
    u32 sdl3_window_flags = SDL_WINDOW_RESIZABLE;

    // parse the window object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_width  = NULL,
                   *p_height = NULL,
                   *p_title  = NULL;

        dict_get(p_dict, "width" , (void **)&p_width);
        dict_get(p_dict, "height", (void **)&p_height);
        dict_get(p_dict, "title" , (void **)&p_title);

        // error check
        if ( p_width  == (void *) 0 ) goto no_width_property;
        if ( p_height == (void *) 0 ) goto no_height_property;
        if ( p_title  == (void *) 0 ) goto no_title_property;

        // type check
        if ( p_width->type  != JSON_VALUE_INTEGER ) goto wrong_width_type;
        if ( p_height->type != JSON_VALUE_INTEGER ) goto wrong_height_type;
        if ( p_title->type  != JSON_VALUE_STRING  ) goto wrong_title_type;
    
        // store the width
        p_instance->window.width = (u32) p_width->integer;

        // store the height
        p_instance->window.height = (u32) p_height->integer;

        // copy the title
        {

            // initialized data
            size_t len = strlen(p_title->string);

            // allocate memory for the string
            p_instance->window.title = default_allocator(0, sizeof(char) * ( len + 1 ));

            // error check
            if ( p_instance->window.title == (void *) 0 ) goto no_mem;

            // initialize
            memset(p_instance->window.title, 0, sizeof(char) * ( len + 1 ));

            // copy the string
            strncpy(p_instance->window.title, p_title->string, len);
        }
    }

    // construct the sdl3 window
    p_instance->window.sdl3.window = SDL_CreateWindow(
        p_instance->window.title,
        (int) p_instance->window.width, 
        (int) p_instance->window.height,
        sdl3_window_flags
    );

    // show the window
    SDL_ShowWindow(p_instance->window.sdl3.window);

    // create a gpu device
    device = SDL_CreateGPUDevice
    (
        SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_METALLIB,
        true, 
        NULL
    );

    // use the window for the game
    SDL_ClaimWindowForGPUDevice(device, p_instance->window.sdl3.window);

    // store the device
    p_instance->graphics.sdl3.device = device;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            no_width_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"width\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            no_height_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"height\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            no_title_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"title\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_width_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"width\" of parameter \"p_value\" must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_height_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"height\" of parameter \"p_value\" must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_title_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"title\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_renderer_from_json ( renderer **pp_renderer, const json_value *p_value )
{

    // argument check
    if ( pp_renderer   ==        (void *) 0 ) goto no_renderer;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    renderer *p_renderer = default_allocator(NULL, sizeof(renderer));

    // error check
    if ( NULL == p_renderer ) goto no_mem;

    // parse the renderer object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name        = NULL,
                   *p_pipelines   = NULL,
                   *p_attachments = NULL,
                   *p_passes      = NULL;
        
        dict_get(p_dict, "name"       , (void **)&p_name);
        dict_get(p_dict, "pipelines"  , (void **)&p_pipelines);
        dict_get(p_dict, "attachments", (void **)&p_attachments);
        dict_get(p_dict, "passes"     , (void **)&p_passes);

        // error check
        if ( p_name        == (void *) 0 ) goto no_name_property;
        if ( p_pipelines   == (void *) 0 ) goto no_pipelines_property;
        if ( p_attachments == (void *) 0 ) goto no_attachments_property;
        if ( p_passes      == (void *) 0 ) goto no_passes_property;

        // type check
        if ( p_name->type        != JSON_VALUE_STRING ) goto wrong_name_type;
        if ( p_pipelines->type   !=  JSON_VALUE_ARRAY ) goto wrong_pipelines_type;
        if ( p_attachments->type !=  JSON_VALUE_ARRAY ) goto wrong_attachments_type;
        if ( p_passes->type      !=  JSON_VALUE_ARRAY ) goto wrong_passes_type;

        // store the name
        strncpy(p_renderer->_name, p_name->string, sizeof(p_renderer->_name) - 1);

        // construct lookups
        dict_construct(&p_renderer->p_attachments, 64, NULL, (fn_key_accessor *)attachment_key_accessor, NULL);

        // iterate through each pipeline json
        for (size_t i = 0; i < array_size(p_pipelines->list); i++)
        {

            // initialized data
            pipeline *p_pipeline = NULL;
            json_value *p_value = NULL;
            
            // store the i'th pipeline json
            array_index(p_pipelines->list, i, (void **)&p_value);

            // construct the i'th pipeline
            g_sdl3_pipeline_from_json(&p_pipeline, p_value);
        }
    
        // iterate through each attachment json
        for (size_t i = 0; i < array_size(p_attachments->list); i++)
        {

            // initialized data
            attachment *p_attachment = NULL;
            json_value *p_value = NULL;
            
            // store the i'th attachment json
            array_index(p_attachments->list, i, (void **)&p_value);

            // construct the i'th attachment
            g_sdl3_attachment_from_json(&p_attachment, p_value);

            // store the i'th attachment
            dict_add(p_renderer->p_attachments, p_attachment);
        }
    
        // construct render passes
        {

            // initiaized data
            size_t len = array_size(p_passes->list);

            // construct an array
            array_construct(&p_renderer->p_passes, len);

            // iterate through each render pass json
            for (size_t i = 0; i < len; i++)
            {

                // initialized data
                render_pass *p_render_pass = NULL;
                json_value *p_value = NULL;
                
                // store the i'th render pass json
                array_index(p_passes->list, i, (void **)&p_value);

                // construct the i'th render pass
                g_sdl3_render_pass_from_json(&p_render_pass, p_value);

                // store the i'th render pass
                array_add(p_renderer->p_passes, p_render_pass);
            }
        }
    }

    // function pointers
    p_renderer->pfn_early = g_sdl3_render_early;
    p_renderer->pfn_draw  = g_sdl3_render_draw;
    p_renderer->pfn_late  = g_sdl3_render_late;
    
    // return a pointer to the caller
    *pp_renderer = p_renderer;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // g10 errors
        {
            failed_to_index:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Failed to index array in call to function %s\n");
                #endif

                // error
                return 0;
        }

        // json errors
        {
            
            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/attachment.json\n");
                #endif

                // error
                return 0;

            no_pipelines_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"pipelines\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            no_attachments_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"attachments\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            no_passes_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"name\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_pipelines_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"pipelines\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_attachments_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"attachments\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_passes_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"passes\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_attachment_from_json ( attachment **pp_attachment, const json_value *p_value )
{
    
    // argument check
    if ( pp_attachment ==        (void *) 0 ) goto no_attachment;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    attachment *p_attachment = default_allocator(0, sizeof(attachment));
    SDL_GPUTextureFormat format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM;
    SDL_GPUTextureUsageFlags usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;

    // error check
    if ( NULL == p_attachment ) goto no_mem;

    // parse the attachment object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name = NULL,
                   *p_type = NULL;

        dict_get(p_dict, "name", (void **)&p_name);
        dict_get(p_dict, "type", (void **)&p_type);

        // error check
        if ( p_name  == (void *) 0 ) goto no_name_property;
        if ( p_type == (void *) 0 ) goto no_type_property;

        // type check
        if ( p_name->type != JSON_VALUE_STRING ) goto wrong_name_type;
        if ( p_type->type != JSON_VALUE_STRING ) goto wrong_type_type;

        // store the name
        strncpy(p_attachment->_name, p_name->string, sizeof(p_attachment->_name) - 1);

        // TODO: type
        if ( 0 == strncmp(p_type->string, "texture", 8) )
            format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM,
            usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
        else if ( 0 == strncmp(p_type->string, "depth", 6) )
            format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
            usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
        else if ( 0 == strncmp(p_type->string, "framebuffer", 11 ) ) goto done;
        
        // create an SDL GPU texture
        {
            SDL_GPUTextureCreateInfo _ci = 
            {
                .type = SDL_GPU_TEXTURETYPE_2D,
                .format = format,
                .usage = usage,
                .width = p_instance->window.width,  
                .height = p_instance->window.height, 
                .layer_count_or_depth = 1,  
                .num_levels = 1,  
                .sample_count = 0
            };

            p_attachment->p_handle = SDL_CreateGPUTexture(p_instance->graphics.sdl3.device, &_ci);
        }
    }

done:
    dict_add(p_instance->cache.p_attachment, p_attachment);

    // return a pointer to the caller
    *pp_attachment = p_attachment;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_attachment:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_attachment\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/attachment.json\n");
                #endif

                // error
                return 0;

            no_type_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"type\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/attachment.json\n");
                #endif

                // error
                return 0;

            // no_passes_property:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"name\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_type_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"type\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            // wrong_passes_type:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Property \"passes\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_pipeline_from_json ( pipeline **pp_pipeline, const json_value *p_value )
{

    // argument check
    if ( pp_pipeline == (void *) 0 ) goto no_pipeline;
    if ( p_value     == (void *) 0 ) goto no_value;

    // initialized data
    g_instance *p_instance = g_active_instance();
    pipeline *p_pipeline = default_allocator(0, sizeof(pipeline));

    // type check
    if ( JSON_VALUE_STRING == p_value->type)
    {

        size_t file_len = load_file(p_value->string, (void *) 0, true);
        char *p_file_contents = default_allocator(0, (file_len + 1) * sizeof(char));
        
        // error check
        if ( file_len == 0 ) goto failed_to_load_file;

        // load the file
        if ( load_file(p_value->string, p_file_contents, true) == 0 ) goto failed_to_load_file;

        // parse the json value
        json_value_parse(p_file_contents, NULL, &p_value);
    }
    
    // error check
    if ( NULL == p_pipeline ) goto no_mem;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // parse the pipeline object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name      = NULL,
                   *p_source    = NULL,
                   *p_vert      = NULL,
                   *p_frag      = NULL,
                   *p_primitive = NULL,
                   *p_uniforms  = NULL,
                   *p_samplers  = NULL,
                   *p_input     = NULL;

        dict_get(p_dict, "name"     , (void **)&p_name);
        dict_get(p_dict, "source"   , (void **)&p_source);
        if ( p_source )
        {
            dict_get(p_source->object, "vert", (void **)&p_vert);
            dict_get(p_source->object, "frag", (void **)&p_frag);
        }
        dict_get(p_dict, "primitive", (void **)&p_primitive);
        dict_get(p_dict, "uniforms" , (void **)&p_uniforms);
        dict_get(p_dict, "samplers" , (void **)&p_samplers);
        dict_get(p_dict, "input"    , (void **)&p_input);

        // error check
        if ( p_name  == (void *) 0 ) goto no_name_property;
        if ( p_source == (void *) 0 ) goto no_source_property;

        // type check
        if ( p_name->type != JSON_VALUE_STRING ) goto wrong_name_type;
        if ( p_source->type != JSON_VALUE_OBJECT ) goto wrong_source_type;

        // store the name
        strncpy(p_pipeline->_name, p_name->string, sizeof(p_pipeline->_name) - 1);

        // construct uniforms
        if ( p_uniforms )
        {

            // initialized data
            size_t len = array_size(p_uniforms->list);

            if ( 0 == len ) goto no_uniforms;

            array_construct(&p_pipeline->p_uniforms, len);
            dict_construct(&p_pipeline->uniforms, len, NULL, (fn_key_accessor *)uniform_key_accessor, NULL);

            // iterate through each uniform json
            for (size_t i = 0; i < len; i++)
            {
                // initialized data
                uniform *p_uniform = NULL;
                json_value *p_value = NULL;
                
                // store the i'th uniform json
                array_index(p_uniforms->list, i, (void **)&p_value);

                // construct the i'th uniform
                g_sdl3_uniform_from_json(&p_uniform, p_value);

                // store the index
                p_uniform->idx = i;

                // store the i'th uniform
                array_add(p_pipeline->p_uniforms, p_uniform);
                dict_add(p_pipeline->uniforms, p_uniform);
            }
        }
        no_uniforms:;

        // construct samplers
        if ( p_samplers )
        {

            // initialized data
            size_t len = array_size(p_samplers->list);

            if ( 0 == len ) goto no_samplers;

            array_construct(&p_pipeline->p_samplers, len);
            dict_construct(&p_pipeline->samplers, len, NULL, NULL, NULL);

            // iterate through each sampler json
            for (size_t i = 0; i < len; i++)
            {
                // initialized data
                sampler *p_sampler = NULL;
                json_value *p_value = NULL;
                
                // store the i'th sampler json
                array_index(p_samplers->list, i, (void **)&p_value);

                // construct the i'th sampler
                g_sdl3_sampler_from_json(&p_sampler, p_value);

                // store the index
                p_sampler->idx = i;

                // store the i'th sampler
                array_add(p_pipeline->p_samplers, p_sampler);
            }
        }
        no_samplers:;

        // construct pipeline
        {

            // initialized data
            SDL_GPUShader *vs = NULL;
            SDL_GPUShader *fs = NULL;
            SDL_GPUGraphicsPipelineCreateInfo p_ci = { 0 };
            SDL_GPUGraphicsPipeline *pipeline = NULL;
            SDL_GPUVertexBufferDescription _vertex_buffer_descriptions[GEOMETRY_QTY] = { 0 };
            SDL_GPUVertexAttribute _vertex_attributes[GEOMETRY_QTY] = { 0 };
            size_t uniform_count = ( p_pipeline->p_uniforms ) ? array_size(p_pipeline->p_uniforms) : 0;
            size_t sampler_count = ( p_pipeline->p_samplers ) ? array_size(p_pipeline->p_samplers) : 0;
            size_t vertex_buffer_quantity = 0;

            // vertex shader
            {

                // initialized data
                SDL_GPUShaderCreateInfo vs_ci = { 0 };
                size_t vs_file_len = load_file(p_vert->string, (void *) 0, true);
                char *p_vs_contents = default_allocator(0, (vs_file_len + 1) * sizeof(char));
                
                // load the file
                load_file(p_vert->string, p_vs_contents, true);

                vs_ci = (SDL_GPUShaderCreateInfo)
                {
                    .code_size = vs_file_len,
                    .code = p_vs_contents,
                    .entrypoint = "vs_main",

                    .format = SDL_GPU_SHADERFORMAT_METALLIB,
                    .stage = SDL_GPU_SHADERSTAGE_VERTEX,
                    
                    .num_samplers         = 0,
                    .num_storage_textures = 0,
                    .num_storage_buffers  = 0,
                    .num_uniform_buffers  = uniform_count
                };

                // compile the vertex shader
                vs = SDL_CreateGPUShader(p_instance->graphics.sdl3.device, &vs_ci);

                // error check
                if ( NULL == vs ) 
                {
                    log_error("[g10] [pipeline] Failed to compile vertex shader in call to function \"%s\"\n[sdl3] %s\n", __FUNCTION__, SDL_GetError());
                }
            }

            // fragment shader
            {

                // initialized data
                SDL_GPUShaderCreateInfo fs_ci = { 0 };
                size_t fs_file_len = load_file(p_frag->string, (void *) 0, true);
                char *p_fs_contents = default_allocator(0, (fs_file_len + 1) * sizeof(char));

                // load the file
                load_file(p_frag->string, p_fs_contents, true);

                fs_ci = (SDL_GPUShaderCreateInfo)
                {
                    .code_size = fs_file_len,
                    .code = p_fs_contents,
                    .entrypoint = "fs_main",

                    .format = SDL_GPU_SHADERFORMAT_METALLIB,
                    .stage = SDL_GPU_SHADERSTAGE_FRAGMENT,

                    .num_samplers         = sampler_count,
                    .num_storage_textures = 0,
                    .num_storage_buffers  = 0,
                    .num_uniform_buffers  = uniform_count
                };

                // compile the fragment shader
                fs = SDL_CreateGPUShader(p_instance->graphics.sdl3.device, &fs_ci);


                // error check
                if ( NULL == fs ) 
                {
                    log_error("[g10] [pipeline] Failed to compile fragment shader in call to function \"%s\"\n[sdl3] %s\n", __FUNCTION__, SDL_GetError());
                }
            }
           
            // parse vertex buffers
            {

                // initialized data
                array *p_array = p_input->list;
                
                // store the number of vertex attributes
                vertex_buffer_quantity = array_size(p_array);

                // iterate through each vertex buffer
                for (size_t i = 0; i < vertex_buffer_quantity; i++)
                {

                    // initialized data
                    json_value *p_value = NULL;
                    
                    // store the i'th vertex buffer
                    array_index(p_array, i, (void **)&p_value);

                    // xyz
                    if ( 0 == strcmp(p_value->string, "xyz") )
                        _vertex_buffer_descriptions[i] = (SDL_GPUVertexBufferDescription)
                        {
                            .slot = i,
                            .pitch = sizeof(f32) * 3,
                            .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                            .instance_step_rate = 0
                        },
                        _vertex_attributes[i] = (SDL_GPUVertexAttribute)
                        {
                            .location = 0,
                            .buffer_slot = 0,
                            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                            .offset = 0
                        };
                    
                    // uv
                    else if ( 0 == strcmp(p_value->string, "uv") )
                        _vertex_buffer_descriptions[i] = (SDL_GPUVertexBufferDescription)
                        {
                            .slot = i,
                            .pitch = sizeof(f32) * 2,
                            .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                            .instance_step_rate = 0
                        },
                        _vertex_attributes[i] = (SDL_GPUVertexAttribute)
                        {
                            .location = i,
                            .buffer_slot = i,
                            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                            .offset = 0
                        };

                    // normal
                    else if ( 0 == strcmp(p_value->string, "nxyz") )
                        _vertex_buffer_descriptions[i] = (SDL_GPUVertexBufferDescription)
                        {
                            .slot = i,
                            .pitch = sizeof(f32) * 3,
                            .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                            .instance_step_rate = 0
                        },
                        _vertex_attributes[i] = (SDL_GPUVertexAttribute)
                        {
                            .location = i,
                            .buffer_slot = i,
                            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                            .offset = 0
                        };
                    
                    // tangent
                    else if ( 0 == strcmp(p_value->string, "txyz") )
                        _vertex_buffer_descriptions[i] = (SDL_GPUVertexBufferDescription)
                        {
                            .slot = i,
                            .pitch = sizeof(f32) * 4,
                            .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                            .instance_step_rate = 0
                        },
                        _vertex_attributes[i] = (SDL_GPUVertexAttribute)
                        {
                            .location = i,
                            .buffer_slot = i,
                            .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                            .offset = 0
                        };
                    
                }
            }
            
            // todo: parse framebuffer
            //

            SDL_GPUPrimitiveType primitive = -1;
            SDL_GPUColorTargetDescription ctd = 
            {
                .format = SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM
            };

            
                 if ( 0 == strcmp(p_primitive->string, "triangle") ) primitive = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
            else if ( 0 == strcmp(p_primitive->string, "point") ) primitive = SDL_GPU_PRIMITIVETYPE_POINTLIST;
            else if ( 0 == strcmp(p_primitive->string, "line") ) primitive = SDL_GPU_PRIMITIVETYPE_LINELIST;
            else {

                log_error("[g10] [pipeline] Invalid primitive type \"%s\" in call to function \"%s\"\n", p_primitive->string, __FUNCTION__);
                return 0;
            }

            p_ci = (SDL_GPUGraphicsPipelineCreateInfo)
            {
                .vertex_shader = vs,
                .fragment_shader = fs,
                .vertex_input_state = 
                {
                    .vertex_buffer_descriptions = _vertex_buffer_descriptions,
                    .num_vertex_buffers = vertex_buffer_quantity,
                    .vertex_attributes = _vertex_attributes,
                    .num_vertex_attributes = vertex_buffer_quantity
                },
                .primitive_type = primitive,
                .rasterizer_state = 
                {
                    .fill_mode = SDL_GPU_FILLMODE_FILL,
                    .cull_mode = SDL_GPU_CULLMODE_BACK,
                    .front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
                    .depth_bias_constant_factor = 0,
                    .depth_bias_clamp = 0,
                    .enable_depth_bias = false,
                    .enable_depth_clip = false
                },
                .multisample_state = 
                {
                    .sample_count = 0,
                    .sample_mask = 0,
                    .enable_mask = false
                },
                .depth_stencil_state = 
                {
                    .compare_op = SDL_GPU_COMPAREOP_LESS,
                    .back_stencil_state = SDL_GPU_STENCILOP_KEEP,
                    .compare_mask = 0,
                    .write_mask = 0,
                    .enable_depth_test = true,
                    .enable_depth_write = true,
                    .enable_stencil_test = false                
                },
                .target_info = 
                {
                    .color_target_descriptions = &ctd,
                    .num_color_targets = 1,
                    .depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
                    .has_depth_stencil_target = true,
                }
            };

            // link the graphics pipeline
            pipeline = SDL_CreateGPUGraphicsPipeline(p_instance->graphics.sdl3.device, &p_ci);

            // error check
            if ( NULL == pipeline ) 
            {
                log_error("[g10] [pipeline] Failed to construct pipeline in call to function \"%s\"\n[sdl3] %s\n", __FUNCTION__, SDL_GetError());
            }

            // store the pipeline handle
            p_pipeline->pipeline = pipeline;
        }
    }

    // construct a static draw list
    array_construct(&p_pipeline->p_static_draw_list, 512);

    // add the pipeline to the cache
    dict_add(p_instance->cache.p_pipeline, p_pipeline);
    
    // return a pointer to the caller
    *pp_pipeline = p_pipeline;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_pipeline:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_pipeline\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/attachment.json\n");
                #endif

                // error
                return 0;

            no_source_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"source\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/attachment.json\n");
                #endif

                // error
                return 0;

            // no_passes_property:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"name\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_source_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"source\" of parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            // wrong_passes_type:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Property \"passes\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to load file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_pipeline_bind ( render_pass *p_render_pass, pipeline *p_pipeline )
{
    
    // initialized data
    g_instance *p_instance = g_active_instance();

    // bind the pipeline
    SDL_BindGPUGraphicsPipeline(p_render_pass->p_handle, p_pipeline->pipeline);

    // success
    return 1;
}

int g_sdl3_pipeline_draw ( render_pass *p_render_pass, pipeline *p_pipeline )
{
    
    // initialized data
    g_instance *p_instance = g_active_instance();
    size_t len = array_size(p_pipeline->p_static_draw_list);
    
    // iterate through each drawable
    for (size_t i = 0; i < len; i++)
    {
        
        // initialized data
        entity *p_drawable = NULL;

        // retrieve the i'th drawable
        array_index(p_pipeline->p_static_draw_list, i, (void **)&p_drawable);

        // bind the drawable
        if ( p_pipeline->pfn_bind_each )
            p_pipeline->pfn_bind_each(p_render_pass, p_pipeline, p_drawable);

        // draw the drawable
        if ( p_pipeline->pfn_draw )
            p_pipeline->pfn_draw(p_render_pass, p_pipeline, p_drawable);
    }
    
    // success
    return 1;
}

int g_sdl3_render_pass_from_json ( render_pass **pp_render_pass, const json_value *p_value )
{
    
    // argument check
    if ( pp_render_pass ==        (void *) 0 ) goto no_render_pass;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    render_pass *p_render_pass = default_allocator(0, sizeof(render_pass));

    // error check
    if ( NULL == p_render_pass ) goto no_mem;

    // parse the render pass object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name        = NULL,
                   *p_framebuffer = NULL,
                   *p_pipelines   = NULL;

        dict_get(p_dict, "name"       , (void **)&p_name);
        dict_get(p_dict, "framebuffer", (void **)&p_framebuffer);
        dict_get(p_dict, "pipelines"  , (void **)&p_pipelines);

        // error check
        if ( p_name  == (void *) 0 ) goto no_name_property;
        if ( p_framebuffer == (void *) 0 ) goto no_framebuffer_property;
        if ( p_pipelines == (void *) 0 ) goto no_pipelines_property;

        // type check
        if ( p_name->type != JSON_VALUE_STRING ) goto wrong_name_type;
        if ( p_framebuffer->type != JSON_VALUE_OBJECT ) goto wrong_framebuffer_type;
        if ( p_pipelines->type != JSON_VALUE_ARRAY ) goto wrong_pipelines_type;

        // store the name
        strncpy(p_render_pass->_name, p_name->string, sizeof(p_render_pass->_name) - 1);

        // construct the framebuffer
        g_sdl3_framebuffer_from_json(&p_render_pass->p_framebuffer, p_framebuffer);

        // iterate through each pipeline json
        {
            
            // initialized data
            size_t len = array_size(p_pipelines->list);
            
            // construct an array
            array_construct(&p_render_pass->p_pipelines, len);

            // iterate through each pipeline 
            for (size_t i = 0; i < array_size(p_pipelines->list); i++)
            {
                
                // initialized data
                pipeline *p_pipeline = NULL;
                json_value *p_value = NULL;
                char *p_name = NULL;
                
                // store the i'th pipeline json
                array_index(p_pipelines->list, i, (void **)&p_value);

                // store the name of the pipeline
                p_name = p_value->string;

                // retrieve the pipeline from the cache
                dict_get(p_instance->cache.p_pipeline, p_name, (void **)&p_pipeline);

                // store the i'th pipeline
                array_add(p_render_pass->p_pipelines, p_pipeline);
            }
        }
    }

    // return a pointer to the caller
    *pp_render_pass = p_render_pass;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_render_pass:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_render_pass\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/render_pass.json\n");
                #endif

                // error
                return 0;

            no_framebuffer_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"framebuffer\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/render_pass.json\n");
                #endif

                // error
                return 0;

            no_pipelines_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"pipelines\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/render_pass.json\n");
                #endif

                // error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"name\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_framebuffer_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"framebuffer\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;
            
            wrong_pipelines_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"pipelines\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif 

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_framebuffer_from_json ( framebuffer **pp_framebuffer, const json_value *p_value )
{
    
    // argument check
    if ( pp_framebuffer ==        (void *) 0 ) goto no_framebuffer;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    framebuffer *p_framebuffer = default_allocator(0, sizeof(framebuffer));

    // error check
    if ( NULL == p_framebuffer ) goto no_mem;

    // parse the framebuffer object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_clear = NULL,
                   *p_color = NULL,
                   *p_depth = NULL;

        dict_get(p_dict, "clear", (void **)&p_clear);
        dict_get(p_dict, "color", (void **)&p_color);
        dict_get(p_dict, "depth", (void **)&p_depth);

        // error check
        if ( p_clear  == (void *) 0 ) goto no_clear_property;
        
        // type check
        if ( p_clear->type != JSON_VALUE_ARRAY ) goto wrong_clear_type;
        
        // parse the clear color
        {

            // initialized data
            json_value *p_i = NULL;

            // checks
            if ( 4 != array_size(p_clear->list) ) goto not_enough_clear_values;

            // iterate through each channel
            for ( size_t i = 0; i < 4; i++ )
            {

                // store the i'th channel
                array_index(p_clear->list, i, (void **)&p_i);

                // error check
                if ( NULL == p_i ) goto channel_is_null;
                if ( p_i->type != JSON_VALUE_NUMBER ) goto channel_is_not_number;

                // store the intensity
                p_framebuffer->clear[i] = p_i->number;
            }
        }

        // parse color attachments
        if ( p_color ) 
        {

            // type check
            if ( p_color->type != JSON_VALUE_ARRAY ) goto wrong_color_type;

            // initialized data
            size_t len = array_size(p_color->list);

            array_construct(&p_framebuffer->p_attachments, len);

            for (size_t i = 0; i < len; i++)
            {
                
                // initialized data
                json_value *p_attachment_name = NULL;
                char *p_name = NULL;
                attachment *p_attachment = NULL;

                // store the i'th attachment name json
                array_index(p_color->list, i, (void **)&p_attachment_name);

                // store the i'th attachment name
                p_name = p_attachment_name->string;

                // retrieve the attachment from the cache
                dict_get(p_instance->cache.p_attachment, p_name, (void **)&p_attachment);

                array_add(p_framebuffer->p_attachments, p_attachment);
            }
        }
    
        if ( p_depth )
        {

            // initialized data
            char *p_name = NULL;
            attachment *p_attachment = NULL;

            // type check
            if ( p_depth->type != JSON_VALUE_STRING ) goto wrong_depth_type;
        

            // store the i'th attachment name
            p_name = p_depth->string;

            // retrieve the attachment from the cache
            dict_get(p_instance->cache.p_attachment, p_name, (void **)&p_attachment);

            p_framebuffer->p_depth = p_attachment;
        }
    }

    // return a pointer to the caller
    *pp_framebuffer = p_framebuffer;

    // success
    return 1;

    not_enough_clear_values:
    channel_is_null:
    channel_is_not_number:
    log_error("not_enough_clear_values\n");
        return 0;

    // error handling
    {

        // argument errors
        {
            no_framebuffer:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_framebuffer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_clear_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"clear\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/framebuffer.json\n");
                #endif

                // error
                return 0;

            no_color_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"color\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/framebuffer.json\n");
                #endif

                // error
                return 0;

            // no_passes_property:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;

            wrong_clear_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"clear\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_color_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"color\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_depth_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"depth\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            // wrong_passes_type:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Property \"passes\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_geometry_from_json ( geometry **pp_geometry, const json_value *p_value )
{
    
    // argument check
    if ( pp_geometry == (void *) 0 ) goto no_geometry;
    if ( p_value     == (void *) 0 ) goto no_value;

    // initialized data
    g_instance *p_instance = g_active_instance();
    geometry *p_geometry = default_allocator(0, sizeof(geometry));
    dict       *p_dict  = NULL;
    json_value *p_name  = NULL,
               *p_xyz   = NULL,
               *p_uv    = NULL,
               *p_nxyz  = NULL,
               *p_txyz  = NULL,
               *p_bxyz  = NULL,
               *p_parts = NULL,
               *p_idx   = NULL;
    f32 *xyz = NULL, *uv = NULL, *nxyz = NULL, *txyz = NULL, *bxyz = NULL;
    i32 *idx = NULL;
    size_t xyz_len = 0, uv_len = 0, nxyz_len = 0, txyz_len = 0, bxyz_len = 0, idx_len = 0, parts_len = 0;
    vec3 min = {  .x = INFINITY, .y = INFINITY, .z = INFINITY }, 
         max = { .x = -INFINITY, .y = -INFINITY, .z = -INFINITY };

    // type check
    if ( JSON_VALUE_STRING == p_value->type )
    {

        size_t file_len = load_file(p_value->string, (void *) 0, true);
        char *p_file_contents = default_allocator(0, (file_len + 1) * sizeof(char));
        
        // error check
        if ( file_len == 0 ) goto failed_to_load_file;

        // load the file
        if ( load_file(p_value->string, p_file_contents, true) == 0 ) goto failed_to_load_file;

        // parse the json value
        json_value_parse(p_file_contents, NULL, &p_value);
    }
    
    // error check
    if ( NULL == p_geometry ) goto no_mem;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;
    
    p_dict  = p_value->object;
    dict_get(p_dict, "name" , (void **)&p_name);
    dict_get(p_dict, "xyz"  , (void **)&p_xyz);
    dict_get(p_dict, "uv"   , (void **)&p_uv);
    dict_get(p_dict, "nxyz" , (void **)&p_nxyz);
    dict_get(p_dict, "txyz" , (void **)&p_txyz);
    dict_get(p_dict, "bxyz" , (void **)&p_bxyz);
    dict_get(p_dict, "idx"  , (void **)&p_idx);
    dict_get(p_dict, "parts", (void **)&p_parts);

    // parse the geometry object
    {

        // todo: error check
        // todo: type check

        // store the name
        strncpy(p_geometry->_name, p_name->string, sizeof(p_geometry->_name) - 1);

        // xyz
        if ( p_xyz ) goto parse_xyz;
        xyz_done:

        // uv
        if ( p_uv ) goto parse_uv;
        uv_done:

        // normal
        if ( p_nxyz ) goto parse_nxyz;
        nxyz_done:

        // tangent
        if ( p_txyz ) goto parse_txyz;
        txyz_done:

        // index
        if ( p_idx ) goto parse_idx;
        idx_done:

        // parts
        if ( p_parts ) goto parse_parts;
        parts_done:
    }

    // upload vertex data
    {

        // initialized data
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.device);
        SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);
        SDL_GPUTransferBuffer* _vertex_transfer_buffers[GEOMETRY_QTY] = { 0 };
        struct
        {
            f32 *p_data;
            size_t len;
            size_t size;
        } 
        _p_attribute[GEOMETRY_QTY] = 
        {
            [GEOMETRY_XYZ] = 
            {
                .p_data = xyz,
                .len = xyz_len,
                .size = xyz_len * sizeof(f32)
            },
            [GEOMETRY_UV] = 
            {
                .p_data = uv,
                .len = uv_len,
                .size = uv_len * sizeof(f32)
            },
            [GEOMETRY_NXYZ] = 
            {
                .p_data = nxyz,
                .len = nxyz_len,
                .size = nxyz_len * sizeof(f32)
            },
            [GEOMETRY_TXYZ] = 
            {
                .p_data = txyz,
                .len = txyz_len,
                .size = txyz_len * sizeof(f32)
            },
            [GEOMETRY_BXYZ] = 
            {
                .p_data = bxyz,
                .len = bxyz_len,
                .size = bxyz_len * sizeof(f32)
            }
        };
        
        // construct vertex buffers
        for ( size_t i = 0; i < GEOMETRY_QTY; i++ )
        {

            // initialized data
            enum geometry_vertex_attribute_e _type = i;
            void* p_mmap = NULL;

            // fast fail
            if ( NULL == _p_attribute[_type].p_data ) continue;

            // construct a transfer buffer
            _vertex_transfer_buffers[_type] = SDL_CreateGPUTransferBuffer
            (
                p_instance->graphics.sdl3.device, 

                &(SDL_GPUTransferBufferCreateInfo)
                {
                    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                    .size = _p_attribute[_type].size
                }
            );

            // construct a gpu buffer
            p_geometry->_p_handles[_type] = SDL_CreateGPUBuffer
            (
                p_instance->graphics.sdl3.device,

                &(SDL_GPUBufferCreateInfo)
                { 
                    .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
                    .size = _p_attribute[_type].size
                }
            );

            // map the transfer buffer into address space
            p_mmap = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.device, _vertex_transfer_buffers[_type], false);

            // copy the vertex data to the transfer buffer
            SDL_memcpy(p_mmap, _p_attribute[_type].p_data, _p_attribute[_type].size);

            // unmap the transfer buffer from address space
            SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.device, _vertex_transfer_buffers[_type]);
        }

        // upload vertices
        for ( size_t i = 0; i < GEOMETRY_QTY; i++ )
        {

            // initialized data
            enum geometry_vertex_attribute_e _type = i;

            // fast fail
            if ( NULL == _p_attribute[_type].p_data ) continue;

            // upload from the transfer buffer to the gpu
            SDL_UploadToGPUBuffer
            (
                copy_pass,

                &(SDL_GPUTransferBufferLocation)
                {
                    .transfer_buffer = _vertex_transfer_buffers[_type],
                    .offset = 0
                },

                &(SDL_GPUBufferRegion)
                {
                    .buffer = p_geometry->_p_handles[_type],
                    .offset = 0, 
                    .size = _p_attribute[_type].size
                },

                false
            );
        }

        // end the copy pass
        SDL_EndGPUCopyPass(copy_pass),
        SDL_SubmitGPUCommandBuffer(cmd);
    }

    // upload index data
    {

        // initialized data
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.device);
        SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);
        SDL_GPUTransferBuffer* _index_transfer_buffer = { 0 };
        
        // upload indices
        if ( p_idx )
        {

            // initialized data 
            void* p_mmap = NULL;

            // construct a transfer buffer
            _index_transfer_buffer = SDL_CreateGPUTransferBuffer
            (
                p_instance->graphics.sdl3.device, 

                &(SDL_GPUTransferBufferCreateInfo)
                {
                    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                    .size = idx_len * sizeof(i32)
                }
            );

            // construct a gpu buffer
            p_geometry->p_index_handle = SDL_CreateGPUBuffer
            (
                p_instance->graphics.sdl3.device,

                &(SDL_GPUBufferCreateInfo)
                { 
                    .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
                    .size = idx_len * sizeof(i32)
                }
            );

            // map the transfer buffer into address space
            p_mmap = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.device, _index_transfer_buffer, false);

            // copy the vertex data to the transfer buffer
            SDL_memcpy(p_mmap, idx, idx_len * sizeof(u32));

            // unmap the transfer buffer from address space
            SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.device, _index_transfer_buffer);

            // upload from the transfer buffer to the gpu
            SDL_UploadToGPUBuffer
            (
                copy_pass,

                &(SDL_GPUTransferBufferLocation)
                {
                    .transfer_buffer = _index_transfer_buffer,
                    .offset = 0
                },

                &(SDL_GPUBufferRegion)
                {
                    .buffer = p_geometry->p_index_handle,
                    .offset = 0, 
                    .size = sizeof(u32) * idx_len
                },

                false
            );
        }

        // upload parts
        if ( p_parts )
        for (size_t i = 0; i < parts_len; i++)
        {

            // initialized data 
            void* p_mmap = NULL;

            // construct a transfer buffer
            _index_transfer_buffer = SDL_CreateGPUTransferBuffer
            (
                p_instance->graphics.sdl3.device, 

                &(SDL_GPUTransferBufferCreateInfo)
                {
                    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                    .size = p_geometry->_parts[i].index_count * sizeof(i32)
                }
            );

            // construct a gpu buffer
            p_geometry->_parts[i].p_handle = SDL_CreateGPUBuffer
            (
                p_instance->graphics.sdl3.device,

                &(SDL_GPUBufferCreateInfo)
                { 
                    .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
                    .size = p_geometry->_parts[i].index_count * sizeof(i32)
                }
            );

            // map the transfer buffer into address space
            p_mmap = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.device, _index_transfer_buffer, false);

            // copy the vertex data to the transfer buffer
            SDL_memcpy(p_mmap, p_geometry->_parts[i].p_data, p_geometry->_parts[i].index_count * sizeof(u32));

            // unmap the transfer buffer from address space
            SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.device, _index_transfer_buffer);

            // upload from the transfer buffer to the gpu
            SDL_UploadToGPUBuffer
            (
                copy_pass,

                &(SDL_GPUTransferBufferLocation)
                {
                    .transfer_buffer = _index_transfer_buffer,
                    .offset = 0
                },

                &(SDL_GPUBufferRegion)
                {
                    .buffer = p_geometry->_parts[i].p_handle,
                    .offset = 0, 
                    .size = sizeof(u32) * p_geometry->_parts[i].index_count
                },

                false
            );
        }

        // end the copy pass
        SDL_EndGPUCopyPass(copy_pass),
        SDL_SubmitGPUCommandBuffer(cmd);
    }

    // return a pointer to the caller
    *pp_geometry = p_geometry;

    // cache the geometry
    // add the geometry to the cache
    dict_add(p_instance->cache.p_geometry, p_geometry);

    // success
    return 1;

    // this branch parses xyz
    parse_xyz:
    {

        // initialized data
        array *p_array = p_xyz->list;
        xyz_len = array_size(p_array);
        
        // allocate memory for vertices
        xyz = default_allocator(NULL, sizeof(f32) * xyz_len);

        // store the vertex count
        p_geometry->vertex_count = xyz_len / 3;

        // parse the vertex data
        for (size_t i = 0; i < xyz_len; i++)
        {

            // initialized data
            json_value *p_value = NULL;

            // store the i'th json value
            array_index(p_array, i, (void **)&p_value);

            // store the i'th number
            xyz[i] = p_value->number;
        }

        // compute the bounds of the geometry
        for (size_t i = 0; i < xyz_len; i++)
        {
            if ( i%3==0 )
                min.x = (min.x > xyz[i]) ? xyz[i] : min.x,
                max.x = (max.x < xyz[i]) ? xyz[i] : max.x;
            
            if ( i%3==1 )
                min.y = (min.y > xyz[i]) ? xyz[i] : min.y,
                max.y = (max.y < xyz[i]) ? xyz[i] : max.y;
            
            if ( i%3==2 )
                min.z = (min.z > xyz[i]) ? xyz[i] : min.z,
                max.z = (max.z < xyz[i]) ? xyz[i] : max.z;
        }

        // construct an aabb 
        aabb_from_bounds(&p_geometry->_bounds, min, max);

        transform_construct(&p_geometry->p_local_transform, (vec3){0.0,0.0,0.0}, (vec3){0.0,0.0,0.0}, (vec3){1.0,1.0,1.0}, NULL);
        
        // done
        goto xyz_done;
    }

    // this branch parses uv
    parse_uv:
    {

        // initialized data
        array *p_array = p_uv->list;
        uv_len = array_size(p_array);
        
        // allocate memory for vertices
        uv = default_allocator(NULL, sizeof(f32) * uv_len);

        // parse the vertex data
        for (size_t i = 0; i < uv_len; i++)
        {

            // initialized data
            json_value *p_value = NULL;

            // store the i'th json value
            array_index(p_array, i, (void **)&p_value);

            // store the i'th number
            uv[i] = p_value->number;
        }

        // done
        goto uv_done;
    }

    // this branch parses normals
    parse_nxyz:
    {

        // initialized data
        array *p_array = p_nxyz->list;
        nxyz_len = array_size(p_array);
        
        // allocate memory for vertices
        nxyz = default_allocator(NULL, sizeof(f32) * nxyz_len);

        // parse the normal data
        for (size_t i = 0; i < nxyz_len; i++)
        {

            // initialized data
            json_value *p_value = NULL;

            // store the i'th json value
            array_index(p_array, i, (void **)&p_value);

            // store the i'th number
            nxyz[i] = p_value->number;
        }

        // done
        goto nxyz_done;
    }

    // this branch parses tangents
    parse_txyz:
    {

        // initialized data
        array *p_array = p_txyz->list;
        txyz_len = array_size(p_array);
        
        // allocate memory for vertices
        txyz = default_allocator(NULL, sizeof(f32) * txyz_len);

        // parse the normal data
        for (size_t i = 0; i < txyz_len; i++)
        {

            // initialized data
            json_value *p_value = NULL;

            // store the i'th json value
            array_index(p_array, i, (void **)&p_value);

            // store the i'th number
            txyz[i] = p_value->number;
        }

        // done
        goto txyz_done;
    }

    // this branch parses indices
    parse_idx:
    {

        // initialized data
        array *p_array = p_idx->list;
        idx_len = array_size(p_array);
        
        // store the vertex count
        p_geometry->index_count = idx_len / 3;

        // allocate memory for indices
        idx = default_allocator(NULL, sizeof(i32) * idx_len);

        // parse the index data
        for (size_t i = 0; i < idx_len; i++)
        {

            // initialized data
            json_value *p_value = NULL;

            // store the i'th json value
            array_index(p_array, i, (void **)&p_value);

            // store the i'th number
            idx[i] = p_value->integer;
        }

        // done
        goto idx_done;
    }

    // this branch parses parts
    parse_parts:
    {

        // initialized data
        array *p_array = p_parts->list;

        // store the quantity of parts
        parts_len = array_size(p_array);
        
        // iterate through each part
        for (size_t i = 0; i < parts_len; i++)
        {
            
            // initialized data
            json_value *p_value = NULL;

            // store the i'th json value
            array_index(p_array, i, (void **)&p_value);

            // parse the part
            {

                // initialized data
                dict *p_dict = p_value->object;

                json_value *p_material = NULL,
                           *p_idx      = NULL;

                dict_get(p_dict, "material", (void **)&p_material);
                dict_get(p_dict, "idx"     , (void **)&p_idx);
                // store the name 
                strncpy(
                    p_geometry->_parts[i]._material_name,
                    p_material->string, 
                    sizeof(p_geometry->_parts[i]._material_name)
                );

                // store the index quantity
                p_geometry->_parts[i].index_count = array_size(p_idx->list);

                // allocate memory
                p_geometry->_parts[i].p_data = default_allocator(0, sizeof(i32) * p_geometry->_parts[i].index_count);

                for (size_t j = 0; j < p_geometry->_parts[i].index_count; j++)
                {
                    
                    // initialized data
                    array *p_array = p_idx->list;
                    json_value *p_value = NULL;

                    // store the i'th json value
                    array_index(p_array, j, (void **)&p_value);

                    // store the i'th number
                    p_geometry->_parts[i].p_data[j] = p_value->integer;
                }
            }
        }

        // done
        goto parts_done;
    }

    // error handling
    {

        // argument errors
        {
            no_geometry:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_geometry\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_clear_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"clear\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/framebuffer.json\n");
                #endif

                // error
                return 0;

            no_color_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"color\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/framebuffer.json\n");
                #endif

                // error
                return 0;

            // no_passes_property:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;

            wrong_clear_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"clear\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_color_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"color\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            // wrong_passes_type:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Property \"passes\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to load file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_geometry_bind ( render_pass *p_render_pass, geometry *p_geometry )
{

    // argument check
    if ( p_geometry == (void *) 0 ) goto no_geometry;

    // initialized data
    SDL_GPUBufferBinding _bindings[GEOMETRY_QTY] = { 0 };
    SDL_GPUBufferBinding _idx_bind = { 0 };
    size_t len = 0;

    // iterate through each vertex attribute
    for ( size_t i = 0; i < GEOMETRY_QTY; i++ )
    {

        // initialized data
        enum geometry_vertex_attribute_e _type = i;

        // fast fail
        if ( NULL == p_geometry->_p_handles[_type] ) continue;

        // populate the binding
        _bindings[len] = (SDL_GPUBufferBinding)
        {
            .buffer = p_geometry->_p_handles[_type],
            .offset = 0,
        };

        // increment the quantity of bindings
        len++;
    }
    
    // bind the drawable geometry
    SDL_BindGPUVertexBuffers(
        p_render_pass->p_handle,
        0,
        (const SDL_GPUBufferBinding *)&_bindings,
        len
    );

    // bind the index buffer
    if ( p_geometry->p_index_handle )
        _idx_bind = (SDL_GPUBufferBinding)
        {
            .buffer = p_geometry->p_index_handle,
            .offset = 0
        },
        SDL_BindGPUIndexBuffer(
            p_render_pass->p_handle,
            &_idx_bind,
            SDL_GPU_INDEXELEMENTSIZE_32BIT
        );

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_geometry:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_geometry\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_texture_construct ( texture **pp_texture, u32 width, u32 height, u32 channels, const void *p_data )
{

    // argument check
    if ( pp_texture == (void *) 0 ) goto no_texture;

    // initialized data
    g_instance *p_instance = g_active_instance();
    texture *p_texture = default_allocator(0, sizeof(texture));

    // error check
    if ( NULL == p_texture ) goto no_mem;

    // create an SDL GPU texture
    {
        SDL_GPUTextureCreateInfo _ci = 
        {
            .type = SDL_GPU_TEXTURETYPE_2D,
            .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_SNORM,
            .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
            .width = width,  
            .height = height, 
            .layer_count_or_depth = 0,  
            .num_levels = 0,  
            .sample_count = 0
        };

        p_texture->p_handle = SDL_CreateGPUTexture(p_instance->graphics.sdl3.device, &_ci);
    }

    // return a pointer to the caller
    *pp_texture = p_texture;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_texture_from_color ( texture **pp_texture, f32 r, f32 g, f32 b, f32 a )
{

    // argument check
    if ( pp_texture == (void *) 0 ) goto no_texture;

    // initialized data
    g_instance *p_instance = g_active_instance();
    texture *p_texture = default_allocator(0, sizeof(texture));

    if ( p_texture == (void *) 0 ) goto no_mem;

    // initialized data
    SDL_GPUTextureCreateInfo _ci = { 0 };
    SDL_GPUTransferBufferCreateInfo _tci = { 0 };
    SDL_GPUTransferBuffer *p_transfer_buffer = NULL;
    void *p_map = NULL;
    SDL_GPUCommandBuffer *p_cmd = NULL;
    SDL_GPUCopyPass *p_copy_pass = NULL;
    SDL_GPUTextureTransferInfo _src = { 0 };
    SDL_GPUTextureRegion _dst = { 0 };
    u8 _color[4] = { (u8)(r*255), (u8)(g*255), (u8)(b*255), (u8)(a*255) };
    void *p_maybe = NULL;

    // name the texture
    snprintf(
        (char *)&p_texture->_name, 
        sizeof(p_texture->_name),

        "#%02hhx%02hhx%02hhx%02hhx",

        _color[0], 
        _color[1], 
        _color[2], 
        _color[3]
    );

    // cache query
    dict_get(p_instance->cache.p_texture, p_texture->_name, (void **)&p_maybe);

    // cache hit
    if ( p_maybe ) 
    {
        
        // log the cache hit
        printf("[g10] [texture] cache hit for: %s\n", p_texture->_name);

        // release the texture
        p_texture = default_allocator(p_texture, 0);

        // return a pointer to the caller
        *pp_texture = p_maybe;

        // success
        return 1;
    }

    // setup texture create info
    _ci = (SDL_GPUTextureCreateInfo)
    {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = 1,  
        .height = 1, 
        .layer_count_or_depth = 1,  
        .num_levels = 1,  
        .sample_count = 0
    };

    // create the texture
    p_texture->p_handle = SDL_CreateGPUTexture(p_instance->graphics.sdl3.device, &_ci);

    // error check
    if ( p_texture->p_handle == NULL ) goto failed_to_create_texture;

    // setup transfer buffer create info
    _tci = (SDL_GPUTransferBufferCreateInfo)
    {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = 4
    };

    // create the transfer buffer
    p_transfer_buffer = SDL_CreateGPUTransferBuffer(p_instance->graphics.sdl3.device, &_tci);

    // map the transfer buffer
    p_map = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.device, p_transfer_buffer, false);

    // copy the pixels
    SDL_memcpy(p_map, &_color, 4);

    // unmap the transfer buffer
    SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.device, p_transfer_buffer);

    // acquire command buffer
    p_cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.device);

    // begin copy pass
    p_copy_pass = SDL_BeginGPUCopyPass(p_cmd);

    // setup source
    _src = (SDL_GPUTextureTransferInfo)
    {
        .transfer_buffer = p_transfer_buffer,
        .offset = 0,
        .pixels_per_row = 1,
        .rows_per_layer = 1
    };

    // setup destination
    _dst = (SDL_GPUTextureRegion)
    {
        .texture = p_texture->p_handle,
        .w = 1,
        .h = 1,
        .d = 1
    };

    // upload
    SDL_UploadToGPUTexture(p_copy_pass, &_src, &_dst, false);

    // end copy pass
    SDL_EndGPUCopyPass(p_copy_pass);

    // submit command buffer
    SDL_SubmitGPUCommandBuffer(p_cmd);

    // release transfer buffer
    SDL_ReleaseGPUTransferBuffer(p_instance->graphics.sdl3.device, p_transfer_buffer);

    // cache the color
    dict_add(p_instance->cache.p_texture, p_texture),
    printf("[g10] [texture] cached: %s\n", p_texture->_name);


    // return a pointer to the caller
    *pp_texture = p_texture;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // image errors
        {
            failed_to_create_texture:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Failed to create texture in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_texture_load ( texture **pp_texture, const char *p_path )
{

    // argument check
    if ( pp_texture == (void *) 0 ) goto no_texture;

    // initialized data
    g_instance *p_instance = g_active_instance();
    texture *p_texture = default_allocator(0, sizeof(texture));

    if ( p_texture == (void *) 0 ) goto no_mem;

    // initialized data
    SDL_Surface *p_surface = NULL;
    SDL_Surface *p_converted = NULL;
    SDL_GPUTextureCreateInfo _ci = { 0 };
    SDL_GPUTransferBufferCreateInfo _tci = { 0 };
    SDL_GPUTransferBuffer *p_transfer_buffer = NULL;
    void *p_map = NULL;
    SDL_GPUCommandBuffer *p_cmd = NULL;
    SDL_GPUCopyPass *p_copy_pass = NULL;
    SDL_GPUTextureTransferInfo _src = { 0 };
    SDL_GPUTextureRegion _dst = { 0 };

    // load the image
    p_surface = IMG_Load(p_path);

    // error check
    if ( p_surface == (void *) 0 ) goto failed_to_load_image;

    // convert to ABGR8888
    p_converted = SDL_ConvertSurface(p_surface, SDL_PIXELFORMAT_ABGR8888);

    // free the original surface
    SDL_DestroySurface(p_surface);

    // error check
    if ( p_converted == (void *) 0 ) goto failed_to_convert_surface;

    // setup texture create info
    _ci = (SDL_GPUTextureCreateInfo)
    {
        .type = SDL_GPU_TEXTURETYPE_2D,
        .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
        .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
        .width = p_converted->w,  
        .height = p_converted->h, 
        .layer_count_or_depth = 1,  
        .num_levels = 1,  
        .sample_count = 0
    };

    // create the texture
    p_texture->p_handle = SDL_CreateGPUTexture(p_instance->graphics.sdl3.device, &_ci);

    // error check
    if ( p_texture->p_handle == NULL ) goto failed_to_create_texture;

    // setup transfer buffer create info
    _tci = (SDL_GPUTransferBufferCreateInfo)
    {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = p_converted->w * p_converted->h * 4
    };

    // create the transfer buffer
    p_transfer_buffer = SDL_CreateGPUTransferBuffer(p_instance->graphics.sdl3.device, &_tci);

    // map the transfer buffer
    p_map = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.device, p_transfer_buffer, false);

    // copy the pixels
    SDL_memcpy(p_map, p_converted->pixels, p_converted->w * p_converted->h * 4);

    // unmap the transfer buffer
    SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.device, p_transfer_buffer);

    // acquire command buffer
    p_cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.device);

    // begin copy pass
    p_copy_pass = SDL_BeginGPUCopyPass(p_cmd);

    // setup source
    _src = (SDL_GPUTextureTransferInfo)
    {
        .transfer_buffer = p_transfer_buffer,
        .offset = 0,
        .pixels_per_row = p_converted->w,
        .rows_per_layer = p_converted->h
    };

    // setup destination
    _dst = (SDL_GPUTextureRegion)
    {
        .texture = p_texture->p_handle,
        .w = p_converted->w,
        .h = p_converted->h,
        .d = 1
    };

    // upload
    SDL_UploadToGPUTexture(p_copy_pass, &_src, &_dst, false);

    // end copy pass
    SDL_EndGPUCopyPass(p_copy_pass);

    // submit command buffer
    SDL_SubmitGPUCommandBuffer(p_cmd);

    // release transfer buffer
    SDL_ReleaseGPUTransferBuffer(p_instance->graphics.sdl3.device, p_transfer_buffer);

    // destroy the surface
    SDL_DestroySurface(p_converted);

    // return a pointer to the caller
    *pp_texture = p_texture;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_texture:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_texture\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // image errors
        {
            failed_to_load_image:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Failed to load image from path \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // error
                return 0;

            failed_to_convert_surface:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Failed to convert surface in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_create_texture:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Failed to create texture in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_sampler_from_json ( sampler **pp_sampler, const json_value *p_value )
{
    
    // argument check
    if ( pp_sampler   ==        (void *) 0 ) goto no_sampler;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    sampler *p_sampler = default_allocator(0, sizeof(sampler));
    SDL_GPUSamplerCreateInfo _ci = { 0 };

    // error check
    if ( NULL == p_sampler ) goto no_mem;

    // parse the sampler object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name    = NULL,
                   *p_filter  = NULL,
                   *p_mipmap  = NULL,
                   *p_address = NULL,
                   *p_compare = NULL;

        dict_get(p_dict, "name"   , (void **)&p_name);
        dict_get(p_dict, "filter" , (void **)&p_filter);
        dict_get(p_dict, "mipmap" , (void **)&p_mipmap);
        dict_get(p_dict, "address", (void **)&p_address);
        dict_get(p_dict, "compare", (void **)&p_compare);

        // defaults
        _ci = (SDL_GPUSamplerCreateInfo)
        {
            .min_filter = SDL_GPU_FILTER_LINEAR,
            .mag_filter = SDL_GPU_FILTER_NEAREST,

            .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,

            .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
            .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
            
            .mip_lod_bias = 0.0f,
            .max_anisotropy = 8.f,

            .compare_op = SDL_GPU_COMPAREOP_ALWAYS,

            .min_lod = 0.0f,
            .max_lod = 1000.0f,

            .enable_anisotropy = true,
            .enable_compare = false
        };

        // filter
        if ( p_filter ) { }
        
        // mipmap
        if ( p_mipmap ) { }
        
        // address
        if ( p_address ) { }
        
        // compare
        if ( p_compare ) { }
        
    }

    // construct the sampler
    p_sampler->p_handle = SDL_CreateGPUSampler(p_instance->graphics.sdl3.device, &_ci);

    // return a pointer to the caller
    *pp_sampler = p_sampler;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_sampler:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_sampler\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_clear_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"clear\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/framebuffer.json\n");
                #endif

                // error
                return 0;

            no_color_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"color\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/framebuffer.json\n");
                #endif

                // error
                return 0;

            // no_passes_property:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"passes\" in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;

            wrong_clear_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"clear\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            wrong_color_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"color\" of parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;

            // wrong_passes_type:
            //     #ifndef NDEBUG
            //         log_error("[g10] [sdl3] Property \"passes\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
            //         log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
            //     #endif

            //     // error
            //     return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int g_sdl3_uniform_from_json ( uniform **pp_uniform, const json_value *p_value )
{

    // argument check
    if ( pp_uniform    ==        (void *) 0 ) goto no_uniform;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    uniform *p_uniform = default_allocator(0, sizeof(uniform));

    // error check
    if ( NULL == p_uniform ) goto no_mem;

    // parse the uniform object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name = NULL;
        
        dict_get(p_dict, "name", (void **)&p_name);

        // error check
        if ( p_name == (void *) 0 ) goto no_name_property;

        // type check
        if ( p_name->type != JSON_VALUE_STRING ) goto wrong_name_type;

        // store the name
        strncpy(p_uniform->_name, p_name->string, sizeof(p_uniform->_name) - 1);
    }

    // return a pointer to the caller
    *pp_uniform = p_uniform;

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_uniform:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"pp_uniform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    
        // json errors
        {
            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/uniform.json\n");
                #endif

                // error
                return 0;

            wrong_name_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"name\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}