﻿/** !
 * Implements general G10 functionality
 * 
 * @file g10.c
 * @author Jacob Smith
 */

// Headers
#include <g10/g10.h>

#ifdef BUILD_G10_WITH_SDL2
extern int g_sdl2_initialize_window ( g_instance *p_instance, json_value *p_value );
#endif

#ifdef BUILD_G10_WITH_VULKAN
extern int g_vulkan_initialize ( g_instance *p_instance, json_value *p_value );
#endif



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
    log_init(0, BUILD_G10_WITH_ANSI_COLOR);

    // Done
    return;
}

int g_init ( g_instance **pp_instance, const char *p_path )
{

    // Early 
    g_init_early();

    // Argument Check
    if ( pp_instance == (void *) 0 ) goto no_instance;
    if ( p_path      == (void *) 0 ) goto no_path;

    // Initialized data
    g_instance  _instance = { 0 },
               *p_instance = 0;
    size_t file_len = g_load_file(p_path, (void *) 0, true);
    char *const p_file_contents = G10_REALLOC(0, (file_len + 1) * sizeof(char));
    const json_value *p_value = 0;

    // Error check
    if ( file_len == 0 ) goto failed_to_load_file;

    // Load the file
    if ( g_load_file(p_path, p_file_contents, true) == 0 ) goto failed_to_load_file;

    // Parse the file into a json value
    if ( parse_json_value(p_file_contents, 0, &p_value) == 0 ) goto failed_to_parse_json_value;

    // Error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto instance_value_is_wrong_type;

    // Parse the json value into an instance
    {

        // Initialized data
        const dict *const p_dict = p_value->object;
        const json_value *p_name_value      = dict_get(p_dict, "name"),
                         *p_server_value    = dict_get(p_dict, "server"),
                         *p_fixed_tick_rate = dict_get(p_dict, "fixed tick rate"),
                         *p_version         = dict_get(p_dict, "version"),
                         *p_vulkan          = dict_get(p_dict, "vulkan"),
                         *p_window          = dict_get(p_dict, "window");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] Consider adding a \"$schema\" property to the instance config\n");

        // Error check
        if ( ! ( p_name_value ) ) goto missing_properties;

        // Error check
        if ( p_name_value->type != JSON_VALUE_STRING ) goto name_property_is_wrong_type;

        // Store the name
        {

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
            _instance.version.major = 1, _instance.version.major = 0, _instance.version.patch = 0;

        // Server
        {
            // Construct a server
            if ( p_server_value == (void *) 0 ) goto no_server;

            //Construct a server
            //if ( server_from_json_value(&_instance.context.p_server, p_server_value) == 0 ) goto no_server;
            no_server:;
        }

        // Set the fixed tick rate
        if ( p_fixed_tick_rate )
        {

            // Error check
            if ( p_fixed_tick_rate->type != JSON_VALUE_INTEGER ) goto fixed_tick_rate_is_wrong_type;

            // Store the fixed tick rate
            _instance.context.fixed_tick_rate = p_fixed_tick_rate->integer;
        }

        // Initialize the window
        {

            // SDL2
            #ifdef BUILD_G10_WITH_SDL2
                g_sdl2_initialize_window(&_instance, p_window);

            // Others? 
            #endif
        }

        // Initialize the graphics API
        {

            // Vulkan
            #ifdef BUILD_G10_WITH_VULKAN
                g_vulkan_initialize(&_instance, p_vulkan);

            // OpenGL
            #elif BUILD_G10_WITH_OPENGL
                g_opengl_initialize(&_instance, p_opengl);

            // Others? 
            #endif
        }
    }

    // Allocate memory for the instance
    p_instance = G10_REALLOC(0, sizeof(g_instance));

    // Error check
    if ( p_instance == (void *) 0 ) goto no_mem;

    // Copy the memory from the stack to the heap
    memcpy(p_instance, &_instance, sizeof(g_instance));

    // Set the instance singleton
    p_active_instance = p_instance;

    // Return an instance pointer to the caller
    *pp_instance = p_instance;

    // Clean up
    free_json_value(p_value);
    G10_REALLOC(p_file_contents, 0);

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

            no_path:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            failed_to_parse_json_value:
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
            free_json_value(p_value);
        error_after_file_allocated:
            G10_REALLOC(p_file_contents, 0);

        return 0;
    }
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

int g_start_server ( g_instance *p_instance )
{

    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Start a thread
    //parallel_thread_start(&p_active_instance->context.p_server->p_web_server_thread, server_listen, g_get_active_instance());

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_instance:
                #ifndef NDEBUG
                    // TODO: 
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
