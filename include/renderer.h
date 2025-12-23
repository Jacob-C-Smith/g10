// header guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>

// structure definitions
struct renderer_s
{
    char  _name[63+1];
    dict *p_attachments;
    array *p_passes;

    fn_render_early *pfn_early;
    fn_render_late *pfn_late;
    fn_render_draw *pfn_draw;
};

/// print
/** 
 *  Print a textual representation of a renderer to standard output
 * 
 * @param p_renderer the renderer
 * 
 * @return 1 on success, 0 on failure
 */
int renderer_info ( renderer *p_renderer );

int renderer_render ( g_instance *p_instance );