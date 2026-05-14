#include <texture.h>

const char *texture_key_accessor ( const texture *const p_texture )
{
    return p_texture->_name;
}

int texture_equality ( const texture *p_a, const texture *p_b )
{
    return p_a == p_b;
}
