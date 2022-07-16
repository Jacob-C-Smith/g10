#include <stdio.h>

#include <SDL2/SDL_gamecontroller.h>

#include <G10/G10.h>
#include <G10/GXScene.h>

#include <G10/GXShader.h>
#include <G10/GXTransform.h>

#ifdef main
#undef main
#endif

int main(int argc, const char* argv[])
{
	GXInstance_t  *instance  = 0;
	GXMaterial_t  *material  = 0;
	GXTexture_t   *texture   = 0;
	GXTransform_t *transform = 0;
	GXShader_t    *shader    = 0;

	// Create a debug instance
	g_init(&instance, "G10/Debug instance.json");

	load_shader(&shader, "G10/shaders/G10 triange.json");

	instance->running = true;
	
	GXBind_t *exit_bind = find_bind(instance->input, "QUIT");
	register_bind_callback(exit_bind, &g_user_exit);

	// CONTROLLER TESTING
	// TODO: Integrate into input system
	{
				/*
		 
		SDL_GameController* controller = NULL;

		for (int i = 0; i < SDL_NumJoysticks(); ++i) {
			if (SDL_IsGameController(i)) {
				controller = SDL_GameControllerOpen(i);
				if (controller) {
					SDL_GameControllerType t = SDL_GameControllerTypeForIndex(i);
					switch (t)
					{
					case SDL_CONTROLLER_TYPE_XBOX360:
						printf("XB360\n\n\n");
					case SDL_CONTROLLER_TYPE_XBOXONE:
						printf("XB1\n\n\n");
					default:
						break;
					}
					break;
				}
				else {
					printf("Could not open gamecontroller %i: %s\n", i, SDL_GetError());
				}
			}
		}
		
		while (SDL_PollEvent(&instance->event)) 
			switch (instance->event.type)
			{


			}
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
				printf("A\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
				printf("B\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
				printf("X\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y))
				printf("Y\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
				printf("RB\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
				printf("LB\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK))
				printf("Back\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_GUIDE))
				printf("XBOX Button\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START))
				printf("Start\r");
			if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN))
				printf("DOWN\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
				printf("RIGHT\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT))
				printf("LEFT\r");
			else if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP))
				printf("UP\r");*/
			
	}

	// Game loop
	while (instance->running)
	{
		process_input(instance);

		draw_scene(0, shader);

	}

	vkDeviceWaitIdle(instance->device);

	g_exit(instance);

	return 0;
}