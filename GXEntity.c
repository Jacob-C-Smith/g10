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
	GXEntity_t *p_entity = calloc(1,sizeof(GXEntity_t));
	
	// Error checking
	if ( p_entity == (void *) 0 )
		goto no_mem;

	// Return a pointer to the caller
	*pp_entity = p_entity;

	// Success
	return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_entity ( GXEntity_t** pp_entity, char* path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( pp_entity == (void *) 0 )
				goto no_entity;
			if ( path == (void *) 0 )
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len  = g_load_file(path, 0, false);
	char   *text = calloc(len+1, sizeof(char));
	
	// Error checking
	if ( text == (void *) 0 )
		goto no_mem;

	// Load the entity file
	if ( g_load_file(path, text, false) == 0 )
		goto failed_to_load_entity;

	// Load the entity as JSON text
	if ( load_entity_as_json(pp_entity, text) == 0 )
		goto failed_to_load_entity_as_json;

	// Clean up the scope
	free(text);

	// Success
	return 1;
	
	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					g_print_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 errors
		{
			failed_to_load_entity_as_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to open file \"%s\" text in call to function \"%s\"\n", path, __FUNCTION__);
				#endif

				// Error
				return 0;
				
			failed_to_load_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to load entity from \"%s\" in call to function \"%s\"\n", path, __FUNCTION__);
				#endif
				
				// Error
				return 0;
		}
	}
}

int load_entity_as_json ( GXEntity_t** pp_entity, char* text )
{

	// Argument check
	{
		#ifndef NDEBUG
			if(pp_entity == (void *)0)
				goto no_entity;
			if (text == (void*)0)
				goto no_text;
		#endif
	}

	// Initialized data
	JSONValue_t *p_value = 0;

	// Parse the JSON
	if ( parse_json_value(text, 0, &p_value) == (void *) 0 )
		goto failed_to_parse_json;

	// Load the entity as a JSON value
	if ( load_entity_as_json_value(pp_entity, p_value) == (void *) 0 )
		goto failed_to_load_entity_as_json_value;		

	// Clean up the scope
	free_json_value(p_value);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_text:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"text\" in call to function \"%s\"\n", __FUNCTION__);
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

		// User errors
		{
			not_enough_info:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Not enough information to construct entity in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

		}

		// G10 errors
		{

			failed_to_parse_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to parse JSON text in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_entity_as_json_value:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to construct entity from JSON text in call to function\"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int load_entity_as_json_value ( GXEntity_t **pp_entity, JSONValue_t *p_value )
{
	
	// Argument Check
	{
		#ifndef NDEBUG
			if ( pp_entity == (void *) 0 )
				goto no_entity;
			if ( p_value == (void *) 0 )
				goto no_value;
		#endif
	}

	// Initialized data
	GXEntity_t  *p_entity          = 0;
	JSONValue_t *p_name_value      = 0,
	            *p_parts_value     = 0,
	            *p_materials_value = 0,
				*p_shader_value    = 0,
				*p_transform_value = 0,
				*p_rigidbody_value = 0,
				*p_collider_value  = 0,
				*p_ai_value        = 0;
				 	
	// Parse the JSON value
	if (p_value->type == JSONobject)
	{

		// Initialized data
		dict *p_entity_value = p_value->object;

		// Get properties from JSON object
		p_name_value      = (JSONValue_t *) dict_get(p_entity_value, "name");
		p_parts_value     = (JSONValue_t *) dict_get(p_entity_value, "parts");
		p_materials_value = (JSONValue_t *) dict_get(p_entity_value, "materials");
		p_shader_value    = (JSONValue_t *) dict_get(p_entity_value, "shader");
		p_transform_value = (JSONValue_t *) dict_get(p_entity_value, "transform");
		p_rigidbody_value = (JSONValue_t *) dict_get(p_entity_value, "rigidbody");
		p_collider_value  = (JSONValue_t *) dict_get(p_entity_value, "collider");
		p_ai_value        = (JSONValue_t *) dict_get(p_entity_value, "ai");
		
		// Check for required data
		if ( !p_name_value )
			goto not_enough_values;
	}

	// Construct the entity
	{

		// Allocate the entity
		if ( create_entity(&p_entity) == 0 )
			goto failed_to_allocate_entity;

		// Copy the name
		if (p_name_value->type == JSONstring)
		{

			// Initialized data
			size_t name_len = strlen(p_name_value->string);

			// Allocate memory for the name
			p_entity->name  = calloc(name_len + 1, sizeof(char));

			// Error checking
			if ( p_entity->name == (void *)0 )
				goto no_mem;

			// Copy the name 
			strncpy(p_entity->name, p_name_value->string, name_len);

		}
		else
			goto wrong_name_type;

		// Parts
		if ( p_parts_value->type == JSONarray )
		{

			// Initialized data
			size_t        part_count = 0;
			JSONValue_t **parts      = 0;

			// Construct a dictionary
			dict_construct(&p_entity->parts, 16);

			// Get the array
			{

				// Get the size of the array
				array_get(p_parts_value->list, 0, &part_count);

				// Allocate memory for a list of parts
				parts = calloc(part_count+1, sizeof(JSONValue_t *));

				// Error check
				if ( parts == (void *) 0 )
					goto no_mem;

				// Get each array element
				array_get(p_parts_value->list, parts, &part_count);
			}

			// Iterate over each array element
			for (size_t i = 0; i < part_count; i++)
			{

				// Initialized data
				GXPart_t *p_part = 0;

				// Load the part
				if ( load_part_as_json_value(&p_part, parts[i]) == 0 )
					goto failed_to_load_part;

				// Add the part to the dictionary
				dict_add(p_entity->parts, p_part->name, p_part);
			}

			// Clean the scope
			free(parts);
		}
		else
			goto wrong_parts_type;

		// Materials

		// Shader
		if ( p_shader_value )
			;//if ( load_shader_as_json_value(&p_entity->shader, p_shader_value) == 0 )
				//goto failed_to_load_transform_as_json_value;

		// Transform
		if ( p_transform_value )
			if ( load_transform_as_json_value(&p_entity->transform, p_transform_value) == 0 )
				goto failed_to_load_transform_as_json_value;

		// Rigidbody

		// Collider

		// AI
		if ( p_ai_value )
			if ( load_ai_as_json_value(&p_entity->ai, p_ai_value) == 0 )
				goto failed_to_load_ai_as_json_value;

		/*
		// Part
		if (p_parts) {

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
		if (p_materials)
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

		*/

		// Return a pointer to the caller
		*pp_entity = p_entity;
	}

	// Success
	return 1;

	// Error handling
	{

		// TODO: 
		wrong_name_type:
		failed_to_load_part:
		wrong_parts_type:
		return 0;

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for parameter \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			no_value:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// G10 errors
		{
			not_enough_values:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Missing JSON properties to parse entity in call to function \"%s\". Consult gschema\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_allocate_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to allocate entity in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// JSON errors
		{

			failed_to_load_transform_as_json_value:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to load transform in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_ai_as_json_value:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Failed to load AI in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}

		// Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    g_print_error("[G10] [Entity] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

				// Error
                return 0;
        }
	}
}

int calculate_entity_force ( GXEntity_t *p_entity )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_entity == (void *) 0 )
				goto no_entity;
		#endif	
	}

	// Initialized data
	vec3* forces = p_entity->rigidbody->forces;

	forces[0] = (vec3){0.f, 0.f, 0.f, 0.f};

	// Calculate each forces effect on the entity
	forces[1] = calculate_force_gravitational ( p_entity );
	forces[2] = calculate_force_applied ( p_entity );
	forces[3] = calculate_force_normal ( p_entity );
	forces[4] = calculate_force_friction ( p_entity );
	forces[5] = calculate_force_tension ( p_entity );
	forces[6] = calculate_force_spring ( p_entity );

	// Summate each force
	for (size_t i = 0; i < 6; i++)
		add_vec3(&forces[0], forces[0], forces[i]);

	// forces[0] = net force

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n");
				#endif

				// Error
				return 0;
		}
	}
}

