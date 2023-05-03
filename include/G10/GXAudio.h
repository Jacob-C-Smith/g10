#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

#ifdef BUILD_G10_WITH_SDL_MIXER
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>
#endif

#ifdef BUILD_G10_WITH_FMOD
#include <FMOD-core/fmod.h>
#include <FMOD-core/fmod_common.h>
#endif

struct GXSound_s
{
	#ifdef BUILD_G10_WITH_FMOD
		FMOD_SOUND **sound;
	#endif
	int i;
};




// Audio processing
DLLEXPORT int				process_audio(GXInstance_t* instance);

DLLEXPORT int				load_sound(GXSound_t** sound, const char* path);
DLLEXPORT int				load_sound_as_json(GXSound_t** sound, char* token_text, size_t len);
DLLEXPORT void				set_3d_listener_and_orient(GXCamera_t* camera);
DLLEXPORT int				play_sound(GXSound_t* sound, bool startPaused);
DLLEXPORT void				stop_channel(int          channel);
DLLEXPORT void				stop_all_channels(void);
DLLEXPORT void				set_channel_3d_position(int          channel, vec3        position);
DLLEXPORT void				set_channel_volume(int          channel, float       dB);
DLLEXPORT bool				is_playing(int          channel);
DLLEXPORT int				destroy_sound(GXSound_t* sound);

#ifdef BUILD_G10_WITH_FMOD
//TODO: May implement this, gotta talk to Jake about it
DLLEXPORT const char*		get_fmod_error_code(FMOD_RESULT r);
#endif
