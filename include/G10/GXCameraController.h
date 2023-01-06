#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

// This camera controller is based off the movement of a certain first person shooter game.
struct GXCameraController_s
{
    // Camera object
    GXCamera_t *camera;

    float v_ang,
          h_ang,
          spdlim;

    // Displacement derivatives
    vec2  orientation;
    vec3  velocity,
          acceleration;
    
};

/* !
 *  Assign the instance's camera controller to a particular camera
 * 
 *  @param instance : The instance
 *  @param camera   : The camera to control
 * 
 *  @sa update_controlee_camera
 * 
 *  @return 1 on success, 0 on error
 */
DLLEXPORT int camera_controller_from_camera ( GXInstance_t *instance , GXCamera_t   *camera );

/* !
 *  This is called once per frame to update the controlee camera
 *
 *  @param instance : The instance
 *  @param camera   : The camera to control
 *
 *  @sa update_controlee_camera
 *
 *  @return 1 on success, 0 on error
 */
DLLEXPORT int update_controlee_camera       ( void );