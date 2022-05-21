#pragma once
#include <G10/GXtypedef.h>
#include <G10/G10.h>
//#include <G10/GXTexture.h>
#include <JSON/JSON.h>

struct GXMaterial_s {
    char   *name;
    dict   *textures;
};

// Allocators
int create_material         ( GXMaterial_t **material );

// Constructors
int load_material           ( GXMaterial_t **material, const char    path[] );
int load_material_as_json   ( GXMaterial_t **material, char         *token_text);
int load_material_as_json_n ( GXMaterial_t **material, char         *token_text, size_t len );

