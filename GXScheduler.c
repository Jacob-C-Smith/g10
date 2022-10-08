#include <G10/GXScheduler.h>

dict* scheduler_tasks = 0;

char* task_names[20] = {
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
	"Load Entity",
	"Load Light Probe",
	"Copy State",
	"Server Recieve",
	"Server Send",
	"Server Parse",
	"Server Serialize",
	"Server Process",
	"Server Wait"
	"Audio"
};

void* task_function_pointers[20] = {
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
	&load_entity_from_queue, 
	&load_light_probe_from_queue, 
	&copy_state,
    &server_recv, // 
	&server_send, // 
	&server_parse,
	&server_serialize,
	&server_process,
	&server_wait, 
	0  // process_audio
};

void init_scheduler        ( void )
{
	
	// Initialized data
	GXInstance_t *instance = g_get_active_instance();

	// Construct a function lookup table
	dict_construct(&scheduler_tasks, 20);

	// Iterate over each task
	for (size_t i = 0; i < 20; i++)
		
		// Add each task to the lookup table
		dict_add(scheduler_tasks, task_names[i], task_function_pointers[i]);

}

int create_task            ( GXTask_t     **task )
{
	// Argument check
	{
		#ifndef NDEBUG
			if(task == (void *)0)
				goto no_task;
		#endif
	}

	GXTask_t *i_task = calloc(1, sizeof(GXTask_t));

	*task = i_task;

	return 1;
	// TODO: Error handling
	no_task:
	return 0;
}

int create_schedule        ( GXSchedule_t **pp_schedule )
{
	// TODO: Argument check

	GXSchedule_t *i_scheduler = calloc(1, sizeof(GXSchedule_t));

	// TODO: Memory check
	*pp_schedule = i_scheduler;

	// TODO: Error handling
	return 1;
}

int create_thread          ( GXThread_t   **pp_thread )
{
	// TODO: Argument check

	GXThread_t *i_thread = calloc(1, sizeof(GXThread_t));

	// TODO: Memory check
	*pp_thread = i_thread;

	// TODO: Error handling
	return 1;
}

