/** !
 * Implements g10 using OpenGL
 * 
 * @file 3rd-party/g_opengl.c
 * 
 * @author Jacob Smith
 */

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/mesh.h>
#include <g10/shader.h>

#ifdef G10_BUILD_WITH_OPENGL

// WSI dependant
#ifdef G10_BUILD_WITH_SDL2
    
    // SDL2
    #include <SDL2/SDL.h>
#elif defined G10_BUILD_WITH_GLFW

    //
#endif

// OpenGL
#include <glad/glad.h>

// Preprocessor definitions
//

// Function declarations
int g_opengl_initialize ( g_instance *p_instance, json_value *p_value )
{

    // Suppress compiler warnings
    (void)p_value;

    #ifdef G10_BUILD_WITH_SDL2
        
        // Context attributes
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

        // Create an OpenGL context
        p_instance->window.sdl2.gl_context = SDL_GL_CreateContext(p_instance->window.sdl2.window);

        // Error check
        if ( p_instance->window.sdl2.gl_context == (void *) 0 ) goto failed_to_create_opengl_context_with_sdl2;        
        
        // Check the glad loader
        if ( gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == false ) goto failed_to_load_glad;
    #endif

    // Enable backface culling
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_FRONT);  
    glFrontFace(GL_CCW);  
 
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);  
    glDepthFunc(GL_LESS);  

    // Enable multisampling
    glEnable(GL_MULTISAMPLE);  

    // Success
    return 1;

    // Error handling
    {
        failed_to_load_glad:
        failed_to_create_opengl_context_with_sdl2:

            // Error
            return 0;
    }
}

int g_opengl_window_resize ( g_instance *p_instance, u32 width, u32 height )
{

    // Unused
    (void) p_instance;

    // Update the viewport
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Success
    return 1;
}

/** !
 * Construct an OpenGL mesh from a struct
 * 
 * @param pp_mesh_data       pointer to result
 * @param p_mesh_create_info pointer to mesh create info struct
 * 
 * @return 1 on success, 0 on error
 */
int g_opengl_mesh_construct
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
    mesh_data *p_mesh_data = G10_REALLOC(0, sizeof(mesh_data));

    // Error check
    if ( p_mesh_data == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_mesh_data, 0, sizeof(mesh_data));

    // Allocate a buffers
    glGenVertexArrays(1, p_mesh_data->opengl.vertex_arrays);
    glGenBuffers(1, p_mesh_data->opengl.vertex_buffers);
    glGenBuffers(1, p_mesh_data->opengl.element_arrays);

    // Bind the vertex array
    glBindVertexArray(p_mesh_data->opengl.vertex_arrays[0]);

    // Buffer the verticies
    glBindBuffer(GL_ARRAY_BUFFER, p_mesh_data->opengl.vertex_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) (_mesh_create_info.verticies.quantity * sizeof(f32)), _mesh_create_info.verticies.p_data, GL_STATIC_DRAW);

    // Buffer the indicies 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_mesh_data->opengl.element_arrays[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(_mesh_create_info.elements.quantity * sizeof(u32)), _mesh_create_info.elements.p_data, GL_STATIC_DRAW);

    // Add vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), 0);

    // Enable the verticies
    glEnableVertexAttribArray(0);

    // Bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);  

    // Set the quantity of indices
    p_mesh_data->opengl.indices = _mesh_create_info.elements.quantity;

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

