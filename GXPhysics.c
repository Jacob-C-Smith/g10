#include <G10/GXPhysics.h>

void init_physics ( void )
{

    // Initialized data
    GXInstance_t* p_instance = g_get_active_instance();

    p_instance->mutexes.move_object       = SDL_CreateMutex();
    p_instance->mutexes.update_force      = SDL_CreateMutex();
    p_instance->mutexes.resolve_collision = SDL_CreateMutex();
    // TODO: Add rig mutex

    return;
}

int detect_collisions ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Initialized data
    GXEntity_t *entity = 0;

    /*
    // Get an entity to test collisions
    {

        // Lock the mutex
        SDL_LockMutex(p_instance->mutexes.resolve_collision);

        // Is there anything left to move?
        if (queue_empty(p_instance->queues.actor_collision))
        {

            // If not, unlock and return
            SDL_UnlockMutex(p_instance->mutexes.resolve_collision);
            return 1;
        }

        queue_dequeue(p_instance->queues.actor_collision, &entity);

        SDL_UnlockMutex(p_instance->mutexes.resolve_collision);
    }*/

    // Successs
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Physics] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int move_objects ( GXInstance_t* p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Initialized data
    GXEntity_t *entity = 0;

    /*
    // Get an entity to move
    {
        // Lock the mutex
        SDL_LockMutex(p_instance->mutexes.move_object);

        // Is there anything left to move?
        if (queue_empty(p_instance->queues.actor_move))
        {

            // If not, unlock and return
            SDL_UnlockMutex(p_instance->mutexes.move_object);
            return 1;
        }

        entity = queue_dequeue(p_instance->queues.actor_move);

        SDL_UnlockMutex(p_instance->mutexes.move_object);
    }

    // Move the entity
    if ( entity )
        move_entity(entity);
    */

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Physics] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int update_forces ( GXInstance_t *p_instance )
{

    // Argument check
    #ifndef NDEBUG
        if ( p_instance == (void *) 0 ) goto no_instance;
    #endif

    // Initialized data
    GXEntity_t* entity = 0;

    /*
    // Get an entity to compute forces for
    {
        // Lock the mutex
        SDL_LockMutex(p_instance->mutexes.update_force);

        // Is there anything left to move?
        if (queue_empty(p_instance->queues.actor_force))
        {

            // If not, unlock and return
            SDL_UnlockMutex(p_instance->mutexes.update_force);
            return 1;
        }

        queue_dequeue(p_instance->queues.actor_force, &entity);

        SDL_UnlockMutex(p_instance->mutexes.update_force);
    }

    // Move the entity
    if ( entity )
        calculate_entity_force(entity);
    */

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Physics] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int update_rigs ( GXInstance_t *p_instance )
{

    return 1;
}
