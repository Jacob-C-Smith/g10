#include <G10/GXShader.h>

int create_shader_module(char* code, size_t code_len, VkShaderModule* shader_module)
{
    GXInstance_t* instance = g_get_active_instance();

    VkShaderModuleCreateInfo* shader_module_create_info = calloc(1, sizeof(VkShaderModuleCreateInfo));
    shader_module_create_info->sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info->codeSize = code_len;
    shader_module_create_info->pCode = code;

    if (vkCreateShaderModule(instance->device, shader_module_create_info, (void*)0, shader_module))
    {
        printf("Failed to create a shader module");
    }

    return 0;
}

int create_shader(GXShader_t** shader)
{
    // TODO: Argument check
    {

    }

    GXShader_t *i_shader = calloc(1, sizeof(GXShader_t));

    // TODO: Check memory

    *shader = i_shader;

    return 1;

    // TODO: Error handling
    {

    }
}

int load_shader(GXShader_t** shader, const char* path)
{
    // TODO: Argument check

    char   *token_text     = 0;
    size_t  token_text_len = g_load_file(path, (void*)0, false);
        
    token_text = calloc(token_text_len + 1, sizeof(char));

    g_load_file(path, token_text, false);

    load_shader_as_json(shader, token_text, token_text_len);

    return 1;

    // TODO: Error handling
}

int load_shader_as_json(GXShader_t** shader, char* token_text, size_t token_text_len)
{
    // TODO: Argument check
    GXShader_t *i_shader                  = 0;
    char       *name                      = 0,
               *vertex_shader_path        = 0,
               *fragment_shader_path      = 0;
    dict       *json_data                 = 0,
               *vertex_shader_json_data   = 0,
               *fragment_shader_json_data = 0;
    
    parse_json(token_text, token_text_len, &json_data);

    create_shader(shader);
    i_shader= * shader;

    // Parse JSON data
    {
        JSONToken_t *token = 0;

        token = dict_get(json_data, "name");
        name = JSON_VALUE(token, JSONstring);
        
        i_shader->name = name;

        // Set up the vertex shader
        {
            token = dict_get(json_data, "vertex shader");
            vertex_shader_json_data = JSON_VALUE(token, JSONobject);
            parse_json(vertex_shader_json_data, strlen(vertex_shader_json_data), &vertex_shader_json_data);

            // Load the vertex shader from the path
            {
                char *vertex_shader_data = 0;
                size_t vertex_shader_data_len = 0;

                token = dict_get(vertex_shader_json_data, "path");
                vertex_shader_path = JSON_VALUE(token, JSONstring);

                vertex_shader_data_len = g_load_file(vertex_shader_path, 0, true);
                vertex_shader_data = calloc(vertex_shader_data_len+1, sizeof(char));
                g_load_file(vertex_shader_path, vertex_shader_data, true);

                create_shader_module(vertex_shader_data, vertex_shader_data_len, &i_shader->vertex_shader_module);

            }

            

        }

        // Set up the fragment shader
        {
            token = dict_get(json_data, "fragment shader");
            fragment_shader_json_data = JSON_VALUE(token, JSONobject);
            parse_json(fragment_shader_json_data, strlen(fragment_shader_json_data), &fragment_shader_json_data);

            // Load the fragment shader from the path
            {
                char* fragment_shader_data = 0;
                size_t fragment_shader_data_len = 0;

                token = dict_get(fragment_shader_json_data, "path");
                fragment_shader_path = JSON_VALUE(token, JSONstring);

                fragment_shader_data_len = g_load_file(fragment_shader_path, 0, true);
                fragment_shader_data = calloc(fragment_shader_data_len + 1, sizeof(char));
                g_load_file(fragment_shader_path, fragment_shader_data, true);

                create_shader_module(fragment_shader_data, fragment_shader_data_len, &i_shader->fragment_shader_module);

            }

            

        }
    }



    return 1;

    // TODO: Error handling
}