#include <pipeline.h>

// function definitions
int pipeline_info ( pipeline *p_pipeline )
{

    log_info(" - %s\n", p_pipeline->_name);
    return 1;
}

const char *pipeline_key_accessor ( const pipeline *const p_pipeline )
{

    // done
    return p_pipeline->_name;
}

int pipeline_equality ( const pipeline *p_a, const pipeline *p_b )
{

    return p_a == p_b;
}
