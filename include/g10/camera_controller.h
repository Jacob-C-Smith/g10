/** !
 * Header file for camera controller
 *
 * @file g10/camera_controller.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <math.h>
#include <float.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

// Function definitions
// Camera controllers
/** !
 * Update the camera 
 * 
 * @param p_camera the camera
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int camera_controller_orbit_update ( camera *p_camera );
