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

/// renderer
#include <renderer.h>
#include <attachment.h>
#include <framebuffer.h>
#include <render_pass.h>
#include <pipeline.h>
#include <aabb.h>
#include <geometry.h>
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
int g_sdl3_texture_load ( texture **pp_texture, const char *p_path );

/// uniform
int g_sdl3_uniform_from_json ( uniform **pp_uniform, const json_value *p_value );

// function definitions
int g_sdl3_init ( g_instance *p_instance )
{

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // initialized data 
    bool ok = true;

    // initialize sdl3
    ok = SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    if ( !ok ) goto failed_to_initialize_sdl3;

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
        array_index(p_passes, i, &p_render_pass);

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
    
    // iterate through each attachment
    for (size_t i = 0; i < attachment_len; i++)
    {

        // initialized data
        attachment *p_attachment = NULL;

        // get the i'th attachment
        array_index(p_framebuffer->p_attachments, i, &p_attachment);

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
    
    // begin the render pass
    p_render_pass->p_handle = SDL_BeginGPURenderPass(p_instance->graphics.sdl3.command_buffer, &_color_target_ci, attachment_len, NULL);
    
    // viewport and scissor
    {
        SDL_SetGPUViewport
        (
            p_render_pass->p_handle, 
            &(SDL_GPUViewport)
            {
                0, 
                0, 
                (int)p_instance->window.width, 
                (int)p_instance->window.height
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
        array_index(p_render_pass->p_pipelines, i, &p_pipeline);

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
        json_value *p_width  = (json_value *)dict_get(p_dict, "width"),
                   *p_height = (json_value *)dict_get(p_dict, "height"),
                   *p_title  = (json_value *)dict_get(p_dict, "title");

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
        json_value *p_name        = (json_value *)dict_get(p_dict, "name"),
                   *p_pipelines   = (json_value *)dict_get(p_dict, "pipelines"),
                   *p_attachments = (json_value *)dict_get(p_dict, "attachments"),
                   *p_passes      = (json_value *)dict_get(p_dict, "passes");

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
        dict_construct(&p_renderer->p_attachments, 64, NULL);

        // iterate through each pipeline json
        for (size_t i = 0; i < array_size(p_pipelines->list); i++)
        {

            // initialized data
            pipeline *p_pipeline = NULL;
            json_value *p_value = NULL;
            
            // store the i'th pipeline json
            array_index(p_pipelines->list, i, &p_value);

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
            array_index(p_attachments->list, i, &p_value);

            // construct the i'th attachment
            g_sdl3_attachment_from_json(&p_attachment, p_value);

            // store the i'th attachment
            dict_add(p_renderer->p_attachments, p_attachment->_name, p_attachment);
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
                array_index(p_passes->list, i, &p_value);

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
        json_value *p_name = (json_value *)dict_get(p_dict, "name"),
                   *p_type = (json_value *)dict_get(p_dict, "type");

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
            format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
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
    dict_add(p_instance->cache.p_attachment, p_attachment->_name, p_attachment);

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
    if ( pp_pipeline ==        (void *) 0 ) goto no_pipeline;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    pipeline *p_pipeline = default_allocator(0, sizeof(pipeline));

    // error check
    if ( NULL == p_pipeline ) goto no_mem;

    // parse the pipeline object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name = (json_value *)dict_get(p_dict, "name"),
                   *p_source = (json_value *)dict_get(p_dict, "source"),
                   *p_vert = (json_value *)dict_get(p_source->object, "vert"),
                   *p_frag = (json_value *)dict_get(p_source->object, "frag"),
                   *p_uniforms = (json_value *)dict_get(p_dict, "uniforms");

        // error check
        if ( p_name  == (void *) 0 ) goto no_name_property;
        if ( p_source == (void *) 0 ) goto no_source_property;

        // type check
        if ( p_name->type != JSON_VALUE_STRING ) goto wrong_name_type;
        if ( p_source->type != JSON_VALUE_OBJECT ) goto wrong_source_type;

        // store the name
        strncpy(p_pipeline->_name, p_name->string, sizeof(p_pipeline->_name) - 1);

        // store the vertex shader path
        p_vert = dict_get(p_source->object, "vert");

        // store the fragment shader path
        p_frag = dict_get(p_source->object, "frag");

        // construct uniforms
        {

            // initialized data
            size_t len = array_size(p_uniforms->list);
            array_construct(&p_pipeline->p_uniforms, len);

            // iterate through each uniform json
            for (size_t i = 0; i < len; i++)
            {
                // initialized data
                uniform *p_uniform = NULL;
                json_value *p_value = NULL;
                
                // store the i'th uniform json
                array_index(p_uniforms->list, i, &p_value);

                // construct the i'th uniform
                g_sdl3_uniform_from_json(&p_uniform, p_value);

                // store the index
                p_uniform->idx = i;

                // store the i'th uniform
                array_add(p_pipeline->p_uniforms, p_uniform);
            }
        }

        // construct pipeline
        {

            // initialized data
            SDL_GPUShader *vs = NULL;
            SDL_GPUShader *fs = NULL;
            SDL_GPUGraphicsPipelineCreateInfo p_ci = { 0 };
            SDL_GPUGraphicsPipeline *pipeline = NULL;
            
            SDL_GPUColorTargetDescription ctd = 
            {
                .format = SDL_GPU_TEXTUREFORMAT_B8G8R8A8_UNORM
            };

            size_t uniform_count = array_size(p_pipeline->p_uniforms);
            
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

                    .num_samplers         = 0,
                    .num_storage_textures = 0,
                    .num_storage_buffers  = 0,
                    .num_uniform_buffers  = uniform_count
                };

                // compile the fragment shader
                fs = SDL_CreateGPUShader(p_instance->graphics.sdl3.device, &fs_ci);
            }

            // define vertex input state
            SDL_GPUVertexBufferDescription vertex_buffer_descriptions[] = {{
                .slot = 0,
                .pitch = sizeof(float) * 3,
                .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                .instance_step_rate = 0
            }};

            SDL_GPUVertexAttribute vertex_attributes[] = {{
                .location = 0,
                .buffer_slot = 0,
                .format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                .offset = 0
            }};

            p_ci = (SDL_GPUGraphicsPipelineCreateInfo)
            {
                .vertex_shader = vs,
                .fragment_shader = fs,
                .vertex_input_state = 
                {
                    .vertex_buffer_descriptions = vertex_buffer_descriptions,
                    .num_vertex_buffers = 1,
                    .vertex_attributes = vertex_attributes,
                    .num_vertex_attributes = 1
                },
                .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
                .rasterizer_state = 
                {
                    .fill_mode = SDL_GPU_FILLMODE_FILL,
                    .cull_mode = SDL_GPU_CULLMODE_NONE,
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
                    .depth_stencil_format = 0,
                    .has_depth_stencil_target = false
                }
            };

            // link the graphics pipeline
            pipeline = SDL_CreateGPUGraphicsPipeline(p_instance->graphics.sdl3.device, &p_ci);

            // store the pipeline handle
            p_pipeline->pipeline = pipeline;
        }
    }

    // construct a static draw list
    array_construct(&p_pipeline->p_static_draw_list, 512);

    // add the pipeline to the cache
    dict_add(p_instance->cache.p_pipeline, p_pipeline->_name, p_pipeline);
    
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
        array_index(p_pipeline->p_static_draw_list, i, &p_drawable);

        // bind the drawable
        if ( p_pipeline->pfn_bind_each )
            p_pipeline->pfn_bind_each(p_render_pass, p_pipeline, p_drawable);

        // draw something
        SDL_DrawGPUPrimitives(p_render_pass->p_handle, p_drawable->p_geometry->vertex_count, 1, 0, 0);
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
        json_value *p_name = (json_value *)dict_get(p_dict, "name"),
                   *p_framebuffer = (json_value *)dict_get(p_dict, "framebuffer"),
                   *p_pipelines = (json_value *)dict_get(p_dict, "pipelines");

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
                array_index(p_pipelines->list, i, &p_value);

                // store the name of the pipeline
                p_name = p_value->string;

                // retrieve the pipeline from the cache
                p_pipeline = dict_get(p_instance->cache.p_pipeline, p_name);

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
        json_value *p_clear = (json_value *)dict_get(p_dict, "clear"),
                   *p_color = (json_value *)dict_get(p_dict, "color");

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
                array_index(p_clear->list, i, &p_i);

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
                array_index(p_color->list, i, &p_attachment_name);

                // store the i'th attachment name
                p_name = p_attachment_name->string;

                // retrieve the attachment from the cache
                p_attachment = dict_get(p_instance->cache.p_attachment, p_name);

                array_add(p_framebuffer->p_attachments, p_attachment);
            }
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
    if ( pp_geometry   ==        (void *) 0 ) goto no_geometry;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // initialized data
    g_instance *p_instance = g_active_instance();
    geometry *p_geometry = default_allocator(0, sizeof(geometry));
    vec3 min = 
    { 
        .x = INFINITY,
        .y = INFINITY,
        .z = INFINITY
    }, 
    max = 
    { 
        .x = -INFINITY,
        .y = -INFINITY,
        .z = -INFINITY        
    };

    // error check
    if ( NULL == p_geometry ) goto no_mem;

    // parse the geometry object
    {

        // initialized data
        dict *p_dict = p_value->object;
        json_value *p_name = (json_value *)dict_get(p_dict, "name"),
                   *p_xyz  = (json_value *)dict_get(p_dict, "xyz"),
                   *p_uv   = (json_value *)dict_get(p_dict, "uv"),
                   *p_nxyz = (json_value *)dict_get(p_dict, "nxyz"),
                   *p_txyz = (json_value *)dict_get(p_dict, "txyz"),
                   *p_bxyz = (json_value *)dict_get(p_dict, "bxyz"),
                   *p_idx  = (json_value *)dict_get(p_dict, "idx");

        // todo: error check
        // todo: type check

        // store the name
        strncpy(p_geometry->_name, p_name->string, sizeof(p_geometry->_name) - 1);

        // xyz
        if ( p_xyz )
        {
            array *p_array = p_xyz->list;
            size_t len = array_size(p_array);
            f32 *xyz = default_allocator(NULL, sizeof(f32) * len);
            SDL_GPUBufferCreateInfo _ci = 
            { 
                .size = sizeof(f32) * len,
                .usage = SDL_GPU_BUFFERUSAGE_VERTEX
            };
            SDL_GPUTransferBufferCreateInfo _transfer_buffer_ci = {
                .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
                .size = sizeof(f32) * len
            };

            for (size_t i = 0; i < len; i++)
            {
                json_value *p_value = NULL;

                array_index(p_array, i, &p_value);

                xyz[i] = p_value->number;
            }

            // compute the bounds of the geometry
            for (size_t i = 0; i < len; i++)
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

            // store the vertex count
            p_geometry->vertex_count = len / 3;

            // construct an aabb 
            aabb_from_bounds(&p_geometry->_bounds, min, max);
            aabb_info(&p_geometry->_bounds);
            
            SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(p_instance->graphics.sdl3.device, &_transfer_buffer_ci);
            p_geometry->p_handle = SDL_CreateGPUBuffer(
                p_instance->graphics.sdl3.device,
                &_ci
            );
            void* mapPtr = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.device, transferBuffer, false);

            // Copy your vertex data into that pointer
            SDL_memcpy(mapPtr, xyz, sizeof(f32) * len);

            // Unmap it once finished
            SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.device, transferBuffer);

            // Begin a command buffer
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.device);

            // Begin the copy pass
            SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

            // Upload from Transfer Buffer -> GPU Vertex Buffer
            SDL_GPUTransferBufferLocation source = { .transfer_buffer = transferBuffer, .offset = 0 };
            SDL_GPUBufferRegion destination = { .buffer = p_geometry->p_handle, .offset = 0, .size = sizeof(f32) * len };

            SDL_UploadToGPUBuffer(copyPass, &source, &destination, false);

            // End the pass and submit
            SDL_EndGPUCopyPass(copyPass);
            SDL_SubmitGPUCommandBuffer(cmd);
        }
    }

    // return a pointer to the caller
    *pp_geometry = p_geometry;
    dict_add(p_instance->cache.p_geometry, p_geometry->_name, p_geometry);

    // success
    return 1;

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
        }
    }
}

int g_sdl3_geometry_bind ( render_pass *p_render_pass, geometry *p_geometry )
{

    // argument check
    if ( p_geometry == (void *) 0 ) goto no_geometry;

    // initialized data
    SDL_GPUBufferBinding _binding = (SDL_GPUBufferBinding) 
    { 
        .buffer = p_geometry->p_handle
    };

    // bind the drawable geometry
    SDL_BindGPUVertexBuffers(p_render_pass->p_handle, 0, &_binding, 1);

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
        json_value *p_name = (json_value *)dict_get(p_dict, "name");

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