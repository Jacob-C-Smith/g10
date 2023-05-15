#include <G10/GXCollision.h>

int create_collision ( GXCollision_t **pp_collision )
{

    // Argument check
    {
        if ( pp_collision == (void *) 0 ) goto no_collision;
    }

    // Initialized data
    GXCollision_t * p_collision = calloc(1, sizeof(GXCollision_t));

    // Error check
    if ( pp_collision == (void *) 0 )
        goto no_mem;

    // Write the return value
    *pp_collision = p_collision;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_collision:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collision] Null pointer provided for parameter \"pp_collision\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
    }
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
