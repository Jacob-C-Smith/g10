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
struct sampler_s
{
    char _name[63+1];
    void *p_handle;
    void *p_texture;
    int idx;
};

#include <g10.h>

// function declarations
int sampler_from_bounds ( sampler *p_sampler, vec3 min, vec3 max );
int sampler_from_transform ( sampler *p_sampler, transform *p_transform );
int sampler_bind ( render_pass *p_render_pass, pipeline *p_pipeline, sampler *p_sampler );

/// print
/** 
 *  Print a textual representation of a sampler to standard output
 * 
 * @param p_sampler the sampler
 * 
 * @return 1 on success, 0 on failure
 */
int sampler_info ( sampler *p_sampler );

/// key accessor
/** 
 *  Get the name of a sampler
 * 
 * @param p_sampler the sampler
 * 
 * @return the name of the sampler
 */
const char *sampler_key_accessor ( const sampler *const p_sampler );

/// equality
/** 
 * Test the equality of two samplers
 * 
 * @param p_a the sampler
 * 
 * @return the name of the sampler
 */
int sampler_equality ( const sampler *p_a, const sampler *p_b );
