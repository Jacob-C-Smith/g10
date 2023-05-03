#pragma once

#include <stdio.h>

#include <SDL_net.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

enum   command_type_e  {
	no_op                 = 0,
	connect_CMD           = 1,
	actor_initialize      = 2,
	actor_displace_rotate = 3,
	actor_detach          = 4,
	chat                  = 5,
	disconnect            = 6 
};

enum   chat_channel_e
{
	chat_channel_all   = 0,
	chat_channel_team  = 1,
	chat_channel_party = 2,
};

struct GXServer_s {

	dict         *clients;  
	GXClient_t  **client_list;  

	size_t		  client_list_size,
		          max_clients;
	IPaddress     ip;
	TCPsocket     sock;
	
	char         *name;
	char         *password;
};

struct GXClient_s
{
	GXThread_t  *thread;
	TCPsocket    socket;
	GXEntity_t **actors;
	size_t       actor_count,
		         recv_len,
				 send_len;

	char        *name;
	 
	u8          *send_data,
		        *recv_data;

	queue       *send_queue;
	queue       *recv_queue;

};

struct GXCommand_s {

	enum command_type_e type;

	union {

		// No operation
		struct {
			size_t i;
		} no_op;

		// Connect 
		struct {
			char *name;
		} connect;

		// Initialize an actor
		struct {
			char       *name;
			vec3        location;
			quaternion  quaternion;
			vec3        scale;
			u16         index;
		} actor_initialize;

		// Actor displace rotate
		struct {
			vec3        location;
			quaternion  quaternion;
			vec3        velocity;
			u16         index;
		} actor_displace_rotate;

		// Stop tracking an actor
		struct {
			u16 index;
		} actor_detach;

		// Send chat
		struct {
			char                *chat;
			enum chat_channel_e  chat_channel;
		} chat;

		// Disconnect 
		struct {
			size_t i;
		} disconnect;
	};
};

// Allocator
/* !
 *  Allocate memory for a server structure. 
 *
 * @param pp_server : return
 *
 * @sa destroy_server
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_server       ( GXServer_t  **pp_server );

// Constructors
/* !
 *  Load a server from a JSON file
 *
 * @param pp_server : return
 * @param path      : The path to a JSON server object on the filesystem
 *
 * @sa load_server_as_json
 * @sa create_server
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_server         ( GXServer_t  **pp_server, char *path );

/* !
 *  Load a server as JSON text
 *
 * @param pp_server  : return
 * @param token_text : The server JSON object text
 * @param len        : The length of the server JSON object text
 *
 * @sa load_server
 * @sa create_server
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_server_as_json ( GXServer_t  **pp_server, char *token_text, size_t len );

// Start
/* !
 *  Start running a server
 *
 * @param p_server  : Pointer to a server
 *
 * @sa stop_server
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int start_server        ( );

// Sending / Recieving over the internet
/* !
 *  Copy data from the TCP socket into client->recv_data 
 *
 * @param client : Pointer to a client
 *
 * @sa server_send
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int server_recv         ( GXClient_t   *client );

/* !
 *  Write client->send_data to the TCP socket
 *
 * @param client : Pointer to a client
 *
 * @sa server_recv
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int server_send         ( GXClient_t   *client );

// Parsing / Serializing commands into queues
/* !
 *  Parse each raw command in client->recv_data into a command queue
 *
 * @param client : Pointer to a client
 *
 * @sa server_serialize
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int server_parse        ( GXClient_t   *client );

/* !
 *  Parse client->recv_data into a command queue
 *
 * @param client : Pointer to a client
 *
 * @sa server_process
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int server_serialize    ( GXClient_t   *client );

// Process command queues
/* !
 *  This function will do different things if you are a server or a client. 
 * 
 *  Run commands in recv command queue, generate new commands for send command queue
 *
 * @param client : Pointer to a client
 *
 * @sa server_process
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int server_process      ( GXClient_t   *client );

// Wait for clients to join
/* !
 *  Start running a server wait thread. 
 *  The wait thread will wait for TCP connections. If the TCP connections are suitable, a new client thread will be spawned.  
 *
 * @param p_server  : Pointer to a server
 *
 * @sa stop_server
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int server_wait         ( GXInstance_t *instance );

// TODO: Document

// Destructors
DLLEXPORT int destroy_server      ( GXServer_t   *p_server );

DLLEXPORT int command_from_data   ( GXCommand_t **ret, void        *data ); 
DLLEXPORT int data_from_command   ( void        **ret, GXCommand_t *conmmand );

DLLEXPORT int create_client(GXClient_t** client);
DLLEXPORT int connect_client (char *name);

//Destroy client instance and close TCP connection with client
DLLEXPORT int destroy_client(GXClient_t* client);
DLLEXPORT int destroy_command(GXCommand_t* command);
