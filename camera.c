/** !
 * Camera implementation
 * 
 * @file camera.c
 * 
 * @author Jacob Smith
 */

// Header file
#include <g10/camera.h>

// Function declarations
/** !
 * Allocate memory for a camera
 *
 * @param pp_camera result
 *
 * @return 1 on success, 0 on error
 */
int camera_create ( camera **pp_camera );

// Function definitions
int camera_create ( camera **pp_camera )
{

    // Argument check
    if ( pp_camera == (void *) 0 ) goto no_camera;

    // Initialized data
    camera *p_camera = G10_REALLOC(0, sizeof(camera));

    // Error check
    if ( p_camera == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_camera, 0, sizeof(camera));

    // Return a pointer to the caller
    *pp_camera = p_camera;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_camera:
                #ifndef NDEBUG
                    log_error("[g10] [camera] Null pointer provided for parameter \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
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

