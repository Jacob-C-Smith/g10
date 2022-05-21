#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/GXtypedef.h>


struct GXEntity_s
{
	char *name;
};

int load_entity(GXEntity_t** entity, char* path);

int load_entity_as_json(GXEntity_t** entity, char* token_text);