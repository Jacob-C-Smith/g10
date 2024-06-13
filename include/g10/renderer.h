/** !
 * Include header for renderer.
 * 
 * @file g10/renderer.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// log module
#include <log/log.h>

// array module
#include <array/array.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// g10
#include <g10/gtypedef.h>
#include <g10/linear.h>

// Structure definitions
struct renderer_s
{

    // Name
    char _name[255 + 1];

    
};

// Function definitions

// Constructors
/** !
 * Construct a renderer from a json value
 * 
 * @param pp_renderer return
 * @param p_value     the json value
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int renderer_from_json
(
    renderer   **pp_renderer,
    json_value  *p_value
);

// Accessors
//

// Destructorss
/** !
 * Destroy and deallocate a renderer
 * 
 * @param pp_renderer pointer to renderer pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int renderer_destroy ( renderer **pp_renderer );
