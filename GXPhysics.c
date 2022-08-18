#include <G10/GXPhysics.h>

int detect_collisions  ( GXInstance_t *instance ) 
{
    // Initialized data
    GXEntity_t *entity = 0;
    bool collision = false;

    // Get an entity to test collisions
    {

        // Lock the mutex 
        SDL_LockMutex(instance->resolve_collision_mutex);

        // Is there anything left to move?
        if (queue_empty(instance->actor_collision_queue))
        {

            // If not, unlock and return
            SDL_UnlockMutex(instance->resolve_collision_mutex);
            return 1;
        }

        entity = queue_dequeue(instance->actor_collision_queue);

        SDL_UnlockMutex(instance->resolve_collision_mutex);
    }

    // Test for collisions
    if(entity) 
    {

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

        {

            // Initialized data
            GXCollider_t  *collider                = entity->collider;
            queue         *entity_collisions       = 0;
            size_t         entity_collision_count  = 0;

            // Test for BV collisions
            {

                // Initialized data
                GXBV_t     *bv            = collider->bv,
                           *parent        = find_parent_bv(instance->active_scene->bvh, bv);
                GXEntity_t *entity        = collider->bv->entity;
                vec3        velocity      = entity->rigidbody->velocity;
                float       search_radius = max(max(velocity.x, velocity.y), velocity.z) * instance->delta_time;

                // Count potential collisions
                float distance = 0.f;
                
                
                {

                    // Expand the search radius
                    while (parent)
                    {

                        // Have we exceeded the search radius?
                        if (bv_distance((bv==parent->left) ? parent->right : parent->left, bv) > search_radius)
                            break;

                        // Expand the search radius
                        parent = find_parent_bv(instance->active_scene->bvh, bv);

                    }

                    // Count entities from the parent bv
                    
                    // Allocate a queue

                    // Populate a queue

                    // Test each queue member for a collision

                        // If there is no collision, pop the queue member. Else, leave the member alone

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

            // Construct collisions
            {

                for (size_t i = 0; i < entity_collision_count; i++)
                {
                    GXCollision_t *collision = 0;
                    GXEntity_t    *test      = entity_collisions->contents[i];

                    construct_collision_from_entities(&collision, entity, entity_collisions->contents[i]);

                    
                }
            }
        }

        // Update collisions
        {
            
        }

        {
            //GXEntity_t* p_entity = 0, *other_entity = 0;

            //// Initilazed data
            //vec3  e_momentum = p_entity->rigidbody->momentum,     // ( kg * ( m / s ) )
            //      e_velocity = p_entity->rigidbody->velocity,     // ( m / s )
            //      o_momentum = other_entity->rigidbody->momentum, // ( kg * ( m / s ) )
            //      o_velocity = other_entity->rigidbody->velocity; // ( m / s )
            //vec3  energy_vec = { 0.f, 0.f, 0.f };
            //float energy     = 0.f;


            //// 1. 
            //// M1 * V1 + M2 * V2 = M1 * V1' + M2 * V2'
            //add_vec3(&energy_vec, e_momentum, o_momentum);
            //energy = length(energy_vec);



            //// 2. 
            //// V1 + V1' = V2 + V2'
            //// V1 - V2 = -V1' + V2'
            //vec3  new_e     = { 0.f, 0.f, 0.f },
            //      new_o     = { 0.f, 0.f, 0.f },
            //      e_m_o     = { 0.f, 0.f, 0.f };

            //float len_e_m_o = 0.f;

            //sub_vec3(&e_m_o, e_velocity, o_velocity);

            //len_e_m_o = length(e_m_o);
                
            // energy    = M1 * V1' + M2 * V2'
            // len_e_m_o =     -V1' +      V2'

            // M1 * ( -V1' + V2' = len_emo )

            // energy         = M1 *  V1' + M2 * V2'
            // M1 * len_e_m_o = M1 * -V1' + M2 * V2'

            // energy         =     M1 * V1'   + M2 * V2'
            // M1 * len_e_m_o = - ( M1 * V1' ) + M2 * V2'

            // V2' = ( energy + len_e_m_o ) / ( M1 + M2 )

            // len_e_m_o =      -V1' +      V2'

            // V1' = -len_emo + V2'

        }

        // TODO: Calculate applied torque
    }

    return 0;
}

int move_objects       ( GXInstance_t* instance )
{

    // Initialized data
    GXEntity_t *entity = 0;

    // Get an entity to move
    {
        // Lock the mutex 
        SDL_LockMutex(instance->move_object_mutex);

        // Is there anything left to move?
        if (queue_empty(instance->actor_move_queue))
        {

            // If not, unlock and return
            SDL_UnlockMutex(instance->move_object_mutex);
            return 1;
        }

        entity = queue_dequeue(instance->actor_move_queue);

        SDL_UnlockMutex(instance->move_object_mutex);
    }

    // Move the entity
    if(entity)
        move_entity(entity);

    return 0;
}

int update_forces      ( GXInstance_t *instance ) 
{
    // Initialized data
    GXEntity_t* entity = 0;

    // Get an entity to move
    {
        // Lock the mutex 
        SDL_LockMutex(instance->update_force_mutex);

        // Is there anything left to move?
        if (queue_empty(instance->actor_force_queue))
        {

            // If not, unlock and return
            SDL_UnlockMutex(instance->update_force_mutex);
            return 1;
        }

        entity = queue_dequeue(instance->actor_force_queue);

        SDL_UnlockMutex(instance->update_force_mutex);
    }

    // Move the entity
    if(entity)
        calculate_entity_force(entity);

    return 0;
}

int update_rigs        ( GXInstance_t *instance )
{

    return 1;
}
