#include <G10/GXScheduler.h>

dict* scheduler_tasks = 0;

void init_scheduler(void)
{
	GXInstance_t *instance = g_get_active_instance();

	dict_construct(&scheduler_tasks, 16);

	dict_add(scheduler_tasks, "Input", &process_input);
	dict_add(scheduler_tasks, "UI", 0);
	dict_add(scheduler_tasks, "AI", 0);
	dict_add(scheduler_tasks, "Pre AI", 0);
	dict_add(scheduler_tasks, "Resolve Collisions", 0);
	dict_add(scheduler_tasks, "Update Forces", 0);
	dict_add(scheduler_tasks, "Move Objects", 0);
	dict_add(scheduler_tasks, "Animation", 0);
	dict_add(scheduler_tasks, "Render", 0);
}

int create_scheduler(GXScheduler_t  **scheduler)
{
	// TODO: Argument check

	GXScheduler_t *i_scheduler = calloc(1, sizeof(GXScheduler_t));

	// TODO: Memory check
	*scheduler = i_scheduler;

	// TODO: Error handling
	return 1;
}