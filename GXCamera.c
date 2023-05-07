#include <G10/GXCamera.h>

mat4 perspective_matrix      ( float       fov,    float       aspect       , float near_clip , float far_clip)
{
	/*
	 * Compute perspective projection, where f = fov, a = aspect, n = near, and r = far
	 * ┌                                                      ┐
	 * │ (a*tan(f/2))^-1, 0,             0,                0, │
	 * │ 0,               (tan(f/2))^-1, 0,                0  │
	 * │ 0,               0,             -((r+n)/(r-n)),  -1  │
	 * │ 0,               0,             -((2*r*n)/(r-n)), 0  │
	 * └                                                      ┘
	 */

	return (mat4) {
		(1 / (aspect * tanf(fov / 2))), 0                , 0                                                  ,  0,
		0                             , 1 / tanf(fov / 2), 0                                                  ,  0,
		0                             , 0                , (-(far_clip + near_clip) / (far_clip - near_clip)) , -1,
		0                             , 0                , (2 * far_clip * near_clip / (near_clip - far_clip)),  0
	};
}

int get_camera_position ( void *ret )
{

	// Argument errors
	{
		#ifndef NDEBUG
			if ( ret == (void *) 0 )
				goto no_return;
		#endif
	}

	// Initialized data
	GXInstance_t *p_instance        = g_get_active_instance();
	vec3          camera_position = p_instance->context.scene->active_camera->location;

	// Write the camera position to the return
	*(vec3 *)ret = camera_position;

	return sizeof(vec3);

	// Error handling
	{

		// Argument errors
		{
			no_return:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"ret\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

mat4 look_at          ( vec3        eye,    vec3        target       , vec3  up ) 
{ 
    // Compute forward direction
    vec3 f = normalize((vec3) {
        .x = eye.x - target.x,
        .y = eye.y - target.y,
        .z = eye.z - target.z
    });

    // Compute left direction as cross product of up and forward
    vec3 l = normalize(cross_product_vec3(up, f));

    // Recompute up
    vec3 u = cross_product_vec3(f, l);
    
    // Return the view matrix
    return (mat4) {
        l.x, u.x, f.x, 0,
        l.y, u.y, f.y, 0,
        l.z, u.z, f.z, 0,
        -dot_product_vec3(l,eye), -dot_product_vec3(u, eye), -dot_product_vec3(f, eye), 1
    };
};

int  create_camera    ( GXCamera_t **pp_camera )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_camera == (void *) 0 )
				goto no_camera;
		#endif
	}

	// Initialized data
	GXCamera_t *p_camera = calloc(1, sizeof(GXCamera_t));

	// Error checking
	{
		#ifndef NDEBUG
			if ( p_camera == (void *) 0 )
				goto no_mem;
		#endif
	}

	// Write the return value
	*pp_camera = p_camera;

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"pp_camera\" in call to function \"%s\"", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int  load_camera ( GXCamera_t **pp_camera, const char *path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_camera == (void *) 0 )
				goto no_camera;
			if ( path == (void *) 0 )
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  file_len  = g_load_file(path, 0, false);
	char   *file_text = calloc(file_len + 1, sizeof(char));

	// Load the file
	if ( g_load_file(path, file_text, false) == 0 )
		goto failed_to_load_file;

	if ( load_camera_as_json(pp_camera, file_text, file_len) == 0 )
		goto failed_to_load_camera_as_json;

	// Clean up the scope
	free(file_text);

	// Success
	return 1;

	// Error handling
	{

		// Arugment check
		{

			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
			
			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 Errors
		{
			failed_to_load_file:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to load file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_camera_as_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to construct camera from file \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;

		}
	}
}

int  load_camera_as_json ( GXCamera_t **pp_camera, char *text )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_camera == (void *) 0 )
				goto no_camera;
			if ( text == (void *) 0 )
				goto no_object_text;
		#endif
	}

	// Initialized data
	char        *name         = 0;
	array	    *p_location   = 0,
		        *p_target     = 0,
		        *p_up         = 0;
	float        near_clip    = 0,
		         far_clip     = 0,
		         aspect_ratio = 0,
		         fov          = 0;
	JSONValue_t *p_value      = 0;

	// Parse the text into a JSON value
	if ( parse_json_value(text, 0, &p_value) == 0 ) 
		goto failed_to_parse_json;

	// Parse the camera JSON
	if ( p_value->type == JSONobject )
	{

		// Initialized data
		dict *p_dict = p_value->object;
		
		// Parse the JSON values into constructor parameters
		name       = ((JSONValue_t *)dict_get(p_dict, "name"))->string;
		near_clip  = ((JSONValue_t *)dict_get(p_dict, "near"))->floating;
		far_clip   = ((JSONValue_t *)dict_get(p_dict, "far"))->floating;  
		fov        = ((JSONValue_t *)dict_get(p_dict, "fov"))->floating;     
		p_location = ((JSONValue_t *)dict_get(p_dict, "location"))->list;
		p_target   = ((JSONValue_t *)dict_get(p_dict, "front"))->list;
		p_up       = ((JSONValue_t *)dict_get(p_dict, "up"))->list;      
 
		// Error checking
		if ( ( name && near_clip && far_clip && fov && p_location && p_target && p_up ) == 0 )
			goto missing_properties;
	}
	else
		goto wrong_type;

	// Construct the camera
	{

		// Initialized data		
		GXCamera_t *p_camera = 0;
		vec3        location  = { 0, 0, 0 },
	                target    = { 0, 0, 0 },
		            up        = { 0, 0, 0, 0 };

		// Allocate memory for a camera
		if ( create_camera(&p_camera) == 0 )
			goto failed_to_allocate_camera;

		// Copy the camera name
		{

			// Initialized data
			size_t name_len = strlen(name);
			
			// Allocate memory for the name
			p_camera->name = calloc(name_len + 1, sizeof(char));

			// Copy the name
			strncpy(p_camera->name, name, name_len);
		}

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

			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the vector
			array_get(p_location, pp_elements, 0 );			
			
			// Set the location
			location = (vec3) { 
				.x = (float) ((JSONValue_t *)pp_elements[0])->floating,
				.y = (float) ((JSONValue_t *)pp_elements[1])->floating,
				.z = (float) ((JSONValue_t *)pp_elements[2])->floating
			};
				
			// Clean the scope
			free(pp_elements);
		}

		// Set the target
		{

			// Initialized data
			JSONValue_t **pp_elements          = 0;
			size_t        vector_element_count = 0;
			
			// Get the quantity of elements
			array_get(p_target, 0, &vector_element_count );

			// Error checking
			if ( vector_element_count != 3 )
				goto target_len_error;

			// Allocate an array for the elements
			pp_elements = calloc(vector_element_count+1, sizeof(JSONValue_t *));

			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the vector
			array_get(p_target, pp_elements, 0 );			
			
			// Set the target
			target = (vec3) { 
				.x = (float) ((JSONValue_t *)pp_elements[0])->floating,
				.y = (float) ((JSONValue_t *)pp_elements[1])->floating,
				.z = (float) ((JSONValue_t *)pp_elements[2])->floating
			};
				
			// Clean the scope
			free(pp_elements);
		}

		// Set the up vector
		{

			// Initialized data
			JSONValue_t **pp_elements          = 0;
			size_t        vector_element_count = 0;
			
			// Get the quantity of elements
			array_get(p_up, 0, &vector_element_count );

			// Error checking
			if ( vector_element_count != 3 )
				goto up_len_error;

			// Allocate an array for the elements
			pp_elements = calloc(vector_element_count+1, sizeof(JSONValue_t *));

			// Error checking
			if ( pp_elements == (void *) 0 )
				goto no_mem;

			// Populate the elements of the vector
			array_get(p_up, pp_elements, 0 );			
			
			// Set the up vector
			up = (vec3) { 
				.x = (float) ((JSONValue_t *)pp_elements[0])->floating,
				.y = (float) ((JSONValue_t *)pp_elements[1])->floating,
				.z = (float) ((JSONValue_t *)pp_elements[2])->floating
			};
				
			// Clean the scope
			free(pp_elements);
		}

		// Construct the camera		
		*p_camera = (GXCamera_t) {
			.name         = name,
			.location     = location,
			.target       = target,
			.up           = up,
			.near_clip    = near_clip,
			.far_clip     = far_clip,
			.aspect_ratio = 16.f / 9.f,
			.fov          = fov,
			.view         = look_at(location, target, up),
			.projection   = perspective_matrix(fov, aspect_ratio, near_clip, far_clip)
		};
		
		// Return the camera to the caller
		*pp_camera = p_camera;
	}

	// Deallocate the JSON value
	FREE_VALUE(p_value);

	// Success
	return 1;

	// Error handling
	{

		// Arugment check
		{

			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
			
			no_object_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_len:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"len\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}
		
		// JSON parsing errors
		{
			failed_to_parse_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to parse JSON in call to function \"%s\"\n", __FUNCTION__);
				#endif
				
				// Error
				return 0;

			wrong_type:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Expected a JSON object in call to function \"%s\"\n", __FUNCTION__);
				#endif
				
				// Error
				return 0;

			missing_properties:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to construct Camera in call to function \"%s\". Missing properties!\n", __FUNCTION__);
				#endif
				
				// Error
				return 0;
			
			location_len_error:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to parse \"location\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
				#endif

				// Error 
				return 0;

			target_len_error:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to parse \"target\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
				#endif

				// Error 
				return 0;

			up_len_error:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to parse \"up\" property in call to function \"%s\". Expected array of length 3\n", __FUNCTION__);
				#endif

				// Error 
				return 0;

		}

		// G10 Errors
		{
			failed_to_allocate_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to allocate memory for camera in call to function \"%s\"\n",  __FUNCTION__);
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

