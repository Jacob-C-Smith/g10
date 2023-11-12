# ai

> 1. [Observations](#observations)
> 
> 2. [Making descriptions from observations](#making-descriptions-from-observations)
>
> 3. [AI tool](#ai-tool)
>
> 4. [Programming the AI](#programming-the-ai)
>>
>> 4.1 [Getting the AI](#getting-the-ai)
>>
>> 4.2 [Writing state functions](#writing-state-functions)
>>
>> 4.3 [Preupdate](#pre-update)
>>
>> 4.4 [Setting up AI callbakcs](#setting-up-ai-callbacks)

# Observations
The Goomba from the Mario franchise is an example of a video game AI. The Goomba can do two things. The Goomba can **walk to the left**, and the Goomba can **attack** the player. These actions can be thought of as states. The "running left" state will move the Goomba to the left. The "attack" state will damage the player, after which the Goomba will return to the "running left" state. For a more complicated example, one may observe the Grunt from the Halo franchise. The Grunt has many states. The Grunt can be idle, alert, sleeping, suicide bombing, attacking the player, or hiding. A Grunt may be in the "attack" state. If you kill the Elite who is leading the grunt, he will get scared, and enter the "hiding" state. A grunt may be in the "idle" state. If the grunt sees you, it may enter the "attack" state, and so on. It would appear that one can describe any automaton by describing its states, and under which circumstances they will change. This abstraction is as simple as it is powerful.

# Making descriptions from observations
Using the AI Schema, one can define each state of the AI, and what state the AI will start in. In the case of a Goomba, one might produce something like this.

Goomba AI.json :
```json
{
    "$schema" : "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/ai-schema.json",
    "name"    : "Goomba",
    "states"  :
    [
        "running left",
        "attack"
    ],
    "initial state" : "running left"
}
```

If you would like to, reader, you may try toconfigure an AI in your browser [right now](https://github.dev/Jacob-C-Smith/G10-Schema/blob/main/examples/ai/goomba%20ai.json). Consider how you want your AI to act, and how you might translate these behaviors into states. Make certain to validate your AI against the [AI Schema](https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/ai-schema.json). If you are using Visual Studio Code, any validation errors in your JSON can be found as warnings in the "Problems" panel of the IDE.  

When you have configured an AI to your liking, you may add to the entity, the AI object, or a path to a file containing the AI JSON on the filesystem. 

goomba entity.json :
```json
{
    "$schema" : "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/entity-schema.json",
    "name"    : "Goomba",
    "parts"   : [
        ...
    ],
    ...
    "ai" : "goomba.json"
}
```

# AI tool
I've written a standalone UI application for working with AI's. 

You can download that [here](https://g10.app/utilities/ai-tool/) and it looks like this

![](https://g10.app/utilities/ai-tool/ai-tool-use.png)

# Programming the AI
The following details how to program an AI.

## Getting the AI
One needs a pointer to the AI which they plan to program. The pointer can be retrieved from a GXEntity_t, or via the caching mechanism.

```c
// Initialized data
g_instance *p_instance  = g_get_active_instance();
entity     *p_goomba    = get_entity(p_instance->context.scene, "Goomba");
ai         *p_goomba_ai = p_goomba->ai;
```
 -or- 
```c
// Initialized data
GXInstance_t *p_instance  = g_get_active_instance();
GXAI_t       *p_ai        = g_find_ai(p_instance, "Goomba AI");
```

## Writing state functions
With a pointer to the AI, one can start to implement states and preupdate functions. Continuing with the Goomba example, one might implement the "running left" and "attack" states like so:

```c

// Forward declaration
void hurt_mario();

void goomba_state_run_left ( GXEntity_t *p_goomba )
{

    // Initialized data
    vec3 *goomba_velocity = &p_goomba->rigidbody->velocity;

    // Set the Goomba's horizontal speed to -1 units / second, 
    goomba_velocity->x = -1.f;
}

void goomba_state_attack ( GXEntity_t *p_goomba )
{

    // Damage Mario once
    hurt_mario();

    // Continue moving to the left
    set_ai_state(p_ai, "running left");
}
```


## Pre Update

Now that one has working states, one can define under which circumstances those states should change. This means implementing a pre update function. In the case of the Goomba, one approach might look like this:

```c
// Forward declarations
bool colliding_with_mario();

void goomba_preupdate ( GXEntity_t *p_goomba )
{

    // Initialized data
    GXInstance_t *p_instance = g_get_active_instance();
    GXAI_t       *p_ai       = p_goomba->ai;

    // Are the Goomba and Mario colliding?
    if( colliding_with_mario() == true )
    {

        // Attack state
        set_ai_state(p_ai, "attack");
    }
}
```

## Setting up AI callbacks
After implementing each state and preupdate, one can attach the state functions and pre update functions to the AI using the ```add_ai_state_callback()``` and ```set_ai_pre_update_callback()``` functions. This should be done before the game loop / scheduler starts, and after the AI has been loaded. You, reader, only need to set your callbacks once. You do not need to set them for every entity using your AI.  

```c
// Initialize G10
...

void goomba_state_run_left ( GXEntity_t *p_goomba );
void goomba_state_attack   ( GXEntity_t *p_goomba );
void goomba_preupdate      ( GXEntity_t *p_goomba );

...

// Set up the Goomba AI
add_ai_state_callback(p_ai, "attack"  , &goomba_state_attack);
add_ai_state_callback(p_ai, "running left", &goomba_state_run_left);
set_ai_pre_update_callback(p_ai, &goomba_preupdate);

// Start the game
...
```

You now know how to design and implement an AI using G10. 

```
Written by Jacob Smith
```