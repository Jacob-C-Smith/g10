/** !
 * Header file for camera controller
 *
 * @file camera_controller.h
 * 
 * @author Jacob Smith
 */

// include guard
#pragma once

// standard library
#include <math.h>
#include <float.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <input.h>

// function definitions
// camera controllers
/** !
 * Update the camera 
 * 
 * @param p_camera the camera
 * 
 * @return 1 on success, 0 on error
 */
int camera_controller_first_person_update ( camera *p_camera );