int load_schedule          ( GXSchedule_t **schedule, char* path)
{

	// Argument check
	{
		if ( schedule == (void*)0 )
			goto no_schedule;
		if ( path == (void *)0 )
			goto no_path;
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len+1, sizeof(char));
	
	g_load_file(path, token_text, false);

	load_schedule_as_json(schedule, token_text, len);

	free(token_text);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_schedule:
				#ifndef NDEBUG
					g_print_log("[G10] [Scheduler] Null pointer provided for \"schedule\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_log("[G10] [Scheduler] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_schedule_as_json  ( GXSchedule_t **schedule, char *token_text, size_t len )
{
	// Argument check
	{
		#ifndef NDEBUG
			if (schedule == (void*)0)
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
		GXSchedule_t *i_scheduler = 0;

		// Allocate a schedule
		create_schedule(schedule);

		i_scheduler = *schedule;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for name
			i_scheduler->name = calloc(name_len + 1, sizeof(char));
			
			// Copy name
			strncpy(i_scheduler->name, name, name_len);
		}

		// Construct each thread
		{
			
			// Count up the blocks
			while (thread_json_objects[++thread_count]);

			dict_construct(&i_scheduler->threads, 16);

			// Iterate over each block JSON object
			for (size_t i = 0; i < thread_count; i++)
			{

				// Initialized data
				GXThread_t *thread = 0;

				// Load the thread from the JSON
				load_thread_as_json(&thread, thread_json_objects[i], strlen(thread_json_objects[i]));

				dict_add(i_scheduler->threads, thread->name, thread);

			}
		}
	}

	return 0;

	// Error handling
	{

		// Argument errors
		{
			no_scheduler:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"schedule\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_token_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Scheduler] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int work                   ( GXThread_t    *thread )
{
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
				GXThread_t *wait_thread = dict_get(instance->active_schedule->threads, thread->tasks[i]->wait_thread);
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
	return 0;
}

int main_work              ( GXThread_t    *thread )
{

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
				GXThread_t *wait_thread = dict_get(instance->active_schedule->threads, thread->tasks[i]->wait_thread);
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
	return 0;
}

int start_schedule         ( GXSchedule_t  *schedule )
{
	// Initialized data
	GXInstance_t *instance              = g_get_active_instance();
	size_t        schedule_thread_count = dict_values(schedule->threads, 0);
	GXThread_t  **schedule_threads      = malloc(schedule_thread_count * sizeof(void *));

	// Set the active schedule
	instance->active_schedule = schedule;

	// Copy the state
	copy_state(instance);

	// Get a list of threads
	dict_values(schedule->threads, schedule_threads);

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
	GXThread_t* main_thread = dict_get(schedule->threads, "Main Thread");

	// Set the thread to run
	main_thread->running = true;
	
	// Call the thread worker function (on the main thread...)
	main_work(main_thread);

	free(schedule_threads);

	return 0;
}

int stop_schedule          ( GXSchedule_t  *schedule )
{

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

}

int load_thread_as_json    ( GXThread_t   **thread   , char *token_text, size_t len )
{
	// TODO: Argument check

	// Initialized data
	dict    *thread_json = 0;
	char    *name        = 0;
	size_t   task_count  = 0;
	char   **tasks       = 0;

	// Parse the JSON
	{
		/*
		{
            "name"        : "Game loop",
            "description" : "Single threaded solution",
            "tasks"       : [
                "Input",
                "Render"
            ]
        }
		*/

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

		// Initialized data
		GXThread_t *i_thread = 0;

		// Allocate a thread
		create_thread(thread);

		i_thread = *thread;

		// Set the name
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate for name
			i_thread->name = calloc(name_len + 1, sizeof(char));
			
			// Copy name
			strncpy(i_thread->name, name, name_len);
		}

		// Construct each task
		{
			
			// Count up the tasks
			while (tasks[++task_count]);

			i_thread->tasks          = calloc(task_count + 1, sizeof(GXTask_t*));
			i_thread->task_count     = task_count;
			i_thread->complete_tasks = calloc(task_count + 1, sizeof(size_t));

			// Iterate over each block JSON object
			for (size_t i = 0; i < task_count; i++)
			{
				load_task_as_json(&i_thread->tasks[i], tasks[i], strlen(tasks[i]));
			}
		}
	}

	return 0;

	// TODO: Error handling
}

int load_task_as_json      ( GXTask_t     **task     , char *token_text, size_t len )
{
	GXTask_t *i_task      = 0;
	dict     *task_json   = 0;
	char     *task_name   = 0,
		     *wait_thread = 0,
		     *wait_task   = 0;

	create_task(task);

	i_task = *task;

	{
		JSONToken_t* token = 0;
		
		parse_json(token_text, len, &task_json);

		token     = dict_get(task_json, "task");
		task_name = JSON_VALUE(token, JSONstring);

		token       = dict_get(task_json, "wait thread");
		wait_thread = JSON_VALUE(token, JSONstring);

		token     = dict_get(task_json, "wait task");
		wait_task = JSON_VALUE(token, JSONstring);
	}

	{
		{
			size_t task_name_len = strlen(task_name);
			i_task->name = calloc(task_name_len+1, sizeof(char));
			strncpy(i_task->name, task_name, task_name_len);
		}
		if(wait_thread)
		{
			size_t wait_thread_len = strlen(wait_thread);
			i_task->wait_thread = calloc(wait_thread_len + 1, sizeof(char));
			strncpy(i_task->wait_thread, wait_thread, wait_thread_len);
		}
		if(wait_task)
		{
			size_t wait_task_len = strlen(wait_task);
			i_task->wait_task = calloc(wait_task_len + 1, sizeof(char));
			strncpy(i_task->wait_task, wait_task, wait_task_len);
		}

	}
	
	i_task->function_pointer = dict_get(scheduler_tasks, i_task->name);

	return 0;
}