int preupdate_entity_ai ( GXEntity_t *p_entity )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_entity == (void *) 0 )
				goto no_entity;
		#endif	
	}

	// Initialized data
	GXAI_t* p_ai = p_entity->ai;

	// Get the callback function associated with the current state 
	void (*preupdate_ai)(GXEntity_t * p_entity) = p_ai->pre_ai;

	// Pre update
	if (preupdate_ai)
		preupdate_ai(p_entity);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n");
				#endif

				// Error
				return 0;
		}
	}
}

int update_entity_ai ( GXEntity_t* p_entity )
{

	// Argument check
	{
		#ifndef NDEBUG
			if ( p_entity == (void *) 0 )
				goto no_entity;
		#endif	
	}

	// Initialized data
	GXAI_t *p_ai = p_entity->ai;

	if (dict_get(p_entity->ai->states, p_entity->ai->current_state))
	{

		// Get the callback function associated with the current state 
		void (*update_ai_function)(GXEntity_t *) = ( void (*)(GXEntity_t*) ) dict_get(p_ai->states, p_ai->current_state);
	
		// Update
		update_ai_function(p_entity);
	}

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for parameter \"p_entity\" in call to function \"%s\"\n");
				#endif

				// Error
				return 0;
		}
	}
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
	GXInstance_t *p_instance        = g_get_active_instance();
	mat4          model_matrix    = { 0 };
	
	transform_model_matrix(p_instance->context.scene->active_entity->transform, &model_matrix);

	// Write the camera position to the return
	*(mat4 *)ret = model_matrix;

	return sizeof(mat4);

	// Error handling
	{

		// Argument errors
		{
			no_return:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"ret\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

vec3 calculate_force_gravitational ( GXEntity_t *p_entity )
{


	vec3 ret = { 0.f, 0.f, -9.8f, 0.f };

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
	
	// Success
	return ret;
}

vec3 calculate_force_normal ( GXEntity_t * p_entity )
{
	GXInstance_t  *p_instance = g_get_active_instance();
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
              mB = b_min.z + ( ( b_max.z - b_min.z ) / 2.f ),
              MB = b_max.z + entity->rigidbody->velocity.z - 0.01f;

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

int load_entity_from_queue(GXInstance_t *p_instance)
{

	// Initialized data
	size_t        i        = 0;
	JSONValue_t  *p_value  = 0;
	GXEntity_t   *p_entity = 0;
	GXScene_t    *p_scene  = p_instance->context.loading_scene;
	
	// TODO: Fix 
	while ( queue_empty(p_instance->queues.load_entity) == false )
	{

		// Lock the loading mutex while we find an entity to load
		SDL_LockMutex(p_instance->mutexes.load_entity);
		
		// If the queue is empty, unlock the mutex and exit
		if (queue_empty(p_instance->queues.load_entity))
		{
		
			// Unlock the mutex
			SDL_UnlockMutex(p_instance->mutexes.load_entity);
		
			// Success
			return 0;
		}
		
		// TODO: Fix 
		// text is either a path -OR- a JSON object
		queue_dequeue(p_instance->queues.load_entity,&p_value);

		// Unlock the mutex
		SDL_UnlockMutex(p_instance->mutexes.load_entity);

		if ( load_entity_as_json_value(&p_entity, p_value) == 0 )
			goto failed_to_load_entity_as_json;

		// Add the entity to the active scene
		append_entity(p_scene, p_entity);
	}
	
	// Success
	return 1;

	// Error handling
	{

		// G10 errors
		{
			failed_to_load_entity_as_json:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Failed to load entity in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;

			failed_to_load_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Scene] Failed to load entity in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}

}

int load_light_probe_from_queue(GXInstance_t* p_instance)
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

	GXInstance_t* p_instance = g_get_active_instance();
	p_instance->context.scene->active_entity = p_entity;

	// Draw the thing
	size_t part_count = dict_values(p_entity->parts, 0);
	GXPart_t **parts  = calloc(part_count, sizeof(void *));
	
	dict_values(p_entity->parts, parts);

	// TODO: Uncomment
	//use_shader(p_entity->shader);

	for (size_t i = 0; i < part_count; i++)
	{

		// Update descriptor sets
		{
			// TODO: Uncomment when shader sets are done
			;//set_shader_camera(p_entity);
		}

		if (p_entity->shader->graphics.push_constant_data)
		{
			// TODO: Uncomment when shader sets are done
			//update_shader_push_constant(p_entity->shader);
			vkCmdPushConstants(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], p_entity->shader->graphics.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, 0, (u32) p_entity->shader->graphics.push_constant_size, p_entity->shader->graphics.push_constant_data);
		}

		for (size_t i = 0; i < p_entity->shader->graphics.set_count; i++)
		{
			vkCmdBindDescriptorSets(p_instance->vulkan.command_buffers[p_instance->vulkan.current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, p_entity->shader->graphics.pipeline_layout, 0, 1, &p_entity->shader->graphics.sets_data[i].descriptor_sets[p_instance->vulkan.current_frame], 0, 0);
		}
		
		// TODO: Uncomment when shader sets are done	
		//draw_part(parts[i]);
	}

	free(parts);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int destroy_entity(GXEntity_t **pp_entity)
{

	// Argument error
	{
		#ifndef NDEBUG
			if ( pp_entity == (void *) 0 )
				goto no_entity;
		#endif
	}

	// Initialized data
	GXEntity_t *p_entity = *pp_entity;

	// No more pointer for caller
	*pp_entity = (void *) 0;

	// Free the name
	free(p_entity->name);

	if (p_entity->shader)
		p_entity->shader = (void *)0;

	if (p_entity->transform)
		destroy_transform(&p_entity->transform);
	
	if(p_entity->rigidbody)
		;//destroy_rigidbody(p_entity->rigidbody);

	if(p_entity->ai)
		destroy_ai(&p_entity->ai);

	// Free the entity
	free(p_entity);

	// Success
	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"pp_entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
	}
}

int move_entity ( GXEntity_t* p_entity )
{
	if (p_entity->rigidbody->mass == 0.f)
		return 0;
	GXInstance_t* p_instance = g_get_active_instance();

	float delta_time = p_instance->time.delta_time;

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
	//resize_bv(p_entity->collider->bv);

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
    g_print_log("name: \"%s\"\n", p_entity->name);

	if ( p_entity->parts )
		dict_foreach(p_entity->parts, &part_info);

	if ( p_entity->ai )
		ai_info(p_entity->ai);

    putchar('\n');

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_entity:
				#ifndef NDEBUG
					g_print_error("[G10] [Entity] Null pointer provided for \"p_entity\" in call to function \"%s\"\n", __FUNCTION__);
				#endif

				// Error
				return 0;
		}
		
	}
}

// TODO: Argument check
// TODO: Memory check
// TODO: Error handling
//			- Argument errors
//          - Standard library errors