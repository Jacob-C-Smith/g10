#include <G10/GXServer.h>

int create_server(GXServer_t** pp_server)
{
	GXServer_t* i_server = calloc(1, sizeof(GXServer_t));

	*pp_server = i_server;

	return 0;
}

int construct_server ( GXServer_t **pp_server )
{

	return 0;
}

int send_server(GXServer_t* p_server)
{

	// Write the command to the data

	// Write the data
	
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

int recv_server(GXServer_t* p_server)
{

	
	return 0;
}