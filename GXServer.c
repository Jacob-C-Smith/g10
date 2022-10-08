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