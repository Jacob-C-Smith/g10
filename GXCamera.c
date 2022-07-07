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

int  create_camera    ( GXCamera_t **camera)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( camera == (void *) 0 )
				goto no_camera;
		#endif
	}

	// Initialized data
	GXCamera_t *ret = calloc(1, sizeof(GXCamera_t));

	// Error checking
	{
		#ifndef NDEBUG
			if(ret == (void*)0)
				goto no_mem;
		#endif
	}

	*camera = ret;

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_camera:
				#ifndef NDEBUG
					g_print_error("[G10] [Camera] Null pointer provided for \"camera\" in call to function \"%s\"", __FUNCSIG__);
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

	// Allocate the camera
	{
		create_camera(camera);

		c=*camera;
	}

	// Construct the camera
	{

		c->location = location;
		c->target   = front;
		c->up       = up;
		
		c->near_clip    = near_clip;
		c->far_clip     = far_clip;
		c->aspect_ratio = aspect_ratio;
		c->fov          = fov;
		
		// Set the view matrix
		


	}


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

int load_camera(GXCamera_t** camera, const char* path)
{
	return 0;
}
