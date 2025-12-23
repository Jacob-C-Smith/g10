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
struct uniform_s
{
    char _name[63+1];
    void *p_data;
    fn_pack *pfn_pack;
    size_t len;
    size_t idx;
    char _buffer[256];
};

// function declarations
int uniform_set_pack_push ( uniform *p_uniform, void *p_data, fn_pack *pfn_pack );
int uniform_pack ( uniform *p_uniform );

/// print
/** 
 *  Print a textual representation of an uniform to standard output
 * 
 * @param p_uniform the uniform
 * 
 * @return 1 on success, 0 on failure
 */
int uniform_info ( uniform *p_uniform );

/// key accessor
/** 
 *  Get the name of an uniform
 * 
 * @param p_uniform the uniform
 * 
 * @return the name of the uniform
 */
const char *uniform_key_accessor ( const uniform *const p_uniform );

/// equality
/** 
 * Test the equality of two uniforms
 * 
 * @param p_a the uniform
 * 
 * @return the name of the uniform
 */
int uniform_equality ( const uniform *p_a, const uniform *p_b );