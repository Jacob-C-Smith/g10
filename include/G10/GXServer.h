#pragma once

#include <stdio.h>

#include <SDL2/SDL_net.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>

enum   command_type_e  {
	no_op                 = 0,
	connect               = 1,
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

	dict* clients; //Clients that have confirmed connection by sending a connect packet
	GXClient_t** client_list; //Clients that have been accepted over TCP
	GXEntity_t** actors;

	size_t		 client_list_size; //Size of client_list
	IPaddress    ip;
	TCPsocket    sock;
	

	char* name;
	char* password;
};

struct GXClient_s
{
	GXThread_t *thread;
	TCPsocket   socket;

	char       *name;
	 
	u8         *send_data,
		       *recv_data;

	queue      *send_queue;
	queue      *recv_queue;

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
			char *connection_json;
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
DLLEXPORT int create_server       ( GXServer_t  **pp_server );
 
// Constructor
DLLEXPORT int construct_server    ( GXServer_t  **pp_server );

DLLEXPORT int load_server         ( GXServer_t  **pp_server, char *path );


DLLEXPORT int load_server_as_json ( GXServer_t  **pp_server, char *token_text, size_t len );

// Start
DLLEXPORT int start_server        ( GXServer_t   *p_server );

DLLEXPORT int process_command     ( GXCommand_t  *p_command );

// Sending / Recieving over the internet
DLLEXPORT int server_recv         ( GXInstance_t *instance );
DLLEXPORT int server_send         ( GXInstance_t *instance );

// Turn a command queue into data or vice versa
DLLEXPORT int server_parse        ( GXInstance_t *instance );
DLLEXPORT int server_serialize    ( GXInstance_t *instance );

// Process server commands
DLLEXPORT int server_process      ( GXInstance_t *instance );

// Wait for clients to join
DLLEXPORT int server_wait         ( GXInstance_t *instance );

// Destructors
DLLEXPORT int destroy_server      ( GXServer_t   *p_server );

DLLEXPORT int command_from_data   ( GXCommand_t **ret, void        *data ); 
DLLEXPORT int data_from_command   ( void        **ret, GXCommand_t *conmmand );

DLLEXPORT int create_client(GXClient_t** client);

DLLEXPORT int connect_client (GXClient_t **client);

//Destroy client instance and close TCP connection with client
DLLEXPORT int destroy_client(GXClient_t* client);
