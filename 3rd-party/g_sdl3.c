#include <g10/g10.h>
#include <log/log.h>

#ifdef G10_BUILD_WITH_SDL3

#include <SDL3/SDL.h> 
#include <SDL3/SDL_gpu.h> 

// Function definitions
int g_sdl3_init ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Initialize SDL3
    SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO);

    // Success
    return 1;
    
    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_sdl3_window_from_json ( g_instance *p_instance, const json_value *p_value )
{

    // Argument check
    if ( p_instance    ==        (void *) 0 ) goto no_instance;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    u32 sdl3_window_flags = SDL_WINDOW_RESIZABLE;

    // Parse the window object
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_width  = (json_value *)dict_get(p_dict, "width"),
                   *p_height = (json_value *)dict_get(p_dict, "height"),
                   *p_title  = (json_value *)dict_get(p_dict, "title");

        // Error check
        if ( p_width  == (void *) 0 ) goto no_width_property;
        if ( p_height == (void *) 0 ) goto no_height_property;
        if ( p_title  == (void *) 0 ) goto no_title_property;

        // Type check
        if ( p_width->type  != JSON_VALUE_INTEGER ) goto wrong_width_type;
        if ( p_height->type != JSON_VALUE_INTEGER ) goto wrong_height_type;
        if ( p_title->type  != JSON_VALUE_STRING  ) goto wrong_title_type;
    
        // Store the width
        p_instance->window.width = (u32) p_width->integer;

        // Store the height
        p_instance->window.height = (u32) p_height->integer;

        // Copy the title
        {

            // Initialized data
            size_t len = strlen(p_title->string);

            // Allocate memory for the string
            p_instance->window.title = G10_REALLOC(0, sizeof(char) * ( len + 1 ));

            // Error check
            if ( p_instance->window.title == (void *) 0 ) goto no_mem;

            // Initialize
            memset(p_instance->window.title, 0, sizeof(char) * ( len + 1 ));

            // Copy the string
            strncpy(p_instance->window.title, p_title->string, len);
        }
    }

    // Construct the sdl3 window
    p_instance->window.sdl3.window = SDL_CreateWindow(p_instance->window.title, (int) p_instance->window.width, (int) p_instance->window.height, sdl3_window_flags);
    
    // Create a GPU device
    p_instance->graphics.sdl3.p_device = SDL_CreateGPUDevice(
        SDL_GPU_SHADERFORMAT_SPIRV,
        true,
        NULL
    );

    // Claim the window
    SDL_ClaimWindowForGPUDevice(p_instance->graphics.sdl3.p_device, p_instance->window.sdl3.window);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            no_width_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"width\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            no_height_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"height\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            no_title_property:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Parameter \"p_value\" is missing required property \"title\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            wrong_width_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"width\" of parameter \"p_value\" must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            wrong_height_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"height\" of parameter \"p_value\" must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;

            wrong_title_type:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Property \"title\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

/** !
 * Construct an SDL3 mesh from a struct
 * 
 * @param pp_mesh_data       pointer to result
 * @param p_mesh_create_info pointer to mesh create info struct
 * 
 * @return 1 on success, 0 on error
 */
