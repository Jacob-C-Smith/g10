#include <pipeline.h>

// function definitions
int pipeline_set_bind_once (pipeline *p_pipeline, fn_pipeline_bind_once *pfn_bind_once)
{

    // store the bind function
    p_pipeline->pfn_bind_once = pfn_bind_once;

    // success
    return 1;
}

int pipeline_set_bind_each (pipeline *p_pipeline, fn_pipeline_bind_each *pfn_bind_each)
{

    // store the bind function
    p_pipeline->pfn_bind_each = pfn_bind_each;

    // success
    return 1;
}

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
