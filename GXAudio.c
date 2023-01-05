#include <G10/GXAudio.h>

#ifdef BUILD_G10_WITH_FMOD

dict* sounds = 0;
int* channels = 0;
int	current_channel = 0;

void init_audio(void)
{
	//Error checking
	FMOD_RESULT r;
	r = FMOD_System_init(g_get_active_instance()->fmod.system, 64, FMOD_INIT_NORMAL, FMOD_OUTPUTTYPE_AUTODETECT);

}

int load_sound(GXSound_t** pp_sound, const char* path)
{
	//Fix pointer and wrapping stuff

	//Error checking
	//Does null work?
	FMOD_RESULT r;

	GXSound_t* p_sound = *pp_sound;
	p_sound = calloc(1, sizeof(GXSound_t));

	r = FMOD_System_CreateSound(g_get_active_instance()->fmod.system, path, FMOD_DEFAULT, NULL, p_sound->sound);
}

int destroy_sound(GXSound_t* p_sound)
{
	FMOD_RESULT r;
	r = FMOD_Sound_Release(p_sound->sound);
	free(p_sound);

}

//Where's the function signature for this?
void free_audio(void)
{
	FMOD_RESULT r;
	r = FMOD_System_Release(g_get_active_instance()->fmod.system);
}

#endif