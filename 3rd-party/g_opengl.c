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

// SDL2
#include <SDL2/SDL.h>

// OpenGL
#include <glad/glad.h>

// Preprocessor definitions
//

// Function declarations
int g_opengl_initialize ( g_instance *p_instance, json_value *p_value )
{

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
    mesh_create_info  *p_mesh_create_info
)
{

    // Argument check
    if ( pp_mesh_data                         == (void *) 0 ) goto no_mesh;
    if ( p_mesh_create_info->verticies.p_data == (void *) 0 ) goto no_verticies;
    if ( p_mesh_create_info->elements.p_data  == (void *) 0 ) goto no_elements;

    // Initialized data
    mesh_data *p_mesh_data = G10_REALLOC(0, sizeof(mesh_data));

    // Error check
    if ( p_mesh_data == (void *) 0 ) goto no_mem;

    // Allocate a buffers
    glGenVertexArrays(1, p_mesh_data->opengl.vertex_arrays);
    glGenBuffers(1, p_mesh_data->opengl.vertex_buffers);

    // Bind the vertex array
    glBindVertexArray(p_mesh_data->opengl.vertex_arrays[0]);

    // Buffer the verticies
    glBindBuffer(GL_ARRAY_BUFFER, p_mesh_data->opengl.vertex_buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, p_mesh_create_info->verticies.quantity, p_mesh_create_info->verticies.p_data, GL_STATIC_DRAW);

    // Buffer the indicies
    glBindBuffer(GL_ARRAY_BUFFER, p_mesh_data->opengl.element_arrays[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_mesh_create_info->elements.quantity, p_mesh_create_info->elements.p_data, GL_STATIC_DRAW);

    // Add vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    // Enable the verticies
    glEnableVertexAttribArray(0);

    // Bind the array buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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
