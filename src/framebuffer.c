#include <framebuffer.h>

// function definitions
int framebuffer_info ( framebuffer *p_framebuffer )
{

    logger_pad(), log_info("Framebuffer @%p\n", p_framebuffer);
    
    logger_push(),

    logger_pad(), printf(
        "clear - [ %.2f, %.2f, %.2f, %.2f ]\n", 
        p_framebuffer->clear[0],
        p_framebuffer->clear[1],
        p_framebuffer->clear[2],
        p_framebuffer->clear[3]
    ),
    
    
    logger_pad(), printf("color:\n");
    logger_push(),
    array_foreach(p_framebuffer->p_attachments, (fn_foreach *)attachment_info),
    logger_pop(),

    logger_pad(), printf("depth:\n");
    logger_push(),
    attachment_info(p_framebuffer->p_depth),
    logger_pop(),
    
    logger_pop();
    
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
