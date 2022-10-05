#pragma once
#include <stdio.h>
#include <stdlib.h>

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
DLLEXPORT int  create_collision                  ( GXCollision_t **pp_collision );

// Constructors
DLLEXPORT int  construct_collision_from_entities ( GXCollision_t **pp_collision, GXEntity_t *a, GXEntity_t *b );

// Collision testing
DLLEXPORT bool test_collision                    ( GXCollision_t *collision );
DLLEXPORT bool test_aabb                         ( GXCollision_t *collision );
DLLEXPORT bool test_obb                          ( GXCollision_t *collision );
DLLEXPORT bool test_convex_hull                  ( GXCollision_t *collision );

// Collision update
DLLEXPORT int  update_collision                  ( GXCollision_t *collision );
DLLEXPORT int  resolve_collision                 ( GXCollision_t *collision );

// Destructors
DLLEXPORT int  destroy_collision                 ( GXCollision_t *collision );