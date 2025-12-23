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
struct attachment_s
{
    char _name[63+1];
};

// function declarations
/// print
/** 
 *  Print a textual representation of an attachment to standard output
 * 
 * @param p_attachment the attachment
 * 
 * @return 1 on success, 0 on failure
 */
int attachment_info ( attachment *p_attachment );

/// key accessor
/** 
 *  Get the name of an attachment
 * 
 * @param p_attachment the attachment
 * 
 * @return the name of the attachment
 */
const char *attachment_key_accessor ( const attachment *const p_attachment );

/// equality
/** 
 * Test the equality of two attachments
 * 
 * @param p_a the attachment
 * 
 * @return the name of the attachment
 */
int attachment_equality ( const attachment *p_a, const attachment *p_b );