#include <G10/GXMaterial.h>

void init_material ( void )
{

	// Initialized data
	GXInstance_t *p_instance = g_get_active_instance();

	// Create a material cache mutex
	p_instance->mutexes.material_cache = SDL_CreateMutex();
}

int create_material ( GXMaterial_t** material)
{

	// Argument check
	{
		#ifndef NDEBUG
			if (material == (void*)0)
				goto no_material;
		#endif
	}

	// Initialized data
	GXMaterial_t *ret = calloc(1, sizeof(GXMaterial_t));

	// Error checking
	if ( ret == (void *) 0 )
		goto no_mem;

	*material = ret;

	return 1;

	// Error handling
	{
		
		// Argument checking
		{
			no_material:
				#ifndef NDEBUG
					g_print_error("[G10] [Material] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_material ( GXMaterial_t **pp_material, const char *path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_material == (void *)0)
				goto no_material;
			if (path == (void *)0)
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len  = g_load_file(path, 0, false);
	char   *text = calloc(len + 1, sizeof(char));
	
	// Error checking
	if ( text == (void *) 0 )
		goto no_mem;

	// Load the file
	if ( g_load_file(path, text, false) == 0 )

		// TODO: goto, error handling
		return 0;

	if ( load_material_as_json(pp_material, text) == 0)
		// TODO: goto, error handling
		return 0;

	free(text);

	// Success
	return 1;

	// Error handling
	{
		
		// Argument checking
		{
			no_material:
				#ifndef NDEBUG
					g_print_error("[G10] [Material] Null pointer provided for \"pp_material\" in call to function \"%s\"\n", __FUNCTION__);
				#endif
	
				// Erorr
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Material] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Erorr
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Erorr
				return 0;
		}
	}
}

int load_material_as_json(GXMaterial_t** material, char* text )
{
	
	// Argument check
	{
		#ifndef NDEBUG
		if ( material == (void *) 0 )
			goto no_material;
		if ( text == (void *) 0 )
			goto no_text;
		#endif
	}

	// Initialized data
	GXInstance_t  *p_instance           = g_get_active_instance();
	GXMaterial_t  *p_material           = 0;
	dict          *material_object_json = 0;
	char          *name                 = 0,
		         **textures             = 0;

	// Parse the JSON into a dictionary
	//parse_json(text, len, &material_object_json);

	/*
	{
		"$schema": "https://raw.githubusercontent.com/Jacob-C-Smith/G10-Schema/main/material-schema.json",
	    "name": "Material",
		"textures": [
			{
				"name": "Texture",
		        "path": "path/to/file.png",
	            "addressing": "repeat",
				"filter": "linear"
			}
		]
	}
	*/

	// Construct the material
	{

		// Copy the name
		{

			// Initialized data
			size_t len = strlen(name);
			
			// Allocate memory for the name
			p_material->name = calloc(1, sizeof(char));

			// Copy the name
			strncpy(p_material->name, name, len);
		}

		// Construct the textures

	}

	// Success
	return 1;

	// Error handling
	{
		
		// Argument checking
		{
			no_material:
				#ifndef NDEBUG
					g_print_error("[G10] [Material] Null pointer provided for \"pp_material\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Material] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			//no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}
