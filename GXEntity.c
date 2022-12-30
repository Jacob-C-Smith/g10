#include <G10/GXEntity.h>

vec3 calculate_force_gravitational(GXEntity_t* entity);
vec3 calculate_force_applied(GXEntity_t* entity);
vec3 calculate_force_normal(GXEntity_t* entity);
vec3 calculate_force_friction(GXEntity_t* entity);
vec3 calculate_force_tension(GXEntity_t* entity);
vec3 calculate_force_spring(GXEntity_t* entity);

int create_entity ( GXEntity_t **pp_entity )
{
	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_entity == (void *) 0 )
				goto no_entity;
		#endif
	}

	// Initialized data
	GXEntity_t *ret = calloc(1,sizeof(GXEntity_t));
	
	// Memory check
	{
		#ifndef NDEBUG
			if(ret == (void *)0)
				goto no_mem;
		#endif
	}

	// Return the allocated memory
	*pp_entity = ret;

	return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_entity ( GXEntity_t** pp_entity, char* path)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_entity == (void *) 0 )
				goto no_entity;
			if ( path      == (void *) 0 )
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len+1, sizeof(char));
	
	// Load the entity file
	if ( g_load_file(path, token_text, false) == 0 )
		goto failed_to_load_entity;

	// Load the entity as JSON text
	if ( load_entity_as_json(pp_entity, token_text, len) == 0 )
		goto failed_to_load_entity_as_json;

	// Free the object text
	free(token_text);

	return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"pp_entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// G10 errors
		{
			failed_to_load_entity_as_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to open file \"%s\" text in call to function \"%s\"\n", path, __FUNCSIG__);
				#endif
				return 0;
			failed_to_load_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to load entity from \"%s\" in call to function \"%s\"\n", path, __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int load_entity_as_json ( GXEntity_t** pp_entity, char* token_text, size_t len)
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_entity == (void *)0)
				goto no_entity;
			if (token_text == (void*)0)
				goto no_token;
		#endif
	}

	// Initialized data
	GXEntity_t *i_entity    = 0;
	dict       *entity_json = 0;

	char       *name        = 0,
		      **parts       = 0,
		      **materials   = 0,
		       *shader      = 0,
		       *transform   = 0,
		       *rigid_body  = 0,
		       *collider    = 0,
		       *ai          = 0;


	// Parse the JSON
	{
		
		// Initialized data
		JSONToken_t *token = 0;

		// Parse the JSON text into a dictionary
		parse_json(token_text, len, &entity_json);

		// Name
		token      = dict_get(entity_json, "name");
		name       = JSON_VALUE(token, JSONstring);

		// Part 
		token      = dict_get(entity_json, "parts");
		parts      = JSON_VALUE(token, JSONarray);

		// Shader path
		token      = dict_get(entity_json, "shader");
		shader     = JSON_VALUE(token, JSONstring);

		// Transform
		token      = dict_get(entity_json, "transform");
		transform  = JSON_VALUE(token, JSONobject);

		// Rigidbody
		token      = dict_get(entity_json, "rigidbody");
		rigid_body = JSON_VALUE(token, JSONobject);

		// Collider
		token      = dict_get(entity_json, "collider");
		collider   = JSON_VALUE(token, JSONobject);

		// AI
		token      = dict_get(entity_json, "ai");
		ai         = JSON_VALUE(token, JSONobject);

	}

	// Is there enough information to construct an entity?
	{
		// TODO: 
	}

	// Construct the entity
	{

		// Allocate the entity
		create_entity(pp_entity);

		// Get a reference to the entity
		i_entity = *pp_entity;

		// Name
		if (name)
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate memory for the name
			i_entity->name  = calloc(name_len + 1, sizeof(char));

			// Error checking
			{
				#ifndef NDEBUG
					if ( i_entity->name == (void *)0 )
						goto no_mem;
				#endif
			}

			// Copy the name 
			strncpy(i_entity->name, name, name_len);

		}

		// Part
		if (parts) {

			// Initialized data
			size_t part_count = 0;

			// Count up parts
			while (parts[++part_count]);

			// Construct a list of parts
			dict_construct(&i_entity->parts, part_count);

			// Iterate over parts
			for (size_t i = 0; i < part_count; i++)
			{

				// Initialized data
				GXPart_t *p = 0;

				// Differentiate objects from paths

				// Object branch
				if (*parts[i] == '{')
					load_part_as_json(&p, parts[i], strlen(parts[i]));

				// Path branch
				else
					load_part(&p, parts[i]);

				// Add the part to the list
				dict_add(i_entity->parts, p->name, p);
			}

		}

		// Material
		if (materials)
		{

			// Initialized data
			size_t materail_count = 0;

			// Count up materials
			while (materials[++materail_count]);

			// Construct a list of materials
			dict_construct(&i_entity->materials, materail_count);

			// Iterate over materials
			for (size_t i = 0; i < materail_count; i++)
			{

				// Initialized data
				GXMaterial_t *m = 0;

				// Differentiate objects from paths

				// Object branch
				if (*materials[i] == '{')
					load_material_as_json(&m, materials[i], strlen(materials[i]));

				// Path branch
				else
					load_material(&m, materials[i]);

				// Add the material to the list
				dict_add(i_entity->materials, m->name, m);
			}
		}

		// Shader
		if (shader) {

			// Differentiate objects from paths

			// Object branch
			if (*shader == '{')
				load_shader_as_json(&i_entity->shader, shader, strlen(shader));

			// Path branch
			else
				load_shader(&i_entity->shader, shader);

		}

		// Transform
		if(transform) {

			// Differentiate objects from paths

			// Object branch
			if (*transform == '{')
			{
				if ( load_transform_as_json(&i_entity->transform, transform, strlen(transform)) == 0 )
					goto failed_to_load_transform_as_json;
			}

			// Path branch
			else
			{
				if ( load_transform(&i_entity->transform, transform) == 0)
					goto failed_to_load_transform;
			}

		}

		// Rigidbody
		if (rigid_body) {

			// Differentiate objects from paths

			// Object branch
			if (*rigid_body == '{')
				load_rigidbody_as_json(&i_entity->rigidbody, rigid_body, strlen(rigid_body));

			// Path branch
			else
				load_rigidbody(&i_entity->rigidbody, rigid_body);
		}

		// Collider
		if (collider) {

			// Differentiate objects from paths

			// Object branch
			if (*collider == '{')
				load_collider_as_json(&i_entity->collider, collider, strlen(collider));

			// Path branch
			else
				load_collider(&i_entity->collider, collider);
			
			i_entity->collider->bv->entity = i_entity;

			resize_bv(i_entity->collider->bv);

		}

		// AI
		if (ai)
		{
			// Differentiate objects from paths

			// Object branch
			if (*ai == '{')
				load_ai_as_json(&i_entity->ai, ai, strlen(ai));

			// Path branch
			else
				load_ai(&i_entity->ai, ai);
		}
	}

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_token:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"token_text\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}

		// User errors
		{
			not_enough_info:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Not enough information to construct entity in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

		}

		// G10 errors
		{

			// Transform errors
			{
				failed_to_load_transform_as_json:
					#ifndef NDEBUG
						g_print_error("[G10] [Entity] Failed to load transform as JSON text in call to function \"%s\"\n", __FUNCSIG__);
					#endif
					return 0;

				failed_to_load_transform:
					#ifndef NDEBUG
						g_print_error("[G10] [Entity] Failed to load transform in call to function \"%s\"\n", __FUNCSIG__);
					#endif
					return 0;
			}
		}
	}
}