int print_camera ( GXCamera_t *p_camera )
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( p_camera == (void *) 0 )
				goto no_camera;
		#endif
	}

	// Initialized data
	mat4 v = p_camera->view,
	     p = p_camera->projection;

	// Formatting 
    g_print_log(" - Camera info - \n");
    
    // Print the name
    g_print_log("name      : \"%s\"\n", p_camera->name);

	// Print the location of the camera
	g_print_log("location  : < %.3f, %.3f, %.3f >\n", p_camera->location.x, p_camera->location.y, p_camera->location.z );

	// Print the up vector
	g_print_log("up        : < %.3f, %.3f, %.3f >\n", p_camera->up.x, p_camera->up.y, p_camera->up.z );

	// Print the target vector
	g_print_log("front     : < %.3f, %.3f, %.3f >\n", p_camera->target.x, p_camera->target.y, p_camera->target.z );

	// Print the near clip distance
	g_print_log("near      : %.3f\n", p_camera->near_clip );

	// Print the far clip distance
	g_print_log("far       : %.1f\n", p_camera->far_clip );

	// Print the aspect ratio
	g_print_log("aspect    : %.2f\n", p_camera->aspect_ratio );

	// Print the FOV
	g_print_log("fov       : %.1f\n", p_camera->fov );

	// Print the view matrix
	g_print_log(
		"view      : < < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
		"              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
		"              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
		"              < %2.2f, %2.2f, %2.2f, %2.2f > >\n",
		v.a, v.b, v.c, v.d,
		v.e, v.f, v.g, v.h,
		v.i, v.j, v.k, v.l,
		v.m, v.n, v.o, v.p );

	// Print the view matrix
	g_print_log(
		"projection: < < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
		"              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
		"              < %2.2f, %2.2f, %2.2f, %2.2f >,\n" \
		"              < %2.2f, %2.2f, %2.2f, %2.2f > >\n",
		p.a, p.b, p.c, p.d,
		p.e, p.f, p.g, p.h,
		p.i, p.j, p.k, p.l,
		p.m, p.n, p.o, p.p );

	putchar('\n');

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_camera: 
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"p_camera\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int destroy_camera ( GXCamera_t **pp_camera )
{
	
	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_camera == (void *) 0 )
				goto no_camera;
		#endif
	}

	// Initialized data
	GXCamera_t *p_camera = *pp_camera;

	// No more pointer for you
	*pp_camera = 0;

	// Free the name
	free(p_camera->name);

	// Free the camera
	free(p_camera);
	
	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_camera: 
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"pp_camera\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}