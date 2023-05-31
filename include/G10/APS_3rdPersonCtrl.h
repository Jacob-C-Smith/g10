/** !
 * @file G10/G10.h
 * @author Jacob Smith
 *
 * Include header for G10.
 */

// Include guard
#pragma once

// Standard Library
#include <stdio.h>
#include <stdlib.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>

/* Aiden's third person character controller.
* Takes a reference to the character entity,
* and to the third person camera for the character.
* */
typedef struct APS_3rdPersonCtrl
{
    // Camera object
    GXCamera_t* camera;
    //Character entity
    GXEntity_t* character;

    float v_ang,
        h_ang,
        spdlim;

    // Displacement derivatives
    vec2  orientation;
    vec3  velocity,
        acceleration;

} APS_3rdPersonCtrl_t;

void aps_3rdpersonctrl_forward(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_backward(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_strafe_left(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_strafe_right(callback_parameter_t state, GXInstance_t* instance);

void aps_3rdpersonctrl_up(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_down(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_left(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_right(callback_parameter_t state, GXInstance_t* instance);

void aps_3rdpersonctrl_zoom_out(callback_parameter_t state, GXInstance_t* instance);
void aps_3rdpersonctrl_zoom_in(callback_parameter_t state, GXInstance_t* instance);

APS_3rdPersonCtrl_t* create_aps_3rdpersonctrl();

int aps_3rdpersonctrl_from_camera_and_entity(GXInstance_t* instance, GXCamera_t* camera, GXEntity_t* entity);
int update_aps_3rdpersonctrl(float delta_time);
