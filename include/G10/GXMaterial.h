#pragma once
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXTexture.h>

#include <JSON/JSON.h>

struct GXMaterial_s {
    char   *name;
    dict   *textures;
};

// Allocators
DLLEXPORT int create_material         ( GXMaterial_t **material );

// Constructors
DLLEXPORT int load_material           ( GXMaterial_t **material, const char    path[] );
DLLEXPORT int load_material_as_json   ( GXMaterial_t **material, char         *token_text);
DLLEXPORT int load_material_as_json_n ( GXMaterial_t **material, char         *token_text, size_t len );