int g_sdl3_mesh_construct
(
    mesh_data        **pp_mesh_data,
    mesh_create_info   _mesh_create_info
)
{

    // Argument check
    if ( pp_mesh_data                       == (void *) 0 ) goto no_mesh;
    if ( _mesh_create_info.verticies.p_data == (void *) 0 ) goto no_verticies;
    if ( _mesh_create_info.elements.p_data  == (void *) 0 ) goto no_elements;

    // Initialized data
    g_instance                      *p_instance                            = g_get_active_instance();
    mesh_data                       *p_mesh_data                           = G10_REALLOC(0, sizeof(mesh_data));
    SDL_GPUBufferCreateInfo          _vertex_buffer                        = { 0 },
                                     _index_buffer                         = { 0 };
    SDL_GPUTransferBufferCreateInfo  _transfer_vertex_buffer_create_info   = { 0 },
                                     _transfer_index_buffer_create_info    = { 0 };
    SDL_GPUTransferBuffer           *p_vertex_transfer                     = (void *) 0,
                                    *p_index_transfer                      = (void *) 0;
    // Error check
    if ( p_mesh_data == (void *) 0 ) goto no_mem;

    _vertex_buffer = (SDL_GPUBufferCreateInfo) 
    {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size  = (_mesh_create_info.verticies.quantity * sizeof(f32)),
        .props = 0
    };

    _index_buffer = (SDL_GPUBufferCreateInfo) 
    {
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size  = (_mesh_create_info.elements.quantity * sizeof(i32)),
        .props = 0
    };

    p_mesh_data->sdl3.p_vertex_buffer = SDL_CreateGPUBuffer(p_instance->graphics.sdl3.p_device, &_vertex_buffer);
    p_mesh_data->sdl3.p_index_buffer  = SDL_CreateGPUBuffer(p_instance->graphics.sdl3.p_device, &_index_buffer);
    p_mesh_data->sdl3.indices         = _mesh_create_info.elements.quantity;
    
    _transfer_vertex_buffer_create_info = (SDL_GPUTransferBufferCreateInfo)
    {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (_mesh_create_info.verticies.quantity * sizeof(f32)),
        .props = 0
    };

    _transfer_index_buffer_create_info = (SDL_GPUTransferBufferCreateInfo)
    {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size  = (_mesh_create_info.elements.quantity * sizeof(i32)),
        .props = 0
    };
    
    p_vertex_transfer = SDL_CreateGPUTransferBuffer(p_instance->graphics.sdl3.p_device, &_transfer_vertex_buffer_create_info);
    p_index_transfer  = SDL_CreateGPUTransferBuffer(p_instance->graphics.sdl3.p_device, &_transfer_index_buffer_create_info);

    {
        SDL_GPUCommandBuffer *cmd;
        SDL_GPUCopyPass *copy_pass;
        SDL_GPUTransferBufferLocation buf_location;
        SDL_GPUBufferRegion dst_region;

        void *map = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.p_device, p_vertex_transfer, false);
        memcpy(map, _mesh_create_info.verticies.p_data, (_mesh_create_info.verticies.quantity * sizeof(f32)));
        SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.p_device, p_vertex_transfer);
        cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.p_device);
        copy_pass = SDL_BeginGPUCopyPass(cmd);
        buf_location.transfer_buffer = p_vertex_transfer;
        buf_location.offset = 0;
        dst_region.buffer = p_mesh_data->sdl3.p_vertex_buffer;
        dst_region.offset = 0;
        dst_region.size = (_mesh_create_info.verticies.quantity * sizeof(f32));
        SDL_UploadToGPUBuffer(copy_pass, &buf_location, &dst_region, false);
        SDL_EndGPUCopyPass(copy_pass);
        SDL_SubmitGPUCommandBuffer(cmd);

        SDL_ReleaseGPUTransferBuffer(p_instance->graphics.sdl3.p_device, p_vertex_transfer);
    }

    {
        SDL_GPUCommandBuffer *cmd;
        SDL_GPUCopyPass *copy_pass;
        SDL_GPUTransferBufferLocation buf_location;
        SDL_GPUBufferRegion dst_region;

        void *map = SDL_MapGPUTransferBuffer(p_instance->graphics.sdl3.p_device, p_index_transfer, false);
        memcpy(map, _mesh_create_info.elements.p_data, (_mesh_create_info.elements.quantity * sizeof(i32)));
        SDL_UnmapGPUTransferBuffer(p_instance->graphics.sdl3.p_device, p_index_transfer);
        cmd = SDL_AcquireGPUCommandBuffer(p_instance->graphics.sdl3.p_device);
        copy_pass = SDL_BeginGPUCopyPass(cmd);
        buf_location.transfer_buffer = p_index_transfer;
        buf_location.offset = 0;
        dst_region.buffer = p_mesh_data->sdl3.p_index_buffer;
        dst_region.offset = 0;
        dst_region.size = (_mesh_create_info.elements.quantity * sizeof(i32));
        SDL_UploadToGPUBuffer(copy_pass, &buf_location, &dst_region, false);
        SDL_EndGPUCopyPass(copy_pass);
        SDL_SubmitGPUCommandBuffer(cmd);

        SDL_ReleaseGPUTransferBuffer(p_instance->graphics.sdl3.p_device, p_index_transfer);
    }

    // Return a pointer to the caller
    *pp_mesh_data = p_mesh_data;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_mesh:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Null pointer provided for parameter \"pp_mesh_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_verticies:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Null pointer provided for parameter \"model.verticies.p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_elements:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Null pointer provided for parameter \"model.elements.p_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}


