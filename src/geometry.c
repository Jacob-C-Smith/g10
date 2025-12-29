#include <geometry.h>

// function definitions
int geometry_bind ( render_pass *p_render_pass, geometry *p_geometry )
{
    extern int g_sdl3_geometry_bind ( render_pass *p_render_pass, geometry *p_geometry );

    return g_sdl3_geometry_bind(p_render_pass, p_geometry);
}

int geometry_info ( geometry *p_geometry )
{

    logger_pad(), log_info("Geometry @%p\n", p_geometry);
    
    logger_push(),
    logger_pad(), printf("name - %s\n", p_geometry->_name),

    logger_pad(), printf("bounds:\n"),
    logger_push(),
    aabb_info(&p_geometry->_bounds),
    logger_pop(),

    logger_pop();
    
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