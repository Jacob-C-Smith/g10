#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXInput.h>

struct GXTask_s
{
	char* name;
	int (*function_pointer)(GXInstance_t*);
	//(*function_pointer)(active_instance, etc);
};
typedef struct GXTask_s GXTask_t;


struct GXScheduler_s
{
	char *name;
	dict *threads;
};

struct GXThread_s
{
	char      *name;
	GXTask_t **tasks;
	 
};


// Allocators
int create_scheduler       ( GXScheduler_t **scheduler);
int create_thread          ( GXThread_t    **thread );

// Constructors
int load_scheduler         ( GXScheduler_t **scheduler, char *path );
int load_scheduler_as_json ( GXScheduler_t **scheduler, char *token_text, size_t len );
int load_thread_as_json    ( GXThread_t    **thread   , char *token_text, size_t len );
int construct_scheduler    ( GXScheduler_t **scheduler, dict *threads );
int construct_thread       ( GXThread_t    **thread   , char *task );

// Destructors
int destroy_scheduler       ( GXScheduler_t **scheduler );
int destroy_thread          ( GXThread_t    **thread );
