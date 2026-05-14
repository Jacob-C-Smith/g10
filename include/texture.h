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

// structure definitions
struct texture_s
{
    char     _name[63+1];
    u32      width;
    u32      height;
    u32      channels;
    void *p_handle;
};

// function declarations
/// key accessor
/** 
 *  Get the name of an texture
 * 
 * @param p_texture the texture
 * 
 * @return the name of the texture
 */
const char *texture_key_accessor ( const texture *const p_texture );

/// equality
/** 
 * Test the equality of two textures
 * 
 * @param p_a the texture
 * 
 * @return the name of the texture
 */
int texture_equality ( const texture *p_a, const texture *p_b );
