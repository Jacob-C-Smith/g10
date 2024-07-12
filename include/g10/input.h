/** !
 * 
 * 
 * @file g10/input.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// g10
#include <g10/g10.h>
#include <g10/gtypedef.h>

// Enumeration definitions
enum gamepad_input_e
{
    INPUT_GAMEPAD_A            = 0,
    INPUT_GAMEPAD_B            = 1,
    INPUT_GAMEPAD_X            = 2,
    INPUT_GAMEPAD_Y            = 3,
    INPUT_GAMEPAD_D_UP         = 4,
    INPUT_GAMEPAD_D_RIGHT      = 5,
    INPUT_GAMEPAD_D_DOWN       = 6,
    INPUT_GAMEPAD_D_LEFT       = 7,
    INPUT_GAMEPAD_BUMPER_LEFT  = 8,
    INPUT_GAMEPAD_BUMPER_RIGHT = 9,
    INPUT_GAMEPAD_START        = 10,
    INPUT_GAMEPAD_SELECT       = 11,
    INPUT_GAMEPAD_QUANTITY     = 12,
};

enum input_state_e
{
    INPUT_STATE_INVALID  = 0,
	INPUT_STATE_KEYBOARD = 1,
	INPUT_STATE_MOUSE    = 2,
	INPUT_STATE_GAMEPAD  = 3
};

// Structure definitions
struct input_bind_s
{
    char   _name[63 + 1];
    float   value;
    size_t  scancode_quantity;
    char   _scancode[][16];
};

struct input_s
{
    char _name[63 + 1];
    float mouse_sensitivity;
    size_t bind_quantity;
    input_bind *_p_binds[];
};

struct callback_parameter_s
{
	enum input_state_e input_state;

	union
    {
		struct { bool depressed; } key;

		struct 
        {
			s32 xrel, yrel;
			u8  button;
		} mouse;

		struct
        {
            struct { vec2  left, right; } stick;
            struct { float left, right; } trigger;
            bool buttons[INPUT_GAMEPAD_QUANTITY];
		} gamepad;
	} inputs;
};

// Function declarations

// Constructors
/** !
 * Construct an input from a json value 
 * 
 * @param pp_input return
 * @param p_value  the json value
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int input_from_json ( input **pp_input, const json_value *const p_value );

// Info
/** !
 *  Print information about an input
 *
 * @param p_input the input
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int input_info ( const input *const p_input );

// Input poll
/** !
 * Update the binds of an instance
 * 
 * @param p_instance the active instance
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int input_poll ( g_instance *p_instance );
