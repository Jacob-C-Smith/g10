#pragma once

// standard library
#include <stdio.h>

// gsdk
#include <core/log.h>
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <texture.h>
#include <uniform.h>

// structure definitions
struct material_s
{
    char _name[63+1];

    // PBR Properties
    vec3 albedo_color;
    float roughness_value;
    float metallic_value;
    vec3 emission_color;

    // Texture Maps
    texture *p_albedo_map;
    texture *p_roughness_map;
    texture *p_metallic_map;
    texture *p_normal_map;
    texture *p_emission_map;
};

// function declarations

/** !
 * Construct a material from a JSON value
 * 
 * @param pp_material result
 * @param p_value     the json object
 * 
 * @return 1 on success, 0 on error
 */
int material_from_json ( material **pp_material, json_value *p_value );

/** !
 * Bind the material to a pipeline
 * 
 * @param p_render_pass the active render pass
 * @param p_pipeline    the active pipeline
 * @param p_material    the material to bind
 * 
 * @return 1 on success, 0 on error
 */
int material_bind ( render_pass *p_render_pass, pipeline *p_pipeline, material *p_material );

/** !
 * Print material info
 */
int material_info ( material *p_material );
