#include <stdio.h>

#include <SDL2/SDL_gamecontroller.h>

#include <G10/G10.h>
#include <G10/GXScene.h>
#include <G10/GXEntity.h>
#include <G10/GXShader.h>
#include <G10/GXTransform.h>
#include <G10/GXPart.h>
#include <G10/GXMaterial.h>


#define DEFAULT_RESOLUTION  1

static int ticks = 0;

static Uint32 SDLCALL
ticktock(Uint32 interval, void* param)
{
    ++ticks;
    return (interval);
}

static Uint32 SDLCALL
callback(Uint32 interval, void* param)
{
    SDL_Log("Timer %d : param = %d", interval, (int)(uintptr_t)param);
    return interval;
}


int main(int argc, const char* argv[])
{
    GXInstance_t  *instance  = 0;
    GXScene_t     *scene     = 0;
    GXEntity_t    *entity    = 0;

    // Create a debug instance
    g_init(&instance, "G10/Debug instance.json");

    {
        int i, desired;
        SDL_TimerID t1, t2, t3;
        Uint32 start32, now32;
        Uint64 start, now, freq = SDL_GetPerformanceFrequency();


        // 0.1 microsecond frequency

        start = SDL_GetPerformanceCounter();
        SDL_Delay(1000);
        now = SDL_GetPerformanceCounter();

        SDL_Log("%f ms according to performance counter", (double)((now - start) * 1000) / freq);

    }
    instance->running = true;
    
    // Set up key binds
    {
        GXBind_t* exit_bind = find_bind(instance->input, "QUIT");
        register_bind_callback(exit_bind, &g_user_exit);
    }

    GXSchedule_t* s = dict_get(instance->schedules, "Simple Schedule");
    
    // 
    start_schedule(s);

    // Main loop
    while (instance->running)
    {
        process_input(instance);
        // UI
        // game_tick(instance);
    }

    stop_schedule(s);

    g_exit(instance);

    return 0;
}