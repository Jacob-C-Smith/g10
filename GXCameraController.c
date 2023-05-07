#include <G10/GXCameraController.h>

float  x_orient,
       y_orient,
       h_ang,
       v_ang,
       camera_acceleration = 1.0f;

GXCameraController_t* camera_controller = 0;

GXCameraController_t* create_camera_controller       ( void )
{
    GXCameraController_t *ret = calloc(1, sizeof(GXCameraController_t));
    
    // Check allocaated memory
    {
        #ifndef NDEBUG
            if( ret == (void *) 0 )
                goto no_mem;
        #endif
    }

    // Success
    return ret;

    // Error handling
    {
        no_mem:
            #ifndef NDEBUG
                g_print_error("[G10] [Camera controller] Unable to allocate memory for camera controller\n");
            #endif
            return 0;
    }
}

void                  camera_controller_forward      ( callback_parameter_t state, GXInstance_t* p_instance )
{
    if (state.input_state == KEYBOARD)
        y_orient = (state.inputs.key.depressed) ? 1.f : 0.f;
}
void                  camera_controller_backward     ( callback_parameter_t state, GXInstance_t* p_instance )
{
    if (state.input_state == KEYBOARD)
        y_orient = (state.inputs.key.depressed) ? -1.f : 0.f;
}
void                  camera_controller_strafe_left  ( callback_parameter_t state, GXInstance_t* p_instance )
{
    if (state.input_state == KEYBOARD)
        x_orient = (state.inputs.key.depressed) ? -1.f : 0.f;
}
void                  camera_controller_strafe_right ( callback_parameter_t state, GXInstance_t* p_instance )
{
    if (state.input_state == KEYBOARD)
        x_orient = (state.inputs.key.depressed) ? 1.f : 0.f;
}

void                  camera_controller_mouse           ( callback_parameter_t state, GXInstance_t* p_instance ) 
{
    if (state.input_state == MOUSE)
    {
        v_ang -= camera_acceleration * p_instance->input->mouse_sensitivity * (state.inputs.mouse_state.yrel / 720.0f);
        h_ang += camera_acceleration * p_instance->input->mouse_sensitivity * (state.inputs.mouse_state.xrel / 1280.0f);
    }

}

int camera_controller_from_camera  ( GXInstance_t* p_instance, GXCamera_t *camera )
{
    // Argument check
    {
        #ifndef NDEBUG
            if ( p_instance == (void *)0 )
                goto no_instance;
            if (camera == (void *)0 )
                goto no_camera;
        #endif
    }

    GXCameraController_t *ret = create_camera_controller();
    u8 errors = 0;
    ret->camera = camera;

    // Displacement binds
    GXBind_t* forward = find_bind(p_instance->input, "FORWARD"),
        * backward = find_bind(p_instance->input, "BACKWARD"),
        * left = find_bind(p_instance->input, "STRAFE LEFT"),
        * right = find_bind(p_instance->input, "STRAFE RIGHT"),

        // Orientation binds
        * mouse = find_bind(p_instance->input, "MOUSE");
    
    // Error checking
    {
        // This error checking is a little bit unorthadox, so I figure its worth explaining whats going on.
        // 8 binds are required to set up a camera controller, and there are eight bits in a byte. If one of
        // the binds is missing, a bit is flipped. If there are any errors, the errors byte will be nonzero.

        // The error byte is checked for nonzero value at the end. If there is a nonzero value, the program 
        // branches to error handling. Else, continue

        #ifndef NDEBUG

        if (forward      == (void*)0)
            errors |= 0x1;
        if (backward     == (void*)0)
            errors |= 0x2;
        if (left         == (void*)0)
            errors |= 0x4;
        if (right        == (void*)0)
            errors |= 0x8;
        if (mouse    == (void*)0)
            errors |= 0x10;


        if (errors)
            goto missing_binds;

        #endif
    }

    ret->spdlim = 0.0025f;

    // Assign displacement callbacks
    register_bind_callback(forward     , &camera_controller_forward);
    register_bind_callback(backward    , &camera_controller_backward);
    register_bind_callback(left        , &camera_controller_strafe_left);
    register_bind_callback(right       , &camera_controller_strafe_right);

    // Assign orientation callbacks
    register_bind_callback(mouse   , &camera_controller_mouse);


    ret->camera = camera;

    camera_controller = ret;

    return 1;
    
    // Error handling
    {

        // Argument error
        {
            no_instance:
            #ifndef NDEBUG
                g_print_error("[G10] [Camera Controller] Null pointer provided for \"p_instance\" in call to \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
            no_camera:
            #ifndef NDEBUG
                g_print_error("[G10] [Camera Controller] Null pointer provided for \"camera\" in call to \"%s\"\n", __FUNCTION__);
            #endif
            return 0;
        }

        // Missing bind for camera controller
        {
            missing_binds:
            if (errors & 0x1)
                g_print_error("[G10] [Camera controller] Missing \"FORWARD\" input bind to set up camera controller\n");
            if ( errors & 0x2  )
                g_print_error("[G10] [Camera controller] Missing \"BACKWARD\" input bind to set up camera controller\n");
            if ( errors & 0x4  )
                g_print_error("[G10] [Camera controller] Missing \"STRAFE LEFT\" input bind to set up camera controller\n");
            if ( errors & 0x8  )
                g_print_error("[G10] [Camera controller] Missing \"STRAFE RIGHT\" input bind to set up camera controller\n");
            if ( errors & 0x10 )
                g_print_error("[G10] [Camera controller] Missing \"MOUSE\" input bind to set up camera controller\n");
            
            // TODO: Free camera controller

            return 0;

        }

    }
}

int                   update_controlee_camera        ( void ) 
{

    // Context check
    {
        if (camera_controller == 0)
            goto no_active_camera_controller;
    }

    // Initialized data
    vec2                  l_orient     = { 0 };
    GXCamera_t           *camera       = camera_controller->camera;

    camera_controller->orientation = (vec2) { x_orient, y_orient };

    l_orient = camera_controller->orientation;

    const float camera_speed = 0.006f;

    if (v_ang > 89.0f)
        v_ang = 89.0f;
    if (v_ang < -89.0f)
        v_ang = -89.0f;

    if (h_ang > 179.99f)
        h_ang = -179.98f;
    if (h_ang < -179.99)
        h_ang = 179.98f;

    // look at
    if (l_orient.x > 0.f)
        add_vec3(&camera->location, camera->location, mul_vec3_f(normalize(cross_product_vec3(camera->target, camera->up)), camera_controller->spdlim));

    if (l_orient.x < 0.f)
        sub_vec3(&camera->location, camera->location, mul_vec3_f(normalize(cross_product_vec3(camera->target, camera->up)), camera_controller->spdlim));
    
    if (l_orient.y < 0.f)
        sub_vec3(&camera->location, camera->location, mul_vec3_f(camera->target, camera_controller->spdlim));

    if (l_orient.y > 0.f)
        add_vec3(&camera->location, camera->location, mul_vec3_f(camera->target, camera_controller->spdlim));

    camera->target.x = cosf(to_radians(h_ang)) * cosf(to_radians(v_ang));
    camera->target.z = sinf(to_radians(v_ang));
    camera->target.y = sinf(to_radians(h_ang)) * cosf(to_radians(v_ang));

    // Success
    return 1;

    // Error handling
    {
        no_active_camera_controller:
        #ifndef NDEBUG
            g_print_warning("[G10] [Camera controller] No active camera, set a camera with \"camera_controller_from_camera()\"\n");        
        #endif
        return 0;
    }
}