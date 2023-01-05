#include <G10/GXScheduler.h>

#define TASK_COUNT 23

#ifdef BUILD_G10_WITH_DISCORD
#include <G10/GXDiscordIntegration.h>
#endif

dict* scheduler_tasks = 0;

char* task_names[TASK_COUNT] = {
	"Input",
	"UI",
	"AI",
	"Pre AI",
	"User Code",
	"Resolve Collisions",
	"Update Forces",
	"Move Objects",
	"Animation",
	"Render",
	"Present",
	"Load Entity",
	"Load Light Probe",
	"Copy State",
	"Server Recieve",
	"Server Send",
	"Server Parse",
	"Server Serialize",
	"Server Process",
	"Server Wait",
	"Audio",
	#ifdef BUILD_G10_WITH_DISCORD
	"Discord Callbacks",
	#else
	0,
	#endif
	0
};

void* task_function_pointers[TASK_COUNT] = {
	&process_input,
	(void*) 0,
	&update_ai, 
	&pre_update_ai,
	&user_code,
	&detect_collisions,
	&update_forces,
	&move_objects,
	&update_rigs,
	&render_frame,
	&present_frame,
	&load_entity_from_queue, 
	&load_light_probe_from_queue, 
	&copy_state,
    &server_recv, 
	&server_send, 
	&server_parse,
	&server_serialize,
	&server_process,
	&server_wait, 
	0,	// process_audio
	#ifdef BUILD_G10_WITH_DISCORD
	&discord_callbacks,
	#else
	0,
	#endif,
	0
};

void init_scheduler        ( void )
{
	
	// Initialized data
	GXInstance_t *instance = g_get_active_instance();

	// Construct a function lookup table
	dict_construct(&scheduler_tasks, TASK_COUNT);

	// Iterate over each task
	for (size_t i = 0; i < TASK_COUNT; i++)
		
		// Add each task to the lookup table
		dict_add(scheduler_tasks, task_names[i], task_function_pointers[i]);

}

int create_task            ( GXTask_t     **pp_task )
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_task == (void *)0)
				goto no_task;
		#endif
	}

	// Initialized data
	GXTask_t *p_task = calloc(1, sizeof(GXTask_t));

	// Error check
	{
		#ifndef NDEBUG
			if ( p_task == (void *)0 )
				goto no_mem;
		#endif
	}

	// Return a pointer to the allocated memory
	*pp_task = p_task;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_task:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"pp_task\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}
	}
}

int create_schedule        ( GXSchedule_t **pp_schedule )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_schedule == (void *) 0 )
				goto no_schedule;
		#endif
	}

	// Initialized data
	GXSchedule_t *p_scheduler = calloc(1, sizeof(GXSchedule_t));

	// Error checking
	{
		#ifndef NDEBUG
			if (p_scheduler == (void *) 0 )
				goto no_mem;
		#endif
	}

	// Return a pointer to the allocated memory
	*pp_schedule = p_scheduler;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}
	}
}

int create_thread          ( GXThread_t   **pp_thread )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_thread == (void *) 0 )
				goto no_thread;
		#endif
	}

	// Initialized data
	GXThread_t *p_thread = calloc(1, sizeof(GXThread_t));

	// Error checking
	{
		#ifndef NDEBUG
			if (p_thread == (void *) 0 )
				goto no_mem;
		#endif
	}

	// Return a pointer to the allocated memory
	*pp_thread = p_thread;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_thread:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"pp_schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}
	}
}

int load_schedule          ( GXSchedule_t **schedule, char* path)
{

	// Argument check
	{
		if ( schedule == (void *) 0 )
			goto no_schedule;
		if ( path     == (void *) 0 )
			goto no_path;
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len+1, sizeof(char));
	
	// Error checking
	{
		#ifndef NDEBUG
			if (token_text == (void *) 0 )
				goto no_mem;
		#endif
	}

	// Load the file
	if ( g_load_file(path, token_text, false) == 0)
		goto failed_to_load_file;

	// Construct the schedule from the file contents
	if ( load_schedule_as_json(schedule, token_text, len) == 0 )
		goto failed_to_load_schedule;

	// Free the file contents
	free(token_text);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					g_print_log("[G10] [Scheduler] Null pointer provided for \"schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_log("[G10] [Scheduler] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}

		// G10 Errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif
			
			// Error
			return 0;

			failed_to_load_schedule:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Failed to load scheduler from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}
	}
}

