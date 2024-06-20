/** !
 * Include header for scene.
 * 
 * @file g10/scene.h
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
#include <g10/entity.h>

// Structure definitions
struct scene_s
{

    // Name
    char _name[255 + 1];

    // Data
    struct
    {
        dict *entities,
             *cameras,
             *lights;
    } data;

    // State
    /*
    struct 
    {
        camera *p_camera;
    } state;
    */
};

// Function definitions

// Constructors
/** !
 * Construct a scene from a json value
 * 
 * @param pp_scene return
 * @param p_value  the json value
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int scene_from_json
(
    scene      **pp_scene,
    json_value  *p_value
);

// Accessors
//

// Mutators
/** !
 *  Add an entity to a scene
 *
 * @param p_scene  the scene
 * @param p_entity the entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int scene_entity_add ( scene *p_scene, entity *p_entity );

// Destructorss
/** !
 * Destroy and deallocate a scene
 * 
 * @param pp_scene pointer to scene pointer
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int scene_destroy ( scene **pp_scene );
