#include <G10/GXTransform.h>

int  create_transform        ( GXTransform_t **pp_transform ) 
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_transform == (void*) 0 )
				goto no_transform;
		#endif
	}

	// Initialized data
	GXTransform_t *p_transform = calloc(1, sizeof(GXTransform_t));

	// Memory check
	if ( p_transform == (void *) 0 )
		goto no_mem;

	// Return the allocated memory
	*pp_transform = p_transform;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int  construct_transform     ( GXTransform_t **pp_transform, vec3 location, quaternion rotation, vec3 scale )
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_transform == (void *)0)
				goto no_transform;
		#endif
	}

	// Initialized data
	GXTransform_t *p_transform = 0;

	// Allocate the transform
	if ( create_transform(&p_transform) == 0 )
		goto failed_to_create_transform;

	// Return the transform 
	*p_transform = (GXTransform_t) { 
		.location = location,
		.rotation = rotation,
		.scale    = scale
	};

	// Compute a model matrix
	transform_model_matrix(p_transform, &p_transform->model_matrix);

	// Return the allocated memory
	*pp_transform = p_transform;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 errors
		{
			failed_to_create_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to create transform in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_transform ( GXTransform_t **pp_transform, const char *path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_transform == (void *)0)
				goto no_transform;
			if(path == (void *)0)
				goto no_path;
		#endif
	}

	// Initialized data
	size_t         file_size   = g_load_file(path, 0, false);;
	char          *file_buffer = calloc(file_size + 1, sizeof(char));

	// Load the file
	if ( g_load_file(path, file_buffer, false) == 0 )
		goto failed_to_load_file;

	// Load the transform as a json string
	if ( load_transform_as_json(pp_transform, file_buffer, file_size) == 0 )
		goto failed_to_load_transform;

	// Free up the memory
	free(file_buffer);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to open file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to load transform from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

		}
	}
}