int load_schedule_as_json  ( GXSchedule_t **pp_schedule, char *token_text, size_t len )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_schedule == (void*)0)
				goto no_scheduler;
			if(token_text == (void *)0)
				goto no_token_text;
		#endif
	}

	// Initialized data
	dict    *scheduler_json      = 0;
	char    *name                = 0;
	size_t   thread_count        = 0;
	char   **thread_json_objects = 0;

	// Parse the JSON
	{

		// Initiaized data
		JSONToken_t *token = 0;

		// Parse the JSON into a dictionary
		parse_json(token_text, len, &scheduler_json);

		// Get the name
		token               = dict_get(scheduler_json, "name");
		name                = JSON_VALUE(token, JSONstring);

		// Get the threads
		token               = dict_get(scheduler_json, "threads");
		thread_json_objects = JSON_VALUE(token, JSONarray);

	}

	// Construct the schedule
	{

		// Initialized data
		GXSchedule_t *p_scheduler = 0;

		// Allocate a schedule
		if ( create_schedule(pp_schedule) == 0 )
			goto failed_to_create_schedule;

		// Return a pointer to the allocated memory
		p_scheduler = *pp_schedule;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for name
			p_scheduler->name = calloc(name_len + 1, sizeof(char));
			
			// Error checking
			{
				#ifndef NDEBUG
					if ( p_scheduler->name == (void *) 0 )
						goto no_mem;
				#endif
			}

			// Copy name
			strncpy(p_scheduler->name, name, name_len);
		}

		// Construct each thread
		{
			
			// Count up the blocks
			while (thread_json_objects[++thread_count]);

			// Construct a dictionary for the threads
			dict_construct(&p_scheduler->threads, 16);

			// Iterate over each JSON object
			for (size_t i = 0; i < thread_count; i++)
			{

				// Initialized data
				GXThread_t *thread = 0;

				// Load the thread from the JSON
				if ( load_thread_as_json(&thread, thread_json_objects[i], strlen(thread_json_objects[i])) == 0 )
					goto failed_to_load_thread;

				// Add the thread to the dictionary
				dict_add(p_scheduler->threads, thread->name, thread);

			}
		}
	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_scheduler:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"schedule\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;
		}

		// G10 Errors
		{
			failed_to_create_schedule:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Failed to create schedule in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;

			failed_to_load_thread:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Failed to load thread in call to function \"%s\"\n", __FUNCTION__);
				#endif
			
			// Error
			return 0;

		}
	}
}

int client_work            ( GXClient_t    *p_client )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (p_client == (void *)0 )
				goto no_client;
		#endif
	}

	// Initialized data
	GXThread_t    *thread   = p_client->thread;
	GXTask_t     **tasks    = thread->tasks;
	GXInstance_t  *instance = g_get_active_instance();

	// Run until told otherwise
	while (thread->running)
	{

		// Iterate over each task in the thread
		for (size_t i = 0; i < thread->task_count; i++)
		{

			// Wait on other things to finish?
			if (thread->tasks[i]->wait_thread)
			{

				// Initialized data
				GXThread_t *wait_thread = dict_get(instance->context.schedule->threads, thread->tasks[i]->wait_thread);
				GXTask_t   *wait_task   = 0;
				size_t      v           = 0;

				// Figure out what task we are waiting on
				for (size_t j = 0; j < wait_thread->task_count; j++)
				{
					if (strcmp(thread->tasks[i]->wait_task, wait_thread->tasks[j]->name) == 0)
						v = j;
				}

				// Wait for the task to finish
				while (wait_thread->complete_tasks[i] == 0);

			}

			// Declare the task function
			int (*function_pointer)(GXClient_t*) = thread->tasks[i]->function_pointer;

			// Run the function
			if(function_pointer)
				function_pointer(p_client);

			// Update the task
			thread->complete_tasks[i] = 1;
		}

		// Reset each frame
		for (size_t i = 0; i < thread->task_count; i++)
			thread->complete_tasks[i] = 0;
	}

	// Success
	return 1;

	no_client:
	return 0;
}

int work                   ( GXThread_t    *thread )
{

	// TODO: Argument check

	// Initialized data
	GXTask_t **tasks = thread->tasks;
	GXInstance_t* instance = g_get_active_instance();

	// Run until told otherwise
	while (thread->running)
	{

		// Iterate over each task
		for (size_t i = 0; i < thread->task_count; i++)
		{

			// Are we waiting on anything else to finish?
			if (thread->tasks[i]->wait_thread)
			{

				// Initialized data
				GXThread_t *wait_thread = dict_get(instance->context.schedule->threads, thread->tasks[i]->wait_thread);
				GXTask_t   *wait_task   = 0;
				size_t      v           = 0;

				// Figure out what task we are waiting on
				for (size_t j = 0; j < wait_thread->task_count; j++)
				{
					if (strcmp(thread->tasks[i]->wait_task, wait_thread->tasks[j]->name) == 0)
						v = j;
				}

				// Wait for the task to finish
				while (wait_thread->complete_tasks[i] == 0);

			}

			int (*function_pointer)(GXInstance_t*) = thread->tasks[i]->function_pointer;

			// Run the function
			if(function_pointer)
				function_pointer(g_get_active_instance());

			// Update the task
			thread->complete_tasks[i]=1;
		}

		// Reset
		for (size_t i = 0; i < thread->task_count; i++)
			thread->complete_tasks[i] = 0;
	}

	return 1;
}

