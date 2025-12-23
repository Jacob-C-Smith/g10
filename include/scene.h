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

// structure definitions
struct scene_s
{
    char _name[63+1];
    dict *entities;
};

// function declarations
int scene_from_json ( scene **pp_scene, json_value *p_value );
