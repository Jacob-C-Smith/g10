#include <G10/GXMaterial.h>

int create_material ( GXMaterial_t** material)
{

	// Argument check
	{
		#ifndef NDEBUG
		if (material == (void*)0)
			goto no_material;
		#endif
	}

	GXMaterial_t *ret = calloc(1, sizeof(GXMaterial_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void *)0)
				goto no_mem;
		#endif
	}

	*material = ret;

	return 1;

	// Error handling
	{
		
		// Argument checking
		{
			no_material:
			#ifndef NDEBUG
				g_print_error("[G10] [Material] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}

		// Standard library errors
		{
			no_mem:
			#ifndef NDEBUG
				g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n",__FUNCSIG__);
			#endif
			return 0;
		}
	}
}

int load_material ( GXMaterial_t **material, const char path[])
{

	// Argument check
	{
		#ifndef NDEBUG
			if (material == (void *)0)
				goto no_material;
			if (path == (void *)0)
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len + 1, sizeof(char));
	
	// Load the file
	if ( g_load_file(path, token_text, false) == 0 )

		// TODO: goto, error handling
		return 0;

	if ( load_material_as_json(material, token_text, len) == 0)
		// TODO: goto, error handling
		return 0;

	free(token_text);

	return 1;

	// Error handling
	{
		
		// Argument checking
		{
			no_material:
			#ifndef NDEBUG
				g_print_error("[G10] [Material] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;

			no_path:
			#ifndef NDEBUG
				g_print_error("[G10] [Material] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}

		// Standard library errors
		{
			no_mem:
			#ifndef NDEBUG
				g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n",__FUNCSIG__);
			#endif
			return 0;
		}
	}
}

int load_material_as_json(GXMaterial_t** material, char* token_text, size_t len)
{
	
	// Argument check
	{
		#ifndef NDEBUG
		if (material == (void *)0)
			goto no_material;
		if (token_text == (void *)0)
			goto no_token_text;
		#endif

		// If there is no length, there is nothing to do
		if (len == 0)
			return 0;
	}

	// Initialized data
	GXInstance_t  *instance             = g_get_active_instance();
	GXMaterial_t  *p_material           = 0;
	dict          *material_object_json = 0;
	JSONToken_t   *token                = 0;
	char          *name                 = 0,
		         **textures             = 0;

	// Parse the JSON into a dictionary
	parse_json(token_text, len, &material_object_json);

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

	// Parse the dictionary
	{

		// Initialized data
		JSONToken_t *t = 0;

		t        = dict_get(material_object_json, "name");
		name     = JSON_VALUE(t, JSONstring);

		t        = dict_get(material_object_json, "textures");
		textures = JSON_VALUE(t, JSONarray);
	}

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

	return 1;

	// Error handling
	{
		
		// Argument checking
		{
			no_material:
			#ifndef NDEBUG
				g_print_error("[G10] [Material] Null pointer provided for \"material\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;

			no_token_text:
			#ifndef NDEBUG
				g_print_error("[G10] [Material] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCSIG__);
			#endif
			return 0;
		}

		// Standard library errors
		{
			no_mem:
			#ifndef NDEBUG
				g_print_error("[Standard Library] Out of memory in call to function \"%s\"\n",__FUNCSIG__);
			#endif
			return 0;
		}
	}
}
