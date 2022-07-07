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
};

int create_camera       ( GXCamera_t** camera);

int construct_camera    ( GXCamera_t** camera, vec3 location, vec3 front, vec3 up, float near_clip, float far_clip, float aspect_ratio, float fov);

int load_camera         ( GXCamera_t** camera, const char* path);

int load_camera_as_json ( GXCamera_t** camera, char* object_text, size_t len);



int destroy_camera      ( GXCamera_t* camera);