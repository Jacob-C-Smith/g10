// header guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>
#include <entity.h>
#include <camera.h>
#include <bv.h>

// structure definitions
struct scene_s
{
    char _name[63+1];
    dict *entities;
    dict *cameras;
    camera *p_active_camera;
    bv *p_bounds;
};

// function declarations
int scene_from_json ( scene **pp_scene, json_value *p_value );

int scene_info ( scene *p_scene );
