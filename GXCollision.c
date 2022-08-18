#include <G10/GXCollision.h>

int create_collision(GXCollision_t **pp_collision)
{
	GXCollision_t* p_collision = calloc(1, sizeof(GXCollision_t));

	*pp_collision = p_collision;

	return 0;
}

int construct_collision_from_entities(GXCollision_t **pp_collision, GXEntity_t* a, GXEntity_t* b)
{
	GXCollision_t* p_collision = 0;

	create_collision(pp_collision);

	p_collision = *pp_collision;

	p_collision->a = a;
	p_collision->b = b;

	return 0;
}
