#include <framebuffer.h>

// function definitions
int framebuffer_info ( framebuffer *p_framebuffer )
{

    log_info("Framebuffer @%p\n", p_framebuffer);

    log_info(
        " - clear : [ %.2f, %.2f, %.2f, %.2f ]\n", 
        p_framebuffer->clear[0],
        p_framebuffer->clear[1],
        p_framebuffer->clear[2],
        p_framebuffer->clear[3]
    );
    
    return 1;
}

const char *framebuffer_key_accessor ( const framebuffer *const p_framebuffer )
{

    // done
    return p_framebuffer->_name;
}

int framebuffer_equality ( const framebuffer *p_a, const framebuffer *p_b )
{

    return p_a == p_b;
}
