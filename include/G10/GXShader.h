#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <G10/G10.h>

struct GXShader_s
{
    char           *name;
    VkShaderModule  vertex_shader_module,
                    fragment_shader_module;
    VkPipeline      pipeline;
};

int create_shader_module(char* code, size_t code_len, VkShaderModule* shader_module);

int create_shader(GXShader_t** shader);
int load_shader(GXShader_t** shader, const char* path);
int load_shader_as_json(GXShader_t** shader, char* token_text, size_t token_text_len);
int construct_shader(GXShader_t** shader, u8* vertex_shader, u8* tesselation_control_shader, u8* tesselation_evaluation_shader, u8* geometry_shader, u8* fragment_shader);

