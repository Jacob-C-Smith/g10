#include <uniform.h>

// function definitions
int uniform_set_pack_push ( uniform *p_uniform, void *p_data, fn_pack *pfn_pack )
{

    // argument check
    if ( NULL == p_uniform ) goto no_uniform;
    if ( NULL ==    p_data ) goto no_data;
    if ( NULL ==  pfn_pack ) goto no_pack;
    
    // initialized data
    g_instance *p_instance = g_active_instance();

    // set
    p_uniform->p_data = p_data;

    // pack
    p_uniform->len = pfn_pack(&p_uniform->_buffer, p_uniform->p_data);

    // push
    SDL_PushGPUVertexUniformData(p_instance->graphics.sdl3.command_buffer, p_uniform->idx, p_uniform->_buffer, p_uniform->len);
    SDL_PushGPUFragmentUniformData(p_instance->graphics.sdl3.command_buffer, p_uniform->idx, p_uniform->_buffer, p_uniform->len);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_uniform:
            no_data:
            no_pack:
                return 0;
        }
    }
}
int uniform_pack ( uniform *p_uniform )
{

    // done
    return p_uniform->pfn_pack(&p_uniform->_buffer, p_uniform->p_data);
}

int uniform_info ( uniform *p_uniform, size_t i )
{
    
    // logs
    logger_pad(), log_info("Uniform #%d @%p\n", i, p_uniform), 
    logger_push(),
    logger_pad(), printf("name - %s\n", p_uniform->_name),
    logger_pop();

    // success
    return 1;
}

const char *uniform_key_accessor ( const uniform *const p_uniform )
{

    // done
    return 0;
}

int uniform_equality ( const uniform *p_a, const uniform *p_b )
{

    return p_a == p_b;
}
