#include <G10/GXEntity.h>

int create_entity(GXEntity_t** entity)
{
	// TODO: Argument check
	GXEntity_t *ret = calloc(1,sizeof(GXEntity_t));
	
	// TODO: Memory check
	*entity = ret;

	return 1;
	
	// TODO: Error handling
	//			- Argument errors
	//          - Standard library errors
}

int load_entity(GXEntity_t** entity, char* path)
{
	// TODO: Argument check
	// TODO: Memory check

	return 0;
	
	// TODO: Error handling
	//			- Argument errors
	//          - Standard library errors
}

int load_entity_as_json(GXEntity_t** entity, char* token_text)
{
	// TODO: Argument check
	// TODO: Memory check

	return 0;

	// TODO: Error handling
	//			- Argument errors
	//          - Standard library errors

}

int move_entity(GXEntity_t* entity, float delta_time)
{
	GXRigidbody_t* rigidbody = entity->rigidbody;
	GXTransform_t* transform = entity->transform;

	// force = ( kg * m / s^2 )
	vec3 force = rigidbody->forces[0]; 

	// Calculate acceleration
	// acceleration = ( m / s^2 )
	div_vec3_f(&rigidbody->acceleration, force, rigidbody->mass);
	
	// Calculate velocity
	// velocity = ( m / s )
	rigidbody->velocity = mul_vec3_f(rigidbody->acceleration, delta_time);

	// Calculate location
	// location = ( m )
	transform->location = mul_vec3_f(rigidbody->velocity, rigidbody->mass);

	return 1;
}

// TODO: Argument check
// TODO: Memory check
// TODO: Error handling
//			- Argument errors
//          - Standard library errors