int calculate_entity_force ( GXEntity_t *p_entity )
{

	// Initialized data
	vec3* forces = p_entity->rigidbody->forces;

	forces[0] = (vec3){0.f, 0.f, 0.f, 0.f};

	// Calculate each forces effect on the entity
	forces[1] = calculate_force_gravitational ( p_entity );
	forces[2] = calculate_force_applied       ( p_entity );
	forces[3] = calculate_force_normal        ( p_entity );
	forces[4] = calculate_force_friction      ( p_entity );
	forces[5] = calculate_force_tension       ( p_entity );
	forces[6] = calculate_force_spring        ( p_entity );

	// Summate each force
	for (size_t i = 0; i < 6; i++)
		add_vec3(&forces[0], forces[0], forces[i]);

	// forces[0] = net force

	return 1;
}

int preupdate_entity_ai ( GXEntity_t* p_entity)
{
	// Initialized data
	GXAI_t* p_ai = p_entity->ai;

	// Get the callback function associated with the current state 
	void (*preupdate_ai)(GXEntity_t * p_entity) = p_ai->pre_ai;

	// Pre update
	if(preupdate_ai)
		preupdate_ai(p_entity);
}

int update_entity_ai ( GXEntity_t* p_entity)
{
	// Initialized data
	GXAI_t *p_ai = p_entity->ai;

	if (dict_get(p_entity->ai->states, p_entity->ai->current_state))
	{

		// Get the callback function associated with the current state 
		void (*update_ai_function)(GXEntity_t * p_entity) = dict_get(p_ai->states, p_ai->current_state);
	
		// Update
		update_ai_function(p_entity);
	}

	return 1;
}

