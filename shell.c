#include <g10/shell.h>

// Function declarations
int shell_evaluate ( char *p_input, char *p_output );

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

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    unsigned char a = (ip_address & 0xff000000) >> 24,
                  b = (ip_address & 0x00ff0000) >> 16,
                  c = (ip_address & 0x0000ff00) >> 8,
                  d = (ip_address & 0x000000ff) >> 0;
    char _buf[1024] = { 0 };
    char _out[4096] = { 0 };
    int r = 1;
    
    // Log the IP
    printf("\r\033[44m\033[[[[[%d.%d.%d.%d:%d CONNECTED]]]\033[0m\n", a, b, c, d, port);
    
    // Reprint the prompt
    printf("\033[42m\033>>>>\033[0m "); fflush(stdout);
    
    // REPL 
    while ( p_instance->running )
    {

        // Clear the input buffer
        memset(&_buf, 0, sizeof(_buf));

        // Prompt
        if ( r == 1 ) socket_tcp_send(_socket_tcp, "\033[42m\033>>>>\033[0m ", 16);
        else          socket_tcp_send(_socket_tcp, "\033[41m\033>>>>\033[0m ", 16);
        
        int z = socket_tcp_receive(_socket_tcp, _buf, sizeof(_buf));
        
        // Receive data from the socket
        if ( z == 0 )
        {

            // Print the disconnect
            printf("\r\033[44m\033[[[[[%d.%d.%d.%d:%d DISCONNECTED]]]\033[0m\n", a, b, c, d, port);

            // Reprint the prompt
            printf("\033[42m\033>>>>\033[0m "); fflush(stdout);

            // Done
            break;
        }

        // Evaluate
        r = shell_evaluate(&_buf, &_out);

        // Error check
        if ( r == -1 )
        {
            
            // Close the connection
            socket_tcp_send(_socket_tcp, "", 0);
            socket_tcp_send(_socket_tcp, "Bye bye!", 8);
            socket_tcp_destroy(&_socket_tcp);

            // Log the disconnect
            printf("\r\033[44m\033[[[[[%d.%d.%d.%d:%d DISCONNECTED]]]\033[0m\n", a, b, c, d, port);

            // Reprint the prompt
            printf("\033[42m\033>>>>\033[0m "); fflush(stdout);
            
            // Done
            break;
        }

        // Print
        socket_tcp_send(_socket_tcp, &_out, strlen(_out));
    } 

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

int shell_loop ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // State check
    if ( feof(stdin) )

        // Sleep forever
        sleep((unsigned int) -1);

    // Initialized data
    char _buf[1024] = { 0 };
    char _out[4096] = { 0 };
    int r = 1;

    // REPL 
    while ( p_instance->running )
    {

        // Prompt
        if ( r == 1 ) printf("\033[42m\033>>>>\033[0m ");
        else          printf("\033[41m\033>>>>\033[0m ");

        // Read
        if ( fgets(_buf, sizeof(_buf), stdin) == 0 ) break; 

        // Evaluate
        r = shell_evaluate(&_buf, &_out);

        // Print
        printf("%s", &_out);
    } 

    // Log the detach
    if ( feof(stdin) ) printf("\r\033[44m\033[[[[[EOF]]]\033[0m\n");

    // Done
    return 1;

    // Error handling
    {
        no_instance:
            #ifndef NDEBUG
                log_error("[g10] [shell] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int shell_evaluate ( char *p_input, char *p_output )
{

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    int r = 1;

    // Blank line
    if ( strlen(p_input) == 1 ) return 1;

    // Quit
    if ( strncmp(p_input, "quit", 4) == 0 )
    {
        
        // Clear the run flag
        p_instance->running = false;

        // Clear the repeat flag
        parallel_schedule_pause(p_instance->p_schedule);

        // Exit
        r = -1;
    }

    // Exit
    else if ( strncmp(p_input, "disconnect", 10) == 0 )
    {

        // Update the result
        r = -1;
    }
    
    // Default
    else 
    {
        r = 0;

        sprintf(p_output, "%.*s: command not found\n", (int) strlen(p_input) - 1, p_input);
    }

    // Done
    return r;
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