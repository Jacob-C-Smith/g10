#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include <SDL2/SDL.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <JSON/JSON.h>

#define UI_M1     0x01
#define UI_M2     0x02
#define UI_M3     0x04
#define UI_M4     0x08
#define UI_M5     0x10
#define UI_SWUP   0x20
#define UI_SWDOWN 0x40

struct GXBind_s
{
	bool              active;
	char             *name,
	   	            **keys;
	void            **callbacks;
	size_t            key_count,
		              callback_count,
		              callback_max;
	struct GXBind_s  *next;
};

struct GXInput_s
{
	char *name;
	dict *bind_lut,
		 *binds;
};

enum input_state_e
{
	KEYBOARD = 1,
	MOUSE    = 2,
	GAMEPAD  = 3
};

struct callback_parameter_s
{
	enum input_state_e input_state;
	union {
		struct {
			bool depressed;
		} key;

		struct mouse_state_s {
			u32 xrel,
				yrel;
			u8  button;
		} mouse_state;
		
		// A la xbox one controller
		struct {

			bool A;
			bool B;
			bool X;
			bool Y;

			bool dpad_down;
			bool dpad_left;
			bool dpad_right;
			bool dpad_up;

			vec2 left_stick;
			vec2 right_stick;

			float left_trigger;
			float right_trigger;

			bool left_bumper;
			bool right_bumper;

			bool start;
			bool select;

		} gamepad_state;
	} inputs;
};

// Allocators
DLLEXPORT int          create_input             ( GXInput_t   **input );
DLLEXPORT int          create_bind              ( GXBind_t    **bind );

// Constructors
DLLEXPORT int          load_input               ( GXInput_t   **input, const char  path[] );
DLLEXPORT int          load_input_as_json       ( GXInput_t   **input, char       *token );
DLLEXPORT int          load_input_as_json_n     ( GXInput_t   **input, char       *token_text, size_t   len );
DLLEXPORT int          load_bind_as_json        ( GXBind_t    **bind , char       *token );
DLLEXPORT int          construct_bind           ( GXBind_t    **bind , char       *name      , char   **keys );
 
// Callback addition
DLLEXPORT int          register_bind_callback   ( GXBind_t *bind       , void                 *function_pointer );

// Callback remove
DLLEXPORT int          unregister_bind_callback ( GXBind_t *bind       , void                 *function_pointer );

// Keycode finder
DLLEXPORT SDL_Scancode find_key                 ( const char *name );

// Key information
DLLEXPORT int          print_all_keys           ( void );
DLLEXPORT int          print_all_binds          ( GXInput_t    *inputs );

// Input processing
DLLEXPORT int          process_input            ( GXInstance_t *instance );

// Bind appending
DLLEXPORT int          append_bind              ( GXInput_t *input, GXBind_t *bind );

// Bind firing
DLLEXPORT int          fire_bind                ( GXBind_t     *bind    , callback_parameter_t input, GXInstance_t* instance);

// Bind operations
DLLEXPORT GXBind_t    *find_bind                ( GXInput_t    *input   , char                *name );
DLLEXPORT int          remove_bind              ( GXInput_t    *input   , GXBind_t            *bind );

// Deallocators
DLLEXPORT int          destroy_bind             ( GXBind_t     *bind );
DLLEXPORT int          destroy_input            ( GXInput_t    *input );