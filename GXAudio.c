#include <G10/GXAudio.h>

#ifdef BUILD_G10_WITH_FMOD

dict* sounds = 0;
int* channels = 0;
int	current_channel = 0;

int init_audio(void)

{
	GXInstance_t* instance = g_get_active_instance();
	FMOD_RESULT r = FMOD_System_Create(&(instance->fmod.system), FMOD_VERSION);

	//FMOD Error Handling
	{
		#ifndef NDEBUG
		if (r != FMOD_OK)
			g_print_error("[FMOD] Error! Error code: %d\n", r);
		#endif
	}

	r = FMOD_System_Init(instance->fmod.system, 64, FMOD_INIT_NORMAL, FMOD_OUTPUTTYPE_AUTODETECT);

	//FMOD Error Handling
	{
		#ifndef NDEBUG
		if (r != FMOD_OK)
			g_print_error("[FMOD] Error! Error code: %d\n", r);
		#endif
	}

	return 1;
}

int  process_audio(GXInstance_t* p_instance)
{
	//Well, that was easy.
	FMOD_RESULT r = FMOD_System_Update(p_instance->fmod.system);

	//FMOD Error Handling
	{
		#ifndef NDEBUG
		if (r != FMOD_OK)
			g_print_error("[FMOD] Error! Error code: %d\n", r);
		#endif
	}

	return 1;
}

int load_sound(GXSound_t** pp_sound, const char* path)
{


	// Argument check
	{
		#ifndef NDEBUG
		if (path == (void*)0)
			goto no_path;
		#endif
	}

	GXSound_t *p_sound = calloc(1, sizeof(GXSound_t));
	*pp_sound = p_sound;

	FMOD_RESULT r = FMOD_System_CreateSound(g_get_active_instance()->fmod.system, path, FMOD_DEFAULT, NULL, &(p_sound->sound));

	// FMOD Error Handling
	{
		#ifndef NDEBUG
		if (r != FMOD_OK)
			g_print_error("[FMOD] Error! Error code: %d\n", r);
		#endif
	}

	// G10 Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input} Null pointer provided for parameter \"path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
        }
    }

}

int play_sound(GXSound_t* p_sound, bool startPaused)
{
	// Argument check
	{
	#ifndef NDEBUG
		if (p_sound == (void*)0)
			goto no_sound;
	#endif
	}

	//Tweak parameters later.
	FMOD_RESULT r = FMOD_System_PlaySound(g_get_active_instance()->fmod.system, p_sound->sound, NULL, startPaused, NULL);

	// FMOD Error Handling
	{
		#ifndef NDEBUG
		if (r != FMOD_OK)
			g_print_error("[FMOD] Error! Error code: %d\n", r);
		#endif
	}

	// G10 Error handling
	{
		// Argument error
		{
			no_sound:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input} Null pointer provided for parameter \"p_sound\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
		}

	}

}

int destroy_sound(GXSound_t* p_sound)
{
	// Argument check
	#ifndef NDEBUG
	if (p_sound == (void*)0)
		goto no_sound;
	#endif

	FMOD_RESULT r = FMOD_Sound_Release(p_sound->sound);

	// FMOD Error Handling
	{
		#ifndef NDEBUG
		if (r != FMOD_OK)
			g_print_error("[FMOD] Error! Error code: %d\n", r);
		#endif
	}

	free(p_sound);

	return 1;

	// G10 Error handling
	{
		// Argument error
		{
			no_sound:
                #ifndef NDEBUG
                    g_print_error("[G10] [Input} Null pointer provided for parameter\"p_sound\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                return 0;
		}

	}
}
#endif