int g_opengl_shader_construct
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
    shader *p_shader = G10_REALLOC(0, sizeof(shader));
    size_t vertex_source_len = 0, fragment_source_len = 0;
    dict *const p_dict = p_value->object;
    const json_value *const p_vertex   = dict_get(p_dict, "vertex"),
                     *const p_layout   = dict_get(p_dict, "layout"),
                     *const p_fragment = dict_get(p_dict, "fragment");
    int success;
    char _vs_src[2048] = { 0 };
    char _fs_src[2048] = { 0 };
    char *p_vs_src = (char *)&_vs_src;
    char *p_fs_src = (char *)&_fs_src;

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
    g_load_file(p_vertex->string, &_vs_src, false);
    g_load_file(p_fragment->string, &_fs_src, false);

    // Compute the length of each file
    vertex_source_len = strlen(_vs_src);
    fragment_source_len = strlen(_fs_src);

    // Create a vertex shader and a fragment shader
    p_shader->opengl.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    p_shader->opengl.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach source code to each shader handle
    glShaderSource(p_shader->opengl.vertex_shader, 1, (const GLchar **)&p_vs_src, (const GLint *)&vertex_source_len);
    glShaderSource(p_shader->opengl.fragment_shader, 1, (const GLchar **)&p_fs_src, (const GLint *)&fragment_source_len);

    // Compile the vertex and fragment shaders
    glCompileShader(p_shader->opengl.vertex_shader);
    glCompileShader(p_shader->opengl.fragment_shader);

    // Create a shader program
    p_shader->opengl.program = glCreateProgram();

    // Attach the shaders to the program
    glAttachShader(
        p_shader->opengl.program,
        p_shader->opengl.vertex_shader
    );
    glAttachShader(
        p_shader->opengl.program,
        p_shader->opengl.fragment_shader
    );

    // Link the shader
    glLinkProgram(p_shader->opengl.program);

    // Get the build status
    glGetProgramiv(p_shader->opengl.program, GL_LINK_STATUS, &success);

    // Error handling
    if ( success == false ) goto failed_to_compile_shader;
    
    // Store the on bind function
    p_shader->functions.pfn_shader_on_bind = (fn_shader_on_bind *) shader_bind_camera;

    // Store the draw function
    p_shader->functions.pfn_shader_on_draw = (fn_shader_on_draw *) mesh_draw;

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

        // OpenGL errors
        failed_to_compile_shader:
        {
            
            // Initialized data
            char _log[512] = { 0 };

            // Get the output
            glGetProgramInfoLog(p_shader->opengl.program, 512, NULL, (GLchar *)&_log);

            // Print the output to standard out
            printf("%s\n", _log);

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

int g_opengl_shader_bind ( shader *p_shader )
{

    // Argument check
    if ( p_shader == (void *) 0 ) goto no_shader;

    // Bind the shader
    glUseProgram(p_shader->opengl.program);

    // Per bind callback
    //if ( p_shader->functions.pfn_shader_on_bind )
    //    p_shader->functions.pfn_shader_on_bind();

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [opengl] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_opengl_shader_bind_vec3 ( shader *p_shader, const char *p_name, void *p_value )
{

    // Bind the vec3 
    glUniform3fv(glGetUniformLocation(p_shader->opengl.program, p_name), 1, p_value); 

    // Success
    return 1;
}

int g_opengl_shader_bind_mat4 ( shader *p_shader, const char *p_name, void *p_value )
{

    // Bind the 4x4 matrix
    glUniformMatrix4fv(glGetUniformLocation(p_shader->opengl.program, p_name), 1, GL_FALSE, p_value); 

    // Success
    return 1;
}

int g_opengl_shader_bind_camera ( shader *p_shader, const camera *const p_camera, ... )
{

    // Argument check
    if ( p_shader == (void *) 0 ) goto no_shader;
    if ( p_camera == (void *) 0 ) goto no_camera;

    // State check
    if ( p_camera->dirty ) goto camera_dirty;
    
    camera_ready:
    
    // Initialized data
    mat4 _p = p_camera->matrix._projection,
         _v = p_camera->matrix._view;
    
    // Bind projection and view matricies
    g_opengl_shader_bind_mat4(p_shader, "P", &_p);
    g_opengl_shader_bind_mat4(p_shader, "V", &_v);

    // Success
    return 1;

    // This branch recomputes the projection and view matricies
    camera_dirty:
    {

        // Initialized data
        g_instance *p_instance = g_get_active_instance();
        
        // Compute the view matrix
        camera_matrix_view
        (
            &p_camera->matrix._view,
            p_camera->view.location,
            p_camera->view.target,
            p_camera->view.up
        );
        
        // Compute the projection matrix
        camera_matrix_projection_perspective
        (
            &p_camera->matrix._projection,
            p_camera->projection.fov,
            (float)((double)p_instance->window.width / (double)p_instance->window.height),
            p_camera->projection.near_clip,
            p_camera->projection.far_clip
        );

        // Done
        goto camera_ready;
    }

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_camera\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_opengl_shader_bind_transform ( shader *p_shader, const transform *const p_transform, ... )
{

    // Argument check
    if ( p_shader    == (void *) 0 ) goto no_shader;
    if ( p_transform == (void *) 0 ) goto no_transform;

    // State check
    if ( p_transform->dirty ) goto transform_dirty;
    
    transform_ready:
    
    // Initialized data
    mat4 _m = p_transform->model;
    
    // Bind model matrix
    g_opengl_shader_bind_mat4(p_shader, "M", &_m);

    // Success
    return 1;

    // This branch recomputes the model matricies
    transform_dirty:
    {

        // Compute the model matrix
        mat4_model_from_vec3(
            &p_transform->model,
            p_transform->location,
            (vec3){ 0, 0, 0 },
            p_transform->scale
        );

        // Done
        goto transform_ready;
    }

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_transform:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_transform\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_opengl_shader_bind_bv ( shader *p_shader, const bv *const p_bv, ... )
{

    // Argument check
    if ( p_shader == (void *) 0 ) goto no_shader;
    if ( p_bv     == (void *) 0 ) goto no_bv;

    // Initialized data
    vec3 _min         = p_bv->bounds.min,
         _max         = p_bv->bounds.max,
         _center      = { 0 },
         _dimension   = { 0 };
    mat4 _translation = { 0 },
         _scale       = { 0 },
         _model       = { 0 };

    // Compute center
    vec3_add_vec3(&_center, _min, _max);
    float scale_factor = 0.5;
    // Compute dimensions
    vec3_sub_vec3(&_dimension, _max, _min);
    vec3_mul_scalar(&_dimension, _dimension, scale_factor);
    vec3_mul_scalar(&_center, _center, scale_factor);

    _translation = (mat4)
    {
        .a = 1         , .b = 0         , .c = 0         , .d = 0,
        .e = 0         , .f = 1         , .g = 0         , .h = 0,
        .i = 0         , .j = 0         , .k = 1         , .l = 0,
        .m = _center.x, .n = _center.y, .o = _center.z, .p = 1
    };

    _scale = (mat4)
    {
        .a = _dimension.x, .b = 0           , .c = 0           , .d = 0,
        .e = 0           , .f = _dimension.y, .g = 0           , .h = 0,
        .i = 0           , .j = 0           , .k = _dimension.z, .l = 0,
        .m = 0           , .n = 0           , .o = 0           , .p = 1
    };

    // Compute the model matrix
    mat4_mul_mat4(&_model, _scale, _translation);
    
    // Bind the model matrix
    g_opengl_shader_bind_mat4(p_shader, "M", &_model);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shader:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_bv:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_bv\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int g_opengl_set_wireframe ( bool set )
{
    
    glPolygonMode(GL_FRONT_AND_BACK, set ? GL_LINE : GL_FILL);

    return 1;
}

#endif