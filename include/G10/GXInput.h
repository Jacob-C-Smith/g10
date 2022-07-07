#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

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
	bool             active;
	char            *name,
	   	           **keys;
	void           **callbacks;
	size_t           key_count,
		             callback_count,
		             callback_max;
	struct GXBind_s *next;
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
	MOUSE    = 2
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
	} inputs;
};

// Allocators
int          create_input             ( GXInput_t   **input );
int          create_bind              ( GXBind_t    **bind );

// Constructors
int          load_input               ( GXInput_t   **input, const char  path[] );
int          load_input_as_json       ( GXInput_t   **input, char       *token );
int          load_input_as_json_n     ( GXInput_t   **input, char       *token_text, size_t   len );
int          load_bind_as_json        ( GXBind_t    **bind , char       *token );
int          construct_bind           ( GXBind_t    **bind , char       *name      , char   **keys );

// Callback addition
int          register_bind_callback   ( GXBind_t *bind       , void                 *function_pointer );

// Callback remove
int          unregister_bind_callback ( GXBind_t *bind       , void                 *function_pointer );

// Keycode finder
SDL_Scancode find_key                 ( const char *name );

// Key information
int          print_all_keys           ( void );
int          print_all_binds          ( GXInput_t    *inputs );

// Input processing
int          process_input            ( GXInstance_t *instance );

// Bind appending
int          append_bind              ( GXInput_t *input, GXBind_t *bind );

// Bind firing
int          fire_bind                ( GXBind_t     *bind    , callback_parameter_t input, GXInstance_t* instance);

// Bind operations
GXBind_t    *find_bind                ( GXInput_t    *input   , char                *name );
int          remove_bind              ( GXInput_t    *input   , GXBind_t            *bind );

// Deallocators
int          destroy_bind             ( GXBind_t     *bind );
int          destroy_input            ( GXInput_t    *input ); 