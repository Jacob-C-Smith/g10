// Header
#include <g10/mesh.h>

enum mesh_shapes_e
{
    G10_BASE_MESH_PLANE,
    G10_BASE_MESH_CUBE,
    // G10_MESH_SHAPE_CIRCLE,
    // G10_MESH_SHAPE_UV_SPHERE,
    // G10_MESH_SHAPE_ICO_SPHERE,
    // G10_MESH_SHAPE_CYLINDER,
    // G10_MESH_SHAPE_CONE,
    // G10_MESH_SHAPE_TORUS,
    G10_MESH_SHAPE_QUANTITY
};

union mesh_data_u
{
    #ifdef G10_BUILD_WITH_VULKAN
        struct
        {
            
        } vulkan;
    #elif defined G10_BUILD_WITH_OPENGL
        struct
        {
            GLuint vertex_buffers[32];
            GLuint vertex_arrays[32];
        } opengl;
    #endif
};

struct mesh_s
{
    size_t quantity;
    struct
    {
        union mesh_data_u *p_mesh_data;
        transform _local;
        //material *p_material;
    } _meshes[];
};

// Data
const struct 
{
    const char _name[16];
    struct
    {
        const size_t  quantity;
        const f32    *_data;
    } verticies;
    struct
    { 
        const size_t  quantity;
        const u32    *_data;
    } elements;
} _base_mesh_data[G10_MESH_SHAPE_QUANTITY] = 
{
    [G10_BASE_MESH_PLANE] = 
    {
        ._name     = "plane",
        .verticies = 
        {
            .quantity = 12,
            ._data    = (const f32[]) { -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, -1.0, 1.0, 0.0, 1.0, 1.0, 0.0 }
        },
        .elements =
        {
            .quantity = 6,
            ._data    = (const u32[]) { 1, 2, 0, 1, 3, 2 }
        }
    },
    [G10_BASE_MESH_CUBE] = 
    {
        ._name     = "cube",
        .verticies = 
        {
            .quantity = 12,
            ._data    = (const f32[]) { -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0 }
        },
        .elements =
        {
            .quantity = 6,
            ._data    = (const u32[]) { 1, 2, 0, 3, 6, 2, 7, 4, 6, 5, 0, 4, 6, 0, 2, 3, 5, 7, 1, 3, 2, 3, 7, 6, 7, 5, 4, 5, 1, 0, 6, 4, 0, 3, 1, 5 }
        }
    }
    // TODO: Finish adding base meshes
};

dict *p_base_mesh_lookup = (void *) 0;

u0 g_init_mesh ( u0 )
{
    
    // Construct a lookup for base meshes
    dict_construct(&p_base_mesh_lookup, G10_MESH_SHAPE_QUANTITY, 0);

    // For each base mesh ...
    for (size_t i = 0; i < G10_MESH_SHAPE_QUANTITY; i++)
    
        // ... add it to the base mesh lookup
        dict_add(p_base_mesh_lookup, &_base_mesh_data[i]._name, &_base_mesh_data);
    
    // Done
    return;
}

u0 g_exit_mesh ( u0 )
{

    // Destroy the base mesh lookup
    dict_destroy(&p_base_mesh_lookup);

    // Done
    return;
}