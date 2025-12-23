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
struct pipeline_s
{
    char _name[63+1];
    void *pipeline;
};

// function declarations
/// print
/** 
 *  Print a textual representation of an pipeline to standard output
 * 
 * @param p_pipeline the pipeline
 * 
 * @return 1 on success, 0 on failure
 */
int pipeline_info ( pipeline *p_pipeline );

/// key accessor
/** 
 *  Get the name of an pipeline
 * 
 * @param p_pipeline the pipeline
 * 
 * @return the name of the pipeline
 */
const char *pipeline_key_accessor ( const pipeline *const p_pipeline );

/// equality
/** 
 * Test the equality of two pipelines
 * 
 * @param p_a the pipeline
 * 
 * @return the name of the pipeline
 */
int pipeline_equality ( const pipeline *p_a, const pipeline *p_b );