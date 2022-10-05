#pragma once

#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

// This camera controller is based off the movement of a certain first person shooter game... Shhhhhh ;^)
struct GXCameraController_s
{
	// Camera object
	GXCamera_t *camera;

    float v_ang,
          h_ang,
          spdlim;

	// Displacement derivatives
    vec2        orientation;
	vec3		velocity,
				acceleration;
    
};

GXCameraController_t *camera_controller_from_camera ( GXInstance_t         *instance , GXCamera_t   *camera );
int                   update_controlee_camera       ( GXCameraController_t *camera_controller, float delta_time );