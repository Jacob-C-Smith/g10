#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include <dict/dict.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXScene.h>
#include <G10/GXPhysics.h>
#include <G10/GXRenderer.h>
#include <G10/GXInput.h>
#include <G10/GXServer.h>
#include <G10/GXUI.h>
#include <G10/GXUserCode.h>

struct GXTask_s
{
	char  *name;
	char  *wait_thread;
	char  *wait_task;
	int  (*function_pointer)(GXInstance_t*);
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
	size_t       task_count,
		        *complete_tasks;
	bool         running;
	GXTask_t   **tasks;
	SDL_Thread  *thread;
};


// Allocators

/* !
 *  Allocate memory for a schedule
 *
 * @param pp_schedule : Double pointer to schedule
 *
 * @sa destroy_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_schedule        ( GXSchedule_t **pp_schedule );

/* !
 *  Allocate memory for a thread
 *
 * @param pp_thread : Double pointer to schedule
 *
 * @sa destroy_thread
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_thread          ( GXThread_t    **pp_thread );

// Constructors

/* !
 *  Load a schedule from a JSON file
 *
 * @param pp_schedule : Double pointer to a schedule
 * @param path        : The path to a JSON file containing an schedule object
 *
 * @sa load_schedule_as_json
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_schedule          ( GXSchedule_t **schedule, char *path );

/* !
 *  Load a schedule from a JSON file
 *
 * @param pp_schedule : return
 * @param token_text  : The schedule JSON object text
 * @param len         : The length of the schedule JSON object text
 *
 * @sa load_schedule
 * @sa create_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_schedule_as_json  ( GXSchedule_t **schedule, char *token_text, size_t len );

DLLEXPORT int load_thread            ( GXThread_t   **thread, char *path );


/* !
 *  Load a thread from a JSON file
 *
 * @param pp_thread   : Double pointer to a thread
 * @param token_text  : The thread JSON object text
 * @param len         : The length of the schedule JSON object text
 *
 * @sa load_ai_as_json
 * @sa create_ai
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_thread_as_json    ( GXThread_t    **thread , char *token_text, size_t len );

// Schedule

/* !
 *  Start running a schedule
 *
 * @param p_schedule  : Pointer to a schedule
 *
 * @sa stop_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int start_schedule         ( GXSchedule_t  *schedule );

/* !
 *  Stop running a schedule
 *
 * @param p_schedule  : Pointer to a schedule
 *
 * @sa start_schedule
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int stop_schedule          ( GXSchedule_t  *schedule );



// Destructors
DLLEXPORT int destroy_schedule       ( GXSchedule_t **schedule );
DLLEXPORT int destroy_thread         ( GXThread_t    **thread );
