#pragma once

#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/transform.h>

struct entity_s
{
    char _name[255+1];
    transform *p_transform;
};

int entity_from_json ( entity **pp_entity, json_value *p_value );