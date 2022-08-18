#include <G10/GXRigidbody.h>

int create_rigidbody(GXRigidbody_t** rigidbody)
{
	GXRigidbody_t* i_rigidbody = calloc(1, sizeof(GXRigidbody_t));

	*rigidbody = i_rigidbody;

	return 1;
}

int update_force(GXRigidbody_t* rigidbody)
{
	rigidbody->forces[0] = (vec3){ 0.f, 0.f, 0.f, 0.f };

	// Newtons first law; summate forces
	for (size_t i = 1; i < rigidbody->force_count; i++)
		add_vec3(&rigidbody->forces[0], rigidbody->forces[0], rigidbody->forces[i]);


}

int load_rigidbody(GXRigidbody_t** rigidbody, const char* path)
{
	size_t  token_text_len = g_load_file(path, 0, false);
	char   *token_text     = calloc(token_text_len+1, sizeof(char));

	g_load_file(path, token_text, false);

	load_rigidbody_as_json(rigidbody, token_text, token_text_len);

	return 0;
}

int load_rigidbody_as_json(GXRigidbody_t** rigidbody, char* token_text, size_t token_text_len)
{
	dict* json_object = 0;
	GXRigidbody_t *i_rigidbody = 0;
	char          *mass   = 0,
		          *active = 0;

	{
		JSONToken_t *token = 0;
	
		parse_json(token_text, token_text_len, &json_object);

		token  = dict_get(json_object, "mass");
		mass   = JSON_VALUE(token, JSONprimative);

		token  = dict_get(json_object, "active");
		active = JSON_VALUE(token, JSONprimative);
	}

	create_rigidbody(rigidbody);
	i_rigidbody = *rigidbody;

	if(mass)
		i_rigidbody->mass = atoi(mass);

	i_rigidbody->active = active;


	i_rigidbody->forces = calloc(16, sizeof(vec3));

	i_rigidbody->torques = calloc(16, sizeof(quaternion));

	return 0;
}

int destroy_rigidbody(GXRigidbody_t* rigidbody)
{
	free(rigidbody->forces);
	free(rigidbody->torques);
	free(rigidbody);

	return 0;
}
