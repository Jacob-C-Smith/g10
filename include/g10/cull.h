/** !
 * Header file for culling 
 * 
 * @file g10/cull.h
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

// Structure definitions
struct cull_operation_s
{
    char _name[63 + 1];
    
};

// Constructors
/** !
 * Construct a cull chain from variadic arguments
 * 
 * @param pp_cull_operation return
 * @param count             the quantity of cull operations
 * @param ...               variadic fn_cull_operation's
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cull_chain_construct
(
    cull_operation *pp_cull_operation,
    size_t          count,
    ...
);

// Culling operations
/** !
 * Cull an entity
 * 
 * @param p_entity the entity
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cull_entity_visible ( entity *p_entity );

// Culling
/** !
 * Cull objects using a cull chain
 * 
 * @param p_cull_chain the cull chain
 * @param pp_result    return
 * @param p_objects    pointer to array of objects to cull
 * @param limit        the maximum quantity of returned objects
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int cull_objects
(
    cull_operation  *p_cull_chain,
    void           **pp_result,
    void           **p_objects,
    size_t           limit
);

// Destructors
DLLEXPORT int cull_chain_destroy ( cull_operation *pp_cull_operation );
