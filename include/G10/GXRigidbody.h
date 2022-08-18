#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

struct GXRigidbody_s
{
	bool        active;                 // Apply displacement and rotational forces?

	float       mass;                   // ( kg )
	vec3        radius;                 // ( m )

	vec3        acceleration;           // ( m / s^2 )
	vec3        velocity;               // ( m / s )
	vec3        momentum;               // ( kg * m / s )

	quaternion  angular_acceleration;   // ( rad / s^2 )
	quaternion  angular_velocity;       // ( rad / s )
	quaternion  angular_momentum;       // ( kg * m^2 / s )

	vec3       *forces;                 // ( kg * m / s^2 )
	size_t      force_count;

	quaternion *torques;                // ( kg * m^2 / s^2 )
	size_t      torque_count;
};

DLLEXPORT int create_rigidbody       ( GXRigidbody_t **rigidbody );

DLLEXPORT int load_rigidbody         ( GXRigidbody_t **rigidbody, const char *path );
DLLEXPORT int load_rigidbody_as_json ( GXRigidbody_t **rigidbody, char *token_text, size_t token_text_len );

DLLEXPORT int destroy_rigidbody      ( GXRigidbody_t *rigidbody );