int main_work              ( GXThread_t    *thread )
{

	// TODO: Argument check

	// Initialized data
	GXTask_t **tasks = thread->tasks;
	GXInstance_t* instance = g_get_active_instance();

	while (thread->running)
	{

		for (size_t i = 0; i < thread->task_count; i++)
		{

			// Are we waiting on anything else to finish?
			if (thread->tasks[i]->wait_thread)
			{

				// Initialized data
				GXThread_t *wait_thread = dict_get(instance->context.schedule->threads, thread->tasks[i]->wait_thread);
				GXTask_t   *wait_task   = 0;
				size_t      v           = 0;

				// Figure out what task we are waiting on
				for (size_t j = 0; j < wait_thread->task_count; j++)
				{
					if (strcmp(thread->tasks[i]->wait_task, wait_thread->tasks[j]->name) == 0)
						v = j;
				}

				// Wait for the task to finish
				while (wait_thread->complete_tasks[i] == 0);

			}

			int (*function_pointer)(GXInstance_t*) = thread->tasks[i]->function_pointer;

			// Run the function
			if(function_pointer)
				function_pointer(g_get_active_instance());

			// Update the task
			thread->complete_tasks[i]=1;
		}

		// Reset
		for (size_t i = 0; i < thread->task_count; i++)
			thread->complete_tasks[i] = 0;

	}
	return 1;

	// TODO: Error handling
}

int start_schedule         ( GXSchedule_t  *p_schedule)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_schedule == (void *) 0 )
				goto no_schedule;
		#endif
	}

	// Initialized data
	GXInstance_t *instance              = g_get_active_instance();
	size_t        schedule_thread_count = dict_values(p_schedule->threads, 0);
	GXThread_t  **schedule_threads      = malloc(schedule_thread_count * sizeof(void *));

	// Set the active schedule
	instance->context.schedule = p_schedule;

	// Copy the state
	copy_state(instance);

	// Get a list of threads
	dict_values(p_schedule->threads, schedule_threads);

	// Iterate over each thread
	for (size_t i = 0; i < schedule_thread_count; i++)
	{

		// Initialized data
		GXThread_t *thread = schedule_threads[i];

		// Don't spawn a new thread for the main thread
		// TODO: user defined main thread name
		if (strcmp(thread->name, "Main Thread") == 0)
			continue;

		// Set the thread to run
		thread->running = true;
		
		// Create the thread
		thread->thread  = SDL_CreateThread(work, thread->name, thread);

	}

	// Get the main thread
	GXThread_t* main_thread = dict_get(p_schedule->threads, "Main Thread");

	// Set the thread to run
	main_thread->running = true;
	
	// Call the thread worker function (on the main thread...)
	main_work(main_thread);

	// Success
	return 1;

	// TODO: Error handling
	no_schedule:
	return 0;
}

int stop_schedule          ( GXSchedule_t  *schedule )
{
	// TODO: Argument check
	
	// Initialized data
	GXInstance_t *instance              = g_get_active_instance();
	size_t        schedule_thread_count = dict_values(schedule->threads, 0),
		          r_stat                = 0;
	GXThread_t  **schedule_threads      = calloc(schedule_thread_count+1, sizeof(void *));

	// Get a list of threads
	dict_values(schedule->threads, schedule_threads);

	// Iterate over each thread
	for (size_t i = 0; i < schedule_thread_count; i++)
	{

		// Initialized data
		GXThread_t* thread = schedule_threads[i];

		// Stop the thrad
		thread->running = false;

		// Wrap up every task
		for (size_t i = 0; i < thread->task_count; i++)
			thread->complete_tasks[i] = 1;
	}

	// Iterate over each thread
	for (size_t i = 0; i < schedule_thread_count; i++)

		// Wait for each thread to finish
		SDL_WaitThread(schedule_threads[i]->thread, &r_stat);

	// Success
	return 1;

	return 0;
}

int load_thread            ( GXThread_t   **thread, char* path )
{

	// Argument check
	{
		if ( thread == (void*)0 )
			goto no_thread;
		if ( path == (void *)0 )
			goto no_path;
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len+1, sizeof(char));
	
	// TODO: Error checking
	g_load_file(path, token_text, false);
	
	// TODO: Error checking
	load_thread_as_json(thread, token_text, len);

	// Free the buffer
	free(token_text);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_thread:
				#ifndef NDEBUG
					g_print_log("[G10] [Scheduler] Null pointer provided for \"thread\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_log("[G10] [Scheduler] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
				return 0;
		}
	}
}

