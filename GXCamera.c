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
	GXInstance_t *instance        = g_get_active_instance();
	vec3          camera_position = instance->context.scene->active_camera->location;

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
				return 0;
		}
	}
}

mat4 look_at          ( vec3        eye,    vec3        target       , vec3  up ) 
{ 
    // Compute forward direction
    vec3 f = normalize((vec3) {
        eye.x - target.x,
        eye.y - target.y,
        eye.z - target.z
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
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif
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

int  load_camera_as_json ( GXCamera_t **pp_camera, char *token_text, size_t len )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_camera == (void *) 0 )
				goto no_camera;
			if (token_text == (void *) 0 )
				goto no_object_text;
			if (len == 0)
				goto no_len;
		#endif
	}

	// Initialized data
	GXCamera_t  *p_camera     = 0;
	char        *name         = 0,
		       **location     = 0,
		       **front        = 0,
		       **up           = 0,
		        *near_clip    = 0,
		        *far_clip     = 0,
		        *aspect_ratio = 0,
		        *fov          = 0;
	dict        *camera_json  = 0;

	// Parse the JSON into a dictionary
	parse_json(token_text, len, &camera_json);

	// Parse the dictionary into constructor parameters
	{

		// Initialized dat
		JSONToken_t *token = 0;

		token     = (JSONToken_t *) dict_get(camera_json, "name");
		name      = JSON_VALUE(token, JSONstring);

		token     = (JSONToken_t *) dict_get(camera_json, "location");
		location  = JSON_VALUE(token, JSONarray);

		token     = (JSONToken_t *) dict_get(camera_json, "front");
		front     = JSON_VALUE(token, JSONarray);

		token     = (JSONToken_t *) dict_get(camera_json, "up");
		up        = JSON_VALUE(token, JSONarray);

		token     = (JSONToken_t *) dict_get(camera_json, "near");
		near_clip = JSON_VALUE(token, JSONprimative);

		token     = (JSONToken_t *) dict_get(camera_json, "far");
		far_clip  = JSON_VALUE(token, JSONprimative);

		token     = (JSONToken_t *) dict_get(camera_json, "fov");
		fov       = JSON_VALUE(token, JSONprimative);

	}

	// Allocate memory for the camera
	if ( create_camera(pp_camera) == 0 )
		goto failed_to_allocate_camera;

	// Get a pointer to the camera
	p_camera = *pp_camera;

	// Construct the camera
	{

		// Initialized data		
		vec3 a_location = { (float)atof(location[0]), (float)atof(location[1]), (float)atof(location[2]) };
		vec3 a_front    = { (float)atof(front[0])   , (float)atof(front[1])   , (float)atof(front[2]) };
		vec3 a_up       = { -(float)atof(up[0])      , -(float)atof(up[1])      , -(float)atof(up[2]) };

		// Copy the name
		{

			// Initialized data
			size_t name_len = strlen(name);
			
			// Allocate memory for the name
			p_camera->name = calloc(name_len + 1, sizeof(char));

			// Copy the name
			strncpy(p_camera->name, name, name_len);
		}

		// Set the view matrix parameters
		p_camera->location = a_location;
		p_camera->target   = a_front;
		p_camera->up       = a_up;
		
		// Set the model matrix parameters
		p_camera->near_clip    = (float)atof(near_clip);
		p_camera->far_clip     = (float)atof(far_clip);
		p_camera->aspect_ratio = 16.f / 9.f;
		p_camera->fov          = (float)atof(fov);
		
		// Set the view and projection matricies
		p_camera->view       = look_at(p_camera->location, p_camera->target, p_camera->up);
		p_camera->projection = perspective_matrix(p_camera->fov, p_camera->aspect_ratio, p_camera->near_clip, p_camera->far_clip);

	}

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
					g_print_error("[G10] [Camera] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCTION__);
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

		// G10 Errors
		{
			failed_to_allocate_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Failed to allocate memory for camera in call to function \"%s\"\n",  __FUNCTION__);
				#endif

				// Error
				return 0;

		}
	}
}

int destroy_camera ( GXCamera_t *p_camera )
{
	// TODO: 
	return 1;
}