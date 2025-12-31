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
void any_print(void *p_element, int i)
{
    logger_pad(), printf("[%d] : %p\n", i, p_element);

    return;
}

int pipeline_info ( pipeline *p_pipeline )
{

    // logs
    logger_pad(), log_info("Pipeline @%p\n", p_pipeline),
    logger_push(),
    logger_pad(), printf("name - %s\n", p_pipeline->_name);
    logger_pad(), printf("uniforms[%d]: \n", array_size(p_pipeline->p_uniforms));
    
    logger_push(), 
    array_fori(p_pipeline->p_uniforms, (fn_fori *)uniform_info);
    logger_pop(),

    logger_pad(), printf("samplers[%d]: \n", (p_pipeline->p_samplers) ? array_size(p_pipeline->p_samplers) : 0);
    if ( p_pipeline->p_samplers )
        logger_push(),
        array_foreach(p_pipeline->p_samplers, (fn_foreach *)sampler_info),
        logger_pop();


    logger_pad(), printf("draw list[%d]: \n", array_size(p_pipeline->p_static_draw_list));
    if ( array_size(p_pipeline->p_static_draw_list) )
        logger_push(),
        array_fori(p_pipeline->p_static_draw_list, (fn_fori *)any_print),
        logger_pop();
        
    logger_pop();

    // success
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
