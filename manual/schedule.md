# !!! WARNING !!! OLD !!! WARNING !!! OLD !!!
# Schedule

> 1 [Background](#background)
>
>> 1.1 [A Simple Solution](#a-simple-solution)
>>
>> 1.2 [A Bad Solution?](#a-bad-solution)
>>
> 2 [Tasks and Threads](#tasks-and-threads)
>> 2.1 [Tasks](#tasks)
>>
>> 2.2 [Threads](#threads)
>
> 3 [Schedules](#schedules)
> 
>> 3.1 [Concurrency](#concurrency)
>>
>> 3.2 [Parallelism](#parallelism)
>>
> 4 [Client vs Server work](#)
>
> 5 [JSON](#json)
>
>> 5.1 [Schema](#schema)
>>
>> 5.2 [Game loop (example)](#the-game-loop)
>>
>> 5.3 [Too much work (example)](#too-much-work)
>>
>> 5.4 [Balancing (example)](#balancing)
>
> 7 [Task list](#task-list)


## Background
Programs have to do work. Word processors have to process words, music players have to play music, and game engines have to create interactive user experiences. To create an interactive user experience, the game engine must manage user input, simulate game dynamics, and render frames *over time*. This is the bare minimum. Simple cases demand simple solutions, but as the interactive experience becomes more complex, the solutions must adapt to the complexity. 

### A Simple Solution
A simple solution to the interactive experience problem is the game loop. The game loop will sequentially poll for user input, simulate physics, and render a frame. After the frame is presented, the game loop will start over. The game loop will continue until the player finishes the game. 

```c
// An example game loop
while(game->should_exit)
{

    // User interaction
    poll_input();

    // Dynamics
    update_physics();

    // Render
    render_frame();
}
```

If a developer wanted to add more functionality, they might run more functions in the game loop. For instance, one could call a process_audio function in the game loop.

### A bad solution?

The game loop has a problem. It's on a time crunch.  An end user expects the game to run at a specified frame rate N. This means that the game engine must finish all the work of drawing a frame in N⁻¹ seconds. Adding more work will, without exception, increase the time to render a frame.

FPS | Time to finish work (milliseconds)
----|-------------------------------------
240 | 4 ⅙
120 | 8 ⅓
90  | 11 ⅑
60  | 16 ⅔
30  | 33 ⅓

## Tasks and Threads

### Tasks
Tasks are pieces of work the game engine does once per frame. We have discussed four different tasks already, those being *poll_input*, *update_physics*, *process_audio*, and *render_frame*. There is no  *```*update_physics* task defined by G10. Updating physics is  Some tasks work and some tasks are being worked on, so consult the [task list](#task-list) to see what is available.

### Threads
Threads are what get work done. Many threads can be run at one time. Threads are a more flexable solution than the game loop. Like the game loop, threads will sequentially do tasks. Many threads can be run at a time. Some threads are created on an as needed basis, while others will  run until the player finishes the game.

## Schedules
![](scheduler.png)

This is a schedule. Just like at your workplace, the rows are the workers, and the columns are work over time. Each thread has work to do. This work is done once every frame[1.2](#a-bad-solution). 

### Concurrency
### Parallelism


\
\
\
.
## JSON
Scheduler JSON describes what work is done when, and how much effort is put into said work. 

### Schema
Schedule schema can be found [here](https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/schedule-schema.json). To fill out a scheduler right now in your web browser, try [this](https://github.dev/Jacob-C-Smith/G10-Schema/blob/main/examples/schedule/game%20loop.json)
### The game loop
```json
{
    "$schema"     : "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/schedule-schema.json",
    "name"        : "Game loop",
    "main thread" : "loop",
    "repeat"      : true,
    "threads"     : [
        {
            "name": "loop",
            "description": "This is the thread the game starts on",
            "tasks": [
                {
                    "task" : "Input"
                },
                {
                    "task" : "Resolve Collisions"
                },
                {
                    "task" : "Update Forces"
                },
                {
                    "task" : "Move Objects"
                },
                {
                    "task" : "Copy State"
                },
                {
                    "task" : "Render"
                }
            ]
        }
    ]
}
```

### Too much work
```json
{
    "$schema"     : "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/schedule-schema.json",
    "name"        : "slow loop",
    "main thread" : "slow loop",
    "repeat"      : true,
    "threads"     : [
        {
            "name": "Game loop",
            "description": "This is the thread the game starts on",
            "tasks": [
                {
                    "task" : "Input"
                },
                {
                    "task" : "Pre AI",
                },
                {
                    "task" : "AI",
                },
                {
                    "task" : "User Code"
                },
                {
                    "task" : "Animation"
                }
                {
                    "task" : "Resolve Collisions"
                },
                {
                    "task" : "Update Forces"
                },
                {
                    "task" : "Move Objects"
                },
                {
                    "task" : "Copy State"
                },
                {
                    "task" : "Render"
                }
            ]
        }
    ]
}
```

### Balancing
```json
{
    "$schema": "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/schedule-schema.json",
    "name"         : "Game loop",
    "threads"      : [
        {
            "name": "main",
            "tasks": [
                {
                    "task"        : "Input",
                    "wait thread" : "Render Thread",
                    "wait task"   : "Render"
                },
                {
                    "task" : "Pre AI",
                },
                {
                    "task" : "AI",
                },
                {
                    "task" : "User Code"
                },
                {
                    "task" : "Copy State"
                }
            ]
        },
        {
            "name": "render",
            "tasks": [
                {
                    "task" : "Render"
                }
            ]
        },
        {
            "name" : "physics",
            "tasks": [
                {
                    "task" : "Animation"
                }
                {
                    "task"        : "Resolve Collisions",
                    "wait thread" : "main",
                    "wait task"   : "AI"
                },
                {
                    "task" : "Update Forces"
                },
                {
                    "task" : "Move Objects"
                }
            ]
        }
    ],
    "main thread"  : "main",
    "repeat"       : true
}
```

### Thread
```json
{
    "name"        : "Render Thread",
    "description" : "Draws frame N-1",
    "tasks"       : [
        {
            "task" : "Render"
        }
    ]
}
```

### Task
```json
{
    "task" : "Animation"
}
```

```json
{
    "task"        : "Input",
    "wait thread" : "Render Thread",
    "wait task"   : "Render"
}
```

## Work
### Client side work
### Server side work

## Task list

| Name                  | Description                                                          | Progress | 
|-----------------------|----------------------------------------------------------------------|----------|
| Input                 | Update input and bind callbacks                                      | ✅      |
| UI                    | Draw the user interface                                              | ❌      |
| AI                    | Update all AIs in the context scene                                  | ✅      |
| Pre AI                | Preupdate all AIs in the context scene                               | ✅      |
| User Code             | Game logic                                                           | ✅      |
| Resolve Collisions    | Collision detection and resolution in the context scene              | 🚧      |
| Update Forces         | Compute net force of all objects in the context scene                | 🚧      |
| Move Objects          | Calculate displacement and rotation derivatives in the context scene | 🚧      |
| Animation             | Update rigs and bones                                                | ❌      |
| Render                | Render the (last) frame                                              | ✔️      |
| Present               | Present the last frame when it is done rendering                     | ❌      |
| Load Entity           | Load an entity from the instance's entity queue                      | ✅      |
| Load Light Probe      | Load a light probe from the instance's light probe queue             | ❌      |
| Copy State            | Create work queues                                                   | ✔️      |
| Server Recieve        | Recieve from the client                                              | 🚧      |
| Server Send           | Send to the client                                                   | 🚧      |
| Server Parse          | Parse the data recieved from the client into a command queue         | 🚧      |
| Server Serialize      | Parse the command queue into data to send to the client              | 🚧      |
| Server Process        | Process a command queue                                              | 🚧      |
| Server Wait           | Wait for connections. Spawn a new thread when a client joins         | ✔️      |
| Audio                 | Update the audio API                                                 | 🚧      |
| Discord Callbacks     | Update the Discord API, if ```BUILD_G10_WITH_DISCORD``` is defined   | 🚧      |

✅ Task is completely implemented
✔️ Task works, and is under development
🚧 Task does not work, and is under development
❌ Task has not been attempted