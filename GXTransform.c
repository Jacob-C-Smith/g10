#include <G10/GXTransform.h>

int create_transform( GXTransform_t **transform ) 
{

	// Argument check
	{
		#ifndef NDEBUG
		if (transform == (void*)0)
			goto no_transform;
		#endif
	}

	// Allocate for the transform
	GXTransform_t *ret = calloc(1, sizeof(GXTransform_t));

	// Memory check
	{
		#ifndef NDEBUG
			if(ret == (void *)0)
				goto no_mem;
		#endif
	}

	// Assign the transform pointer
	*transform = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to funciton \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int construct_transform(GXTransform_t** transform, vec3 location, quaternion rotation, vec3 scale)
{

	// Argument check
	{
		#ifndef NDEBUG
			if(transform == (void *)0)
				goto no_transform;
		#endif
	}

	// Initialized data
	GXTransform_t *i_transform = 0;

	// Allocate the transform
	{
		create_transform(transform);
		i_transform = *transform;
	}

	// Set the location, rotation, and scale
	{
		i_transform->location = location;
		i_transform->rotation = rotation;
		i_transform->scale    = scale;

		transform_model_matrix(&i_transform->model_matrix, i_transform);
	}

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_transform( GXTransform_t **transform, const char* path)
{
	// Argument check
	{
		#ifndef NDEBUG
			if(transform == (void *)0)
				goto no_transform;
			if(path == (void *)0)
				goto no_path;
		#endif
	}

	// Initialized data
	GXTransform_t *t      = 0;
	size_t         i      = g_load_file(path, 0, false);;
	char          *buffer = calloc(i + 1, sizeof(char));

	// Load the file
	g_load_file(path, buffer, false);

	// Load the transform as a json string
	load_transform_as_json(transform, buffer, i);

	// Free up the memory
	free(buffer);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_transform_as_json(GXTransform_t** transform, char* object_text, size_t len)
{
	// Argument check
	{
		#ifndef NDEBUG
			if(transform == (void *)0)
				goto no_transform;
		#endif
	}

	// Initialized data
	GXTransform_t *t        = 0;
	dict          *d        = 0;
	vec3           location = { 0, 0, 0 };
	quaternion     rotation = { 0, 0, 0, 0 };
	vec3           scale    = { 1, 1, 1 };
	JSONToken_t   *token    = 0;

	// Debug only
	#ifndef NDEBUG
	size_t         i        = 0;
	#endif

	// Parse the JSON into a dictionary
	parse_json(object_text, len, &d);

	// Get the data for the constructor
	{

		// Set the location
		{
			token = dict_get(d, "location");

			// Error handling
			{
				#ifndef NDEBUG

					// Is the token present?
					if (token == (void*)0)
						goto no_location;

					// Is the token an array?
					if (token->type != JSONarray)
						goto location_type_error;

					// Count array length
					for (i = 0; token->value.a_where[i]; i++);

					// Does the array have the right number of values?
					if (i != 3)
						goto location_len_error;

				#endif
			}

			location.x = atof(token->value.a_where[0]);
			location.y = atof(token->value.a_where[1]);
			location.z = atof(token->value.a_where[2]);
		}

		// Set the rotation
		{
			
			// Get rotation in euler angle
			token = dict_get(d, "rotation");

			// Parse as an euler angle
			if (token)
			{

				// Error checking
				{
					#ifndef NDEBUG
						// Is the token an array?
						if (token->type != JSONarray)
							goto rotation_type_error;

						// Count array length
						for (i = 0; token->value.a_where[i]; i++);

						// Does the array have the right number of values?
						if (i != 3)
							goto rotation_len_error;
					#endif
				}

				vec3 eu_rot = { 0, 0, 0 };

				eu_rot.x = atof(token->value.a_where[0]);
				eu_rot.y = atof(token->value.a_where[1]);
				eu_rot.z = atof(token->value.a_where[2]);

				rotation = quaternion_from_euler_angle(eu_rot);

			}

			// Parse as a quaternion
			else
			{
				token = dict_get(d, "quaternion");

				// Error checking
				{
					#ifndef NDEBUG

						// Is the token present?
						if (token == (void*)0)
							goto no_quaternion;

						// Is the token an array?
						if (token->type != JSONarray)
							goto quaternion_type_error;

						// Count array length
						for (i = 0; token->value.a_where[i]; i++);

						// Does the array have the right number of values?
						if (i != 4)
							goto quaternion_len_error;
						
					#endif
				}
	
				rotation.u = atof(token->value.a_where[0]);
				rotation.i = atof(token->value.a_where[1]);
				rotation.j = atof(token->value.a_where[2]);
				rotation.k = atof(token->value.a_where[3]);
			
			}
		}

		// Set the scale
		{
			token = dict_get(d, "scale");

			// Error handling
			{
				#ifndef NDEBUG

					// Is the token present?
					if (token == (void*)0)
						goto no_scale;

					// Is the token an array?
					if (token->type != JSONarray)
						goto scale_type_error;

					// Count array length
					for (i = 0; token->value.a_where[i]; i++);

					// Does the array have the right number of values?
					if (i != 3)
						goto scale_len_error;

				#endif
			}

			scale.x = atof(token->value.a_where[0]);
			scale.y = atof(token->value.a_where[1]);
			scale.z = atof(token->value.a_where[2]);
		}

	}

	// Construct the transform
	construct_transform(transform, location, rotation, scale);

	// Free the dictionary
	dict_destroy(d);

	return 1;

	// Error handling 
	{

		// Argument errors
		{
			no_transform:
				#ifndef NDEBUG
					g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}

		
		// JSON errors
		{

			// Location errors
			{
				no_location:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] No \"location\" property found in call to function \"%s\"\n",__FUNCSIG__);
					#endif
					return 0;

				location_type_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"location\" property should be of type \"array\", not \"%s\" in call to function \"%s\"\n", token_types[token->type], __FUNCSIG__);
					#endif
					return 0;

				location_len_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] Expected \"location\" property to be of length 3, not %d, in call to function \"%s\"\n",i, __FUNCSIG__);
					#endif
					return 0;
			}

			// Rotation errors
			{
				no_rotation:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] No \"rotation\" property found in call to function \"%s\"\n",__FUNCSIG__);
					#endif
					return 0;
				rotation_type_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"rotation\" property should be of type \"array\", not \"%s\" in call to function \"%s\"\n", token_types[token->type], __FUNCSIG__);
					#endif
					return 0;
				rotation_len_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"rotation\" property to be of length 3, not %d, in call to function \"%s\"\n",i, __FUNCSIG__);
					#endif
					return 0;
			}

			// Quaternion errors
			{
				no_quaternion:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] No \"quaternion\" property found in call to function \"%s\"\n",__FUNCSIG__);
					#endif
					return 0;
				quaternion_type_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"quaternion\" property should be of type \"array\", not \"%s\" in call to function \"%s\"\n", token_types[token->type], __FUNCSIG__);
					#endif
					return 0;
				quaternion_len_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"quaternion\" property to be of length 4, not %d, in call to function \"%s\"\n",i, __FUNCSIG__);
					#endif
					return 0;
			}

			// Scale errors
			{
				no_scale:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] No \"scale\" property found in call to function \"%s\"\n",__FUNCSIG__);
					#endif
					return 0;
				scale_type_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"scale\" property should be of type \"array\", not \"%s\" in call to function \"%s\"\n", token_types[token->type], __FUNCSIG__);
					#endif
					return 0;
				scale_len_error:
					#ifndef NDEBUG
						g_print_error("[G10] [Transform] \"scale\" property to be of length 3, not %d, in call to function \"%s\"\n",i, __FUNCSIG__);
					#endif
					return 0;
			}
		}
	}
}

