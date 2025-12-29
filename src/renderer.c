// header
#include <renderer.h>
#include <attachment.h>
#include <render_pass.h>

int renderer_info ( renderer *p_renderer )
{

    logger_pad(), log_info("Renderer @%p\n",p_renderer),
    logger_push(),
    logger_pad(),printf("name - %s\n", p_renderer->_name),
    logger_pad(), printf("attachments:\n"),
    
    logger_push(),
    dict_foreach(p_renderer->p_attachments, (void (*)(const void *const, size_t))attachment_info),
    logger_pop(),

    logger_pad(), printf("render passes: \n"),
    
    logger_push(),
    array_foreach(p_renderer->p_passes, (fn_foreach *)render_pass_info);
    logger_pop();

    // success
    return 1;
}

int renderer_render ( g_instance *p_instance ) 
{
    // argument check
    if ( NULL == p_instance ) goto no_renderer;
    if ( NULL == p_instance->context.p_renderer ) goto no_renderer;
    
    // initialized data
    renderer *p_renderer = p_instance->context.p_renderer;
    size_t len = array_size(p_renderer->p_passes);
    
    // early
    if ( 0 == p_renderer->pfn_early(p_instance) ) return 1;

    // draw
    p_renderer->pfn_draw(p_instance);

    // late
    p_renderer->pfn_late(p_instance);

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_renderer:
                #ifndef NDEBUG
                    log_error("[g10] [renderer] Null pointer provided for parameter \"p_renderer\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}
