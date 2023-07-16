#include <G10/GXCollision.h>

int create_collision ( GXCollision_t **pp_collision )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_collision == (void *) 0 ) goto no_collision;
    #endif

    // Initialized data
    GXCollision_t *p_collision = calloc(1, sizeof(GXCollision_t));

    // Error check
    if ( pp_collision == (void *) 0 ) goto no_mem;

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

int construct_collision_from_entities ( GXCollision_t **pp_collision, GXEntity_t *a, GXEntity_t *b )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_collision == (void *) 0 ) goto no_collision;
        if ( a            == (void *) 0 ) goto no_a;
        if ( b            == (void *) 0 ) goto no_b;
    #endif

    // Initialized data
    GXInstance_t  *p_instance  = g_get_active_instance();
    GXCollision_t *p_collision = 0;

    // Allocate memory for a collision
    if ( create_collision(pp_collision) == 0 ) goto failed_to_allocate_collision;

    // Return a pointer to the caller
    p_collision = *pp_collision;

    // Set entity A
    p_collision->a = a;

    // Set entity B
    p_collision->b = b;

    // Set the begin tick
    p_collision->begin_tick = p_instance->time.ticks;

    // Set the colliding flag
    p_collision->aabb_colliding = true;

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

            no_a:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collision] Null pointer provided for parameter \"a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_b:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collision] Null pointer provided for parameter \"b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_allocate_collision:
                #ifndef NDEBUG
                    g_print_error("[G10] [Collision] Failed to allocate collision in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
