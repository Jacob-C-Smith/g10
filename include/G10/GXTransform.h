#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// JSON parsing
#include <dict/dict.h>
#include <JSON/JSON.h>

// G10 includes
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXLinear.h>
#include <G10/GXQuaternion.h>

struct GXTransform_s
{
	vec3       location;
	quaternion rotation;
	vec3       scale;

	mat4       model_matrix;
};


int create_transform          ( GXTransform_t **transform );

int construct_transform       ( GXTransform_t **transform, vec3 location, quaternion rotation, vec3 scale );

void transform_model_matrix   ( GXTransform_t* transform, mat4* r);


int load_transform            ( GXTransform_t **transform, const char *path );
int load_transform_as_json    ( GXTransform_t **transform, char       *object_text, size_t len );

int destroy_transform         ( GXTransform_t  *transform );

