#include <attachment.h>

// function definitions
int attachment_info ( attachment *p_attachment )
{

    log_info(" - %s\n", p_attachment->_name);
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
