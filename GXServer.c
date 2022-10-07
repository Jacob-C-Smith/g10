#include <G10/GXServer.h>

int create_server(GXServer_t** pp_server)
{

	// Argument check
	GXServer_t* i_server = calloc(1, sizeof(GXServer_t));

	*pp_server = i_server;

	return 0;
}

int construct_server ( GXServer_t **pp_server )
{

	return 0;
}

int start_server(GXServer_t* p_server)
{

	p_server->sock = SDLNet_TCP_Open(&p_server->ip);

	return 0;
}

int server_recv(GXInstance_t* instance)
{
	return 0;
}

int server_send(GXInstance_t* instance)
{
	return 0;
}

int server_parse     ( GXInstance_t *instance )
{

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

int server_wait(GXInstance_t* instance)
{

	// TODO: Argument check instance->active_server

	while (true)
	{
		SDLNet_TCP_Accept(instance->active_server->sock);
	}

	return 0;
}

int recv_server(GXServer_t* p_server)
{

	// Argument check
	{

	}

	// Write 

	// Write the data

	// 
	
	return 0;
}