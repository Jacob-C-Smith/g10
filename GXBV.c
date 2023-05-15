#include <G10/GXBV.h>

int create_bv ( GXBV_t **pp_bv )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_bv == (void *) 0 ) goto no_bv;
        #endif
    }

    // Initialized data
    GXBV_t *p_bv = calloc(1, sizeof(GXBV_t));

    // Check if the memory was allocated
    if ( p_bv == (void *) 0 )
        goto no_mem;

    // Return a pointer to the caller
    *pp_bv = p_bv;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_bv:
                #ifndef NDEBUG
                    g_print_error("[G10] [BV] Null pointer provided for \"pp_bv\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[G10] [BVH] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int construct_bv ( GXBV_t **pp_bv, vec3 min, vec3 max )
{
    // Argument check
    {
        #ifndef NDEBUG
            if ( pp_bv == (void *) 0 ) goto no_bv;
        #endif
    }

    // Initialized data
    GXBV_t *p_bv = 0;

    // Try to create the BV
    if ( create_bv(pp_bv) == 0 )
        goto failed_to_create_bv;

    // Return the BV
    p_bv = *pp_bv;

    // Set the minimum and maximum
    p_bv->maximum = max;
    p_bv->minimum = min;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_bv:
                #ifndef NDEBUG
                    g_print_error("[G10] [BV] Null pointer provided for \"pp_bv\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // G10 errors
        {
            failed_to_create_bv:
                #ifndef NDEBUG
                    g_print_error("[G10] [BVH] Out of memory\n");
                #endif

                // Error
                return 0;
        }
    }
}

int construct_bv_from_bvs ( GXBV_t **pp_bv, GXBV_t *a, GXBV_t* b )
{


    GXBV_t* p_bv = 0;

    create_bv(pp_bv);

    p_bv = *pp_bv;

    p_bv->left  = a,
    p_bv->right = b;

    resize_bv(p_bv);

    return 1;
}

int construct_bvh_from_scene ( GXBV_t **bv, GXScene_t *scene )
{

    // Commentary
    {
        /*
         * This function constructs a bounding volume hierarchy from a scene. The algorithm is
         * relatively simple, and constructs the tree on O(n^3) time. The algorithm creates the
         * tree in a bottom up fashion.
         *
         * The algorithm starts by generating a list of bounding volume heierarchies from the list
         * of entities from the scene. The algorithm then compute which two bounding volumes are
         * the closest together. This is an O(n^2) operation. The closest objects are removed from
         * the list, and are combined to form a new bounding volume that is concometently
         * reinserted to the list. Removing two and insterting one. The algorithm does this for as
         * many bounding boxes are in the list, until only one bounding volume is left. This is the
         * bounding volume of the scene, and contains every entity in the scene.
         */
    }

    // Argument check
    {
        #ifndef NDEBUG
            if ( scene == (void *) 0 ) goto no_scene;
        #endif
    }

    // Uninitialized data
    GXEntity_t **actor_list        = 0; // List of entities
    GXBV_t     **bounding_volumes  = 0; // List of qualifying entities

    // Initialized data
    size_t       actors_in_scene       = dict_values(scene->actors, 0), // How many entities are in the scene?
                 actors_with_colliders = 0,
                 i                     = 0,                           // Iterators      ...
                 j                     = 0,                           //                ...
                 best_i                = 0,                           // Best indicies  ...
                 best_j                = 0;                           //                ...
    GXBV_t      *ret                   = 0;

    if (actors_in_scene == 0)
        goto no_actors;

    // Allocate a double pointer list
    actor_list       = calloc(actors_in_scene, sizeof(void*));
    bounding_volumes = calloc(actors_in_scene, sizeof(void*));
    dict_values(scene->actors, actor_list);

    // Error checking
    {
        #ifndef NDEBUG
            if ( actor_list        == (void *) 0 )
                goto no_mem;
            if ( bounding_volumes  == (void *) 0 )
                goto no_mem;
        #endif
    }

    // Populate the double pointer list with bounding volumes
    for (i = 0; i < actors_in_scene; i++)
    {

        // Is there a collider?
        if (actor_list[i]->collider)
        {

            // Is there a bounding volume on the collider?
            if (actor_list[i]->collider->bv)
            {
                bounding_volumes[actors_with_colliders++] = actor_list[i]->collider->bv;
                resize_bv(bounding_volumes[actors_with_colliders-1]);
                continue;
            }
        }
        else
            continue;
    }

    // Iterate through the list until there are no more bounding volumes to combine
    while (actors_with_colliders > 1)
    {
        float   best = FLT_MAX;

        GXBV_t *a = 0,
               *b = 0;

        // Iterate over bounding volumes with i
        for (i = 0; i < actors_with_colliders; i++)

            // Iterate over bounding volumes with j
            for (j = 0; j < actors_with_colliders; j++)
            {
                // Make sure that i and j are not the same. If they are, set the test distance to float max so it will always fail
                float pD = (bounding_volumes[i] != bounding_volumes[j]) ? bv_distance(bounding_volumes[i], bounding_volumes[j]) : FLT_MAX;

                // If there is a better distance, record the distance and the indicies to the bounding volume list
                if ((bounding_volumes[i] != bounding_volumes[j]) && pD < best)
                    best = pD,
                    best_j   = j,
                    best_i   = i;

            }


        // Reset the best distance to float max
        best = FLT_MAX;

        GXBV_t *insert = 0,
               *t      = bounding_volumes[actors_with_colliders - 1];

        construct_bv_from_bvs(&bounding_volumes[best_i], bounding_volumes[best_i], bounding_volumes[best_j]);

        // XOR swap the best
        (size_t)bounding_volumes[best_j]                    ^= (size_t)bounding_volumes[actors_with_colliders - 1],
        (size_t)bounding_volumes[actors_with_colliders - 1] ^= (size_t)bounding_volumes[best_j],
        (size_t)bounding_volumes[best_j]                    ^= (size_t)bounding_volumes[actors_with_colliders - 1];

        actors_with_colliders--;
        best_i = 0,
        best_j = 0;
    }

    ret = (bounding_volumes) ? *bounding_volumes : 0;

    //free(BVList);
    *bv = ret;

    // Don't want to dereference a null pointer
    return 1;

    // Error handling
    {

        // Standard library errors
        {
            no_mem:
            #ifndef NDEBUG
                g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        }

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    g_print_error("[G10] [BV] Null pointer provided for \"scene\" in call to function %s\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_actors:
                #ifndef NDEBUG
                    g_print_error("[G10] [BV] No actors in \"scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

float bv_distance ( GXBV_t *a, GXBV_t *b )
{
    float  ret             = 0.f;
    vec3   distance_vector = { 0.f, 0.f, 0.f, 0.f };

    distance_vector.x = max(0, fabsf( -a->maximum.x + b->minimum.x )),
    distance_vector.y = max(0, fabsf( -a->maximum.y + b->minimum.y )),
    distance_vector.z = max(0, fabsf( -a->maximum.z + b->minimum.z )),

    ret = sqrtf(dot_product_vec3(distance_vector, distance_vector));

    return ret;
}

GXBV_t *find_parent_bv ( GXBV_t *bvh, GXBV_t *bv )
{
    GXBV_t *b   = bvh,
           *ret = 0;

    if (bvh->left == bv)
        return bvh;

    if (bvh->right == bv)
        return bvh;

    if (b->left)
    {
        if (aabb_intersect(b->left, bv))
        {
            GXBV_t* p_ret = find_parent_bv(b->left, bv);

            ret = (p_ret) ? p_ret : ret;
        }
    }

    if (b->right)
    {
        if (aabb_intersect(b->right, bv))
        {
            GXBV_t *p_ret = find_parent_bv(b->right, bv);

            ret = (p_ret) ? p_ret : ret;
        }
    }

    return ret;
}

int insert_bv ( GXBV_t *bvh, GXBV_t *bv )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( bvh == 0 ) goto no_bvh;
        #endif
    }

    // Branching
    {
        if (bvh->entity) return 0;
        if (bvh->left == 0 && bvh->right == 0) goto no_left_no_right;
        if (bvh->left      && bvh->right == 0) goto no_right;
        if (bvh->left == 0 && bvh->right     ) goto no_left;
        if (bvh->left      && bvh->right     ) goto left_and_right;
    }

    no_left_no_right:
    {
        bvh->left = bv;

        goto exit;
    }

    no_right:
    {
        if ( aabb_intersect(bvh->left, bv) )
        {
            if ( bvh->left->entity )
            {

            }
            else
            {
                insert_bv(bvh->left, bv);
            }
        }
        else
            bvh->right = bv;

        goto exit;
    }

    no_left:
    {
        if ( aabb_intersect(bvh->right, bv) )
            insert_bv(bvh->right, bv);
        else
            bvh->left = bv;

        goto exit;
    }

    left_and_right:
    {
        if ( bv_distance(bvh->left, bv) > bv_distance(bvh->right, bv) )
        {
            if (bvh->left->entity)
            {
                GXBV_t *t_bv = 0;
                construct_bv_from_bvs(&t_bv, bv, bvh->left);
                bvh->left = t_bv;
                return 0;
            }
            insert_bv(bvh->left, bv);
        }
        else
            insert_bv(bvh->right, bv);

        goto exit;
    }

    exit:
    return 1;

    // Argument errors
    {
        no_bvh:
            #ifndef NDEBUG
                g_print_error("[G10] [BV] Null pointer provided for \"bvh\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
    }
}

int resize_bv ( GXBV_t *bv )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( bv == 0 ) goto no_bv;
        #endif
    }

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();

    // Flow control
    {

        // Resize the parent from the two childern
        if (bv->left && bv->right)
            goto left_and_right;

        // Remove the unneccisary bounding volume
        else if (bv->left  && bv->right == 0)
            goto no_right;

        // Remove the unneccisary bounding volume
        else if (bv->right && bv->left  == 0)
            goto no_left;

        else if (bv->entity)
            goto entity;

        // Remove the node
        else

            // TODO:
            return 0;
    }

    // Commentary
    {
        /*
         *
         * ┌───────────┬───────────┐
         * │           │           │
         * │   ┌───────┴───────┐   │
         * │   │   Parent BV   │   │
         * │   └───────┬───────┘   │
         * │           │           │
         * │     ┌─────┼─────┐     │
         * │   ┌─┴─┐ ┌─┴─┐ ┌─┴─┐   │
         * │   │ A │ │ E │ │ B │   │
         * │   └─┬─┘ └───┘ └─┬─┘   │
         * │     │           │     │
         * └─────┴───────────┴─────┘
         *
         * Diagram of bv state. This operation will resize the parent bv to fit A and B.
         *
         * If you can't read this, try a monospaced font
         */
    }

    left_and_right:
    {
        GXEntity_t *left  = bv->left->entity,
                   *right = bv->right->entity;

        vec3        a_max = bv->left->maximum,
                    a_min = bv->left->minimum,
                    b_max = bv->right->maximum,
                    b_min = bv->right->minimum;

        vec3        max   = { 0, 0, 0, 0 },
                    min   = { 0, 0, 0, 0 };

        // Recursively resize entities to get accurate state
        {
            if (left)
                resize_bv(bv->left);

            if (right)
                resize_bv(bv->right);
        }

        // Commentary
        {
            /*
             * The token ( a > b ) will be evaluated as 1 when a > b, and 0 when b > a.
             *
             * The following min/max equations can be understood as
             *
             *  ( a > b ) * a + ( a < b ) * b
             *
             * Such that when a > b, ( a > b ) will evaluate to 1, and ( b > a ) must
             * evaluate to 0
             *
             * 1 * a + 0 * b =
             * 1 * a =
             * a
             *
             * or in the opposite case, when ( b > a ) = 1, and ( a > b ) = 0,
             *
             * 0 * a + 1 * b =
             * 1 * b =
             * b
             *
             * This avoids brances, and is easily vectorizable. Time for some SIMD.
             * TODO: vectorize
             */
        }

        max.x = ( ( a_max.x >= b_max.x ) * a_max.x ) + ( ( a_max.x < b_max.x ) * b_max.x ),
        max.y = ( ( a_max.y >= b_max.y ) * a_max.y ) + ( ( a_max.y < b_max.y ) * b_max.y ),
        max.z = ( ( a_max.z >= b_max.z ) * a_max.z ) + ( ( a_max.z < b_max.z ) * b_max.z );

        min.x = ( ( a_min.x < b_min.x ) * a_min.x ) + ( ( a_min.x >= b_min.x ) * b_min.x ),
        min.y = ( ( a_min.y < b_min.y ) * a_min.y ) + ( ( a_min.y >= b_min.y ) * b_min.y ),
        min.z = ( ( a_min.z < b_min.z ) * a_min.z ) + ( ( a_min.z >= b_min.z ) * b_min.z );

        bv->maximum.x = max.x,
        bv->maximum.y = max.y,
        bv->maximum.z = max.z,

        bv->minimum.x = min.x,
        bv->minimum.y = min.y,
        bv->minimum.z = min.z;

        return 0;
    }

    // Commentary
    {
        /*
         *
         * ┌───────────┬───────────┐
         * │           │           │
         * │   ┌───────┴───────┐   │
         * │   │   Parent BV   │   │
         * │   └───────┬───────┘   │
         * │           │           │
         * │     ┌─────┼─────┐     │
         * │   ┌─┴─┐ ┌─┴─┐ ┌─┴─┐   │
         * │   │ A │ │ E │ │ B │   │
         * │   └─┬─┘ └───┘ └───┘   │
         * │     │                 │
         * └─────┴─────────────────┘
         *
         * This operation will copy B into the parent BV, and free the memory of B
         *
         * If you can't read this, try a monospaced font
         */

    }

    no_right:
    {

        // Initialized data
        GXBV_t *left = bv->left;

        // Copy the left node into the parent bounding volume
        {
            bv->entity  = left->entity;
            bv->left    = left->left;
            bv->right   = left->right;
            bv->maximum = left->maximum;
            bv->minimum = left->minimum;
        }

        // Reparent the entity
        if (left->entity)
            bv->entity->collider->bv = bv;

        // Free the orphan vertex
        free(left);

        return 0;
    }

    // Commentary
    {
        /*
         *
         * ┌───────────┬───────────┐
         * │           │           │
         * │   ┌───────┴───────┐   │
         * │   │   Parent BV   │   │
         * │   └───────┬───────┘   │
         * │           │           │
         * │     ┌─────┼─────┐     │
         * │   ┌─┴─┐ ┌─┴─┐ ┌─┴─┐   │
         * │   │ A │ │ E │ │ B │   │
         * │   └───┘ └───┘ └─┬─┘   │
         * │                 │     │
         * └─────────────────┴─────┘
         *
         * This operation will copy B into the parent BV, and free the memory of B
         *
         * If you can't read this, try a monospaced font
         */
    }

    no_left:
    {

        // Initialized data
        GXBV_t *right = bv->right;

        // Copy the left node into the parent bounding volume
        {
            bv->entity  = right->entity;
            bv->left    = right->left;
            bv->right   = right->right;
            bv->maximum = right->maximum;
            bv->minimum = right->minimum;
        }

        // Reparent the entity
        if (right->entity)
            bv->entity->collider->bv = bv;

        // Free the orphan vertex
        free(right);

        return 0;
    }

    // Commentary
    {
        /*
         *
         * ┌───────────┬───────────┐
         * │           │           │
         * │   ┌───────┴───────┐   │
         * │   │   Parent BV   │   │
         * │   └───────┬───────┘   │
         * │           │           │
         * │     ┌─────┼─────┐     │
         * │   ┌─┴─┐ ┌─┴─┐ ┌─┴─┐   │
         * │   │ A │ │ E │ │ B │   │
         * │   └───┘ └─┬─┘ └───┘   │
         * │           │           │
         * └───────────┴───────────┘
         *
         * This operation resizes an entity from its model matrix
         *
         * If you can't read this, try a monospaced font
         */
    }

    entity:
    {

        // Get the model matrix of the entity

        // Initialized data
        GXCollider_t *collider = bv->entity->collider;
        mat4          m        = bv->entity->transform->model_matrix;

        // Compute axis aligned radius
        float x_radius = (fabsf(m.a) + fabsf(m.b) + fabsf(m.c) ),
              y_radius = (fabsf(m.e) + fabsf(m.f) + fabsf(m.g) ),
              z_radius = (fabsf(m.i) + fabsf(m.j) + fabsf(m.k) );

        // Compute min and max from radius and location
        bv->maximum.x = ( collider->aabb_max.x * x_radius ) + m.m,
        bv->maximum.y = ( collider->aabb_max.y * y_radius ) + m.n,
        bv->maximum.z = ( collider->aabb_max.z * z_radius ) + m.o;

        bv->minimum.x = ( collider->aabb_min.x * x_radius ) + m.m,
        bv->minimum.y = ( collider->aabb_min.y * y_radius ) + m.n,
        bv->minimum.z = ( collider->aabb_min.z * z_radius ) + m.o;

        return 0;
    }

    resize_bv(find_parent_bv(p_instance->context.scene->bvh, bv));

    return 0;

    // Error handling
    {
        no_bv:
        #ifndef NDEBUG
            g_print_error("[G10] [BVH] Null pointer provided for \"bv\" in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }
}

int bv_info ( GXBV_t *p_bv, size_t d )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( p_bv == (void *) 0 ) goto no_bv;
        #endif
    }

    // Base case, print out a header
    if (d == 0)
        printf("[G10] [BV] Bounding volume hierarchy\n[ entity name ] - < location > - < dimension >\n\n");

    // Indent proportional to the deapth of the BVH
    for (size_t i = 0; i < d * 4; i++)
        putchar(' ');

    // If the bounding volume is an entity, print the entities name
    if (p_bv->entity)
    {
        printf("\"%s\"", p_bv->entity->name);
        printf(" - < %.2f %.2f %.2f > - < %.2f %.2f %.2f >\n", p_bv->maximum.x, p_bv->maximum.y, p_bv->maximum.z, p_bv->minimum.x, p_bv->minimum.y, p_bv->minimum.z );
    }
    else

        // Print the location and scale of the bounding volume
        printf("%s - < %.2f %.2f %.2f > - < %.2f %.2f %.2f >\n", "Volume", p_bv->maximum.x, p_bv->maximum.y, p_bv->maximum.z, p_bv->minimum.x, p_bv->minimum.y, p_bv->minimum.z);

    // Print the left node
    if (p_bv->left)
    {
        printf("L");
        bv_info(p_bv->left, d + 1);
    }

    // Print the right node
    if (p_bv->right)
    {
        printf("R");
        bv_info(p_bv->right, d + 1);
    }

    // If there is nothing else to print, print a few newlines
    if (d == 0)
        printf("\n\n");

    return 1;

    // Error handling
    {
        no_bv:
        #ifndef NDEBUG
            g_print_error("[G10] [BVH] Null pointer provided for \"bv\" in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }
}

size_t get_entities_from_bv ( GXBV_t *bv, queue *entity_queue )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( bv == (void *) 0 ) goto no_bv;
        #endif
    }

    // If the bounding volume is an entity, print the entities name
    if (bv->entity)
        queue_enqueue(entity_queue, bv->entity);

    // Print the left node
    if (bv->left)
        get_entities_from_bv(bv->left, entity_queue);

    // Print the right node
    if (bv->right)
        get_entities_from_bv(bv->right, entity_queue);

    return 0;

    // Error handling
    {
        no_bv:
        #ifndef NDEBUG
            g_print_error("[G10] [BVH] Null pointer provided for \"bv\" in call to function \"%s\"\n", __FUNCTION__);
        #endif
        return 0;
    }
}

bool aabb_intersect ( GXBV_t *a, GXBV_t *b )
{

    // Argument check
    {
        #ifndef NDEBUG
            if ( a == (void *) 0 ) goto no_a;
            if ( b == (void *) 0 ) goto no_b;
        #endif
    }

    // Initialized data
    vec3 a_min = a->minimum,
         a_max = a->maximum,
         b_min = b->minimum,
         b_max = b->maximum;

    // Intersection test
    return ( (a_min.x <= b_max.x) && (a_max.x >= b_min.x) &&
             (a_min.y <= b_max.y) && (a_max.y >= b_min.y) &&
             (a_min.z <= b_max.z) && (a_max.z >= b_min.z)    );

    // Error handling
    {

        // Argument errors
        {
            no_a:
                #ifndef NDEBUG
                    g_print_error("[G10] [BV] Null pointer provided for \"a\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return false;
            no_b:
                #ifndef NDEBUG
                    g_print_error("[G10] [BV] Null pointer provided for \"b\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return false;
        }
    }
}

int destroy_bv ( GXBV_t *bv )
{
    free(bv);

    return 0;
}
