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

// SDL2
#include <SDL2/SDL.h>

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

    // Update the viewport
    glViewport(0, 0, width, height);    

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

    // Allocate a buffers
    glGenVertexArrays(1, p_mesh_data->opengl.vertex_arrays);
    glGenBuffers(1, p_mesh_data->opengl.vertex_buffers);
    glGenBuffers(1, p_mesh_data->opengl.element_arrays);

    // Bind the vertex array
    glBindVertexArray(p_mesh_data->opengl.vertex_arrays[0]);

    // Buffer the verticies
    glBindBuffer(GL_ARRAY_BUFFER, p_mesh_data->opengl.vertex_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, _mesh_create_info.verticies.quantity * sizeof(f32), _mesh_create_info.verticies.p_data, GL_STATIC_DRAW);

    // Buffer the indicies
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_mesh_data->opengl.element_arrays[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _mesh_create_info.elements.quantity * sizeof(u32), _mesh_create_info.elements.p_data, GL_STATIC_DRAW);

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
    shader     **pp_shader,
    json_value  *p_value
)
{

    // Argument check
    if ( pp_shader     ==        (void *) 0 ) goto no_shader;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    shader *p_shader = G10_REALLOC(0, sizeof(shader));
    size_t vertex_source_len   = 0,
           fragment_source_len = 0;
    dict *const p_dict = p_value->object;
    const json_value *const p_vertex   = dict_get(p_dict, "vertex"),
                     *const p_layout   = dict_get(p_dict, "layout"),
                     *const p_fragment = dict_get(p_dict, "fragment");
    int success;
    char _vs_src[512] = { 0 };
    char _fs_src[512] = { 0 };
    char *p_vs_src = &_vs_src;
    char *p_fs_src = &_fs_src;

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
    glShaderSource(p_shader->opengl.vertex_shader, 1, &p_vs_src, &vertex_source_len);
    glShaderSource(p_shader->opengl.fragment_shader, 1, &p_fs_src, &fragment_source_len);

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
    
    // 
    p_shader->functions.pfn_shader_on_bind = (fn_shader_on_bind) shader_bind_camera;
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

        // OpenGL errors
        {
            failed_to_compile_shader:
            
                // Initialized data
                char _log[512] = { 0 };

                // Get the output
                glGetProgramInfoLog(p_shader->opengl.program, 512, NULL, &_log);

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

int g_opengl_shader_bind_mat4 ( shader *p_shader, const char *p_name, void *p_value )
{
    glUniformMatrix4fv(glGetUniformLocation(p_shader->opengl.program, p_name), 1, GL_FALSE, p_value);
}

int g_opengl_shader_bind_camera ( shader *p_shader, const camera *const p_camera, ... )
{
    mat4 _p = p_camera->matrix._projection,
         _v = p_camera->matrix._view;
        
    g_opengl_shader_bind_mat4(p_shader, "P", &_p);
    g_opengl_shader_bind_mat4(p_shader, "V", &_v);
    
}

