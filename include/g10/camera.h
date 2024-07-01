/** !
 * Header file for camera
 * 
 * @file g10/camera.h
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>

// array submodule
#include <array/array.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// Structure definitions
struct camera_s
{
    char _name[63 + 1];
    bool dirty;
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

// Constructors
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
DLLEXPORT int camera_construct
(
    camera **const pp_camera,
    vec3           location,
    vec3           front,
    vec3           up,
    float          near_clip,
    float          far_clip,
    float          aspect_ratio,
    float          fov
);

/** !
 * Load a camera from a json value
 *
 * @param pp_camera return
 * @param p_name    the name of the camera -OR- null
 * @param p_value   the camera json value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int camera_from_json
(
    camera           **const pp_camera,
    const char        *const p_name,
    const json_value  *const p_value
);

// Calculations
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
DLLEXPORT u0 camera_matrix_view
(
    mat4 *const p_view,
    vec3        eye,
    vec3        target,
    vec3        up
);

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
DLLEXPORT u0 camera_matrix_projection_perspective
(
    mat4  *const p_projection,
    float        fov,
    float        aspect,
    float        near_clip,
    float        far_clip
);

// Info
/** !
 *  Print information about a camera
 *
 * @param p_camera the camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int camera_info ( const camera *const p_camera );

// Destructors
/** !
 *  Destroy a camera
 *
 * @param pp_camera pointer to camera pointer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int camera_destroy ( camera **const pp_camera );
