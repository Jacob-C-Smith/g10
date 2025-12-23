// header guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>

// structure definitions
struct render_pass_s
{
    char _name[63+1];
    framebuffer *p_framebuffer;
    array *p_pipelines;
    void *p_handle;
};

// function declarations
/// print
/** 
 *  Print a textual representation of a render_pass to standard output
 * 
 * @param p_render_pass the render_pass
 * 
 * @return 1 on success, 0 on failure
 */
int render_pass_info ( render_pass *p_render_pass );

/// key accessor
/** 
 *  Get the name of a render_pass
 * 
 * @param p_render_pass the render_pass
 * 
 * @return the name of the render_pass
 */
const char *render_pass_key_accessor ( const render_pass *const p_render_pass );

/// equality
/** 
 * Test the equality of two render_passes
 * 
 * @param p_a the render_pass
 * 
 * @return the name of the render_pass
 */
int render_pass_equality ( const render_pass *p_a, const render_pass *p_b );