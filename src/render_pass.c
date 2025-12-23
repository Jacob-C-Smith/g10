#include <render_pass.h>

// function definitions
int render_pass_info ( render_pass *p_render_pass )
{

    log_info("Render Pass @%p\n", p_render_pass);
    log_info(" - name : \"%s\"\n", p_render_pass->_name);
    log_info(" - pipelines : \n");
    array_foreach( p_render_pass->p_pipelines, (fn_foreach *) pipeline_info );
    
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