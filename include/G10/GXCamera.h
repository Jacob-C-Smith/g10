#pragma once
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>
#include <array/array.h>
#include <dict/dict.h>
#include <json/json.h>

struct GXCamera_s {

    // Name
	char *name;

    // View
    vec3  location,
          target,
          up;

    // Projection
    float fov,
          near_clip,
          far_clip,
          aspect_ratio;

    // Matricies
    mat4  view,
          projection;
};

// Allocators

/** !
 *  Allocate memory for a camera
 *
 * @param pp_camera : return
 *
 * @sa destroy_camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_camera ( GXCamera_t **pp_camera );

// Constructors
/** !
 *  Construct a camera from view and projection parameters
 *
 * @param pp_camera    : return
 * @param location     : The location of the camera in 3D space
 * @param front        : The vector pointing from the camera to where it is looking
 * @param up           : The camera's up vector
 * @param near_clip    : The near clipping plane
 * @param far_clip     : The far clipping plane
 * @param aspect_ratio : Window width divided by window height
 * @param fov          : The camera's field of view, in degrees
 *
 * @sa load_camera_as_json
 * @sa create_camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int construct_camera ( GXCamera_t **pp_camera, vec3 location, vec3 front, vec3 up, float near_clip, float far_clip, float aspect_ratio, float fov );

/** !
 *  Load a camera from a JSON file
 *
 * @param pp_camera : return
 * @param path      : The path to a JSON file containing a camera object
 *
 * @sa load_camera_as_json
 * @sa create_camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_camera ( GXCamera_t **pp_camera, const char *path );

/** !
 *  Load a camera from JSON text
 *
 * @param pp_camera  : return
 * @param token_text : The camera JSON object text
 * @param len        : The length of the camera JSON object text
 *
 * @sa load_camera_as_json
 * @sa create_camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_camera_as_json ( GXCamera_t **pp_camera, char *text, size_t len);

// Functions
/** !
 *  Construct a 4x4 view matrix from it's parameters
 *
 * @param eye    : The object's position in 3D space
 * @param target : The direction the object is pointing in
 * @param up     : The up vectors
 *
 * @sa perspective_matrix
 *
 * @return a view mat4 
 */
DLLEXPORT mat4 look_at ( vec3 eye, vec3 target, vec3 up );

/** !
 *  Construct a 4x4 perspective matrix from it's parameters
 *
 * @param fov       : The field of view of the camera
 * @param aspect    : The window's width divided by its height
 * @param near_clip : The near clipping plane
 * @param far_clip  : The far clipping plane
 *
 * @sa look_at
 *
 * @return a projection mat4 
 */
DLLEXPORT mat4 perspective_matrix ( float fov, float aspect, float near_clip, float far_clip );

// Accessors
/** !
 *  Get the position of the active instance's active scene's active camera
 *
 * @param ret : the pointer to the active... ya know
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int get_camera_position ( void *ret );

// Info
/** !
 *  Print information about a camera
 *
 * @param p_camera : the camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int print_camera ( GXCamera_t *p_camera );

// Deallocators
/** !
 *  Destroy a camera
 *
 * @param pp_camera : Pointer to pointer to camera to destroy
 *
 * @sa create_camera
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_camera ( GXCamera_t **pp_camera );