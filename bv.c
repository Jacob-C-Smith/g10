/** !
 * Header file for bounding volume heierarchy
 *
 * @file g10/bv.h
 * 
 * @author Jacob Smith
 */

// Header
#include <g10/bv.h>

// Data
mesh_data *p_cube = (void *) 0;

// Function declarations
/** !
 * Allocate memory for a bv
 * 
 * @param pp_bv result
 * 
 * @return 1 on success, 0 on error
 */
int bv_create ( bv **pp_bv );

u0 g_init_bv ( u0 )
{
    mesh_shape_construct(&p_cube, G10_BASE_MESH_CUBE, 0);
}

int bv_bounds_from_mesh_data ( mesh_data *p_mesh_data, vec3 *p_min, vec3 *p_max )
{

    // TODO: Argument check

    // Initialized data
    vec3 _min   = p_mesh_data->local_bounds.min,
         _max   = p_mesh_data->local_bounds.max;
    vec3 _loc   = p_mesh_data->p_transform->location;
    mat3 _model = { 0 };
    
    // Demote the 4x4 model matrix 
    mat4_to_mat3(&_model, p_mesh_data->p_transform->model);

    // Compute and store the minimum bound
    mat3_mul_vec3(&_min, _model, _min);

    // Compute and store the maximum bound
    mat3_mul_vec3(&_max, _model, _max);

    vec3_add_vec3(p_min, _min, _loc);
    vec3_add_vec3(p_max, _max, _loc);

    // Success
    return 1;

    // TODO: Error handling
    { }
}

// Function definitions
int bv_create ( bv **pp_bv )
{

    // Argument check
    if ( pp_bv == (void *) 0 ) goto no_bv;

    // Initialized data
    bv *p_bv = G10_REALLOC(0, sizeof(bv));

    // Error check
    if ( p_bv == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_bv, 0, sizeof(bv));

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
                    log_error("[g10] [bv] Null pointer provided for parameter \"pp_bv\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int bvh_from_scene ( scene *p_scene )
{

    // TODO: Argument check
    //

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    queue *p_queue = (void *) 0;
    shader *p_bv_shader = dict_get(p_instance->cache.p_shaders, "bv");
    size_t key_quantity = 0;
    size_t bv_quantity = 0;
    const char *_p_keys[512] = { 0 };
    bv *_p_bvs[512] = { 0 };

    // Construct a queue
    // TODO: Error check
    //queue_construct(&p_queue);

    key_quantity = dict_keys(p_scene->data.entities, 0);
    dict_keys(p_scene->data.entities, (const char **const)&_p_keys);

    // Construct a bounding volume foreach entity in the scene
    for (size_t i = 0; i < key_quantity; i++)
    {
        
        // Initialized data
        entity *p_entity = dict_get(p_scene->data.entities, _p_keys[i]);
        mesh   *p_mesh   = p_entity->p_mesh;
        
        // Construct a bounding volume foreach mesh in the entity
        for (size_t j = 0; j < p_mesh->quantity; j++)
        {
            
            // Initialized data
            mesh_data *p_mesh_data = p_mesh->_p_meshes[j];
            bv        *p_bv        = (void *) 0;

            // Construct the bounding box
            bv_from_mesh_data(&p_bv, p_mesh_data);

            // Add the bounding volume to the queue
            _p_bvs[bv_quantity] = p_bv;

            bv_quantity++;
        }
    }

    if ( p_bv_shader )
        for (size_t i = 0; i < bv_quantity; i++)
        {
            
            shader_draw_item_add(p_bv_shader, _p_bvs[i]);
        }

    // Success
    return 1;
    
    // TODO: Error handling
    { }
}

int bv_from_mesh_data ( bv **pp_bv, mesh_data *p_mesh_data )
{

    // Argument check
    if ( pp_bv       == (void *) 0 ) goto no_bv;
    if ( p_mesh_data == (void *) 0 ) goto no_mesh_data;

    // Initialized data
    bv *p_bv = (void *) 0;
    
    // Allocate a bounding volume
    if ( bv_create(&p_bv) == 0 ) goto failed_to_allocate_bv;

    // Populate the bounding volume
    *p_bv = (bv)
    {
        .p_object = p_mesh_data,
        .p_left   = (void *) 0,
        .p_right  = (void *) 0,
        .bounds   =
        {
            .min = { 0 },
            .max = { 0 }
        },
        .pfn_bv_bounds_getter = (fn_bv_bounds_getter *) bv_bounds_from_mesh_data
    };

    // Compute the bounds of the object
    p_bv->pfn_bv_bounds_getter(p_bv->p_object, &p_bv->bounds.min, &p_bv->bounds.max);

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
                    log_error("[g10] [bv] Null pointer provided for parameter \"pp_bv\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
            no_mesh_data:
                #ifndef NDEBUG
                    log_error("[g10] [bv] Null pointer provided for parameter \"p_mesh_data\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
        // Standard library errors
        {
            failed_to_allocate_bv:
                #ifndef NDEBUG
                    log_error("[g10] [bv] Failed to allocate bounding volume in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int bv_update_bounds ( bv *p_bv )
{

    // Argument check
    if ( p_bv == (void *) 0 ) goto no_bv;

    // Done
    return p_bv->pfn_bv_bounds_getter(p_bv->p_object, &p_bv->bounds.min, &p_bv->bounds.max);
        
    // Error handling
    {

        // Argument errors
        {
            no_bv:
                #ifndef NDEBUG
                    log_error("[g10] [bv] Null pointer provided for parameter \"p_bv\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int bv_info ( bv *p_bv )
{

    // TODO: Argument check
    //

    // Print the bounding volume
    log_info("=== bv @ %p ===\n", p_bv);
    printf(" - min    : [ %+3.2f, %+3.2f, %+3.2f ]\n", p_bv->bounds.min.x, p_bv->bounds.min.y, p_bv->bounds.min.z );
    printf(" - max    : [ %+3.2f, %+3.2f, %+3.2f ]\n", p_bv->bounds.max.x, p_bv->bounds.max.y, p_bv->bounds.max.z );
    printf(" - object : %p\n", p_bv->p_object );
    
    // Success
    return 1;

    // TODO: Error handling
    { }
}

int bv_draw ( shader *p_shader, bv *p_bv )
{

    // Argument check
    if ( p_shader == (void *) 0 ) goto no_shader;
    if ( p_bv     == (void *) 0 ) goto no_bv;

    // Bind the bv 
    shader_bind_bv(p_shader, p_bv);

    // Graphics API dependant implementation
    #ifdef G10_BUILD_WITH_OPENGL

        // Bind the vertex array
        glBindVertexArray(p_cube->opengl.vertex_arrays[0]);

        // Draw the mesh
        glDrawElements(GL_TRIANGLES, (GLsizei)p_cube->opengl.indices, GL_UNSIGNED_INT, 0);
    #endif

    // Success
    return 1;

    // Error handling
    {

        no_shader:
            #ifndef NDEBUG
                log_error("[g10] [bv] Null pointer provided for paramter \"p_shader\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        no_bv:
            #ifndef NDEBUG
                log_error("[g10] [bv] Null pointer provided for paramter \"p_bv\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

//int bv_from_mesh ( bv **pp_bv, mesh *p_mesh );
//bool bv_aabb_intersect ( bv *p_a, bv *p_b );
//int destroy_bv ( bv **pp_bv );
