﻿/** !
 * Implements general G10 functionality
 * 
 * @file g10.c
 * 
 * @author Jacob Smith
 */

// Headers
#include <g10/g10.h>

#ifdef G10_BUILD_WITH_SDL2
extern int g_sdl2_initialize_window ( g_instance *p_instance, json_value *p_value );
#endif

#ifdef G10_BUILD_WITH_VULKAN
extern int g_vulkan_initialize ( g_instance *p_instance, json_value *p_value );
#endif

// Data
const char *_p_signal_data[] = 
{
    [SIGHUP]  = " hangup",
    [SIGINT]  = " program interrupt",
    [SIGQUIT] = " quit signal",
    [SIGILL]  = "n illegal instruction",
    [SIGABRT] = "n abort signal",
    [SIGFPE]  = " floating point exception",
    [SIGKILL] = " kill signal",
    [SIGSEGV] = " segmentation violation",
    [SIGTERM] = " terminate signal"
};

// Static initialized data
static g_instance *p_active_instance = 0;

/** !
 * Initialize libraries
 * 
 * @param void
 * 
 * @return void
 */
u0 g_init_early ( void )
{

    // Initialize log library
    log_init();

    // Initialize parallel
    parallel_init();

    // Add core scheduler tasks
    parallel_register_task("user code", (fn_parallel_task *) user_code_callback);
    parallel_register_task("pre ai"   , (fn_parallel_task *) ai_preupdate);
    parallel_register_task("ai"       , (fn_parallel_task *) ai_update);
    parallel_register_task("render"   , (fn_parallel_task *) renderer_render);

    // Add 3rd party scheduler tasks
    #ifdef BUILD_G10_WITH_SDL2

        // External functions
        extern int g_sdl2_poll_window ( g_instance *p_instance );
        parallel_register_task("sdl2 poll", (fn_parallel_task *) g_sdl2_poll_window);
    #endif

    // Done
    return;
}

/** !
 * Signal handler
 * 
 * @param signal_number the signal number
 * 
 * @return void
*/
u0 g_signal_handler ( int signal_number )
{

    // Initialized data
    size_t i = 0;

    // Print the signal
    log_error("\n[%s] A%s was encountered.\n\n", p_active_instance->_name, _p_signal_data[signal_number]);

    // Dump the message log
    while ( circular_buffer_empty(p_active_instance->debug) == false )
    {

        // Initialized data
        const char *p_message = (void *) 0;

        // Get the message
        circular_buffer_pop(p_active_instance->debug, &p_message);

        // Print the message
        log_error("[%d] %s\n", i, p_message);

        // Increment the counter
        i++;
    }

    // Done
    exit(EXIT_FAILURE);
}

