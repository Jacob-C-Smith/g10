#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

#include <SDL2/SDL_audio.h>

struct GXSound_s
{
	int i;
};

DLLEXPORT void load_sound                 ( GXSound_t  **sound  , const char *path );
DLLEXPORT void unload_sound               ( GXSound_t   *sound );
DLLEXPORT void set_3d_listener_and_orient ( GXCamera_t  *camera );
DLLEXPORT int  play_sound                 ( GXSound_t   *sound  , vec3        position, float dB);
DLLEXPORT void stop_channel               ( int          channel );
DLLEXPORT void stop_all_channels          ( void );
DLLEXPORT void set_channel_3d_position    ( int          channel, vec3        position );
DLLEXPORT void set_channel_volume         ( int          channel, float       dB );
DLLEXPORT bool is_playing                 ( int          channel );
