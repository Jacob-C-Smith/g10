#include <g10/shell.h>

// Function declarations
/** !
 * Run a g10 shell
 * 
 * @param p_shell the shell
 * 
 * @return 1 on success, 0 on error
*/
int shell_loop ( shell *p_shell );

// Function definitions
int shell_construct ( shell **pp_shell )
{

    // Argument check
    if ( pp_shell == (void *) 0 ) goto no_shell;

    // Initialized data
    shell *p_shell = G10_REALLOC(0, sizeof(shell));

    // Error check
    if ( p_shell == (void *) 0 ) goto no_mem;

    // Log the attach
    printf("\r\033[44m\033[[[[[ATTACHED]]]\033[0m\n");

    // Construct a thread for the shell loop
    parallel_thread_start(&p_shell->p_thread, (fn_parallel_task *) shell_loop, p_shell);

    // Return a pointer to the caller
    *pp_shell = p_shell;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shell:
                #ifndef NDEBUG
                    log_error("[g10] [shell] Null pointer provided for parameter \"pp_shell\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int shell_accept_connection ( socket_tcp _socket_tcp, unsigned long ip_address, unsigned short port )
{

    // Uninitialized data
    char buffer[4096];

    // Initialized data
    unsigned char a = (ip_address & 0xff000000) >> 24,
                  b = (ip_address & 0x00ff0000) >> 16,
                  c = (ip_address & 0x0000ff00) >> 8,
                  d = (ip_address & 0x000000ff) >> 0;
    
    // Log the IP
    printf("\r\033[44m\033[[[[[CONNECTED]]]\033[0m\n");
    
    // Reprint the prompt
    printf("\033[42m\033>>>>\033[0m "); fflush(stdout);

    // Print the prompt
    socket_tcp_send(_socket_tcp, "\033[42m\033>>>>\033[0m ", 16);

    // Receive data from the socket
    if ( socket_tcp_receive(_socket_tcp, buffer, 4096) == 0 )
    {
        printf("\r\033[44m\033[[[[[DISCONNECTED]]]\033[0m\n");
        printf("\033[42m\033>>>>\033[0m "); fflush(stdout);
    }

    if ( strncmp(buffer, "name", 4) == 0 ) socket_tcp_send(_socket_tcp, (g_get_active_instance())->_name, strlen((g_get_active_instance())->_name));

    // Success
    return 1;
}

int shell_network_listener ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Initialized data
    socket_tcp _tcp_socket = 0;

    // Create a socket
    socket_tcp_create(&_tcp_socket, socket_address_family_ipv4, 6710);

    // Listen for connections
    while ( p_instance->running ) socket_tcp_listen(_tcp_socket, (socket_tcp_accept_callback_function_t) shell_accept_connection, 0);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_info("[g10] [shell] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int shell_loop ( shell *p_shell )
{

    // Argument check
    if ( p_shell == (void *) 0 ) goto no_shell;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    char _buf[1024] = { 0 };
    int r = 1;

    // REPL 
    while ( p_instance->running )
    {

        // Prompt
        if ( r == 1 )
            printf("\033[42m\033>>>>\033[0m ");
        else 
            printf("\033[41m\033>>>>\033[0m ");

        // Read
        if ( fgets(_buf, 1024, stdin) == 0 ) break;

        // Evaluate
        if ( strlen(_buf) == 1 ) continue;

        // Info
        if ( strncmp(_buf, "info", 4) == 0 )
        {

            // Dump the message log
            while ( circular_buffer_empty(p_instance->debug) == false )
            {

                // Initialized data
                const char *p_message = (void *) 0;

                // Get the message
                circular_buffer_pop(p_instance->debug, &p_message);

                // Print the message
                log_info("%s\n", p_message);
            }

            r = 1;
        }

        // Camera info
        else if ( strncmp(_buf, "camera", 6) == 0 ) r = camera_info(p_instance->context.p_scene->context.p_camera);
        
        // Input info
        else if ( strncmp(_buf, "input", 5) == 0 ) r = input_info(p_instance->context.p_input);
        
        // Instance info
        else if ( strncmp(_buf, "instance", 8) == 0 ) r = g_info(p_instance);
        
        // Renderer info
        else if ( strncmp(_buf, "renderer", 8) == 0 ) r = renderer_info(p_instance->context.p_renderer);
        
        // Clear
        else if ( strncmp(_buf, "clear", 5) == 0 ) {
            printf("\033[2J\033[H");
            r = 1;
        }

        // Shader
        else if ( strncmp(_buf, "shader", 6) == 0 ) {
            char *t = strtok(_buf, " \n");
            t = strtok(0, " \n");
            const shader *p_shader = dict_get(p_instance->cache.p_shaders, t);

            shader_info(p_shader);
        }

        // Exit
        else if ( strncmp(_buf, "exit", 4) == 0 )
        {
            
            // Clear the run flag
            p_instance->running = false;

            // Done
            break;
        }
        
        // Default
        else 
        {
            r = 0;

            printf("%.*s: command not found\n", (int) strlen(_buf) - 1, _buf);
        }
    } 

    // Log the detach
    if ( feof(stdin) ) printf("\r\033[44m\033[[[[[EOF]]]\033[0m\n");

    // Done
    return 1;

    // Error handling
    {
        no_shell:
            #ifndef NDEBUG
                log_error("[g10] [shell] Null pointer provided for parameter \"p_shell\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int shell_detach ( shell *p_shell )
{

    // Argument check
    if ( p_shell == (void *) 0 ) goto no_shell;
    
    // Cancel the thread
    parallel_thread_cancel(p_shell->p_thread);

    // Log the detach
    if ( feof(stdin) == false ) printf("\r\033[44m\033[[[[[DETACHED]]]\033[0m\n");

    // Join
    parallel_thread_destory(&p_shell->p_thread);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_shell:
                #ifndef NDEBUG
                    log_error("[g10] [shell] Null pointer provided for parameter \"p_shell\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}