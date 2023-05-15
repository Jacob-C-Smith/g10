/** !
 * @file G10/GXCollision.h
 * @author Jacob Smith
 *
 * Collision events
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>

// G10
#include <G10/GXtypedef.h>
#include <G10/G10.h>

struct GXCollision_s
{
	GXEntity_t *a,
		       *b;
	size_t      begin_tick,
		        end_tick;
	bool        aabb_colliding,
		        convex_hull_colliding;
	vec3        a_collision_normal,
		        b_collision_normal,
		        a_in_b,
		        b_in_a;
	float       depth;
};

// Allocators
/** !
 *  Allocate memory for a collision event
 *
 * @param pp_collision : return
 *
 * @sa destroy_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  create_collision                  ( GXCollision_t **pp_collision );

// Constructors
/** !
 *  Construct a collision event from two colliding entities
 *
 * @param pp_collision : return
 * @param a            : an entity in the collision
 * @param b            : an entity in the collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  construct_collision_from_entities ( GXCollision_t **pp_collision, GXEntity_t *a, GXEntity_t *b );

// Collision testing
/** !
 * Test a collision event
 *
 * @param p_collision : the collision event
 *
 * @return true if colliding else false
 */
DLLEXPORT bool test_collision ( GXCollision_t *p_collision );

/** !
 * Test a collision event using axis aligned bounding boxex
 *
 * @param p_collision : the collision event
 *
 * @return true if colliding else false
 */
DLLEXPORT bool test_aabb ( GXCollision_t *p_collision );

/** !
 * Test a collision event using oriented bounding boxex
 *
 * @param p_collision : the collision event
 *
 * @return true if colliding else false
 */
DLLEXPORT bool test_obb ( GXCollision_t *p_collision );

/** !
 * Test a collision event using a convex hull
 *
 * @param p_collision : the collision event
 *
 * @return true if colliding else false
 */
DLLEXPORT bool test_convex_hull ( GXCollision_t *p_collision );

// Collision update
/** !
 *  Update a collision event
 *
 * @param p_collision : the collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_collision ( GXCollision_t *p_collision );

/** !
 *  Called when a collision event is finished
 *
 * @param p_collision : the collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int resolve_collision ( GXCollision_t *p_collision );

// Destructors
/** !
 *  Destroy a collision
 *
 * @param pp_collision : Pointer to collision pointer
 *
 * @sa create_collision
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int  destroy_collision ( GXCollision_t **pp_collision );