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
	chat                  = 4,
	disconnect            = 5 
};

enum   chat_channel_e
{
	chat_channel_all   = 0,
	chat_channel_team  = 1,
	chat_channel_party = 2,
};

struct GXServer_s {

	dict *clients;
};

struct GXClient_s
{
	u8    *send_data,
		  *recv_data;

	queue *send_queue;
	queue *recv_queue;
	
};

struct GXCommand_s {

	enum command_type_e command_type;
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

		} actor_initialize;

		// Actor displace rotate
		struct {
			vec3        location;
			quaternion  quaternion;
		} actor_displace_rotate;

		// Send chat
		struct {
			char                *chat;
			enum chat_channel_e  chat_channel;
		} chat;

		// Disconnect 
		struct {
			size_t i;
		} disconnect;

	}      commands;
};

int create_server    ( GXServer_t  **pp_server );

int construct_server ( GXServer_t  **pp_server );

int process_command  ( GXCommand_t  *p_command );

int server_recv      ( GXInstance_t *instance );

int server_send      ( GXInstance_t *instance );

int send_command     ( GXServer_t   *p_server );

int recv_command     ( GXServer_t   *p_server );

int destroy_server   ( GXServer_t   *p_server );

