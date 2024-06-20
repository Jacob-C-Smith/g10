#include <g10/g10.h>

#include <SDL2/SDL.h>

int g_sdl2_initialize_window ( g_instance *p_instance, json_value *p_value )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    if ( p_value    == (void *) 0 ) goto no_value;

    // Initialized data
    u32 sdl2_window_flags = SDL_WINDOW_SHOWN;

    // Graphics API
    #ifdef G10_BUILD_WITH_VULKAN
        sdl2_window_flags |= SDL_WINDOW_VULKAN;
    #elif defined G10_BUILD_WITH_OPENGL
        sdl2_window_flags |= SDL_WINDOW_OPENGL;
    #endif

    // Parse the window json
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Parse the window object
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_width  = (json_value *)dict_get(p_dict, "width"),
                   *p_height = (json_value *)dict_get(p_dict, "height"),
                   *p_title  = (json_value *)dict_get(p_dict, "title");

        // Error check
        if ( ! ( p_width && p_height && p_title ) ) goto missing_properties;

        // Error check
        if ( p_width->type  != JSON_VALUE_INTEGER ) goto wrong_width_type;
        if ( p_height->type != JSON_VALUE_INTEGER ) goto wrong_height_type;
        if ( p_title->type  != JSON_VALUE_STRING  ) goto wrong_title_type;
    
        // Store the width
        p_instance->window.width = (u32) p_width->integer;

        // Store the height
        p_instance->window.height = (u32) p_height->integer;

        // Copy the title
        {

            // Initialized data
            size_t len = strlen(p_title->string);

            // Allocate memory for the string
            p_instance->window.title = G10_REALLOC(0, sizeof(char) * ( len + 1 ));

            // Error check
            if ( p_instance->window.title == (void *) 0 ) goto no_mem;

            // Initialize
            memset(p_instance->window.title, 0, sizeof(char) * ( len + 1 ));

            // Copy the string
            strncpy(p_instance->window.title, p_title->string, len);
        }
    }

    // Construct the SDL2 window
    p_instance->window.sdl2.window = SDL_CreateWindow(p_instance->window.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int) p_instance->window.width, (int) p_instance->window.height, sdl2_window_flags);

    // Success
    return 1;

    no_instance:
    no_value:
    wrong_type:
    missing_properties:
    wrong_width_type:
    wrong_height_type:
    wrong_title_type:
    no_mem:
    
        // Error
        return 0;
}

int g_sdl2_destroy_window ( g_instance *p_instance )
{

    // Destroy the window
    SDL_DestroyWindow(p_instance->window.sdl2.window);

    // Success
    return 1;
}

int g_sdl2_poll_window ( g_instance *p_instance )
{

    // Process each event
    while (SDL_PollEvent(&p_instance->window.sdl2.event))
    {
        
        // Switch on the event type
        switch (p_instance->window.sdl2.event.type)
        {

            // Process should exit
            case SDL_QUIT:
            {
                p_instance->running = false;
                break;
            }

            // Mouse moves
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                printf("[ %d, %d ]\n", p_instance->window.sdl2.event.motion.x, p_instance->window.sdl2.event.motion.y);
                break;
            }
        }
    }

    // Success
    return 1;
}
