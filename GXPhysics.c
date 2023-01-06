#include <G10/GXPhysics.h>

void init_physics(void)
{

    // Initialized data
    GXInstance_t* instance = g_get_active_instance();

    instance->mutexes.move_object       = SDL_CreateMutex();
    instance->mutexes.update_force      = SDL_CreateMutex();
    instance->mutexes.resolve_collision = SDL_CreateMutex();
    // TODO: Add rig mutex

    return;
}


int detect_collisions  ( GXInstance_t *instance ) 
{

    // Argument check
    {
        #ifndef NDEBUG
            if (instance == (void *) 0)
                goto no_instance;
        #endif
    }

    // Initialized data
    GXEntity_t *entity = 0;

    // Get an entity to test collisions
    {

        // Lock the mutex 
        SDL_LockMutex(instance->mutexes.resolve_collision);

        // Is there anything left to move?
        if (queue_empty(instance->queues.actor_collision))
        {

            // If not, unlock and return
            SDL_UnlockMutex(instance->mutexes.resolve_collision);
            return 1;
        }

        entity = queue_dequeue(instance->queues.actor_collision);

        SDL_UnlockMutex(instance->mutexes.resolve_collision);
    }

    // Test for collisions
    if(entity) 
    {

        // Initialized data
        queue* possible_entity_collisions = 0;

        // Entity suitability checks
        {
            
            // Is there a rigidbody?
            if (entity->rigidbody == (void*)0)
                return 0;

            // Is the rigidbody active or passive?
            if (entity->rigidbody->active == false)
                return 0;

            // Is there a collider
            if (entity->collider == (void *)0)
                return 0;
        }

        // Update active collisions
        {
            
            // Initialized data
            dict   *collisions_dict        = entity->collider->collisions;
            size_t  active_collision_count = dict_values(collisions_dict, 0);

            // Is the entity colliding with anything right now?
            if (active_collision_count) 
            {

                // Initialized data
                GXCollision_t **collisions = calloc(active_collision_count, sizeof(void *));
                
                // Populate the collisions list
                dict_values(collisions_dict, collisions);

                // Iterate over each active collision
                for (size_t i = 0; i < active_collision_count; i++)
                {

                    // Initialized data
                    GXCollision_t *collision = collisions[i];
                    GXEntity_t    *a         = collision->a,
                                  *b         = collision->b;

                    // Are the objects still touching each other?
                    if (aabb_intersect(a->collider->bv, b->collider->bv)) continue;
                    
                    // Set the end tick 
                    collision->end_tick = instance->time.ticks;

                    // Flip the AABB collision flag
                    collision->aabb_colliding = false;
                    
                    // Fire the end aabb collision callbacks
                    for (size_t i = 0; i < a->collider->aabb_callback_count; i++)
                    {

                        // Initialized data
                        int (*callback) (GXCollision_t*) = a->collider->aabb_end_callbacks[i];

                        // Call the callback function
                        //callback(collision);
                    }

                    dict_item trash = { 0 };

                    SDL_LockMutex(instance->mutexes.resolve_collision);

                    if (dict_get(collisions_dict, b->name) == 0)
                    {
                        SDL_UnlockMutex(instance->mutexes.resolve_collision);
                        goto exit;
                    }

                    // Remove the entity
                    dict_pop(collisions_dict, collision->b->name);

                    SDL_UnlockMutex(instance->mutexes.resolve_collision);

                    continue;
                }

                // Free the list
                free(collisions);

            }
        }

        // Test for collisions
        {

            // Initialized data
            GXCollider_t  *collider                = entity->collider;
            
            size_t         entity_collision_count  = 0;

            // Find possible collisions using the active scene's bounding volume hierarchy
            {
            
            
                find_neighbors:
                // Initialized data
                GXBV_t     *bv            = collider->bv,
                           *parent        = find_parent_bv(instance->context.scene->bvh, bv);
                GXEntity_t *entity        = collider->bv->entity;
                vec3        velocity      = entity->rigidbody->velocity;
                float       search_radius = length(velocity);
                float       distance      = 0.f;

                if (parent) {

                    // Expand the search radius
                    while (parent)
                    {

                        // Have we exceeded the search radius?
                        if (bv_distance((bv == parent->left) ? parent->right : parent->left, bv) > search_radius)
                            break;

                        // Expand the search radius
                        parent = find_parent_bv(instance->context.scene->bvh, bv);

                    }

                    // Allocate a queue for collisions
                    queue_construct(&possible_entity_collisions, 4);

                    // Count entities from the parent bv
                    get_entities_from_bv(parent, possible_entity_collisions);

                }
                else
                {


                    SDL_LockMutex(instance->mutexes.resolve_collision);

                    // Reinsert the entity
                    GXBV_t* epbv = find_parent_bv(instance->context.scene->bvh, entity->collider->bv);
    
                    if (epbv == 0)
                    {
                        printf("ERROR\n"); 
                        bv_info(instance->context.scene->bvh, 0);
                        while (1);
                    }
                    if (epbv->left == entity->collider->bv)
                        epbv->left = 0;
                    else if (epbv->right == entity->collider->bv)
                        epbv->right = 0;
                    
                    insert_bv(instance->context.scene->bvh, bv);

                    SDL_UnlockMutex(instance->mutexes.resolve_collision);

                    goto find_neighbors;
                }
            }

            // Test for AABB collisions
            {
                while (queue_empty(possible_entity_collisions)==false)
                {

                    // Initialized data
                    GXEntity_t *e = queue_dequeue(possible_entity_collisions);
                    
                    // Skip if null pointer
                    if ( e == (void *)0 )                                continue;
                    
                    // Skip if it's the entity
                    if ( e == entity    )                                continue;
                    
                    // Skip if the possible entity
                    if (dict_get(entity->collider->collisions, e->name)) continue;

                    // Test for AABB collisions
                    if (aabb_intersect(entity->collider->bv, e->collider->bv))
                    {

                        SDL_LockMutex(instance->mutexes.resolve_collision);

                        // Thread safe
                        {
                            // Initialized data
                            GXCollision_t *collision = 0;

                            if (dict_get(entity->collider->collisions, e->name))
                            {
                                SDL_UnlockMutex(instance->mutexes.resolve_collision);
                                goto exit;
                            }

                            // Construct a collision struct
                            construct_collision_from_entities(&collision, entity, e);
 
                            // Add the collision to the colliding objects
                            dict_add(entity->collider->collisions, e->name, collision);
                        }

                        SDL_UnlockMutex(instance->mutexes.resolve_collision);
                    }

                }
            }

            // Test for OBB collisions
            {
                // Test each queue member for a collision

                // If there is no collision, pop the queue member. Else, leave the member alone
            }

            // Test for convex hull collisions
            {
                // Test each queue member for a collision

                // If there is no collision, pop the queue member. Else, leave the member alone
            }

        }

        queue_destroy(possible_entity_collisions);

    }

exit:;
    
    return 1;

    // Error handling
    {

        // Argument errors
        {   
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Physics] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }
}

