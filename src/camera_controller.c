/** !
 * Camera controller implementation
 * w
 * @file camera_controller.c
 * 
 * @author Jacob Smith
 */

// header file
#include <camera_controller.h>

// function definitions
int camera_controller_first_person_update ( camera *p_camera )
{

    // movement
    {
        
        // initialized data
        //vec3 _location = p_camera->view.location;
        vec3 _displacement = { 0 };
        vec3 _right =
        {
            .x = p_camera->matrix._view.a,
            .y = p_camera->matrix._view.e,
            .z = p_camera->matrix._view.i
        };
        vec3 _back =
        {
            .x = p_camera->matrix._view.c,
            .y = p_camera->matrix._view.g,
            .z = p_camera->matrix._view.k
        };
        vec3 _forward;
        vec3_mul_scalar(&_forward, _back, -1.0f);

        vec3 _forward_prime = { 0 },
             _right_prime   = { 0 };
        float forward_backward = 0.2f * ( input_bind_value("FORWARD") - input_bind_value("BACKWARD") ),
              left_right       = 0.2f * ( input_bind_value("STRAFE RIGHT") - input_bind_value("STRAFE LEFT") );

        // scale the forward / backward displacement
        vec3_mul_scalar(&_forward_prime, _forward, forward_backward);

        // scale the left / right displacement
        vec3_mul_scalar(&_right_prime, _right, left_right);

        // compute net displacement 
        vec3_add_vec3(&_displacement, _forward_prime, _right_prime);

        // accumulate 
        p_camera->view.location.x += _displacement.x,
        p_camera->view.location.y += _displacement.y;
        p_camera->view.location.z += _displacement.z;

        // up / down
        vec3 _up_prime = { 0 };
        vec3_mul_scalar(&_up_prime, p_camera->view.up, 0.2f * ( input_bind_value("JUMP") - input_bind_value("PRONE") ) );
        vec3_add_vec3(&p_camera->view.location, p_camera->view.location, _up_prime);
    }

    // orientation
    {
        float up_down   = 0.05f * ( input_bind_value("CAMERA DOWN") - input_bind_value("CAMERA UP") ),
              side_side = 0.05f * ( input_bind_value("CAMERA LEFT") - input_bind_value("CAMERA RIGHT") );

        // Construct a forward vector from the view matrix 
        // Note: The third column of the view matrix is the camera's Z axis (Backwards)
        vec3 back = 
        {
            .x = p_camera->matrix._view.c,
            .y = p_camera->matrix._view.g,
            .z = p_camera->matrix._view.k
        };

        vec3 front;
        vec3_mul_scalar(&front, back, -1.0f);
        vec3_normalize(&front, front);

        // Convert cartesian direction vector to polar coordinates
        float pitch = asinf(front.z);
        float yaw   = atan2f(front.y, front.x);

        // Update polar coordinates
        pitch -= up_down;
        yaw   += side_side;

        // Clamp Pitch
        if ( pitch > 1.55f ) pitch = 1.55f;
        if ( pitch < -1.55f ) pitch = -1.55f;

        // Update the forward vector 
        front.x = cosf(yaw) * cosf(pitch);
        front.y = sinf(yaw) * cosf(pitch);
        front.z = sinf(pitch);

        // Update the target
        vec3_add_vec3(&p_camera->view.target, p_camera->view.location, front);
    }

    // Update the matrix
    camera_matrix_view(
        &p_camera->matrix._view,
        p_camera->view.location,
        p_camera->view.target,
        p_camera->view.up
    );

    // Success
    return 1;
}
