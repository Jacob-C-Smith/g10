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
#include <aabb.h>
#include <g10.h>

// enumeration definitions
enum geometry_vertex_attribute_e
{ 
    GEOMETRY_XYZ, 
    GEOMETRY_UV, 
    GEOMETRY_NXYZ, 
    GEOMETRY_TXYZ, 
    GEOMETRY_BXYZ, 
    GEOMETRY_QTY
};

// structure definitions
struct geometry_s
{
    char _name[63+1];
    aabb _bounds;
    u32 vertex_count;
    void *p_handle;
    void *_p_handles[GEOMETRY_QTY];
};

// function declarations
int geometry_bind ( render_pass *p_render_pass, geometry *p_geometry );

/// print
/** 
 *  Print a textual representation of an geometry to standard output
 * 
 * @param p_geometry the geometry
 * 
 * @return 1 on success, 0 on failure
 */
int geometry_info ( geometry *p_geometry );

/// key accessor
/** 
 *  Get the name of an geometry
 * 
 * @param p_geometry the geometry
 * 
 * @return the name of the geometry
 */
const char *geometry_key_accessor ( const geometry *const p_geometry );

/// equality
/** 
 * Test the equality of two geometrys
 * 
 * @param p_a the geometry
 * 
 * @return the name of the geometry
 */
int geometry_equality ( const geometry *p_a, const geometry *p_b );