void transform_model_matrix( GXTransform_t* transform, mat4 *r )
{

    // Argument check
    {
        if(r ==  (void*)0)
            goto no_result;

        if (transform == (void*)0)
            goto no_transform;
    }

    mat4 sca = scale_mat4(transform->scale),
         rot = rotation_mat4_from_quaternion(transform->rotation),
         tra = translation_mat4(transform->location),
         M   = mul_mat4_mat4(sca, mul_mat4_mat4(rot, tra));

	// Set up the 4x4 matrix
    r->a = M.a, r->b = M.b, r->c = M.c, r->d = M.d,
    r->e = M.e, r->f = M.f, r->g = M.g, r->h = M.h,
    r->i = M.i, r->j = M.j, r->k = M.k, r->l = M.l,
    r->m = M.m, r->n = M.n, r->o = M.o, r->p = M.p;

    return;

    // Error handling
    {
        
        // Argument errors
        {
            no_result:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for \"r\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return;

            no_transform:
            #ifndef NDEBUG
                g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n", __FUNCSIG__);
            #endif
            return;
        }
    }
}

//int            rotate_about_quaternion ( GXTransform_t *transform, quaternion axis, float theta)
//{
//    // Argument check
//    {
//        if ( transform == (void *) 0 )
//            goto no_transform;
//    }
//
//    /*
//    * To rotate around a quaternion, multiply the transform
//    * quaternion a special quaternion called 'p', derived by multiplying
//    * u by sin( theta/2 ) and multiplying i, j, and k by cos( theta/2 ).
//    * Then, multiply the inverse of p, henceforth " p' ". This is analogous
//    * to complex multiplication.
//    */
//
//    float      halfAngle = theta / 2,
//               cosHalf   = cosf(halfAngle),
//               sinHalf   = sinf(halfAngle);
//    vec3       newIJK    = { transform->rotation.i,transform->rotation.j,transform->rotation.k };
//    
//    rotate_vec3_by_quaternion(&newIJK, newIJK, axis);
//    transform->rotation = (quaternion){ cosHalf, sinHalf * newIJK.x, sinHalf * newIJK.y, sinHalf * newIJK.z };
//
//    return 0;
//
//    // Error handling
//    {
//        no_transform:
//        #ifndef NDEBUG
//            g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to funciton \"%s\"\n",__FUNCSIG__);
//        #endif
//        return 0;
//    }
//
//}

int            destroy_transform      ( GXTransform_t *transform )
{

    // Argument check
    {
        if (transform == (void*)0)
            goto noTransform;
    }

    // Free the transform
    free(transform);

    return 0;

    //  Error handling
    {

		// Argument error
		{
	        noTransform:
		    #ifndef NDEBUG
			    g_print_error("[G10] [Transform] Null pointer provided for \"transform\" in call to function \"%s\"\n", __FUNCSIG__);
		    #endif
	        return 0;
		}
    }
}
