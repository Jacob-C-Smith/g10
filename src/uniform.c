#include <uniform.h>

// function definitions
int uniform_info ( uniform *p_uniform )
{
    
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
