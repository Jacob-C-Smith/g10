#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXPart.h>
#include <G10/GXTransform.h>
#include <G10/GXRigidbody.h>

struct GXEntity_s
{
	char          *name;
	dict          *parts;
	dict          *materials;
	GXShader_t    *shader;
	GXTransform_t *transform;
	GXRigidbody_t *rigidbody;
};

DLLEXPORT int create_entity ( GXEntity_t** entity );

DLLEXPORT int load_entity ( GXEntity_t** entity, char* path );

DLLEXPORT int load_entity_as_json ( GXEntity_t** entity, char* token_text, size_t len );

DLLEXPORT int draw_entity ( GXEntity_t *entity );