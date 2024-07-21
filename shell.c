#include <g10/shell.h>

int shell_construct ( shell **pp_shell )
{

    // Argument check
    if ( pp_shell == (void *) 0 ) goto no_shell;

    // Initialized data
    shell *p_shell = G10_REALLOC(0, sizeof(shell));

    // Error check
    if ( p_shell == (void *) 0 ) goto no_mem;

    // Construct a thread for the shell loop
    parallel_thread_start(&p_shell->p_thread, shell_loop, p_shell);

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

int shell_loop ( shell *p_shell )
{

    // Argument check
    if ( p_shell == (void *) 0 ) goto no_shell;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);

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
        if ( fgets(&_buf, 1024, stdin) == 0 ) break;

        // Evaluate
        if ( strlen(&_buf) == 1 ) continue;

        // Info
        if ( strncmp(&_buf, "info", 4) == 0 )
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
        else if ( strncmp(&_buf, "camera", 6) == 0 ) r = camera_info(p_instance->context.p_scene->context.p_camera);
        
        // Input info
        else if ( strncmp(&_buf, "input", 5) == 0 ) r = input_info(p_instance->context.p_input);
        
        // Instance info
        else if ( strncmp(&_buf, "instance", 8) == 0 ) r = g_info(p_instance);
        
        // Renderer info
        else if ( strncmp(&_buf, "renderer", 8) == 0 ) r = renderer_info(p_instance->context.p_renderer);
        
        // Clear
        else if ( strncmp(&_buf, "clear", 5) == 0 ) {
            printf("\033[2J\033[H");
            r = 1;
        }

        // Shader
        else if ( strncmp(&_buf, "shader", 6) == 0 ) {
            char *t = strtok(&_buf, " \n");
            t = strtok(0, " \n");
            shader *p_shader = dict_get(p_instance->cache.p_shaders, t);

            shader_info(p_shader);
        }

        // Exit
        else if ( strncmp(&_buf, "exit", 4) == 0 )
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

            printf("%.*s: command not found\n", strlen(_buf) - 1, &_buf);
        }

        // Print
        //
    } 

    // Logout
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
