#include <G10/GXEntity.h>

int create_entity(GXEntity_t** entity)
{
	// TODO: Argument check
	GXEntity_t *ret = calloc(1,sizeof(GXEntity_t));
	
	// TODO: Memory check
	*entity = ret;

	return 1;
	
	// TODO: Error handling
	//			- Argument errors
	//          - Standard library errors
}

int load_entity(GXEntity_t** entity, char* path)
{
	// TODO: Argument check
	// TODO: Memory check
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len+1, sizeof(char));
	g_load_file(path, token_text, false);

	load_entity_as_json(entity, token_text, len);

	return 0;
	
	// TODO: Error handling
	//			- Argument errors
	//          - Standard library errors
}

/* !
 *  Load an entity as JSON object text
 *
 * @param entity     : Double pointer to entity
 * @param token_text : The entity JSON object text
 * @param len        : The length of the entity JSON object text
 *
 * @sa destroy_entity
 *
 * @return 1 on success, 0 on error
 */
int load_entity_as_json(GXEntity_t** entity, char* token_text, size_t len)
{
	// Argument check
	{
		#ifndef NDEBUG
			if(entity == (void *)0)
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
		       *rigid_body  = 0;


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
		token      = dict_get(entity_json, "rigid body");
		rigid_body = JSON_VALUE(token, JSONobject);

	}

	// Is there enough information to construct an entity?
	{

		// Initialized data
		bool suitable = true;

		// If the device is unsuitable, abort
		if (suitable == false)
			goto not_enough_info;
	}

	// Construct the entity
	{

		// Allocate the entity
		create_entity(entity);

		// Get a reference to the entity
		i_entity = *entity;

		// Name
		if (name)
		{

			// Initialized data
			size_t name_len = strlen(name);

			// Allocate memory for the name
			i_entity->name = calloc(name_len + 1, sizeof(char));

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
				load_transform_as_json(&i_entity->transform, transform, strlen(transform));

			// Path branch
			else
				load_transform(&i_entity->transform, transform);
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

	}

	return 0;

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
	}
}

int load_entity_from_queue(GXScene_t *scene)
{

	// Initialized data
	GXInstance_t *instance = g_get_active_instance();
	size_t        i        = 0;
	char         *text     = 0;
	GXEntity_t   *entity   = 0;


	// Lock the loading mutex while we find an entity to load
	SDL_LockMutex(instance->load_entity_mutex);
	
	if (dict_keys(instance->load_entity_queue, 0) == 0)
	{
		SDL_UnlockMutex(instance->load_entity_mutex);
		return 0;
	}


	// Atomic
	{

		// Find an entity
		while (instance->load_entity_queue->entries[i] == 0)
		{
			if (i >= instance->load_entity_queue->entry_count - 1)
			{
				SDL_UnlockMutex(instance->load_entity_mutex);
				return 0;
			}
			else i++;
		}
		text = instance->load_entity_queue->entries[i]->key;

		// Remove the entity from the list
		
		dict_pop(instance->load_entity_queue, text, 0);
	}

	// Unlock the mutex
	SDL_UnlockMutex(instance->load_entity_mutex);

	// Load the entity
	if (*text == '{')
		load_entity_as_json(&entity, text, strlen(text));
	else
		load_entity(&entity, text);

	// Add the entity to the active scene
	append_entity(scene, entity);

	return 0;
}

int draw_entity(GXEntity_t* entity)
{
	GXInstance_t* instance = g_get_active_instance();
	
	// Draw the thing
	size_t part_count = dict_values(entity->parts, 0);
	GXPart_t **parts  = calloc(part_count, sizeof(void *));
	
	dict_values(entity->parts, parts);

	use_shader(entity->shader);

	for (size_t i = 0; i < part_count; i++)
		draw_part(parts[i]);

	free(parts);


	return 1;
}

int move_entity(GXEntity_t* entity, float delta_time)
{
	GXRigidbody_t* rigidbody = entity->rigidbody;
	GXTransform_t* transform = entity->transform;

	// force = ( kg * m / s^2 )
	vec3 force = rigidbody->forces[0]; 

	// Calculate acceleration
	// acceleration = ( m / s^2 )
	div_vec3_f(&rigidbody->acceleration, force, rigidbody->mass);
	
	// Calculate velocity
	// velocity = ( m / s )
	rigidbody->velocity = mul_vec3_f(rigidbody->acceleration, delta_time);

	// Calculate location
	// location = ( m )
	transform->location = mul_vec3_f(rigidbody->velocity, rigidbody->mass);

	return 1;
}

// TODO: Argument check
// TODO: Memory check
// TODO: Error handling
//			- Argument errors
//          - Standard library errors