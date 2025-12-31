#include <render_pass.h>

// function definitions
int render_pass_info ( render_pass *p_render_pass )
{

    // logs
    logger_pad(), log_info("Render Pass @%p\n", p_render_pass),

    logger_push(),
    logger_pad(), printf("name - %s\n", p_render_pass->_name),
    
    logger_pad(), printf("framebuffer: \n"),
    logger_push(),
    framebuffer_info(p_render_pass->p_framebuffer),
    logger_pop(),

    logger_pad(), printf("pipelines: \n"),
    logger_push(),
    array_foreach( p_render_pass->p_pipelines, (fn_foreach *) pipeline_info ),
    logger_pop(),
    
    logger_pop();
    
    // success
    return 1;
}

const char *render_pass_key_accessor ( const render_pass *const p_render_pass )
{

    // done
    return p_render_pass->_name;
}

int render_pass_equality ( const render_pass *p_a, const render_pass *p_b )
{

    return p_a == p_b;
}

int render_pass_draw ( renderer *p_renderer )
{

    return 1;
}