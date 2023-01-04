#include <G10/APS_3rdPersonCtrl.h>

APS_3rdPersonCtrl_t *thirdpersonctrl = 0;
float moveX, moveY;
float angX, angY;
float zoom;

void aps_3rdpersonctrl_forward(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == KEYBOARD)
        moveY = (state.inputs.key.depressed) ? 1 : 0;
}

void aps_3rdpersonctrl_backward(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == KEYBOARD)
        moveY = (state.inputs.key.depressed) ? -1 : 0;
}

void aps_3rdpersonctrl_strafe_left(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == KEYBOARD)
        moveX = (state.inputs.key.depressed) ? -1 : 0;
}

void aps_3rdpersonctrl_strafe_right(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == KEYBOARD)
        moveX = (state.inputs.key.depressed) ? 1 : 0;
}

void aps_3rdpersonctrl_up(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == MOUSE)
        angY += instance->time.delta_time * 100.f * fabsf(state.inputs.mouse_state.yrel);
}
void aps_3rdpersonctrl_down(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == MOUSE)
        angY -= instance->time.delta_time * 100.f * fabsf(state.inputs.mouse_state.yrel);
}
void aps_3rdpersonctrl_left(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == MOUSE)
        angX -= instance->time.delta_time * 100.f * fabsf(state.inputs.mouse_state.xrel);
}
void aps_3rdpersonctrl_right(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == MOUSE)
        angX += instance->time.delta_time * 100.f * fabsf(state.inputs.mouse_state.xrel);
}

void aps_3rdpersonctrl_zoom_out(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == KEYBOARD && !state.inputs.key.depressed)
        zoom += 5.0 * instance->time.delta_time;
}

void aps_3rdpersonctrl_zoom_in(callback_parameter_t state, GXInstance_t* instance)
{
    if (state.input_state == KEYBOARD && !state.inputs.key.depressed)
        zoom -= 5.0 * instance->time.delta_time;
}

APS_3rdPersonCtrl_t* create_aps_3rdpersonctrl()
{
    APS_3rdPersonCtrl_t* ret = calloc(1, sizeof(APS_3rdPersonCtrl_t));

    #ifndef NDEBUG
        if (!ret) {
            g_print_error("[%s] Unable to allocate memory for 3rd person controller\n", __FUNCTION__);
            return NULL;
        }
    #endif

    return ret;
}

int aps_3rdpersonctrl_from_camera_and_entity(GXInstance_t* instance, GXCamera_t* camera, GXEntity_t* entity)
{
    // Argument check
    {
    #ifndef NDEBUG
        if (instance == (void*)0) {
            g_print_error("[%s] Null pointer passed as 'instance' argument\n", __FUNCTION__);
            return 0;
        }
        if (camera == (void*)0) {
            g_print_error("[%s] Null pointer passed as 'camera' argument\n", __FUNCTION__);
            return 0;
        }
        if (entity == (void*)0) {
            g_print_error("[%s] Null pointer passed as 'entity' argument\n", __FUNCTION__);
            return 0;
        }

    #endif
    }

    APS_3rdPersonCtrl_t* ret = create_aps_3rdpersonctrl();

    // Movement binds
    GXBind_t* forward   = find_bind(instance->input, "FORWARD"),
            * backward  = find_bind(instance->input, "BACKWARD"),
            * strafeleft      = find_bind(instance->input, "STRAFE LEFT"),
            * straferight     = find_bind(instance->input, "STRAFE RIGHT");
    //Camera look binds
    GXBind_t* up = find_bind(instance->input, "UP"),
            * down = find_bind(instance->input, "DOWN"),
            * left = find_bind(instance->input, "LEFT"),
            * right = find_bind(instance->input, "RIGHT");
    //Zoom binds
    GXBind_t *zoomOut = find_bind(instance->input, "ZOOM OUT"),
             *zoomIn = find_bind(instance->input, "ZOOM IN");

    //Missing binds error check
    #ifndef NDEBUG
    if (!(forward && backward && left && right && up && down && left && right && zoomOut && zoomIn)) {
        g_print_error("[%s] Missing input binds\n", __FUNCTION__);
        return 0;
    }
    #endif

    //Bind assignment
    register_bind_callback(forward, &aps_3rdpersonctrl_forward);
    register_bind_callback(backward, &aps_3rdpersonctrl_backward);
    register_bind_callback(strafeleft, &aps_3rdpersonctrl_strafe_left);
    register_bind_callback(straferight, &aps_3rdpersonctrl_strafe_right);

    register_bind_callback(up, &aps_3rdpersonctrl_up);
    register_bind_callback(down, &aps_3rdpersonctrl_down);
    register_bind_callback(left, &aps_3rdpersonctrl_left);
    register_bind_callback(right, &aps_3rdpersonctrl_right);

    register_bind_callback(zoomOut, &aps_3rdpersonctrl_zoom_out);
    register_bind_callback(zoomIn, &aps_3rdpersonctrl_zoom_in);


    ret->camera = camera;
    ret->character = entity;
    thirdpersonctrl = ret;

    angX = 0;
    angY = -30;
    zoom = 10.f;

    return 1;
}

int update_aps_3rdpersonctrl(float delta_time)
{
#ifndef NDEBUG
    if (!thirdpersonctrl) {
        g_print_warning("[%s] No active 3rd person controller, use aps_3rdpersonctrl_from_camera_and_entity() to set one\n", __FUNCTION__);
        return 0;
    }
#endif

    //Camera angle bounding and wrapping
    if (angY > 89.0f)
        angY = 89.0f;
    if (angY < -89.0f)
        angY = -89.0f;

    if (angX > 179.99f)
        angX = -179.98f;
    if (angX < -179.99)
        angX = 179.98f;
    
    //Zoom bounding
    if (zoom < 2.f)
        zoom = 2.f;
    if (zoom > 50.f)
        zoom = 50.f;

    //Set camera target according to controller angle
    thirdpersonctrl->camera->target = (vec3){
        cosf(to_radians(angX)) * cosf(to_radians(angY)),
        sinf(to_radians(angX)) * cosf(to_radians(angY)),
        sinf(to_radians(angY))
    };

    //Move character in direction of camera angle
    GXTransform_t* characterPos = thirdpersonctrl->character->transform;
    characterPos->location.x += ((cosf(to_radians(angX)) * moveY) + (sin(to_radians(angX)) * -moveX)) * delta_time;
    characterPos->location.y += ((sinf(to_radians(angX)) * moveY) + (cos(to_radians(angX)) * moveX)) * delta_time;

    //Make camera follow player and zoom out camera
    float elevation = 2.5;
    thirdpersonctrl->camera->location = (vec3){ 
        characterPos->location.x - thirdpersonctrl->camera->target.x * zoom,
        characterPos->location.y - thirdpersonctrl->camera->target.y * zoom,
        characterPos->location.z - thirdpersonctrl->camera->target.z * zoom + elevation };

    return 0;
}
