#pragma once
// TODO: Implement an FSA

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dict/dict.h>
#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

struct GXAI_s
{
	char  *name;
	dict  *state;
	char  *current_state;
	void (*pre_ai)(/*TODO: PARAMETERS*/);
};

// Allocators
int create_ai                  ( GXAI_t **ai );

// Constructors
int load_ai                    ( GXAI_t **ai, char *path );
int load_ai_as_json            ( GXAI_t **ai, char *token_text );

// User callbacks
int set_ai_state_callback      ( GXAI_t *ai, char  *state_name                             , int(*function_pointer)(/*TODO: PARAMETERS*/));
int set_ai_pre_update_callback ( GXAI_t *ai, int  (*function_pointer)(/*TODO: PARAMETERS*/));

// Deallocators
int destroy_ai                 ( GXAI_t *ai );