int g_sdl3_shader_construct
(
    shader           **pp_shader,
    const json_value  *p_value
)
{

    // Argument check
    if ( pp_shader     ==        (void *) 0 ) goto no_shader;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    g_instance *p_instance = g_get_active_instance(); 
    shader *p_shader = G10_REALLOC(0, sizeof(shader));
    size_t vertex_source_len = 0, fragment_source_len = 0;
    dict *const p_dict = p_value->object;
    const json_value *const p_vertex   = dict_get(p_dict, "vertex"),
                     *const p_layout   = dict_get(p_dict, "layout"),
                     *const p_fragment = dict_get(p_dict, "fragment");
    int success;
    char *p_vs_src = (void *) 0;
    char *p_fs_src = (void *) 0;
    SDL_GPUShaderCreateInfo           _vertex_shader_create_info   = { 0 },
                                      _fragment_shader_create_info = { 0 };
    SDL_GPUGraphicsPipelineCreateInfo _pipeline_create_info        = { 0 };
    SDL_GPUVertexBufferDescription    _vertex_buffer_description   =
    {
        .slot = 0,
        .input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
        .instance_step_rate = 0,
        .pitch = sizeof(f32) * 3
    };
    SDL_GPUVertexAttribute _vertex_attributes[2] = 
    {
        [0] = (SDL_GPUVertexAttribute)
        {
            .location    = 0,
            .buffer_slot = 0,
            .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            .offset      = 0
        }
    };

    // Unused 
    (void) p_layout;

    // Error check
    if ( p_shader == (void *) 0 ) goto no_mem;
    
    // Initialize memory
    memset(p_shader, 0, sizeof(shader));

    // Property check
    if ( p_vertex   == (void *) 0 ) goto no_vertex_property;
    if ( p_fragment == (void *) 0 ) goto no_fragment_property;

    // Load the shader source code
    vertex_source_len   = g_load_file(p_vertex->string, 0, false);
    fragment_source_len = g_load_file(p_fragment->string, 0, false);

    p_vs_src = G10_REALLOC(0, vertex_source_len);
    p_fs_src = G10_REALLOC(0, fragment_source_len);

    // Compute the length of each file
    g_load_file(p_vertex->string, p_vs_src, false);
    g_load_file(p_fragment->string, p_fs_src, false);

    // Populate the vertex shader create info struct
    _vertex_shader_create_info = (SDL_GPUShaderCreateInfo)
    {
        .code_size            = vertex_source_len,
        .code                 = (const Uint8 *)p_vs_src,
        .entrypoint           = "main",
        .format               = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage                = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_samplers         = 0,
        .num_storage_textures = 0,
        .num_storage_buffers  = 0,
        .num_uniform_buffers  = 1,
        .props                = 0
    };

    // Populate the fragment shader create info struct
    _fragment_shader_create_info = (SDL_GPUShaderCreateInfo)
    {
        .code_size            = fragment_source_len,
        .code                 = (const Uint8 *)p_fs_src,
        .entrypoint           = "main",
        .format               = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage                = SDL_GPU_SHADERSTAGE_VERTEX,
        .num_samplers         = 0,
        .num_storage_textures = 0,
        .num_storage_buffers  = 0,
        .num_uniform_buffers  = 1,
        .props                = 0
    };

    // Create a vertex shader 
    p_shader->sdl3.p_vertex_shader = SDL_CreateGPUShader(p_instance->graphics.sdl3.p_device, &_vertex_shader_create_info);
    
    // Create a fragment shader
    p_shader->sdl3.p_fragment_shader = SDL_CreateGPUShader(p_instance->graphics.sdl3.p_device, &_fragment_shader_create_info);

    _pipeline_create_info = (SDL_GPUGraphicsPipelineCreateInfo)
    {
        .vertex_shader   = p_shader->sdl3.p_vertex_shader, 
        .fragment_shader = p_shader->sdl3.p_fragment_shader,
        .vertex_input_state = (SDL_GPUVertexInputState)
        {
            .vertex_buffer_descriptions = &_vertex_buffer_description,
            .num_vertex_buffers         = 1,
            .vertex_attributes          = (const SDL_GPUVertexAttribute *) &_vertex_attributes,
            .num_vertex_attributes      = 1
        }, 
        .primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, 
        .rasterizer_state = (SDL_GPURasterizerState)
        {
            .fill_mode                  = SDL_GPU_FILLMODE_FILL,
            .cull_mode                  = SDL_GPU_CULLMODE_NONE,
            .front_face                 = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE,
            .depth_bias_constant_factor = 0,
            .depth_bias_clamp           = 0,
            .depth_bias_slope_factor    = 0,
            .enable_depth_bias          = 0,
            .enable_depth_clip          = 0
        }, 
        .multisample_state = (SDL_GPUMultisampleState)
        {
            .sample_count = 0,
            .sample_mask  = 0,
            .enable_mask  = 0
        }, 
        .depth_stencil_state = (SDL_GPUDepthStencilState)
        {
            .compare_op          = SDL_GPU_COMPAREOP_LESS_OR_EQUAL, 
            .back_stencil_state  = 0, 
            .front_stencil_state = 0, 
            .compare_mask        = 0, 
            .write_mask          = 0, 
            .enable_depth_test   = true, 
            .enable_depth_write  = true, 
            .enable_stencil_test = false
        }, 
        .target_info = (SDL_GPUGraphicsPipelineTargetInfo)
        {
            .color_target_descriptions = &(SDL_GPUColorTargetDescription)
            {
                .format = SDL_GetGPUSwapchainTextureFormat(p_instance->graphics.sdl3.p_device, p_instance->window.sdl3.window)
            },
            .num_color_targets         = 1,
            .depth_stencil_format      = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
            .has_depth_stencil_target  = true
        }, 

        .props = 0
    };

    p_shader->sdl3.p_pipeline = SDL_CreateGPUGraphicsPipeline(p_instance->graphics.sdl3.p_device, &_pipeline_create_info);

    // Store the on bind function
    p_shader->functions.pfn_shader_on_bind = (fn_shader_on_bind) shader_bind_camera;

    // Store the draw function
    p_shader->functions.pfn_shader_on_draw = (fn_shader_on_draw) mesh_draw;

    // Return a pointer to the caller
    *pp_shader = p_shader;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Null pointer provided for parameter \"pp_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            no_vertex_property:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Parameter \"p_value\" is missing required property \"vertex\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/shader.json\n");
                #endif

                // Error
                return 0;

            no_fragment_property:
                #ifndef NDEBUG
                    log_error("[g10] [shader] Parameter \"p_value\" is missing required property \"fragment\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/shader.json\n");
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}


int g_sdl3_window_poll ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // Initialized data
    input *p_input = p_instance->context.p_input;

    // Process each event
    while (SDL_PollEvent(&p_instance->window.sdl3.event))
    {
        
        // Switch on the event type
        switch (p_instance->window.sdl3.event.type)
        {

            // Process should exit
            case SDL_EVENT_QUIT:
            {

                // Clear the running flag
                p_instance->running = false;

                // Done
                break;
            }
        }
    }
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

#endif