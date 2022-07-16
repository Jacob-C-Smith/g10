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

	// A bounding volume heierarchy tree containing entities
	GXBV_t     *bvh;

	// The camera to be used while drawing the scene
	GXCamera_t *active_camera;
};

// Allocators
int            create_scene         ( GXScene_t  **scene );

// Constructors
int            load_scene           ( GXScene_t  **scene, const char   path[] );
int            load_scene_as_json   ( GXScene_t  **scene, char*        token_text );
int            load_scene_as_json_n ( GXScene_t  **scene, char*        token_text, size_t len );
int            construct_scene      ( GXScene_t  **scene, char*        name      , char **entities, char  **cameras, char **lights, char *skybox );

// Appenders
int            append_entity        ( GXScene_t   *scene, GXEntity_t    *entity );
int            append_camera        ( GXScene_t   *scene, GXCamera_t    *camera );
int            append_light         ( GXScene_t   *scene, GXLight_t     *light );
int            append_collision     ( GXScene_t   *scene, GXCollision_t *collision );

// Drawing 
int            draw_scene           ( GXScene_t* scene, GXShader_t *shader);
int            draw_lights          ( GXScene_t   *scene, GXPart_t      *light_part, GXShader_t *shader );

// Physics 
int            compute_physics      ( GXScene_t   *scene, float          delta_time );

// Getters
GXEntity_t    *get_entity           ( GXScene_t   *scene, const char     name[] );
GXCamera_t    *get_camera           ( GXScene_t   *scene, const char     name[] );
GXLight_t     *get_light            ( GXScene_t   *scene, const char     name[] );

// Setters
int            set_active_camera    ( GXScene_t   *scene, const char    name[] );

// Removers
GXEntity_t    *remove_entity        ( GXScene_t   *scene, const char     name[] );
GXCamera_t    *remove_camera        ( GXScene_t   *scene, const char     name[] );
GXLight_t     *remove_light         ( GXScene_t   *scene, const char     name[] );
GXCollision_t *remove_collision     ( GXScene_t   *scene, GXCollision_t *collision );

// Destructors
int            destroy_scene        ( GXScene_t   *scene );