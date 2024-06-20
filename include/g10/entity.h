/** !
 * Header file for entity
 * 
 * @file g10/entity.h
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

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/ai.h>
#include <g10/transform.h>

// Structure definitions
struct entity_s
{
    char _name[255+1];

    transform *p_transform;
    ai        *p_ai;
};

// Constructors
/** !
 * Construct an entity from a json value
 * 
 * @param pp_entity result
 * @param p_value   the json value
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int entity_from_json ( entity **pp_entity, json_value *p_value );

/** !
 * Get the bounds of an entity
 * 
 * @param p_entity the entity
 * @param p_min    the minimum bound
 * @param p_max    the maximum bound
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int entity_bounding_box ( entity *p_entity, vec3 *p_min, vec3 *p_max );
