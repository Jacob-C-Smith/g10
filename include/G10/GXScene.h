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
	char       *name;

	// Scene data
	dict       *entities;
	dict       *cameras;
	dict       *lights;
	dict       *collisions;

	// A list of entities with rigidbodies
	dict       *actors;

	// A list of entities with rigs
	dict       *rigs;

	// A list of entities with AIs
	dict       *ais;

	// A bounding volume heierarchy tree containing entities with colliders
	GXBV_t     *bvh;

	// The camera to be used while drawing the scene
	GXCamera_t *active_camera;
};

// Allocators
DLLEXPORT int            create_scene                ( GXScene_t  **scene );

// Constructors
DLLEXPORT int            load_scene                  ( GXScene_t  **scene, const char   path[] );
DLLEXPORT int            load_scene_as_json          ( GXScene_t  **scene, char*        token_text, size_t len );
DLLEXPORT int            construct_scene             ( GXScene_t  **scene, char*        name      , char **entities, char  **cameras, char **lights, char *skybox );

// Appenders
DLLEXPORT int            append_entity               ( GXScene_t   *scene, GXEntity_t    *entity );
DLLEXPORT int            append_camera               ( GXScene_t   *scene, GXCamera_t    *camera );
DLLEXPORT int            append_light                ( GXScene_t   *scene, GXLight_t     *light );
DLLEXPORT int            append_collision            ( GXScene_t   *scene, GXCollision_t *collision );

// Drawing 
DLLEXPORT int            draw_scene                  ( GXScene_t   *scene );
DLLEXPORT int            draw_lights                 ( GXScene_t   *scene, GXPart_t      *light_part, GXShader_t *shader );

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