// header guard
#pragma once

// standard library
#include <stdio.h>
#include <math.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <transform.h>
#include <geometry.h>
#include <texture.h>
#include <render_pass.h>
#include <sampler.h>
#include <material.h>

// structure definitions
struct entity_s
{
    char _name[63+1];
    transform *p_transform;
    geometry *p_geometry;
    material *p_material;
    mat3 _inv_normal;
    char *pipeline;
};

// function declarations
int entity_info ( entity *p_entity );

int entity_from_json ( entity **pp_entity, json_value *p_value );

int entity_bind ( render_pass *p_render_pass, pipeline *p_pipeline, entity *p_entity );