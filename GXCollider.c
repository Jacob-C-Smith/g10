#include <G10/GXCollider.h>

#define COLLIDER_TYPE_COUNT 8

char* collider_type_names[COLLIDER_TYPE_COUNT] = {
    "invalid",
    "quad",
    "box",
    "sphere",
    "capsule",
    "cylinder",
    "cone",
    "convexhull"
};

collider_type_t collider_type_enums[COLLIDER_TYPE_COUNT] = {
    collider_invalid,
    collider_quad,
    collider_box,
    collider_sphere,
    collider_capsule,
    collider_cylinder,
    collider_cone,
    collider_convexhull
};

dict* collider_type_dict = 0;

void init_collider ( void )
{
    dict_construct(&collider_type_dict, COLLIDER_TYPE_COUNT);

    for (size_t i = 0; i < COLLIDER_TYPE_COUNT; i++)
        dict_add(collider_type_dict, collider_type_names[i], (void*)collider_type_enums[i]);
}

int create_collider ( GXCollider_t **pp_collider )
{
    // TODO: Argument check

    // Initialized data
    GXCollider_t * p_collider = calloc(1, sizeof(GXCollider_t));
    
    // Write the return value
    *pp_collider = p_collider;

    // Success
    return 1;

    // Error handling
}

int load_collider(GXCollider_t** collider, const char* path)
{
    size_t  len   = g_load_file(path, 0, false);
    char   *token = calloc(len+1, sizeof(char));

    g_load_file(path, token, false);

    load_collider_as_json(collider, token, len);

    free(token);

    return 1;
}

int load_collider_as_json(GXCollider_t** collider, char* token_text, size_t len)
{
    GXCollider_t  *i_collider    = 0;
    dict          *collider_json = 0;
    char          *type          = 0,
                 **max           = 0,
                 **min           = 0;

    {
        JSONToken_t *token = 0;

        parse_json(token_text, len, &collider_json);

        token = dict_get(collider_json, "type");
        type  = JSON_VALUE(token, JSONstring);

        token = dict_get(collider_json, "max");
        max   = JSON_VALUE(token, JSONarray);

        token = dict_get(collider_json, "min");
        min   = JSON_VALUE(token, JSONarray);

    }

    create_collider(collider);

    i_collider = *collider;

    i_collider->aabb_max = (vec3){ atof(max[0]), atof(max[1]), atof(max[2]), 0 };
    i_collider->aabb_min = (vec3){ atof(min[0]), atof(min[1]), atof(min[2]), 0 };

    i_collider->type = (collider_type_t)dict_get(collider_type_dict, type);


    construct_bv(&i_collider->bv, i_collider->aabb_min, i_collider->aabb_max);

    // TODO: Change 16 to something smarter
    dict_construct(&i_collider->collisions, 16);

    return 1;
}

int add_aabb_start_collision_callback(GXCollider_t* collider, void* function_pointer)
{
    return 1;
}

