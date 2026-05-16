#pragma once
#include <gtypedef.h>
#include <g10.h>
#include <texture.h>
#include <geometry.h>

struct skybox_s {
    char _name[63+1];
    texture *p_texture;
    geometry *p_geometry;
    char *pipeline;
};

/** !
 * Load a skybox from a json value
 * 
 * @param pp_skybox result
 * @param p_value   the skybox json value
 * 
 * @return 1 on success, 0 on error
 */
int skybox_from_json ( skybox **pp_skybox, json_value *p_value );

/** !
 * Bind the skybox to a pipeline
 * 
 * @param p_render_pass the active render pass
 * @param p_pipeline    the active pipeline
 * @param p_drawable    the skybox
 * 
 * @return 1 on success, 0 on error
 */
int skybox_bind ( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable );

/** !
 * Draw the skybox
 * 
 * @param p_render_pass the active render pass
 * @param p_pipeline    the active pipeline
 * @param p_drawable    the skybox
 * 
 * @return 1 on success, 0 on error
 */
int skybox_draw ( render_pass *p_render_pass, pipeline *p_pipeline, void *p_drawable );

/** !
 * Bind camera parameters for the skybox (removing translation)
 * 
 * @param p_render_pass the active render pass
 * @param p_pipeline    the active pipeline
 * 
 * @return 1 on success, 0 on error
 */
int skybox_camera_bind ( render_pass *p_render_pass, pipeline *p_pipeline );
