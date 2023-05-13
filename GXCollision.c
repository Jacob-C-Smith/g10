#include <G10/GXCollision.h>

int create_collision ( GXCollision_t **pp_collision )
{

	// TODO: Argument check

	// Initialized data
	GXCollision_t* p_collision = calloc(1, sizeof(GXCollision_t));

	// TODO: Error check

	// Write the return value
	*pp_collision = p_collision;

	// Success
	return 1;

	// TODO: Error handling
}

int construct_collision_from_entities ( GXCollision_t **pp_collision, GXEntity_t* a, GXEntity_t* b )
{

	// TODO: Argument check
	
	// Initialized data
	GXCollision_t *p_collision = 0;

	create_collision(pp_collision);

	p_collision = *pp_collision;

	p_collision->a = a;
	p_collision->b = b;

	p_collision->begin_tick = p_instance->time.ticks;
	p_collision->aabb_colliding = true;

	// Success
	return 1;
}
