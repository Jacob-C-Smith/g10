#include <stdio.h>

#include <G10/G10.h>
#include <G10/GXScene.h>

#ifdef main
#undef main
#endif

int main(int argc, const char* argv[])
{
	GXInstance_t *instance = 0;
	GXScene_t    *scene    = 0;

	// Create a debug instance
	g_init(&instance, "G10/Debug instance.json");

	load_scene(&scene, "G10/demo scene.json");

	instance->running = true;
	
	while (instance->running)
	{
		process_input(instance);
	}

	g_exit(instance);

	return 0;
}