#include <G10/GXAudio.h>

dict *sounds          = 0;
int  *channels        = 0;
int   current_channel = 0;

void init_audio ( void )
{
	dict_construct(&sounds, 64);
	channels = calloc(64, sizeof(int));
}

int create_sound ( GXSound_t **pp_sound )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_sound == (void *) 0)
				goto no_sound;
		#endif
	}

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_sound:
				#ifndef NDEBUG
					g_print_error("[G10] [Audio] Null pointer provided for \"pp_sound\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_sound ( GXSound_t** sound, const char* path )
{

}
