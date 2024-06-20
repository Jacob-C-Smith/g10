# G10 Tutorial
G10 is a cross platform game engine. 

> 1. Writing an instance
>
> 2. Loading an instance
>
> 3. Window
>
> 4. Game logic
>
> 5. 

## Writing an instance
Before writing any code, one must write an instance file. The instance file is a description of your game. Let's start with a barebones instance.

*instance.json*
```json
{
    "$schema" : "https://schema.g10.app/instance.json",
    "name"    : "my game"
}
```

Save the instance to your build directory. I highly recommend exploring the schema interactively with vscode. 

## Loading an instance
Now that one has a barebones description of their game, one can load the instance. To load the instance, one would use ```g_init``` function. Here's the documentation.
```c
/** !
 *  Construct a g10 instance
 *
 * @param pp_instance result
 * @param path        path to instance file
 *
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
int g_init ( g_instance **pp_instance, const char *p_path );
```

In main, we start by declaring a ```g_instance``` pointer.

Subsequently, we call ```g_init```, referencing the ```g_instance``` pointer, and the path to our instance file. If the result is ```0```, we print the error to standard out, and return ```EXIT_FAILURE```. Otherwise, we are ready to proceed.

*main.c*
```c
// standard library
#include <stdio.h>
#include <stdlib.h>

// g10
#include <g10/g10.h>

// Entry point
int main ( int argc, const char *argv[] ) 
{

    // Initialized data
    g_instance *p_instance = NULL;

    // Load a g10 instance
    if ( g_init(&p_instance, "instance.json") == 0 )
    {

        // Print an error to standard out
        printf("Error: Failed to load instance!\n");

        // Error
        return EXIT_FAILURE;
    }

    // Success
    return EXIT_SUCCESS;
}
```

If your build system is set up incorrectly, this is where you will find out. 

## Window
Back in ```instance.json```, we can add a window property so that something shows up on the screen. You can adjust your width, height, and title as you see fit. 

*instance.json*
```json
{
    "$schema" : "https://schema.g10.app/instance.json",
    "name"    : "my game",
    "window"  :
    {
        "width"  : 800,
        "height" : 600,
        "title"  : "My game"
    }
}
```

These changes will not effect the game yet, because the game immediately exits. To make the window visable, we have to define a schedule. Schedules are an abstraction for coordinating work over time. 

*instance.json*
```json
{
    "$schema" : "https://schema.g10.app/instance.json",
    "name"    : "my game",
    ...
    "schedule" :
    {
        "name"    : "my schedule",
        "repeat"  : true,
        "threads" :
        {
            "main" : 
            [
                { "task" : "sdl2 poll" }
            ]
        }
    }
}
```

The following schedule will keep the window open until the user closes it. Back in main, set the instance's running flag, and start the schedule with ```g_start```

*main.c*
```c
// standard library
#include <stdio.h>
#include <stdlib.h>

// g10
#include <g10/g10.h>

// Entry point
int main ( int argc, const char *argv[] ) 
{

    // Initialized data
    g_instance *p_instance = NULL;

    // Load a g10 instance
    if ( g_init(&p_instance, "instance.json") == 0 )
    {

        // Print an error to standard out
        printf("Error: Failed to load instance!\n");

        // Error
        return EXIT_FAILURE;
    }

    // Start the schedule
    g_start(p_instance);

    // Block
    while ( p_instance->running );

    // Success
    return EXIT_SUCCESS;
}
```

For more on schedules, see the [scheduler](https://github.com/Jacob-C-Smith/g10/wiki/Scheduler) page in the manual. 

## Game Logic
Now that we have a window, we can write some game logic. First, we modify the schedule. 

*instance.json*
```json
{
    "$schema" : "https://schema.g10.app/instance.json",
    "name"    : "my game",
    ...
    "schedule" :
    {
        "name"    : "my schedule",
        "repeat"  : true,
        "threads" :
        {
            "main" : 
            [
                { "task" : "sdl2 poll" }
                { "task" : "user code" }
            ]
        }
    }
}
```

The ```user code``` task executes game logic once per frame. First, define a function of type ```fn_user_code_callback```

*main.c*
```c
// standard library
#include <stdio.h>
#include <stdlib.h>

// g10
#include <g10/g10.h>

int game_logic ( g_instance *p_instance )
{

    // Log
    printf("[game logic]\n");

    // Success
    return 1;
}

// Entry point
int main ( int argc, const char *argv[] ) 
{

    // Initialized data
    g_instance *p_instance = NULL;

    // Load a g10 instance
    if ( g_init(&p_instance, "instance.json") == 0 )
    {

        // Print an error to standard out
        printf("Error: Failed to load instance!\n");

        // Error
        return EXIT_FAILURE;
    }

    // Set a user code callback
    user_code_callback_set(p_instance, user_code_main);

    // Start the schedule
    g_start(p_instance);

    // Block
    while ( p_instance->running );

    // Success
    return EXIT_SUCCESS;
}
```