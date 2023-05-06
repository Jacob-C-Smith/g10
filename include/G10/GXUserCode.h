/** !
 * @file G10/GXUserCode.h
 * @author Jacob Smith
 * 
 * User code will call a function one time per frame. Use this for per frame game logic.
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// G10
#include <G10/G10.h>

/** !
 * Calls the user code callback function
 *
 * @param instance: The active instance
 *
 * @sa add_user_code_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int user_code              ( GXInstance_t *instance );

/** !
 * Set the user code callback. When the scheduler calls the "User Code" task,
 * user_code_callback is called.
 *
 * @param instance           : The active instance
 * @param user_code_callback : Pointer to a callback function.
 *
 * @sa user_code
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int add_user_code_callback ( GXInstance_t *instance, int ( *user_code_callback ) ( GXInstance_t *instance ) );