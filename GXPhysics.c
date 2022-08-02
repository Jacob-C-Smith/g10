#include <G10/GXPhysics.h>

int move_entities(GXInstance_t* instance)
{
	size_t       entity_count = dict_values(instance->actor_queue, 0);
	float        delta_time   = instance->delta_time;
	GXEntity_t **entities     = calloc(entity_count+1, sizeof(void *));

	dict_values(instance->actor_queue, entities);
	
	for (size_t i = 0; i < entity_count; i++)
	{
		GXEntity_t    *iter = entities[i];
		GXRigidbody_t *r    = iter->rigidbody;
		GXTransform_t *t    = iter->transform;

		printf("%s\n", iter->name);

	}

	return 0;
}