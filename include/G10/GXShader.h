/** !
 * @file G10/GXShader.h
 * @author Jacob Smith
 * 
 * Include header for graphics, compute, and mesh shaders
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vulkan
#include <vulkan/vulkan.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXCamera.h>

struct GXDescriptor_s
{
    char           *name;

};

struct GXSet_s
{
    char                  *name;
    size_t                 index;
    VkDescriptorPool       descriptor_pool;
    VkDescriptorSet       *descriptor_sets;
    struct GXDescriptor_s *descriptors_data;
};

struct GXShader_s
{

    char   *name;
    size_t  users;

    union 
    {
        struct 
        {
            char                  **push_constant_properties;
            void                   *push_constant_data;

            VkShaderModule          vertex_shader_module,
                                    tessellation_control_shader_module,
                                    tessellation_evaluation_shader_module,
                                    geometry_shader_module,
                                    task_shader_module,
                                    mesh_shader_module,
                                    fragment_shader_module;
        
            VkPipeline              pipeline;
            VkPipelineLayout        pipeline_layout;
            VkDescriptorPool        descriptor_pool;
            VkDescriptorSet        *descriptor_sets;
            VkDescriptorSetLayout   descriptor_set_layout;
            VkDescriptorSetLayout  *descriptor_set_layouts;

            VkBuffer               *uniform_buffers;
            VkDeviceMemory         *uniform_buffers_memory;

            size_t                  push_constant_size, 
                                    set_count;

            struct GXSet_s         *sets_data;
        } graphics;

        struct {
            VkPipeline       pipeline;
            VkPipelineLayout pipeline_layout;

        } compute;
    };
};


DLLEXPORT int create_shader_module            ( char        *code, size_t code_len, VkShaderModule* shader_module);

// Allocators
/** !
 *  Allocate memory for a shader
 *
 * @param pp_shader : return
 *
 * @sa destroy_shader
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_shader                   ( GXShader_t **pp_shader );

// Constructors
/** !
 *  Load a shader from a JSON file
 *
 * @param pp_shader : return
 * @param path      : The path to a JSON file containing a shader object
 *
 * @sa load_shader_as_json
 * @sa load_shader_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_shader                     ( GXShader_t **pp_shader, const char* path );

/** !
 *  Load a shader from JSON text
 *
 * @param pp_shader : return
 * @param text      : The shader JSON text
 *
 * @sa load_shader
 * @sa load_shader_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_shader_as_json             ( GXShader_t **pp_shader, char* text );

/** !
 *  Load a shader from a JSON value
 *
 * @param pp_shader : return
 * @param p_value   : The camera JSON value
 *
 * @sa load_shader
 * @sa load_shader_as_json
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_shader_as_json_value       ( GXShader_t **pp_shader, JSONValue_t *p_value );

DLLEXPORT int use_shader                      ( GXShader_t  *p_shader );

DLLEXPORT int update_shader_push_constant     ( GXShader_t  *p_shader );
DLLEXPORT int add_shader_push_constant_getter ( char *getter_name, int (*getter_function) (void *) );

DLLEXPORT int set_shader_camera               ( GXEntity_t  *p_entity );

// Destructors
/** !
 *  Destroy a camera
 *
 * @param pp_camera : Pointer to pointer to camera to destroy
 *
 * @sa create_camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_shader                  ( GXShader_t  *p_shader );