// header guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
#include <core/log.h>
#include <reflection/json.h>

// g10
#include <gtypedef.h>
#include <linear.h>

typedef enum {
    G10_LIGHT_TYPE_DIRECTIONAL = 0,
    G10_LIGHT_TYPE_POINT       = 1,
    G10_LIGHT_TYPE_SPOT        = 2
} g10_light_type;

struct light_s {
    char _name[64];
    g10_light_type type;
    vec3 location;
    vec3 direction;
    vec3 color;
    float intensity;
    float radius;
    float inner_angle;
    float outer_angle;
};

// GPU compatible structure
typedef struct {
    vec4 position;  // xyz, w = 0 for Dir, 1 for Point/Spot
    vec4 color;     // rgb, a = intensity
    vec4 direction; // xyz, w = inner angle
    vec4 params;    // x = radius, y = outer angle, z = type, w = padding
} light_gpu_t;

typedef struct {
    light_gpu_t lights[16];
    vec4 light_count_and_pad; // x = count, yzw = pad
    vec4 ambient_color;
} lighting_uniform_t;

// function declarations
int light_from_json ( light **pp_light, json_value *p_value );
int lighting_pack ( void *p_buffer, scene *p_scene );
const char *light_key_accessor ( const light *const p_light );
int lighting_bind_active ( render_pass *p_render_pass, pipeline *p_pipeline );
int lighting_bind_once ( render_pass *p_render_pass, pipeline *p_pipeline );
