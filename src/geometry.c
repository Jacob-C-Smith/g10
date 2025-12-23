#include <geometry.h>

// function definitions
int geometry_info ( geometry *p_geometry )
{

    log_info(" - %s\n", p_geometry->_name);
    return 1;
}

const char *geometry_key_accessor ( const geometry *const p_geometry )
{

    // done
    return p_geometry->_name;
}

int geometry_equality ( const geometry *p_a, const geometry *p_b )
{

    return p_a == p_b;
}
