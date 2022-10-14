#include <G10/GXServer.h>

int create_server(GXServer_t** pp_server)
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_server == (void *) 0)
				goto no_server;
		#endif
	}

	// Initialized data
	GXServer_t* i_server = calloc(1, sizeof(GXServer_t));

	// Error handling
	{
		#ifndef NDEBUG
			if(i_server == (void *)0)
				goto no_mem;
		#endif
	}

	// Return the memory pointer 
	*pp_server = i_server;

	return 0;

	// Error handling
	{

		// Argument errors
		{
			no_server:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for \"pp_server\" in call to function \"%s\"\n", __FUNCSIG__);
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

	}
}

int load_server ( GXServer_t **pp_server, char *path )
{

	// Argument check
	{
		#ifndef NDEBUG
			if (pp_server == (void *) 0)
				goto no_server;
			if (path == (void*)0)
				goto no_path;
		#endif
	}

	// Initialized data
	size_t  len        = g_load_file(path, 0, false);
	char   *token_text = calloc(len+1, sizeof( char *));

	// Error checking
	{
		#ifndef NDEBUG
			if (token_text == (void *)0)
				goto no_mem;
		#endif
	}

	// Load the file
	g_load_file(path, token_text, false);

	// Parse the JSON into a GXServer
	load_server_as_json(pp_server, token_text, len);

	// Free the file text
	free(token_text);

	return 0;

	// Error handling
	{

		// Argument errors
		{
			no_server:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for \"pp_server\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;

			no_path:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for \"path\" in call to function \"%s\"\n", __FUNCSIG__);
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
	}
}

/*
"server"                     : {
		"name"        : "localhost",
		"host"        : "127.0.0.1",
		"port"        : 8877,
		"max players" : 16,
		"password"    : "password",
		"bans"        : [
			{
				"host"       : "11.20.22.39",
				"expiration" : 1623996636,
				"note"       : ""
			},
			{
				"name"       : "Chris",
				"expiration" : 1624000000,
				"note"       : "Text chat spamming"
			},
			{
				"host"      : "168.183.212.227",
				"expiation" : 0,
				"note"      : "Permabanned for being a bot"
			}
		],
		"tick rate"   : 50
	}
*/
int load_server_as_json(GXServer_t** pp_server, char* token_text, size_t len)
{
	GXServer_t* server;
	char* name = 0,
		* host = 0,
		* port = 0,
		* max_players = 0,
		* password = 0,
		* tick_rate = 0;

	dict* json;
	parse_json(token_text, len, &json);
	JSONToken_t* token = 0;

	token = dict_get(json, "name");
	name = JSON_VALUE(token, JSONstring);
	
	token = dict_get(json, "host");
	host = JSON_VALUE(token, JSONstring);

	token = dict_get(json, "port");
	port = JSON_VALUE(token, JSONprimative);

	token = dict_get(json, "max players");
	max_players = JSON_VALUE(token, JSONprimative);

	token = dict_get(json, "password");
	password = JSON_VALUE(token, JSONstring);

	token = dict_get(json, "tick rate");
	tick_rate = JSON_VALUE(token, JSONprimative);

	//Create server

	create_server(pp_server);
	server = *pp_server;

	size_t name_len = strlen(name);
	server->name = calloc(name_len + 1, sizeof(char));
	strncpy(server->name, name, name_len);

	size_t retries = 0;
	const size_t MAX_RETRIES = 32;
	retryConnection:
	if (SDLNet_ResolveHost(&server->ip, NULL, atoi(port)))
	{
		if (++retries == MAX_RETRIES + 1)
			return 0;
		g_print_error("[G10] [Networking] Failed to resolve \"%s\":%hu. Attempting reconnect %d/%d\r", host, port, (int)retries, MAX_RETRIES);
		SDL_Delay(100);
		goto retryConnection;
	}

	return 0;
};

int start_server()
{

	// Initialized data
	GXInstance_t *instance = g_get_active_instance();
	GXServer_t   *p_server = instance->server;

	p_server->sock = SDLNet_TCP_Open(&p_server->ip);

	if(!p_server->sock)
		g_print_error("[SDLNet Error]: %s\n", SDLNet_GetError());

	return 0;
}

int process_command(GXClient_t *client, GXCommand_t* p_command)
{
	switch (p_command->type)
	{
	case chat:
		printf("%s says %s\n", client->name, p_command->chat.chat);
		break;
	}
}

