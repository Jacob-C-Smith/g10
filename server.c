#include <g10/server.h>

int server_from_json_value ( const server **pp_server, const json_value *const p_value )
{

    // Argument check
    if ( pp_server == (void *) 0 ) goto no_server;
    if ( p_value   == (void *) 0 ) goto no_value;

    // Initialized data
    server *p_server = (void *) 0;
    web_server *p_web_server = (void *) 0;

    // Error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto server_value_is_wrong_type;

    // Parse the json value into an server
    {

        // Initialized data
        const dict *const p_dict = p_value->object;
        const json_value *p_web_value = 0;

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [server] Consider adding a \"$schema\" property to the server config\n");

        // Get the name
        p_web_value = dict_get(p_dict, "web");

        // Check for missing properties
        if ( ! ( p_web_value ) ) goto missing_properties;

        // Error checking
        if ( p_web_value->type != JSON_VALUE_OBJECT ) goto web_property_is_wrong_type;

        // Construct the HTTP server
        if ( web_server_from_json_value(&p_web_server, p_web_value) == 0 ) goto failed_to_construct_web_server;

    }

    // Allocate memory for the server
    p_server = G10_REALLOC(0, sizeof(server));

    // Populate the server struct
    *p_server = (server)
    {
        .p_web_server = p_web_server
    };

    // Start running the web server
    p_server->p_web_server->context.running = true;

    // Return a pointer to the caller
    *pp_server = p_server;

    // Success
    return 1;

    // Error
    server_value_is_wrong_type:
    missing_properties:
    web_property_is_wrong_type:
    failed_to_construct_web_server:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_server:
                #ifndef NDEBUG
                    log_error("[g10] [server] Null pointer provided for parameter \"pp_server\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [server] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int server_listen ( g_instance *const p_instance )
{
    
    // Get a new client
    socket_tcp_listen(p_instance->context.p_server->p_web_server->server.socket, &web_server_accept, p_instance->context.p_server->p_web_server);

    // Success
    return 1;

}
