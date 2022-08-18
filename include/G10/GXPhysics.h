#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/G10.h>
#include <G10/GXCollider.h>
#include <G10/GXCollision.h>
#include <G10/GXEntity.h>

DLLEXPORT int detect_collisions ( GXInstance_t *instance );
DLLEXPORT int move_objects      ( GXInstance_t *instance );
DLLEXPORT int update_forces     ( GXInstance_t *instance );
DLLEXPORT int update_rigs       ( GXInstance_t *instance );