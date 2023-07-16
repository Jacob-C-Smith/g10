#include <G10/GXRigidbody.h>

#define FORCE_CALCULATOR_COUNT 1

dict *force_calculators = 0;



int create_rigidbody ( GXRigidbody_t** pp_rigidbody )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_rigidbody == (void *) 0 ) goto no_rigidbody;
    #endif

    // Initialized data
    GXRigidbody_t *p_rigidbody = calloc(1, sizeof(GXRigidbody_t));

    // Error check
    {
        #ifndef NDEBUG
            if ( p_rigidbody == (void *) 0 )
                goto no_mem;
        #endif
    }

    // Write the return value
    *pp_rigidbody = p_rigidbody;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_rigidbody:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Null pointer provided for parameter \"pp_rigidbody\" in call to function \"%s\"\n", __FUNCTION__);
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

int update_force ( GXRigidbody_t *rigidbody )
{
    rigidbody->forces[0] = (vec3)
    {
        .x = 0.f,
        .y = 0.f,
        .z = 0.f,
        .w = 0.f
    };

    // Newtons first law; summate forces
    for (size_t i = 1; i < rigidbody->force_count; i++)
        add_vec3(&rigidbody->forces[0], rigidbody->forces[0], rigidbody->forces[i]);

    return 1;
}

int load_rigidbody ( GXRigidbody_t **pp_rigidbody, const char *path )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_rigidbody == (void *) 0 ) goto no_rigidbody;
        if ( path         == (void *) 0 ) goto no_path;
    #endif

    // Initialized data
    size_t  len  = g_load_file(path, 0, true);
    char   *text = calloc(len+1, sizeof(char));

    // Error check
    if ( text == (void *) 0 )
        goto no_mem;

    // Load the file
    if ( g_load_file(path, text, true) == 0 )
        goto failed_to_load_file;

    // Load the rigidbody from the JSON text
    if ( load_rigidbody_as_json_text(pp_rigidbody, text) == 0 )
        goto failed_to_load_rigidbody_as_json;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_rigidbody:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Null pointer provided for parameter \"pp_rigidbody\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
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

        // G10 errors
        {
            failed_to_load_file:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_load_rigidbody_as_json:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Failed to load rigidbody from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int load_rigidbody_as_json_text ( GXRigidbody_t** pp_rigidbody, char *text )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_rigidbody == (void *) 0 ) goto no_rigidbody;
        if ( text         == (void *) 0 ) goto no_text;
    #endif

    /*
    // Initialized data
    dict          *json_object = 0;
    GXRigidbody_t *p_rigidbody = 0;
    char          *mass        = 0,
                  *active      = 0;

    // Parse the JSON into a dictionary
    parse_json(text, len, &json_object);

    // Parse the dictionary into constructor parameters
    {

        // Initialized data
        JSONToken_t *token = 0;

        token  = (JSONToken_t *)dict_get(json_object, "mass");
        mass   = JSON_VALUE(token, JSONprimative);

        token  = (JSONToken_t *)dict_get(json_object, "active");
        active = JSON_VALUE(token, JSONprimative);
    }

    // Allocate memory for a rigidbody
    if ( create_rigidbody(pp_rigidbody) == 0 )
        goto failed_to_allocate_rigidbody;

    // Get a pointer to the rigidbody
    p_rigidbody = *pp_rigidbody;

    // Set the mass of the rigidbody
    if (mass)
        p_rigidbody->mass = (float)atof(mass);

    // Set the rigidbody type (true is active, false is passive)
    p_rigidbody->active = active;

    // Allocate memory for forces list
    p_rigidbody->forces = calloc(16, sizeof(vec3));

    // Error check
    {
        #ifndef NDEBUG
            if ( p_rigidbody->forces == (void *) 0 )
                goto no_mem;
        #endif
    }

    // Allocate memory for torques list
    p_rigidbody->torques = calloc(16, sizeof(quaternion));

    // Error check
    {
        #ifndef NDEBUG
            if ( p_rigidbody->torques == (void *) 0 )
                goto no_mem;
        #endif
    }*/

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_rigidbody:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Null pointer provided for parameter \"pp_rigidbody\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_text:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Null pointer provided for parameter \"text\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

        }
    }
}

int destroy_rigidbody ( GXRigidbody_t **pp_rigidbody )
{

    // Argument check
    #ifndef NDEBUG
        if ( pp_rigidbody == (void *) 0 ) goto no_rigidbody;
    #endif

    // Initialized data
    GXRigidbody_t *p_rigidbody = *pp_rigidbody;
    
    // Error check
    if ( p_rigidbody == (void *) 0 )
        goto pointer_to_null;

    // No more pointer for caller
    *pp_rigidbody = 0;

    // Free the forces
    free(p_rigidbody->forces);

    // Free the torques
    free(p_rigidbody->torques);

    // Free the rigidbody
    free(p_rigidbody);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_rigidbody:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Null pointer provided for parameter \"pp_rigidbody\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            pointer_to_null:
                #ifndef NDEBUG
                    g_print_error("[G10] [Rigidbody] Parameter \"pp_rigidbody\" points to null pointer in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
                
        }
    }
}