int server_recv(GXClient_t *client)
{

	// Argument check
	{
		#ifndef NDEBUG
			if(client == (void *)0)
				goto no_client;
		#endif
	}

	SDLNet_TCP_Recv(client->socket, client->recv_data, 4096);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_client:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for \"client\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int server_send(GXClient_t* client)
{	

	// Argument check
	{
		#ifndef NDEBUG
			if(client == (void *)0)
				goto no_client;
		#endif
	}

	SDLNet_TCP_Send(client->socket, client->send_data, 4096);

	return 1;

	// Error handling
	{

		// Argument errors
		{
			no_client:
				#ifndef NDEBUG
					g_print_error("[G10] [Server] Null pointer provided for \"client\" in call to function \"%s\"\n", __FUNCSIG__);
				#endif
				return 0;
		}
	}
}

int server_parse     ( GXClient_t *client )
{
	GXCommand_t *command = 0;
	size_t       i       = 0;

	command_from_data(&command, &client->recv_data[i]);

	queue_enqueue(client->recv_queue, command);

	while (i < 4096)
	{
		i += command_from_data(&command, &client->recv_data[i]);

		queue_enqueue(client->recv_queue, command);

	}

	return 0;
}

int server_serialize ( GXClient_t *client )
{

	GXCommand_t *command = 0;
	size_t       i       = 0;

	while (i < 4096 && !queue_empty(client->send_queue))
	{
		char* a = 0;
		size_t j = 0;

		command = queue_dequeue(client->send_queue);

		j = data_from_command(&a , command);
		memcpy(&client->send_data[i], a, j);

		i += j;


	}
	return 0;
}

int server_process   ( GXClient_t *client )
{



	return 0;
}

int server_wait    ( GXInstance_t* instance )
{

	// Argument check 
	{
		#ifndef NDEBUG
			if (instance == (void *)0)
				goto no_instance;
		#endif
	}

	while (true)
	{

		// Initialized data
		GXServer_t *server = instance->server;
		TCPsocket   sock   = SDLNet_TCP_Accept(server->sock);

		if (sock) {
			g_print_log("[SDLNet] New TCP connection accepted\n");

			//Resize client list
			if (server->client_list == NULL) {
				server->client_list = malloc(sizeof(GXClient_t*));
				server->client_list_size = 1;
			}
			else {
				++server->client_list_size;
				server->client_list = realloc(server->client_list, sizeof(GXClient_t*) * server->client_list_size);
			}

			// Add client to list
			GXClient_t *client        = 0;
			GXThread_t *server_thread = 0;

			create_client(&client);

			char* buf = calloc(4096, sizeof(char));
			client->socket = sock;

			SDLNet_TCP_Recv(client->socket, buf, 4096);
			
			GXCommand_t *connect_command = 0;

			command_from_data(&connect_command, buf);

			if(connect_command->type == connect)
				printf("%s connected\n", connect_command->connect.name);

			load_thread(&server_thread, "G10/client thread.json");

			client->thread = server_thread;

			dict_add(instance->active_schedule->threads, client->thread->name, client->thread);


			server->client_list[0] = client;
			client->name = connect_command->connect.name;

			// TODO: Dynamically switch actors 
			size_t       active_scene_actors_count = dict_values(instance->active_scene->actors, 0);
			GXEntity_t **actor_list                = calloc(active_scene_actors_count+1, sizeof(void *));

			dict_values(instance->active_scene->actors, actor_list);

			for (size_t i = 0; i < active_scene_actors_count; i++)
			{
				GXEntity_t *entity = actor_list[i];
				GXCommand_t *actor_init_command                 = calloc(1, sizeof(GXCommand_t));
				actor_init_command->type                        = actor_initialize;
				actor_init_command->actor_initialize.name       = entity->name;
				actor_init_command->actor_initialize.location   = entity->transform->location;
				actor_init_command->actor_initialize.quaternion = entity->transform->rotation;
				actor_init_command->actor_initialize.scale      = entity->transform->scale;
				actor_init_command->actor_initialize.index      = 0;

				
				{
					vec3 l = entity->transform->location;

					printf("\n\"entity\" @ %.2f %.2f %.2f\n", l.x, l.y, l.z);
				}

				queue_enqueue(client->send_queue, actor_init_command);
			}

			server_serialize(client);
			server_send(client);


			client->thread->running = true;

			extern int client_work(GXThread_t * thread);

			client->thread->thread = SDL_CreateThread(client_work, client->thread->name, client);
		}
	}

	// TODO: 
	no_instance:
	return 0;
}

int create_client  ( GXClient_t** client)
{
	GXClient_t* c = calloc(1, sizeof(GXClient_t));
	*client = c;

	// TODO: Make a constructor 
	c->socket = NULL;
	c->name = NULL;
	c->send_data = calloc(4096, sizeof(u8));
	c->recv_data = calloc(4096, sizeof(u8));
	queue_construct(&c->send_queue, 64);
	queue_construct(&c->recv_queue, 64);
	return 0;
}
DLLEXPORT int process_command(GXCommand_t* p_command);

int connect_client(char* name)
{
	GXInstance_t* instance = g_get_active_instance();
	GXClient_t *i_client = 0;

	create_client(&instance->client);
	i_client = instance->client;

	i_client->name = name;

	IPaddress addr = { 0 };
	SDLNet_ResolveHost(&addr, "localhost", 9999);

	i_client->socket = SDLNet_TCP_Open(&addr);

	size_t nameLen = strlen(i_client->name);

	i_client->send_data = calloc(4096, 1);

	GXCommand_t* connect_command = calloc(1, sizeof(GXCommand_t));

	connect_command->type = connect;
	connect_command->connect.name = name;


	size_t connect_command_len = data_from_command(&i_client->send_data, connect_command);


	SDLNet_TCP_Send(i_client->socket, i_client->send_data, connect_command_len);

	GXThread_t* server_thread = 0;

	char* buf = calloc(4096, sizeof(char));

	load_thread(&server_thread, "G10/client thread.json");

	i_client->thread = server_thread;
	i_client->thread->running = true;

	dict_add(instance->active_schedule->threads, i_client->thread->name, i_client->thread);

	extern int client_work(GXThread_t * thread);

	i_client->thread->thread = SDL_CreateThread(client_work, i_client->thread->name, i_client);
	

	return 0;
}

int command_from_data(GXCommand_t** ret, void* data)
{
	GXCommand_t *i_ret = calloc(1, sizeof(GXCommand_t));

	u16 command_type = *((u16*)data);

	i_ret->type = command_type;

	switch (command_type)
	{
		case no_op:
			break;
		case connect:
		{
			{
				size_t name_len = strlen(&((u8*)data)[2]);
				char* name = (char *)data+2;
					
				i_ret->connect.name = calloc(name_len+1, sizeof(u8));
				
				strncpy(i_ret->connect.name, name, name_len);
			}
		}
			break;
		case actor_initialize:
			{
				size_t name_len = strlen(&((u8*)data)[0x34]);
				char* name = (char *)data+ 0x34;
					
				i_ret->actor_initialize.name = calloc(name_len+1, sizeof(u8));
				i_ret->actor_initialize.location = *(vec3*)(&((u8*)data)[0x4]);

				GXEntity_t *actor = get_entity(instance->active_scene, i_ret->actor_initialize.name);

				actor->transform->location = i_ret->actor_initialize.location;

				strncpy(i_ret->connect.name, name, name_len);
			}
			break;
		case actor_displace_rotate:
			break;
		case actor_detach:
			break;
		case chat:
		{
			u16   chat_channel = *((u16*)data+1),
                  chat_len     = *((u16*)data+2);
			char* text_chat    = (char *)data+6;

			i_ret->chat.chat_channel = chat_channel;
			i_ret->chat.chat         = calloc(chat_len + 1, sizeof(char));
			strncpy(i_ret->chat.chat, text_chat, chat_len);
		}
		break;
		case disconnect:
			break;
		default:
			break;
	}

	*ret = i_ret;
	return 0;
}

int data_from_command(void** ret, GXCommand_t* command)
{
	size_t ret_len = 0;

	switch (command->type)
	{
		case no_op:
			break;
		case connect:
		{
			
			size_t chat_len = strlen(command->chat.chat);
			ret_len += chat_len;
			ret_len += 2 + 1;

			char *retn = calloc(ret_len, sizeof(u8));

			*(u16*)retn = connect;
			strncpy(&retn[2], command->chat.chat, chat_len);

			*ret = retn;
		}
			break;
		case actor_initialize:
		{
			
			size_t chat_len = strlen(command->actor_initialize.name);
			ret_len += chat_len;
			ret_len += 2 + 2 + 1 + (2 * sizeof(vec3) ) + sizeof(quaternion);

			char *retn = calloc(ret_len, sizeof(u8));

			*(u16*)retn                              = actor_initialize;
			*((u16*)retn+1)                          = command->actor_initialize.index;
			*((vec3*)       ((u8*)retn + 4))                = command->actor_initialize.location;
			*((quaternion*) ((u8*)retn + 4 + sizeof(vec3)                      )) = command->actor_initialize.quaternion;
			*((vec3*)       ((u8*)retn + 4 + sizeof(vec3) + sizeof(quaternion) )) = command->actor_initialize.scale;


			strncpy( (u8*) ((u8*)retn + 4 + ( 2 * sizeof(vec3) ) + sizeof(quaternion) ), command->actor_initialize.name, chat_len);

			*ret = retn;
		}
			break;
		case actor_displace_rotate:
			break;
		case actor_detach:
			break;
		case chat:
		{
			size_t ret_len = 0,
				   chat_len = strlen(command->chat.chat);
			ret_len += chat_len;
			ret_len += 2 + 2 + 2 + 1;

			char *retn = calloc(ret_len, sizeof(u8));

			*(u16*)retn = chat;
			*((u16*)retn+1) = command->chat.chat_channel;
			*((u16*)retn+2) = (u16)chat_len;
			strncpy(&retn[6], command->chat.chat, chat_len);

			*ret = retn;
		}
		break;
		case disconnect:
			break;
		default:
			break;
	}

	return ret_len;
}


int destroy_client ( GXClient_t *client )
{
	free(client->name);
	free(client->send_data);
	free(client->recv_data);
	queue_destroy(client->send_queue);
	queue_destroy(client->recv_queue);

	SDLNet_TCP_Close(client->socket);

	free(client);

	return 0;
}