int  load_transform_as_json  ( GXTransform_t **pp_transform, char *text, size_t len)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_transform == (void *)0 )
				goto no_transform;
			if (text == (void*)0)
				goto no_text;
		#endif
	}

	// Initialized data
	JSONValue_t *p_value    = 0;
	array       *p_location = 0,
	            *p_rotation = 0,
				*p_scale    = 0;
	
	// Parse the transform JSON
	if ( parse_json_value(text, 0, &p_value) == 0 ) 
		goto failed_to_parse_json;
	
	// Parse the transform JSON
    if (p_value->type == JSONobject)
    {
	
        // Initialized data
		dict *p_dict = p_value->object;

        // Parse the transform 
        p_location   = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "location")), JSONarray);
        p_rotation   = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "rotation")), JSONarray);
        p_scale      = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "scale"))   , JSONarray);

		// Quaternion?
		if ( p_rotation == 0 )
			p_rotation = (array *) JSON_VALUE(((JSONValue_t *)dict_get(p_dict, "quaternion")), JSONarray);

		// Error checking
		if ( ( p_location && p_rotation && p_scale ) == 0 )
			goto missing_properties;
	}
	else
		goto wrong_type;

	// Construct the transform
	{

		// Initialized data
		GXTransform_t *p_transform = 0;
		vec3           location    = { 0, 0, 0 },
	                   scale       = { 1, 1, 1 };
		quaternion     rotation    = { 0, 0, 0, 0 };

		// Set the location
		{

			// Initialized data
			JSONValue_t **pp_elements          = 0;
			size_t        vector_element_count = 0;
			
			// Get the quantity of elements
			array_get(p_location, 0, &vector_element_count );

			// Error checking
			if ( vector_element_count != 3 )
				goto location_len_error;

			// Allocate an array for the elements
			pp_elements = calloc(vector_element_count+1, sizeof(JSONValue_t *));

			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the vector
			array_get(p_location, pp_elements, 0 );			
			
			// Set the location
			location = (vec3) {
				.x = (float) ((JSONValue_t *)pp_elements[0])->floating,
				.y = (float) ((JSONValue_t *)pp_elements[0])->floating,
				.z = (float) ((JSONValue_t *)pp_elements[0])->floating
			};
				
			// Clean the scope
			free(pp_elements);

		}

		// Set the rotation
		{

			// Initialized data
			JSONValue_t **pp_elements          = 0;
			size_t        vector_element_count = 0;
			
			// Get the quantity of elements
			array_get(p_rotation, 0, &vector_element_count );

			// Error checking
			if ( !( vector_element_count == 3 && vector_element_count == 4 )  )
				goto rotation_len_error;

			// Allocate an array for the elements
			pp_elements = calloc(vector_element_count+1, sizeof(JSONValue_t *));
			
			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the vector
			array_get(p_rotation, pp_elements, 0 );			

			// Parse the data as a quaternion or an euler angle
			if ( vector_element_count == 3 )
			{

				// Set the rotation
				rotation = quaternion_from_euler_angle(
					(vec3) {
						.x = (float) ((JSONValue_t *)pp_elements[0])->floating,
						.y = (float) ((JSONValue_t *)pp_elements[1])->floating,
						.z = (float) ((JSONValue_t *)pp_elements[2])->floating
					}
				);

				// Clean the scope
				free(pp_elements);
			}
			else if ( vector_element_count == 4 )
			{

				// Set the rotation
				rotation = (quaternion) {
					.u = (float) ((JSONValue_t *)pp_elements[0])->floating,
					.i = (float) ((JSONValue_t *)pp_elements[1])->floating,
					.j = (float) ((JSONValue_t *)pp_elements[2])->floating,
					.k = (float) ((JSONValue_t *)pp_elements[3])->floating
				};

				// Clean the scope
				free(pp_elements);

			}
		}

		// Set the scale
		{

			// Initialized data
			JSONValue_t **pp_elements          = 0;
			size_t        vector_element_count = 0;
			
			// Get the quantity of elements
			array_get(p_scale, 0, &vector_element_count );

			// Error checking
			if ( vector_element_count != 3 )
				goto scale_len_error;

			// Allocate an array for the elements
			pp_elements = calloc(vector_element_count+1, sizeof(JSONValue_t *));

			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the vector
			array_get(p_scale, pp_elements, 0 );			
			
			// Set the scale
			scale = (vec3) {
				.x = (float) ((JSONValue_t *)pp_elements[0])->floating,
				.y = (float) ((JSONValue_t *)pp_elements[1])->floating,
				.z = (float) ((JSONValue_t *)pp_elements[2])->floating
			};
				
			// Clean the scope
			free(pp_elements);
		}
	
		// Construct the transform
		if ( construct_transform(&p_transform, location, rotation, scale) == 0 )
			goto failed_to_create_transform;

		// Return the transform to the caller
		*pp_transform = p_transform;
	}

	// Deallocate the JSON value
	FREE_VALUE(p_value);

	// Success
	return 1;

	// Error handling 
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"text\" in call to function \"%s\"\n",__FUNCTION__);
				#endif
				
				// Error
				return 0;
		}

		// JSON errors
		{

			failed_to_parse_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to parse JSON in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;

			wrong_type:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Expected a JSON object in call to function \"%s\"\n", __FUNCTION__);
				#endif
				
				// Error
				return 0;

			missing_properties:
				#ifndef NDEBUG
					g_print_error("[G10] [AI] Failed to construct AI in call to function \"%s\". Missing properties!\n", __FUNCTION__);
				#endif
				
				// Error
				return 0;

			location_len_error:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to parse \"location\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			rotation_len_error:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to parse rotation property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
				#endif

				// Error
				return 0;
						
			scale_len_error:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to parse \"scale\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
				#endif

				// Error
				return 0;
			
		}

		// G10 errors
		{
			failed_to_create_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Failed to create transform in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	
		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

void transform_model_matrix  ( GXTransform_t  *p_transform , mat4       *r )
{

    // Argument check
    {
        if( r == (void *) 0 )
            goto no_result;

        if ( p_transform == (void *) 0 )
            goto no_transform;
    }

    mat4 sca = scale_mat4(p_transform->scale),
         rot = rotation_mat4_from_quaternion(p_transform->rotation),
         tra = translation_mat4(p_transform->location),
         M   = mul_mat4_mat4(sca, mul_mat4_mat4(rot, tra));

	// Set up the 4x4 matrix
    r->a = M.a, r->b = M.b, r->c = M.c, r->d = M.d,
    r->e = M.e, r->f = M.f, r->g = M.g, r->h = M.h,
    r->i = M.i, r->j = M.j, r->k = M.k, r->l = M.l,
    r->m = M.m, r->n = M.n, r->o = M.o, r->p = M.p;

	// Done
    return;

    // Error handling
    {
        
        // Argument errors
        {
            no_result:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for \"r\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return;

            no_transform:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n", __FUNCTION__);
            #endif
            return;
        }
    }
}

int  rotate_about_quaternion ( GXTransform_t  *p_transform , quaternion  axis,        float      theta)
{
    // Argument check
    {
        if ( p_transform == (void *) 0 )
            goto no_transform;
    }

    /*
    * To rotate around a quaternion, multiply the transform
    * quaternion a special quaternion called 'p', derived by multiplying
    * u by sin( theta/2 ) and multiplying i, j, and k by cos( theta/2 ).
    * Then, multiply the inverse of p, henceforth " p' ". This is analogous
    * to complex multiplication.
    */

    float      halfAngle = theta / 2,
               cosHalf   = cosf(halfAngle),
               sinHalf   = sinf(halfAngle);
    vec3       newIJK    = { p_transform->rotation.i, p_transform->rotation.j, p_transform->rotation.k };
    
    rotate_vec3_by_quaternion(&newIJK, newIJK, axis);
	p_transform->rotation = (quaternion){ cosHalf, sinHalf * newIJK.x, sinHalf * newIJK.y, sinHalf * newIJK.z };

	// Success
    return 1;

    // Error handling
    {
        no_transform:
        #ifndef NDEBUG
            g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCTION__);
        #endif
        return 0;
    }

}

int  destroy_transform       ( GXTransform_t  *p_transform )
{

    // Argument check
    {
        if (p_transform == (void*)0)
            goto no_transform;
    }

    // Free the transform
    free(p_transform);

	// Success
    return 1;

    //  Error handling
    {

		// Argument error
		{
	        no_transform:
		    #ifndef NDEBUG
			    g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n", __FUNCTION__);
		    #endif
	        return 0;
		}
    }
}
