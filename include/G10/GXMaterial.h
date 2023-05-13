/** !
 * @file G10/GXMaterial.h
 * @author Jacob Smith
 * 
 * Material
 */

// Include guard
#pragma once

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXTexture.h>

// json submodule
#include <json/json.h>

struct GXMaterial_s {
    char   *name;
    dict   *textures;
	dict   *mutable_ints; // TODO
    dict   *mutable_floats; // TODO
};

// Allocators

/** !
 *  Allocate memory for a material
 *
 * @param pp_material : return
 *
 * @sa destroy_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_material ( GXMaterial_t **pp_material );

// Constructors
/** !
 *  Load a material from a JSON file
 *
 * @param pp_material : return
 * @param path        : The path to a JSON file containing a material object
 *
 * @sa load_material_as_json
 * @sa create_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_material ( GXMaterial_t **pp_material, const char path[] );

/** !
 *  Load a material from JSON text
 *
 * @param pp_material : return
 * @param text        : The material JSON text
 *
 * @sa load_material
 * @sa create_material
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_material_as_json ( GXMaterial_t **pp_material, char *text );

// Destructors
// TODO