int load_thread_as_json    ( GXThread_t   **pp_thread   , char *token_text, size_t len )
{
	// TODO: Argument check

	// Initialized data
	GXThread_t *p_thread = 0;
	dict    *thread_json = 0;
	char    *name        = 0;
	size_t   task_count  = 0;
	char   **tasks       = 0;

	// Parse the JSON
	{

		// Initiaized data
		JSONToken_t *token = 0;

		// Parse the JSON into a dictionary
		parse_json(token_text, len, &thread_json);

		// Get the name
		token = dict_get(thread_json, "name");
		name  = JSON_VALUE(token, JSONstring);

		// Get the tasks
		token = dict_get(thread_json, "tasks");
		tasks = JSON_VALUE(token, JSONarray);

	}

	// Construct the thread
	{

		// Allocate a thread
		if ( create_thread(pp_thread) == 0 )
			goto failed_to_create_thread;

		// Get a pointer to the allocated memory
		p_thread = *pp_thread;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for name
			p_thread->name = calloc(name_len + 1, sizeof(char));
			
			// TODO: Error checking
			
			// Copy name
			strncpy(p_thread->name, name, name_len);
		}

		// Construct tasks
		{
			
			// Count up the tasks
			while (tasks[++task_count]);

			p_thread->task_count = task_count;

			// Allocate memory for task lists
			p_thread->tasks          = calloc(task_count + 1, sizeof(GXTask_t*));
			
			// TODO: Error checking
			
			// Allocate memory for task lists
			p_thread->complete_tasks = calloc(task_count + 1, sizeof(size_t));

			// TODO: Error checking
			
			// Iterate over each JSON object
			for (size_t i = 0; i < task_count; i++)
			{
				if ( load_task_as_json(&p_thread->tasks[i], tasks[i], strlen(tasks[i])) == 0 )
					goto failed_to_construct_task;
			}
		}
	}

	return 1;

	// TODO: Error handling
	failed_to_create_thread:
	failed_to_construct_task:
	return 0;
}

int load_task_as_json      ( GXTask_t     **pp_task     , char *token_text, size_t len )
{
	// TODO: Argument check
	
	// Initialized data
	GXTask_t *p_task      = 0;
	dict     *task_json   = 0;
	char     *task_name   = 0,
		     *wait_thread = 0,
		     *wait_task   = 0;

	// Allocate memory for the task struct
	if ( create_task(pp_task) == 0 )
		goto failed_to_create_task;

	// Get a pointer to the allocated memory
	p_task = *pp_task;

	// Parse the JSON into a dictionary
	parse_json(token_text, len, &task_json);

	// Parse the dictionary into constructor parameters
	{

		// Initialized data
		JSONToken_t* token = 0;
		
		token       = dict_get(task_json, "task");
		task_name   = JSON_VALUE(token, JSONstring);

		token       = dict_get(task_json, "wait thread");
		wait_thread = JSON_VALUE(token, JSONstring);

		token       = dict_get(task_json, "wait task");
		wait_task   = JSON_VALUE(token, JSONstring);
	}

	// Construct the task
	{

		// Copy the name
		{

			// Initialized data
			size_t task_name_len = strlen(task_name);

			// Allocate memory for the name
			p_task->name = calloc(task_name_len+1, sizeof(char));

			// TODO: Error handling

			// Copy the name
			strncpy(p_task->name, task_name, task_name_len);
		}

		// Copy the name of the thread that this task waits on 
		if(wait_thread)
		{

			// Initialized data
			size_t wait_thread_len = strlen(wait_thread);
			
			// Allocate memory for the name
			p_task->wait_thread = calloc(wait_thread_len + 1, sizeof(char));
			
			// TODO: Error handling

			// Copy the name
			strncpy(p_task->wait_thread, wait_thread, wait_thread_len);
		}

		// Copy the name of the task that this task waits on 
		if(wait_task)
		{

			// Initialized data
			size_t wait_task_len = strlen(wait_task);

			// Allocate memory for the name
			p_task->wait_task = calloc(wait_task_len + 1, sizeof(char));

			// TODO: Error handling

			// Copy the name
			strncpy(p_task->wait_task, wait_task, wait_task_len);
		}

	}
	
	// Get a function pointer from the list
	p_task->function_pointer = dict_get(scheduler_tasks, p_task->name);

	// Success
	return 1;

	// TODO: Error handling
	failed_to_create_task:
	return 0;
}