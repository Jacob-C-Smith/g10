#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/G10.h>
#include <G10/GXCollider.h>
#include <G10/GXCollision.h>
#include <G10/GXEntity.h>

/* !
 *  Detect and update collisions in the instances active scene
 *
 * @param instance         : Pointer to instance
 *
 * @sa move_objects
 * @sa update_forces
 * @sa update_rigs
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int detect_collisions ( GXInstance_t *instance );

/* !
 *  Updates location and rotation derivatives for each object in the instances active scene.
 * 
 * Updates linear and angular momentum, acceleration, velocity, and the base unit (location / rotation)
 * 
 * @param instance         : Pointer to instance
 *
 * @sa detect_collisions
 * @sa update_forces
 * @sa update_rigs
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int move_objects      ( GXInstance_t *instance );

/* !
 *  Calculates forces and torque for each entity in the instances active scene. 
 * 
 * @param instance         : Pointer to instance
 *
 * @sa detect_collisions
 * @sa move_objects
 * @sa update_rigs
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_forces     ( GXInstance_t *instance );

/* !
 *  Updates each rig in the instances active scene
 *
 * @param instance         : Pointer to instance
 *
 * @sa detect_collisions
 * @sa move_objects
 * @sa update_forces
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_rigs       ( GXInstance_t *instance );