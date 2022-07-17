#include <G10/GXMaterial.h>

int create_material(GXMaterial_t** material)
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

int load_material(GXMaterial_t** material, const char path[])
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

	char   *token_text = 0;
	size_t  len        = 0;
	
	// Load the file
	{
		len = g_load_file(path, 0, false);
		token_text = calloc(len + 1, sizeof(char));
		g_load_file(path, token_text, false);
	}

	load_material_as_json_n(material, token_text, len);



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

int load_material_as_json(GXMaterial_t** material, char* token_text)
{
	// Argument check
	{
		#ifndef NDEBUG
		if (material == (void *)0)
			goto no_material;
		if (token_text == (void *)0)
			goto no_token_text;
		#endif
	}

	size_t  len        = strlen(token_text);

	load_material_as_json_n(material, token_text, len);

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
	}
}

int load_material_as_json_n(GXMaterial_t** material, char* token_text, size_t len)
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

	//// Initialized data
	//GXInstance_t *instance             = g_get_active_instance();
	//GXMaterial_t *i_material           = 0;
	//dict         *material_object_json = 0;
	//JSONToken_t  *token                = 0;

	//// JSON data
	//char         *name                 = 0;

	//// Allocate for the material
	//create_material(material);

	//// Get a pointer to the material
	//i_material = *material;

	//// Parse token_text object into a dictionary
	//parse_json(token_text, len, &material_object_json);

	//// Construct the material
	//{
	//	
	//	// Initialized data
	//	char    *textures_object_json = 0,
	//		   **keys                 = 0,
	//		   **values               = 0;
	//	size_t   textures_len         = 0;

	//	// Get the name
	//	token                = dict_get(material_object_json, "name");
	//	i_material->name     = (token) ? token->value.n_where : 0;

	//	// Get the textures object
	//	token                = dict_get(material_object_json, "textures");
	//	textures_object_json = (token) ? token->value.n_where : 0;

	//	// Parse the JSON into a texture dictionary
	//	parse_json(textures_object_json, strlen(textures_object_json), &i_material->textures);

	//	// Get the texture count
	//	textures_len = i_material->textures->n_entries;

	//	// Allocate space for pointer lists
	//	keys   = calloc(textures_len + 1, sizeof(void*));
	//	values = calloc(textures_len + 1, sizeof(void*));

	//	// Populate the list of keys and values
	//	dict_keys(i_material->textures, keys);
	//	dict_values(i_material->textures, values);

	//	// Iterate over each texture
	//	for (size_t i = 0; i < textures_len; i++)
	//	{

	//		// Initialized data
	//		GXTexture_t* texture = 0;

	//		// Get the JSON token
	//		token = dict_get(i_material->textures, keys[i]);

	//		// Error checking
	//		{
	//			#ifndef NDEBUG
	//				if(token->type != JSONstring)
	//				{
	//					g_print_warning("[G10] [Material] Failed to load texture \"%s\" in call to function \"%s\". Texture will display as missing.\n", token->key, __FUNCSIG__);
	//				}
	//			#endif
	//		}

	//		load_texture(&texture, token->value.n_where);

	//		dict_add(i_material->textures, keys[i], texture);

	//	}

	//	free(keys);
	//	free(values);

	//}

	//// Add the constructed material to the instacne cache
	//dict_add(instance->cached_materials, i_material->name, i_material);

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
