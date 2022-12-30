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

int get_camera_position(void* ret)
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
					g_print_error("[G10] [Camera] Null pointer provided for \"ret\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

mat4 look_at          ( vec3        eye,    vec3        target       , vec3  up )                                        // ✅ Computes a view matrix from eye, target, and up vectors
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
			if(p_camera == (void*)0)
				goto no_mem;
		#endif
	}

	*pp_camera = p_camera;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"pp_camera\" in call to function \"%s\"", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int  construct_camera ( GXCamera_t **camera, vec3 location, vec3 front, vec3 up, float near_clip, float far_clip, float aspect_ratio, float fov)
{

	// Argument check
	{
		#ifndef NDEBUG
			if(camera == (void *)0)
				goto no_camera;
		#endif
	}

	// Initialized data
	GXCamera_t *c = 0;




	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"camera\" in call to function \"%s\"\n",__FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int  load_camera(GXCamera_t** camera, const char* path)
{
	size_t  file_len  = g_load_file(path, 0, false);
	char* file_text = calloc(file_len + 1, sizeof(char));
	g_load_file(path, file_text, false);

	load_camera_as_json(camera, file_text, file_len);

	return 0;
}

int  load_camera_as_json(GXCamera_t** camera, char* object_text, size_t len)
{

	GXCamera_t  *i_camera     = 0;
	char        *name         = 0,
		       **location     = 0,
		       **front        = 0,
		       **up           = 0,
		        *near_clip    = 0,
		        *far_clip     = 0,
		        *aspect_ratio = 0,
		        *fov          = 0;

	dict *camera_json = 0;

	{
		JSONToken_t *token = 0;

		parse_json(object_text, len, &camera_json);

		token = dict_get(camera_json, "name");
		name  = JSON_VALUE(token, JSONstring);

		token    = dict_get(camera_json, "location");
		location = JSON_VALUE(token, JSONarray);

		token = dict_get(camera_json, "front");
		front = JSON_VALUE(token, JSONarray);

		token = dict_get(camera_json, "up");
		up    = JSON_VALUE(token, JSONarray);

		token = dict_get(camera_json, "near");
		near_clip = JSON_VALUE(token, JSONprimative);

		token = dict_get(camera_json, "far");
		far_clip = JSON_VALUE(token, JSONprimative);

		token = dict_get(camera_json, "fov");
		fov = JSON_VALUE(token, JSONprimative);

	}

	// Allocate the camera
	{
		create_camera(camera);

		i_camera = *camera;
	}

	// Construct the camera
	{

		{
			size_t name_len = strlen(name);
			i_camera->name = calloc(name_len + 1, sizeof(char));
			strncpy(i_camera->name, name, name_len);
		}

		vec3 a_location = { (float)atof(location[0]), (float)atof(location[1]), (float)atof(location[2]) };
		vec3 a_front    = { (float)atof(front[0])   , (float)atof(front[1])   , (float)atof(front[2]) };
		vec3 a_up       = { -(float)atof(up[0])      , -(float)atof(up[1])      , -(float)atof(up[2]) };

		i_camera->location = a_location;
		i_camera->target   = a_front;
		i_camera->up       = a_up;
		
		i_camera->near_clip    = (float)atof(near_clip);
		i_camera->far_clip     = (float)atof(far_clip);
		i_camera->aspect_ratio = 16.f / 9.f;
		i_camera->fov          = (float)atof(fov);
		
		// Set the matricies
		i_camera->view       = look_at(i_camera->location, i_camera->target, i_camera->up);
		i_camera->projection = perspective_matrix(i_camera->fov, i_camera->aspect_ratio, i_camera->near_clip, i_camera->far_clip);

	}

	return 0;
}

int destroy_camera ( GXCamera_t *p_camera )
{
	// TODO: 
	return 1;
}