int get_model_matrix(void* ret)
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
	mat4          model_matrix    = instance->context.scene->active_entity->transform->model_matrix;

	// Write the camera position to the return
	*(mat4 *)ret = model_matrix;

	return sizeof(mat4);

	// Error handling
	{

		// Argument errors
		{
			no_return:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"ret\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

vec3 calculate_force_gravitational ( GXEntity_t * p_entity )
{
	vec3 ret = { 0.f, 0.f, -9.8, 0 };

	// -50 m / s terminal velocity
	if (p_entity->rigidbody->velocity.z < -0.55f)
		ret.z = 0.f;

	if (p_entity->rigidbody->active == false)
		ret.z = 0.f;

	return ret;
}

vec3 calculate_force_applied ( GXEntity_t * p_entity )
{
	vec3 ret = { 0, 0, 0, 0 };

	// TODO: Get collisions
	dict* collisions = p_entity->collider->collisions;

	for (size_t i = 0; i < collisions->entry_count; i++)
	{

		// Initialized data
		dict_item *di = collisions->entries[i];

		if ( di )
		{
			// Initialized data
			GXCollision_t *collision    = di->value;
			GXEntity_t    *other_entity = (p_entity == collision->a) ? (collision->b) : (collision->a);
			

		}
		
	}
	
	return ret;
}

vec3 calculate_force_normal ( GXEntity_t * p_entity )
{
	GXInstance_t  *instance = g_get_active_instance();
	vec3           ret      = { 0, 0, 0, 0 };

    GXEntity_t    *entity   = p_entity;
    GXCollider_t  *a        = entity->collider,
                  *b        = 0;
    GXCollision_t *c        = 0;

    dict_values(a->collisions, &c);
    
    if ( c ) {

        b = c->b->collider;
        
        vec3  a_min = a->bv->minimum,
              a_max = a->bv->maximum,
              b_min = b->bv->minimum,
              b_max = b->bv->maximum;

        float mA = a_min.z,
              MA = a_max.z,
              mB = b_min.z + ( ( b_max.z - b_min.z ) / 2 ),
              MB = b_max.z + entity->rigidbody->velocity.z - 0.01;

        if ( mA > MB  )
        {
			entity->rigidbody->forces[1].z = 0.f;
			entity->rigidbody->velocity.z = 0.f;
        }
    }

	return ret;
}

vec3 calculate_force_friction ( GXEntity_t* p_entity)
{
	vec3 ret = { 0, 0, 0, 0 };

	return ret;
}

vec3 calculate_force_tension ( GXEntity_t* p_entity)
{
	vec3 ret = { 0, 0, 0, 0 };

	return ret;
}

vec3 calculate_force_spring ( GXEntity_t* p_entity)
{
	vec3 ret = { 0, 0, 0, 0 };

	return ret;
}

int load_entity_from_queue(GXInstance_t *instance)
{

	// Initialized data
	size_t        i        = 0;
	char         *text     = 0;
	GXEntity_t   *entity   = 0;
	GXScene_t    *scene    = instance->context.loading_scene;

	while ( queue_empty(instance->queues.load_entity) == false )
	{

		// Lock the loading mutex while we find an entity to load
		SDL_LockMutex(instance->mutexes.load_entity);

		// If the queue is empty, unlock the mutex and exit
		if (queue_empty(instance->queues.load_entity))
		{

			// Unlock the mutex
			SDL_UnlockMutex(instance->mutexes.load_entity);

			// Success
			return 0;
		}

		// text is either a path -OR- a JSON object
		text = queue_dequeue(instance->queues.load_entity);

		// Unlock the mutex
		SDL_UnlockMutex(instance->mutexes.load_entity);

		// Load the entity as JSON object text
		if ( *text == '{' )
		{
			if ( load_entity_as_json(&entity, text, strlen(text)) == 0 )
				goto failed_to_load_entity_as_json;
		}

		// Load the entity as a path
		else
		{
			if ( load_entity(&entity, text) == 0 )
				goto failed_to_load_entity;
		}

		// Add the entity to the active scene
		append_entity(scene, entity);
	}
	
	return 0;

	// Error handling
	{

		// G10 errors
		{
			failed_to_load_entity_as_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Failed to load entity in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
			failed_to_load_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Failed to load entity in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}

}

int load_light_probe_from_queue(GXInstance_t* instance)
{

	// TODO:

	return 0;
}

int draw_entity(GXEntity_t* p_entity)
{

	// Argument check
	{
		#ifndef NDEBUG
			if (p_entity == (void *)0)
				goto no_entity;
		#endif
	}

	if (p_entity->parts == 0)
		return 0;

	GXInstance_t* instance = g_get_active_instance();
	instance->context.scene->active_entity = p_entity;

	// Draw the thing
	size_t part_count = dict_values(p_entity->parts, 0);
	GXPart_t **parts  = calloc(part_count, sizeof(void *));
	
	dict_values(p_entity->parts, parts);

	use_shader(p_entity->shader);

	for (size_t i = 0; i < part_count; i++)
	{

		// Update descriptor sets
		{
			// TODO: Uncomment when shader sets are done
			set_shader_camera(p_entity, instance->context.scene->active_camera);
		}

		if (p_entity->shader->graphics.push_constant_data)
		{
			update_shader_push_constant(p_entity->shader);
			vkCmdPushConstants(instance->vulkan.command_buffers[instance->vulkan.current_frame], p_entity->shader->graphics.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, p_entity->shader->graphics.push_constant_size, p_entity->shader->graphics.push_constant_data);
		}

		for (size_t i = 0; i < p_entity->shader->graphics.set_count; i++)
		{
			vkCmdBindDescriptorSets(instance->vulkan.command_buffers[instance->vulkan.current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, p_entity->shader->graphics.pipeline_layout, 0, 1, &p_entity->shader->graphics.sets_data[i].descriptor_sets[instance->vulkan.current_frame], 0, 0);
		}

		draw_part(parts[i]);
	}

	free(parts);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"p_entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int destroy_entity(GXEntity_t* p_entity)
{

	free(p_entity->name);

	if (p_entity->parts)
	{
		size_t     part_count = dict_keys(p_entity->parts, 0);
		GXPart_t **parts      = calloc(part_count, sizeof(void*));

		dict_values(p_entity->parts, parts);

		for (size_t i = 0; i < part_count; i++)
			destroy_part(parts[i]);

		free(parts);

		dict_destroy(p_entity->parts);

	}

	if (p_entity->materials)
	{
		size_t         material_count = dict_keys(p_entity->materials, 0);
		GXMaterial_t **materials      = calloc(material_count, sizeof(void*));

		dict_values(p_entity->materials, materials);

		// TODO:
		for (size_t i = 0; i < material_count; i++)
			;//destroy_material(materials[i]);

		free(materials);

		dict_destroy(p_entity->materials);
	}

	if (p_entity->shader)
		destroy_shader(p_entity->shader);

	if (p_entity->transform)
		destroy_transform(p_entity->transform);
	
	if(p_entity->rigidbody)
		destroy_rigidbody(p_entity->rigidbody);


	free(p_entity);

	return 0;
}

int move_entity ( GXEntity_t* p_entity )
{
	if (p_entity->rigidbody->mass == 0.f)
		return 0;
	GXInstance_t* instance = g_get_active_instance();

	float delta_time = instance->time.delta_time;

	GXRigidbody_t* rigidbody = p_entity->rigidbody;
	GXTransform_t* transform = p_entity->transform;

	// Calculate displacement and its derivatives from force
	{
		// force = ( kg * m / s^2 )

		// Calculate acceleration
		// acceleration = ( m / s^2 )
		div_vec3_f(&rigidbody->acceleration, rigidbody->forces[0], rigidbody->mass);

		// Calculate velocity
		// velocity = ( m / s )
		add_vec3(&rigidbody->velocity, mul_vec3_f(rigidbody->acceleration, delta_time), rigidbody->velocity);

		// Calculate linear momentum
		// momentum = kg * ( m / s ) 
		rigidbody->momentum = mul_vec3_f(rigidbody->velocity, rigidbody->mass);

		// Calculate location
		// location = ( m )
		add_vec3(&transform->location, mul_vec3_f(rigidbody->velocity, delta_time), transform->location);
	}

	// Calculate rotation and its derivatives from torque
	{

		// torque = ( kg * m^2 / s^2 )

		// Calculate angular acceleration
		// α = ( m^2 / s^2 )

	}

	// Update the model matrix
	transform_model_matrix(transform, &transform->model_matrix);
	resize_bv(p_entity->collider->bv);

	return 1;
}

int entity_info(GXEntity_t* p_entity)
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_entity == (void *)0 )
				goto no_entity;
		#endif
	}

	// Formatting 
    g_print_log(" - Entity info - \n");
    
    // Print the name
    g_print_log("name          : \"%s\"\n", p_entity->name);

	// TODO: Print each struct field.


    putchar('\n');

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"p_entity\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
		
	}
}

// TODO: Argument check
// TODO: Memory check
// TODO: Error handling
//			- Argument errors
//          - Standard library errors