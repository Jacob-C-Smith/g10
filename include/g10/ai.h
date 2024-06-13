/** !
 * G10 finite state automaton
 *
 * @file g10/ai.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// dict module
#include <dict/dict.h>

// json module
#include <json/json.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

struct ai_s
{
	char _name[64],
	     _current_state[64];
};

// Constructors
/** !
 *  Load an AI from the filesystem
 *
 * @param pp_ai  return
 * @param p_path The path to the file
 *
 * @sa ai_load
 * @sa ai_load_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_load ( ai **const pp_ai, const char *p_path );

/** !
 *  Load an AI from a json value
 *
 * @param pp_ai   return
 * @param p_value the json value
 *
 * @sa ai_load
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_load_as_json_value ( ai **pp_ai, const json_value *const p_value );

/** !
 *  Clone an AI
 *
 * @param pp_ai return
 * @param p_ai  the AI
 *
 * @sa ai_create
 * @sa ai_load
 * @sa ai_load_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_clone ( ai **pp_ai, ai *p_ai );

// Info
/** !
 *  Print info about an AI
 *
 * @param p_ai the AI
 *
 * @sa ai_load
 * @sa ai_load_as_json_value
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_info ( const ai *const p_ai );

// Tasks
/** !
 *  Update each AI in the active scene
 *
 * @param p_instance the active instance
 *
 * @sa ai_preupdate
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_update ( g_instance *p_instance );

/** !
 *  Preupdate each AI in the active scene
 *
 * @param p_instance the active instance
 *
 * @sa ai_update
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_preupdate ( g_instance *p_instance );

// User callbacks
/** !
 *  Add a callback function for a state
 *
 * @param p_ai             Pointer to AI
 * @param state_name       The name of the state
 * @param pfn_state_update Pointer to a function with GXEntity_t pointer argument and int return
 *
 * @sa set_ai_state
 * @sa set_ai_pre_update_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_add_state_callback ( ai *p_ai, char *state_name );

/** !
 *  Changes the state of the AI
 *
 * @param p_ai             : Pointer to AI
 * @param state_name       : The name of the state
 *
 * @sa add_ai_state_callback
 * @sa set_ai_pre_update_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_set_state ( ai *p_ai, const char *state_name );

/** !
 *  Set the preupdate function for an AI
 *
 * @param p_ai          the AI
 * @param pfn_preupdate the preupdate function
 *
 * @sa ai_add_state_callback
 * @sa ai_set_state
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_set_preupdate_callback ( ai *p_ai );

// Destructors
/** !
 *  Free an AI and all its contents
 *
 * @param pp_ai pointer to AI pointer
 *
 * @sa ai_create
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int ai_destroy ( ai **pp_ai );
