#include <attachment.h>

// function definitions
int attachment_info ( attachment *p_attachment )
{

    logger_pad(), log_info("Attachment @%p\n", p_attachment),

    logger_push(),
    logger_pad(), printf("name   - %s\n", p_attachment->_name),
    logger_pad(), printf("handle - %p\n", p_attachment->p_handle),
    logger_pop();

    // success
    return 1;
}

const char *attachment_key_accessor ( const attachment *const p_attachment )
{

    // done
    return p_attachment->_name;
}

int attachment_equality ( const attachment *p_a, const attachment *p_b )
{

    return p_a == p_b;
}
