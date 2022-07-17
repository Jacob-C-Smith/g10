#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

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

int create_rigidbody(GXRigidbody_t **rigidbody)
{
	GXRigidbody_t *i_rigidbody = calloc(1, sizeof(GXRigidbody_t));

	*rigidbody = i_rigidbody;

	return 1;
}

int update_force(GXRigidbody_t* rigidbody)
{
	rigidbody->forces[0] = (vec3) {0.f, 0.f, 0.f, 0.f};

	// Newtons first law; summate forces
	for (size_t i = 1; i < rigidbody->force_count; i++)
		add_vec3(&rigidbody->forces[0], rigidbody->forces[0], rigidbody->forces[i]);

}