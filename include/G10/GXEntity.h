#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXPart.h>
#include <G10/GXTransform.h>
#include <G10/GXRigidbody.h>
#include <G10/GXCollider.h>
#include <G10/GXAI.h>

struct GXEntity_s
{
	char          *name;
	dict          *parts;
	dict          *materials;
	GXShader_t    *shader;
	GXTransform_t *transform;
	GXRigidbody_t *rigidbody;
	GXCollider_t  *collider;
	GXAI_t        *ai;
};

// Allocators

/* !
 *  Allocate memory for an entity
 *
 * @param pp_entity : Double pointer to entity
 *
 * @sa destroy_entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_entity          ( GXEntity_t **pp_entity );

// Constructors

/* !
 *  Load an entity from a JSON file
 *
 * @param pp_entity : Double pointer to entity
 * @param path      : The path to a JSON file containing an entity object
 *
 * @sa destroy_entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_entity            ( GXEntity_t **pp_entity, char* path );

/* !
 *  Load an entity as JSON object text
 *
 * @param pp_entity  : Double pointer to entity
 * @param token_text : The entity JSON object text
 * @param len        : The length of the entity JSON object text
 *
 * @sa destroy_entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_entity_as_json    ( GXEntity_t **pp_entity, char* token_text, size_t len );

// Physics

/* !
 * Calculate the net force on the object.
 * 6 Contact forces are summated Gravity,
 * Normal, Applied, Friction, Tension, Spring. 
 *
 * @param p_entity : Pointer to entity
 * 
 * @sa move_entity
 * TODO: See also, collision function
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int calculate_entity_force ( GXEntity_t *p_entity );

/* !
 * Updates location, and rotation, and their first and second order derivatives from force and torque
 *
 * @param p_entity : Pointer to entity
 *
 * @sa calculate_entity_force
 * TODO: See also, collision function
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int move_entity            ( GXEntity_t *p_entity );

// AI

/* !
 * Updates AI State
 *
 * @param p_entity : Pointer to entity
 *
 * @sa update_entity_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int preupdate_entity_ai    ( GXEntity_t *p_entity );

/* !
 * Run state update function
 *
 * @param p_entity : Pointer to entity
 *
 * @sa preupdate_entity_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_entity_ai       ( GXEntity_t *p_entity );

// Renderer

/* !
 * Draw each part with its respective shader and material
 *
 * @param p_entity : Pointer to entity
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int draw_entity            ( GXEntity_t *p_entity );

// Destructor
/* !
 * Frees an entity, and all its contents
 *
 * @param p_entity : Pointer to entity
 *
 * @sa create_entity
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int destroy_entity         ( GXEntity_t *p_entity );