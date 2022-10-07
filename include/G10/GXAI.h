#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dict/dict.h>
#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

struct GXAI_s
{
	char  *name;
	dict  *states;
	char  *current_state;
	void (*pre_ai)(GXEntity_t*);
};

// Allocators

/* !
 *  Allocate memory for an AI
 *
 * @param pp_ai : Double pointer to AI
 *
 * @sa destroy_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_ai                  ( GXAI_t **pp_ai );

// Constructors

/* !
 *  Load an AI from a JSON file
 *
 * @param pp_ai : Double pointer to AI
 * @param path  : The path to a JSON file containing an AI object
 *
 * @sa load_ai_as_json
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_ai                    ( GXAI_t **pp_ai, char *path );

/* !
 *  Load an AI from JSON text
 *
 * @param pp_ai      : Double pointer to AI
 * @param token_text : The AI JSON object text
 * @param len        : The length of the AI JSON object text
 *
 * @sa load_ai_as_json
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_ai_as_json            ( GXAI_t **pp_ai, char *token_text, size_t len );

// Tasks

/* !
 *  Update each AI entity in the instances active scene
 *
 * @param instance         : Pointer to instance
 *
 * @sa pre_update_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_ai                  ( GXInstance_t *instance );

/* !
 *  Preupdate each AI entity in the instances active scene
 *
 * @param instance         : Pointer to instance
 *
 * @sa update_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int pre_update_ai              ( GXInstance_t *instance );

// User callbacks

/* !
 *  Add a callback function for a state
 *
 * @param p_ai             : Pointer to AI
 * @param state_name       : The name of the state
 * @param function_pointer : Pointer to a function with GXEntity_t pointer argument and int return
 *
 * @sa set_ai_state
 * @sa set_ai_pre_update_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int add_ai_state_callback      ( GXAI_t  *p_ai, char       *state_name, int(*function_pointer)(GXEntity_t *entity));

/* !
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
DLLEXPORT int set_ai_state               ( GXAI_t  *p_ai, const char *state_name );

/* !
 *  Changes the AI preupdate function
 *
 * @param p_ai             : Pointer to AI
 * @param function_pointer : Pointer to a function that with GXEntity_t * argument and int return
 *
 * @sa add_ai_state_callback
 * @sa set_ai_state
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int set_ai_pre_update_callback ( GXAI_t  *p_ai, int  (*function_pointer)(GXEntity_t *entity) );

// Deallocators

/* !
 *  Free an AI and all its contents
 *
 * @param p_ai : Pointer to AI
 *
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_ai                 ( GXAI_t  *p_ai );