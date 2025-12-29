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
struct framebuffer_s
{
    char _name[63+1];
    array *p_attachments;
    attachment *p_depth;
    f32 clear[4];
};

// function declarations
/// print
/** 
 *  Print a textual representation of an framebuffer to standard output
 * 
 * @param p_framebuffer the framebuffer
 * 
 * @return 1 on success, 0 on failure
 */
int framebuffer_info ( framebuffer *p_framebuffer );

/// key accessor
/** 
 *  Get the name of an framebuffer
 * 
 * @param p_framebuffer the framebuffer
 * 
 * @return the name of the framebuffer
 */
const char *framebuffer_key_accessor ( const framebuffer *const p_framebuffer );

/// equality
/** 
 * Test the equality of two framebuffers
 * 
 * @param p_a the framebuffer
 * 
 * @return the name of the framebuffer
 */
int framebuffer_equality ( const framebuffer *p_a, const framebuffer *p_b );