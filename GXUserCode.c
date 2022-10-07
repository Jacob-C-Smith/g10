#include <G10/GXUserCode.h>

int user_code(GXInstance_t* instance)
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if(instance == (void *)0 )
				goto no_instance;
		#endif
	}

	// Check for a user callback
	if (instance->user_code_callback)

		// Call the callback
		instance->user_code_callback(instance);


	return 0;

	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_error("[G10] [User code] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif	
				return 0;
		}
	}
}


int add_user_code_callback (GXInstance_t *instance, int (*user_code_callback)(GXInstance_t* instance) )
{

	// Argument check
	{
		#ifndef NDEBUG
			if(instance == (void *)0 )
				goto no_instance;
			if (user_code_callback == (void*)0)
				goto no_user_code_callback;
		#endif
	}

	instance->user_code_callback = user_code_callback;

	return 0; 

	// Error handling
	{

		// Argument errors
		{
			no_instance:
				#ifndef NDEBUG
					g_print_error("[G10] [User code] Null pointer provided for \"instance\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif	
				return 0;
			no_user_code_callback:
				#ifndef NDEBUG
					g_print_error("[G10] [User code] Null pointer provided for \"user_code_callback\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}
