// header file
#include <sampler.h>

// function definitions
int sampler_bind ( render_pass *p_render_pass, pipeline *p_pipeline, sampler *p_sampler )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    
    // success
    return 1;
}

// function definitions
int sampler_info ( sampler *p_sampler )
{

    // logs
    printf("Sampler @%p\n", p_sampler);
    
    // success
    return 1;
}
