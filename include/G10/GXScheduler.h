#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <dict/dict.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXPhysics.h>
#include <G10/GXInput.h>

struct GXTask_s
{
	char* name;
	int (*function_pointer)(GXInstance_t*);
};
typedef struct GXTask_s GXTask_t;

struct GXScheduler_s
{
	char *name;
	dict *threads;
};

struct GXThread_s
{
	char        *name;
	size_t       task_count;
	bool         running;
	GXTask_t   **tasks;
	SDL_Thread  *thread;
};


// Allocators
int create_schedule        ( GXSchedule_t **schedule );
int create_thread          ( GXThread_t    **thread );

// Constructors
int load_schedule          ( GXSchedule_t **schedule, char *path );
int load_schedule_as_json  ( GXSchedule_t **schedule, char *token_text, size_t len );
int load_thread_as_json    ( GXThread_t    **thread , char *token_text, size_t len );
int construct_schedule     ( GXSchedule_t **schedule, dict *threads );
int construct_thread       ( GXThread_t    **thread , char *task );

// Schedule
int start_schedule         ( GXSchedule_t  *schedule );
int stop_schedule          ( GXSchedule_t  *schedule );

// Thread 
int start_threads          ( GXSchedule_t  *scheduler );

// Destructors
int destroy_schedule       ( GXSchedule_t **scheduler );
int destroy_thread         ( GXThread_t    **thread );
