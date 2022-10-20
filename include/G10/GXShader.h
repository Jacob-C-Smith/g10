#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXCamera.h>

struct GXShader_s
{
    char                   *name;
    char                  **push_constant_properties;
    void                   *push_constant_data;

    VkShaderModule          vertex_shader_module,
                            tessellation_control_shader_module,
                            tessellation_evaluation_shader_module,
                            geometry_shader_module,
                            fragment_shader_module;

    VkPipeline              pipeline;
    VkPipelineLayout        pipeline_layout;
    VkDescriptorPool        descriptor_pool;
    VkDescriptorSet        *descriptor_sets;
    VkDescriptorSetLayout   descriptor_set_layout;
    VkDescriptorSetLayout  *descriptor_set_layouts;

    VkBuffer               *uniform_buffers;
    VkDeviceMemory         *uniform_buffers_memory;

    size_t                  descriptor_set_count,
                            descriptor_set_layout_count,
                            push_constant_size;

    dict                   *sets;

    size_t                  users;
};

DLLEXPORT int create_shader_module        ( char        *code, size_t code_len, VkShaderModule* shader_module);

DLLEXPORT int create_shader               ( GXShader_t **shader );
DLLEXPORT int load_shader                 ( GXShader_t **shader, const char* path );
DLLEXPORT int load_shader_as_json         ( GXShader_t **shader, char* token_text, size_t token_text_len );

DLLEXPORT int use_shader                  ( GXShader_t  *shader );

DLLEXPORT int update_shader_push_constant ( GXShader_t  *shader );
DLLEXPORT int add_shader_push_constant_getter ( char *getter_name, int (*getter_function) (void *));

DLLEXPORT int set_shader_camera               ( GXEntity_t  *p_entity );

DLLEXPORT int destroy_shader                  ( GXShader_t  *shader );