int g_init ( g_instance **pp_instance, const char *p_path )
{

    // Early 
    g_init_early();

    // Argument Check
    if ( pp_instance == (void *) 0 ) goto no_instance;
    if ( p_path      == (void *) 0 ) goto no_path;

    // Initialized data
    size_t file_len          = g_load_file(p_path, (void *) 0, true),
           debug_message_max = 64;
    char *const p_file_contents = G10_REALLOC(0, (file_len + 1) * sizeof(char));
    const json_value *p_value = 0;
    g_instance  _instance = { 0 },
               *p_instance = 0;

    // Error check
    if ( file_len == 0 ) goto failed_to_load_file;

    // Load the file
    if ( g_load_file(p_path, p_file_contents, true) == 0 ) goto failed_to_load_file;

    // Parse the file into a json value
    if ( json_value_parse(p_file_contents, 0, &p_value) == 0 ) goto failed_to_json_parse_value;

    // Error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto instance_value_is_wrong_type;

    // Parse the json value into an instance
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name_value      = dict_get(p_dict, "name"),
                         *p_version         = dict_get(p_dict, "version"),
                         *p_schedule        = dict_get(p_dict, "schedule"),
                         *p_renderer        = dict_get(p_dict, "renderer"),
                         *p_fixed_tick_rate = dict_get(p_dict, "fixed tick rate"),
                         *p_vulkan          = dict_get(p_dict, "vulkan"),
                         *p_scene           = dict_get(p_dict, "initial scene"),
                         *p_debug           = dict_get(p_dict, "debug"),
                         *p_window          = dict_get(p_dict, "window");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] Consider adding a \"$schema\" property to the instance\n");

        // Missing properties
        if ( p_name_value == (void *) 0 ) goto missing_name_property;

        // Store the name
        if ( p_name_value )
        {

            // Error check
            if ( p_name_value->type != JSON_VALUE_STRING ) goto name_property_is_wrong_type;

            // Initialized data
            size_t len = strlen(p_name_value->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the instance name
            strncpy(_instance._name, p_name_value->string, 255);

            // Store a null terminator
            _instance._name[len] = '\0';
        }

        // Store the version
        if ( p_version )
        {

            // Error check
            if ( p_version->type != JSON_VALUE_OBJECT ) goto wrong_version_type;

            // Parse the json value into a version
            {

                // Initialized data
                dict *p_dict = p_version->object;
                json_value *p_major = dict_get(p_dict, "major"),
                           *p_minor = dict_get(p_dict, "minor"),
                           *p_patch = dict_get(p_dict, "patch");
                
                // Error check
                if ( ! ( p_major && p_minor && p_patch ) ) goto missing_version_properties;

                // Extra checking
                if ( p_major->type != JSON_VALUE_INTEGER ) goto version_major_wrong_type;
                if ( p_minor->type != JSON_VALUE_INTEGER ) goto version_minor_wrong_type;
                if ( p_patch->type != JSON_VALUE_INTEGER ) goto version_patch_wrong_type;
                
                // Store each version number
                _instance.version.major = p_major->integer,
                _instance.version.minor = p_minor->integer,
                _instance.version.patch = p_patch->integer;
            }
        }

        // Default to 1.0.0
        else
            _instance.version.major = 1,
            _instance.version.major = 0,
            _instance.version.patch = 0;

        // Set the fixed tick rate
        if ( p_fixed_tick_rate )
        {

            // Error check
            if ( p_fixed_tick_rate->type != JSON_VALUE_INTEGER ) goto fixed_tick_rate_is_wrong_type;

            // Store the fixed tick rate
            _instance.context.fixed_tick_rate = p_fixed_tick_rate->integer;
        }

        // Set up the debug system
        if ( p_debug )
        {
            
            // Extra checking
            if ( p_debug->type != JSON_VALUE_INTEGER ) goto debug_wrong_type;

            // Store the 
            debug_message_max = p_debug->integer;
        }            

        // Initialize the window
        #ifdef BUILD_G10_WITH_SDL2
            
            // SDL2
            g_sdl2_initialize_window(&_instance, p_window);
        #else

            // Others? 
        #endif

        // Initialize the graphics API
        #ifdef G10_BUILD_WITH_VULKAN
    
            // External functions
            g_vulkan_initialize(&_instance, p_vulkan);
        #elif defined G10_BUILD_WITH_OPENGL

            // External functions            
            extern int g_opengl_initialize ( g_instance *p_instance, json_value *p_value );

            // OpenGL
            g_opengl_initialize(&_instance, (void *) 0);
        #else
            
            // Others? 
        #endif

        // Initialize the scheduler
        if ( p_schedule )
            if ( parallel_schedule_load_as_json_value(&_instance.p_schedule, p_schedule) == 0 ) goto failed_to_load_schedule_from_json_value;
        
        // Load the renderer
        if ( p_renderer )
            if ( renderer_from_json(&_instance.context.p_renderer, p_renderer) == 0 ) goto failed_to_load_renderer;

        // Load the initial scene
        if ( p_scene )
            if ( scene_from_json(&_instance.context.p_scene, p_scene) == 0 ) goto failed_to_load_initial_scene;        
    }

    // Construct a circular buffer for messages
    circular_buffer_construct(&_instance.debug, debug_message_max);

    // Set up signal handlers
    signal(SIGHUP , g_signal_handler);
    signal(SIGINT , g_signal_handler);
    signal(SIGQUIT, g_signal_handler);
    signal(SIGILL , g_signal_handler);
    signal(SIGABRT, g_signal_handler);
    signal(SIGFPE , g_signal_handler);
    signal(SIGKILL, g_signal_handler);
    signal(SIGSEGV, g_signal_handler);
    signal(SIGTERM, g_signal_handler);
    
    // Allocate memory for the instance
    p_instance = G10_REALLOC(0, sizeof(g_instance));

    // Error check
    if ( p_instance == (void *) 0 ) goto no_mem;

    // Copy the memory from the stack to the heap
    memcpy(p_instance, &_instance, sizeof(g_instance));

    // Set the instance singleton
    p_active_instance = p_instance;

    // Store a timestamp
    p_active_instance->time.init = timer_high_precision();

    // Return an instance pointer to the caller
    *pp_instance = p_instance;

    // Clean up
    json_value_free(p_value);
    G10_REALLOC(p_file_contents, 0);

    // Success
    return 1;

    failed_to_load_renderer:
    debug_wrong_type:
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_load_initial_scene:
                #ifndef NDEBUG
                    log_error("[g10] Failed to construct scene in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            failed_to_json_parse_value:
                #ifndef NDEUBG
                    log_error("[g10] Failed to parse file \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // Error
                goto error_after_file_allocated;

            instance_value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] Value must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;
                
            missing_name_property:
                #ifndef NDEUBG
                    log_error("[g10] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // Error
                goto error_after_json_parsed;

            fixed_tick_rate_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] Value must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            missing_properties:
                #ifndef NDEBUG
                    log_error("[g10] Missing required properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            missing_version_properties:
                #ifndef NDEBUG
                    log_error("[g10] \"version\" property is missing required properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            version_major_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"major\" property of \"version\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            version_minor_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"minor\" property of \"version\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            version_patch_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"patch\" property of \"version\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;
            
            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;

            wrong_version_type:
                #ifndef NDEBUG
                    log_error("[g10] \"version\" property of instance object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                goto error_after_json_parsed;
        }

        // Parallel errors
        {
            failed_to_load_schedule_from_json_value:
                #ifndef NDEBUG
                    log_error("[g10] Failed to construct schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Memory allocator returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to load file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                goto error_after_file_allocated;
        }

        // Clean up
        error_after_json_parsed:
            json_value_free(p_value);
        error_after_file_allocated:
            G10_REALLOC(p_file_contents, 0);

        // Error
        return 0;
    }
}

int g_info ( g_instance *p_instance )
{

    // Argument check
    //

    // Initialized data
    //

    // Print the instance
    printf("Instance:\n");
    printf(" - name    : %s\n", p_instance->_name);
    printf(" - version : v%d.%d.%d\n", p_instance->version.major, p_instance->version.minor, p_instance->version.patch);
    printf(" - window  :\n");
    printf("    - width  : %d\n", p_instance->window.width);
    printf("    - height : %d\n", p_instance->window.height);
    printf("    - title  : %s\n", p_instance->window.title);

    // Success
    return 1;
}

g_instance *g_get_active_instance ( void )
{

    // Get
    return p_active_instance;
}

size_t g_load_file ( const char *const p_path, void *const p_buffer, bool binary_mode )
{

    // Argument checking 
    if ( p_path == 0 ) goto no_path;

    // Initialized data
    size_t ret = 0;
    FILE *f = fopen(p_path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( p_buffer ) ret = fread(p_buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for parameter \"p_path\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // File errors
        {
            invalid_file:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to load file \"%s\". %s\n", p_path, strerror(errno));
                #endif

                // Error
                return 0;
        }
    }
}

int g_exit ( g_instance **pp_instance )
{

    // Argument Check
    if ( pp_instance == (void *) 0 ) goto no_instance;

    // Initialized data
    g_instance *p_instance = *pp_instance;
    
    // No more singleton for caller
    p_active_instance = 0;

    // No more pointer for caller
    *pp_instance = 0;

    // Store a timestamp
    //p_instance->time.exit = timer_high_precision();

    // Clean up
    G10_REALLOC(p_instance, 0);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

u0 g_exit_late ( void ) 
{

    // Done
    return;
}
