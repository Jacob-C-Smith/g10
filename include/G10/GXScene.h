#pragma once

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXEntity.h>
#include <G10/GXCamera.h>
#include <G10/GXLight.h>

#include <G10/GXShader.h>

struct GXScene_s
{
	
	// The name of the scene
	char           *name;

	// Scene data
	dict           *entities;
	dict           *cameras;
	dict           *lights;
	GXCollision_t **collisions;

	// A list of entities with rigidbodies
	dict           *actors;

	// A list of entities with rigs
	dict           *rigs;

	// A list of entities with AIs
	dict           *ais;

	// A bounding volume hierarchy tree containing entities with colliders
	GXBV_t         *bvh;

	// The camera to be used while drawing the scene
	GXCamera_t     *active_camera;
	GXEntity_t     *active_entity;
};

// Allocators

/* !
 *  Allocate memory for a scene
 *
 * @param pp_scene : return
 *
 * @sa destroy_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            create_scene                ( GXScene_t  **pp_scene );

// Constructors

/* !
 *  Load a scene from a JSON file
 *
 * @param pp_scene : return
 * @param path     : The path to a JSON file containing a scene object
 *
 * @sa load_scene_as_json
 * @sa create_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            load_scene                  ( GXScene_t  **pp_scene, const char   path[] );

/* !
 *  Load a scene from JSON text
 *
 * @param pp_scene   : return
 * @param token_text : The scene JSON object text
 * @param len        : The length of the scene JSON object text
 *
 * @sa load_scene_as_json
 * @sa create_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            load_scene_as_json          ( GXScene_t  **pp_scene, char*        token_text, size_t len );

// Appenders

/* !
 *  Append an entity to a scene
 *
 * @param p_scene  : The scene
 * @param p_entity : The entity to append
 *
 * @sa append_camera
 * @sa append_light
 * @sa append_collision
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            append_entity               ( GXScene_t   *scene, GXEntity_t    *entity );

/* !
 *  Append a camera to a scene
 *
 * @param p_scene  : The scene
 * @param p_camera : The camera to append
 *
 * @sa append_entity
 * @sa append_light
 * @sa append_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            append_camera               ( GXScene_t   *scene, GXCamera_t    *camera );

/* !
 *  Append a light to a scene
 *
 * @param p_scene : The scene
 * @param p_light : The light to append
 *
 * @sa append_entity
 * @sa append_camera
 * @sa append_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            append_light                ( GXScene_t   *scene, GXLight_t     *light );

/* !
 *  Append a collision to a scene
 *
 * @param p_scene     : The scene
 * @param p_collision : The collision to append
 *
 * @sa append_entity
 * @sa append_camera
 * @sa append_light
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            append_collision            ( GXScene_t   *scene, GXCollision_t *collision );

// Drawing 
DLLEXPORT int            draw_scene                  ( GXScene_t   *scene );
DLLEXPORT int            draw_lights                 ( GXScene_t   *scene, GXPart_t      *light_part, GXShader_t *shader );

// Info

/* !
 *  Print info about a scene
 *
 * @param p_scene  : pointer to scene to log
 *
 * @sa load_scene
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int            scene_info                  ( GXScene_t   *p_scene );

// Getters
DLLEXPORT GXEntity_t    *get_entity                  ( GXScene_t   *scene, const char     name[] );
DLLEXPORT GXCamera_t    *get_camera                  ( GXScene_t   *scene, const char     name[] );
DLLEXPORT GXLight_t     *get_light                   ( GXScene_t   *scene, const char     name[] );

// Setters
DLLEXPORT int            set_active_camera           ( GXScene_t   *scene, const char    name[] );

// Instance
DLLEXPORT int            load_entity_from_queue      ( GXInstance_t *instance ); 
DLLEXPORT int            load_light_probe_from_queue ( GXInstance_t *instance );

// Removers
DLLEXPORT GXEntity_t    *remove_entity               ( GXScene_t   *scene, const char     name[] );
DLLEXPORT GXCamera_t    *remove_camera               ( GXScene_t   *scene, const char     name[] );
DLLEXPORT GXLight_t     *remove_light                ( GXScene_t   *scene, const char     name[] );
DLLEXPORT GXCollision_t *remove_collision            ( GXScene_t   *scene, GXCollision_t *collision );

// Destructors
DLLEXPORT int            destroy_scene               ( GXScene_t   *scene );