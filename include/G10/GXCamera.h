#pragma once
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

struct GXCamera_s {

    // Name
	char *name;

    // View
    vec3  location,
          target,
          up;

    // Projection
    float fov,
          near_clip,
          far_clip,
          aspect_ratio;

    // Matricies
    mat4  view,
          projection;

    void *uniform_data;
};

// Allocators
DLLEXPORT int  create_camera       ( GXCamera_t** camera);

// Constructors
DLLEXPORT int  construct_camera    ( GXCamera_t** camera, vec3 location, vec3 front, vec3 up, float near_clip, float far_clip, float aspect_ratio, float fov);
DLLEXPORT int  load_camera         ( GXCamera_t** camera, const char* path);
DLLEXPORT int  load_camera_as_json ( GXCamera_t** camera, char* object_text, size_t len);

DLLEXPORT mat4 look_at             ( vec3        eye,    vec3        target       , vec3  up );
DLLEXPORT mat4 perspective_matrix  ( float       fov,    float       aspect       , float near_clip , float far_clip);

DLLEXPORT int  get_camera_position ( void       *ret );

// Deallocators
DLLEXPORT int  destroy_camera      ( GXCamera_t* camera);