int move_objects       ( GXInstance_t* instance )
{

    // Argument check
    {
        #ifndef NDEBUG
                if (instance == (void *) 0)
                goto no_instance;
        #endif
    }

    // Initialized data
    GXEntity_t *entity = 0;

    // Get an entity to move
    {
        // Lock the mutex 
        SDL_LockMutex(instance->mutexes.move_object);

        // Is there anything left to move?
        if (queue_empty(instance->queues.actor_move))
        {

            // If not, unlock and return
            SDL_UnlockMutex(instance->mutexes.move_object);
            return 1;
        }

        entity = queue_dequeue(instance->queues.actor_move);

        SDL_UnlockMutex(instance->mutexes.move_object);
    }

    // Move the entity
    if(entity)
        move_entity(entity);

    return 1;

    // Error handling
    {

        // Argument errors
        {   
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Physics] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }
}

int update_forces      ( GXInstance_t *instance ) 
{

    // Argument check
    {
        #ifndef NDEBUG
            if (instance == (void *) 0)
                goto no_instance;
        #endif
    }

    // Initialized data
    GXEntity_t* entity = 0;

    // Get an entity to compute forces for
    {
        // Lock the mutex 
        SDL_LockMutex(instance->mutexes.update_force);

        // Is there anything left to move?
        if (queue_empty(instance->queues.actor_force))
        {

            // If not, unlock and return
            SDL_UnlockMutex(instance->mutexes.update_force);
            return 1;
        }

        entity = queue_dequeue(instance->queues.actor_force);

        SDL_UnlockMutex(instance->mutexes.update_force);
    }

    // Move the entity
    if(entity)
        calculate_entity_force(entity);

    return 1;

    // Error handling
    {

        // Argument errors
        {   
            no_instance:
                #ifndef NDEBUG
                    g_print_error("[G10] [Physics] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }
}

int update_rigs        ( GXInstance_t *instance )
{

    return 1;
}
