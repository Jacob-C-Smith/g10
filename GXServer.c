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

int start_server(GXServer_t* p_server)
{

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

int server_recv(GXInstance_t* instance)
{
	// Argument check
	{
		GXClient_t* c = instance->server->client_list[0];
		SDLNet_TCP_Recv(c->socket, c->recv_data, 4096);

	}

	// Write the data

	return 0;
}

int server_send(GXInstance_t* instance)
{
	return 0;
}

int server_parse     ( GXInstance_t *instance )
{

	GXClient_t* c = instance->server->client_list[0];
	GXCommand_t *command = 0;
	command_from_data(&command, c->recv_data);

	queue_enqueue(c->recv_queue, command);



	return 0;
}

int server_serialize ( GXInstance_t *instance )
{

	return 0;
}

int server_process   ( GXInstance_t *instance )
{

	return 0;
}

int server_wait    ( GXInstance_t* instance )
{

	// TODO: Argument check instance->active_server

	while (true)
	{
		GXServer_t* server = instance->server;
		TCPsocket sock = SDLNet_TCP_Accept(server->sock);
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
			
			load_thread(&server_thread, "G10/client thread.json");

			client->thread = server_thread;
			client->thread->running = true;

			dict_add(instance->active_schedule->threads, client->thread->name, client->thread);

			extern int work(GXThread_t * thread);

			client->thread->thread = SDL_CreateThread(work, client->thread->name, client->thread);

			client->socket = sock;
			server->client_list[server->client_list_size - 1] = client;
			
			char *buf = calloc(4096, sizeof(char));
			SDLNet_TCP_Recv(client->socket, buf, 4096);

			printf("CLIENT SAYS: %s\n", buf);
			
			if (*((u16*)buf) == connect) {
				char* clName = &buf[2];
				size_t nameLen = strlen(clName);
				client->name = calloc(nameLen + 1, sizeof(char));
				strncpy(client->name, clName, nameLen+1);
				printf("%s connected\n", client->name);
			}
		}
	}

	return 0;
}

int create_client  ( GXClient_t** client)
{
	GXClient_t* c = malloc(sizeof(GXClient_t));
	*client = c;
	c->socket = NULL;
	c->name = NULL;
	c->send_data = NULL;
	c->recv_data = NULL;
	queue_create(&c->send_queue);
	queue_create(&c->recv_queue);
	return 0;
}
DLLEXPORT int process_command(GXCommand_t* p_command);

int connect_client ( GXClient_t** client )
{
	GXClient_t *i_client = 0;

	create_client(client);

	i_client = *client;

	i_client->name = "Parma Jawn";

	IPaddress addr = { 0 };
	SDLNet_ResolveHost(&addr, "localhost", 9999);

	i_client->socket = SDLNet_TCP_Open(&addr);

	size_t nameLen = strlen(i_client->name);

	i_client->send_data = malloc(4096);
	*((u16*)&i_client->send_data[0]) = connect;
	strncpy(&i_client->send_data[2], i_client->name, nameLen+1);
	SDLNet_TCP_Send(i_client->socket, i_client->send_data, 4096);

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
			break;
		case actor_initialize:
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

	switch (command->type)
	{
		case no_op:
			break;
		case connect:
			break;
		case actor_initialize:
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

	return 0;
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
