/** !
 * User code will call a function one time per frame. Use this for per frame game logic.
 * 
 * @file g10/user_code.h
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// G10
#include <g10/g10.h>

/** !
 *  Calls the user code callback function
 *
 * @param p_instance: The active instance
 *
 * @sa add_user_code_callback
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int user_code_callback ( g_instance *p_instance );

/** !
 *  Set the user code callback. When the scheduler calls the "User Code" task,
 *  user_code_callback is called.
 *
 * @param p_instance         : The active instance
 * @param user_code_callback : Pointer to a callback function.
 *
 * @sa user_code
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int user_code_callback_set ( g_instance *p_instance, fn_user_code_callback pfn_user_code_callback );
