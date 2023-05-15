/** !
 * @file G10/GXUI.h
 * @author Jacob Smith
 *
 * TODO
 *
 * Include header for user interface integration.
 */

// Include guard
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Standard library
#include <G10/G10.h>
#include <G10/GXCollider.h>
#include <G10/GXCollision.h>
#include <G10/GXEntity.h>

//#include <UI/UI.h>
/** !
 *  Update the user interface
 *
 * @param p_instance : active instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int update_ui ( GXInstance_t *p_instance );