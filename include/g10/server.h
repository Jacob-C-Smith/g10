/** !
 * Include header for server.
 * 
 * @file g10/g10.h
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// log module
#include <log/log.h>

// socket module
#include <socket/socket.h>

// json module
#include <json/json.h>

// web module
#include <web/web.h>
#include <web/server.h>
#include <web/route.h>

// parallel module
#include <parallel/parallelize.h>
#include <parallel/thread.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>

struct server_s
{
    web_server *p_web_server;
    parallel_thread *p_web_server_thread;
};

DLLEXPORT int server_from_json_value ( const server **pp_server, const json_value *const p_value );

DLLEXPORT int server_listen ( g_instance *const p_instance );
