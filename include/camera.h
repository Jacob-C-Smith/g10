// headeer guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
/// core module
#include <core/log.h>
#include <core/interfaces.h>

/// data module
#include <data/array.h>
#include <data/dict.h>

/// reflection module
#include <reflection/json.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <linear.h>

// structure definitions
struct camera_s
{
    char _name[63 + 1];
    struct
    {
        vec3 location, target, up;
    } view;
    
    struct
    {
        float fov, near_clip, far_clip, aspect_ratio;
    } projection;
    
    struct
    {
        mat4 _view, _projection;
    } matrix;
};

// function declarations
/// constructors
/** !
 *  Construct a camera from view and projection parameters
 *
 * @param pp_camera    result
 * @param location     the location of the camera in 3D space
 * @param front        the vector from the camera to the subject
 * @param up           the up vector
 * @param near_clip    the distance to the near clipping plane
 * @param far_clip     the distance to the far clipping plane
 * @param aspect_ratio the viewport width divided by the viewport height
 * @param fov          the camera's field of view of
 *
 * @return 1 on success, 0 on error
 */
int camera_construct
(
    camera **pp_camera,
    vec3 location,
    vec3 front,
    vec3 up,
    float near_clip,
    float far_clip,
    float aspect_ratio,
    float fov
);

/** !
 * Load a camera from a json value
 *
 * @param pp_camera return
 * @param p_value   the camera json value
 *
 * @return 1 on success, 0 on error
 */
int camera_from_json ( camera **pp_camera, json_value *p_value );

/// computation 
/** !
 *  Construct a 4x4 view matrix 
 *
 * @param p_view result
 * @param eye    the camera's position in 3D space
 * @param target the vector from the camera to the subject
 * @param up     the up vector
 *
 * @sa perspective_matrix
 *
 * @return a view mat4
 */
u0 camera_matrix_view ( mat4 *p_view, vec3 eye, vec3 target, vec3 up );

/** !
 *  Construct a 4x4 perspective matrix from it's parameters
 *
 * @param p_projection result
 * @param fov          the camera's field of view of
 * @param aspect       the viewport width divided by the viewport height
 * @param near_clip    distance to the near clipping plane
 * @param far_clip     distance to the far clipping plane
 *
 * @return a 4x4 projection matrix
 */
u0 camera_matrix_projection_perspective ( mat4 *p_projection, float fov, float aspect, float near_clip, float far_clip );

/// info
/** !
 *  Print information about a camera
 *
 * @param p_camera the camera
 *
 * @return 1 on success, 0 on error
 */
int camera_info ( camera *p_camera );

/// pack
int camera_pack(void *p_buffer, camera *p_camera);
