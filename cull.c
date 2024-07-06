/** !
 * Cull implementation
 * 
 * @file cull.c
 * 
 * @author Jacob Smith
 */

// Header file
#include <g10/cull.h>

// Function declarations
//

// Function definitions
int cull_chain_construct
(
    cull_operation *pp_cull_operation,
    size_t          count,
    ...
)
{

    // Argument check
    //

    // Success
    return 1;

    // Error handling
    {

    }
}

int cull_entity_visible ( entity *p_entity )
{

    // Argument check
    if ( p_entity == (void *) 0 ) goto no_entity;

    // Success
    return 1;

    // Error handling
    {
    
        // Argument errors
        {
            no_entity:
                #ifndef NDEBUG
                    log_error("[g10] [cull] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

// Culling
int cull_objects
(
    cull_operation  *p_cull_chain,
    void           **pp_result,
    void           **p_objects,
    size_t           limit
)
{

    // Argument check
    //

    // Initialized data
    //

    done:

    // Success
    return 1;

    no_cull:
    {

        // Copy "limit" objects to result
        memcpy(pp_result, p_objects, sizeof(void *) * limit);

        // Done
        goto done;
    }

    // Error handling
    {
        
        // Argument errors
        {

        }
    }
}

int cull_chain_destroy ( cull_operation *pp_cull_operation )
{

    // Success
    return 1;
}
