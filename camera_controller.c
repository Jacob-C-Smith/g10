/** !
 * Camera controller implementation
 * 
 * @file camera_controller.c
 * 
 * @author Jacob Smith
 */

// Header file
#include <g10/camera_controller.h>

// Function definitions
int camera_controller_orbit_update ( camera *p_camera )
{

    // Movement
    {
        
        // Initialized data
        vec3 _location = p_camera->view.location;
        vec3 _displacement = { 0 };
        vec3 _left =
        {
            .x = p_camera->matrix._view.a,
            .y = p_camera->matrix._view.e,
            .z = p_camera->matrix._view.i
        };
        vec3 _front =
        {
            .x = p_camera->matrix._view.c,
            .y = p_camera->matrix._view.g,
            .z = p_camera->matrix._view.k
        };
        vec3 _front_prime = { 0 },
             _left_prime  = { 0 };
        float forward_backward = 0.2 * ( input_bind_value("BACKWARD") - input_bind_value("FORWARD") ),
              left_right       = 0.2 * ( input_bind_value("STRAFE LEFT") - input_bind_value("STRAFE RIGHT") );

        // Scale the forward / backward displacement
        vec3_mul_scalar(&_front_prime, _front, forward_backward);

        // Scale the left / right displacement
        vec3_mul_scalar(&_left_prime, _left, left_right);

        // Compute net displacement 
        vec3_add_vec3(&_displacement, _front_prime, _left_prime);

        // Accumulate 
        p_camera->view.location.x += _displacement.x,
        p_camera->view.location.y += _displacement.y;

        // Up / Down
        p_camera->view.location.z += 0.2 * ( input_bind_value("JUMP") - input_bind_value("PRONE") );
    }

    // TODO: Orientation
    {

    }
    
    // Set the dirty flag
    p_camera->dirty = true;

    // Success
    return 1;
}
