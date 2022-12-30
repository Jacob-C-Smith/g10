#pragma once
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXTexture.h>

#include <JSON/JSON.h>

struct GXMaterial_s {
    char   *name;
    dict   *textures;
	dict   *mutable_ints; // TODO
    dict   *mutable_floats; // TODO
};

// Allocators

/* !
 *  Allocate memory for a material
 *
 * @param pp_material : return
 *
 * @sa destroy_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_material         ( GXMaterial_t **material );

// Constructors
/* !
 *  Load a material from a JSON file
 *
 * @param pp_ai : return
 * @param path  : The path to a JSON file containing a material object
 *
 * @sa load_material_as_json
 * @sa create_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_material           ( GXMaterial_t **material, const char    path[] );

/* !
 *  Load a material from JSON text
 *
 * @param pp_material : return
 * @param token_text  : The material JSON object text
 * @param len         : The length of the material JSON object text
 *
 * @sa load_material_as_json
 * @sa create_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_material_as_json   ( GXMaterial_t **material, char         *token_text, size_t len );

// Destructors