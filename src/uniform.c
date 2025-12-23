#include <uniform.h>

// function definitions
int uniform_pack ( uniform *p_uniform )
{

    // done
    return p_uniform->pfn_pack(&p_uniform->_buffer, p_uniform->p_data);
}

int uniform_info ( uniform *p_uniform )
{
    
    // logs
    log_info("Uniform @%p\n", p_uniform),
    log_info("name - %s\n", p_uniform->_name);

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
