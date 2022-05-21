#pragma once

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXEntity.h>

struct GXScene_s
{
	char       *name;

	dict       *entities;
	dict       *cameras;
	dict       *lights;
	dict       *collisions;

	GXBV_t     *bvh;

	GXCamera_t *active_camera;


};

int            create_scene         ( GXScene_t  **scene );

int            load_scene           ( GXScene_t  **scene, const char   path[] );
int            load_scene_as_json   ( GXScene_t  **scene, char*        token_text );
int            load_scene_as_json_n ( GXScene_t  **scene, char*        token_text, size_t len );
int            construct_scene      ( GXScene_t  **scene, char*        name      , char **entities, char  **cameras, char **lights, char *skybox );

int            append_entity        ( GXScene_t   *scene, GXEntity_t    *entity );
int            append_camera        ( GXScene_t   *scene, GXCamera_t    *camera );
int            append_light         ( GXScene_t   *scene, GXLight_t     *light );
int            append_collision     ( GXScene_t   *scene, GXCollision_t *collision );

int            draw_scene           ( GXScene_t   *scene );
int            draw_lights          ( GXScene_t   *scene, GXPart_t      *light_part, GXShader_t *shader );

int            compute_physics      ( GXScene_t   *scene, float          delta_time );

GXEntity_t    *get_entity           ( GXScene_t   *scene, const char     name[] );
GXCamera_t    *get_camera           ( GXScene_t   *scene, const char     name[] );
GXLight_t     *get_light            ( GXScene_t   *scene, const char     name[] );

int            set_active_camera    ( GXScene_t   *scene, const char    name[] );

GXEntity_t    *remove_entity        ( GXScene_t   *scene, const char     name[] );
GXCamera_t    *remove_camera        ( GXScene_t   *scene, const char     name[] );
GXLight_t     *remove_light         ( GXScene_t   *scene, const char     name[] );

GXCollision_t *remove_collision     ( GXScene_t   *scene, GXCollision_t *collision );

int            destroy_scene        ( GXScene_t   *scene );