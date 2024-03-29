/** !
 * @file G10/GXRigidbody.h
 * @author Jacob Smith
 *
 * Rigidbody
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

typedef enum forces_flag_bits 
{
	force_gravity                   = 0b000001,
	force_applied                   = 0b000010,
	force_normal                    = 0b000100,
	force_friction                  = 0b001000,
	force_tension                   = 0b010000,
	force_spring                    = 0b100000,
	force_collision                 = 0b001111,
	force_collision_no_grav         = force_collision ^ force_gravity,
	force_collision_tension         = force_collision | force_tension,
	force_collision_tension_no_grav = force_collision_tension ^ force_gravity
} forces_flag_bits;

struct GXRigidbody_s
{

	forces_flag_bits  flags;                  // Used to quickly compute forces

	bool              active;                 // Apply displacement and rotational forces?

	float             mass;                   // ( kg )
	vec3              radius;                 // ( m )

	vec3              acceleration;           // ( m / s^2 )
	vec3              velocity;               // ( m / s )
	vec3              momentum;               // ( kg * m / s )

	quaternion        angular_acceleration;   // ( rad / s^2 )
	quaternion        angular_velocity;       // ( rad / s )
	quaternion        angular_momentum;       // ( kg * m^2 / s )

	vec3             *forces;                 // ( kg * m / s^2 )
	size_t            force_count;

	quaternion       *torques;                // ( kg * m^2 / s^2 )
	size_t            torque_count;			  
};


// Allocators
/** !
 *  Allocate memory for a rigidbody
 *
 * @param pp_rigidbody : return
 *
 * @sa destroy_rigidbody
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_rigidbody ( GXRigidbody_t **pp_rigidbody );

// Constructors
/** !
 *  Load a rigidbody from a JSON file
 *
 * @param pp_rigidbody : return
 * @param path         : The path to a JSON file containing a rigidbody object
 *
 * @sa load_rigidbody_as_json_text
 * @sa create_rigidbody
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_rigidbody ( GXRigidbody_t **pp_rigidbody, const char *path );

/** !
 *  Load a rigidbody from JSON text
 *
 * @param pp_rigidbody : return
 * @param text         : The JSON text
 *
 * @sa load_rigidbody
 * @sa create_rigidbody
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_rigidbody_as_json_text ( GXRigidbody_t **pp_rigidbody, char *text );

// Destructors
/** !
 *  Destroy a rigidbody
 *
 * @param pp_rigidbody : Pointer to rigidbody pointer
 *
 * @sa create_rigidbody
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_rigidbody ( GXRigidbody_t **pp_rigidbody );