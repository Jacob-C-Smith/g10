/** !
 * Include header for G10.
 * 
 * @file g10/g10.h
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// log submodule
#include <log/log.h>

// g10
#include <g10/gtypedef.h>

// 3rd party

// Discord
#ifdef BUILD_G10_WITH_DISCORD
    #include <discord_game_sdk.h>
#endif

// FMOD
#ifdef BUILD_G10_WITH_FMOD
    #include <FMOD-core/fmod.h>
#endif

// Platform dependent macros
#ifdef _WIN64
    #define DLLEXPORT extern __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

// Set reallocator macros
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef STACK_REALLOC
    #undef STACK_REALLOC
    #define STACK_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef QUEUE_REALLOC
    #undef QUEUE_REALLOC
    #define QUEUE_REALLOC(p, sz) realloc(p, sz)
#endif

#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

#ifndef G10_REALLOC
    #define G10_REALLOC(p, sz) realloc(p,sz)
#endif

// Build version
#define G10_VERSION_MAJOR 1
#define G10_VERSION_MINOR 0
#define G10_VERSION_PATCH 0

// Build options
#define BUILD_G10_WITH_ANSI_COLOR true
//#define BUILD_G10_WITH_DISCORD
//#define BUILD_G10_WITH_FMOD

// Structures
struct g_instance_s
{

    // Name
    const char *const name; 

    // Running?
    bool running; 
};

// Allocators

/** !
 *  Construct a g10 instance
 *
 * @param pp_instance : return
 * @param path        : Path to instance json file
 *
 * @sa g_exit
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_init ( g_instance **pp_instance, const char *p_path );

// Destructors
/** !
 * Destroy a g10 instance
 *
 * @param p_instance : pointer to the instance pointer
 *
 * @sa g_init
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int g_exit ( g_instance **pp_instance );