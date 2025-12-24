// header
#include <g10.h>

// data
static g_instance *p_active_instance;

int g_init 
(
    g_instance **pp_instance,

    const char *p_path
)
{

    // argument check
    if ( pp_instance == (void *) 0 ) goto no_instance;
    if ( p_path      == (void *) 0 ) goto no_path;

    // initialized data
    size_t file_len = load_file(p_path, (void *) 0, true), debug_message_max = 64;
    json_value *p_value = 0;
    g_instance *p_instance = 0;
    char *p_file_contents = default_allocator(0, (file_len + 1) * sizeof(char));

    // error check
    if ( file_len == 0 ) goto failed_to_load_file;

    // load the file
    if ( load_file(p_path, p_file_contents, true) == 0 ) goto failed_to_load_file;

    // parse the file into a json value
    if ( json_value_parse(p_file_contents, 0, (json_value **const)&p_value) == 0 ) goto failed_to_json_parse_value;

    // error check
    if ( p_value->type != JSON_VALUE_OBJECT ) goto instance_value_is_wrong_type;

    // allocate memory for the instance
    p_instance = default_allocator(0, sizeof(g_instance));

    // error check
    if ( p_instance == (void *) 0 ) goto no_mem;

    // set the active instance
    p_active_instance = p_instance;

    // construct caches
    dict_construct(&p_instance->cache.p_attachment, 64, NULL);
    dict_construct(&p_instance->cache.p_pipeline, 64, NULL);
    dict_construct(&p_instance->cache.p_texture, 64, NULL);
    dict_construct(&p_instance->cache.p_geometry, 64, NULL);

    // parse the json value into an instance
    {

        // initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name_value      = dict_get(p_dict, "name"),
                         *p_version         = dict_get(p_dict, "version"),
                         *p_schedule        = dict_get(p_dict, "schedule"),
                         *p_renderer        = dict_get(p_dict, "renderer"),
                         *p_fixed_tick_rate = dict_get(p_dict, "fixed tick rate"),
                         *p_input           = dict_get(p_dict, "input"),
                         *p_vulkan          = dict_get(p_dict, "vulkan"),
                         *p_scene           = dict_get(p_dict, "scene"),
                         *p_debug           = dict_get(p_dict, "debug"),
                         *p_window          = dict_get(p_dict, "window");
    
                
        // store the name
        if ( p_name_value )
        {

            // error check
            if ( p_name_value->type != JSON_VALUE_STRING ) goto name_property_is_wrong_type;

            // initialized data
            size_t len = strlen(p_name_value->string);

            // error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // copy the instance name
            strncpy(p_instance->_name, p_name_value->string, 255);

            // store a null terminator
            p_instance->_name[len] = '\0';
        }

        // store the version
        if ( p_version )
        {

            // error check
            if ( p_version->type != JSON_VALUE_OBJECT ) goto wrong_version_type;

            // parse the json value into a version
            {

                // initialized data
                dict *p_version_dict = p_version->object;
                const json_value *const p_major = dict_get(p_version_dict, "major"),
                                 *const p_minor = dict_get(p_version_dict, "minor"),
                                 *const p_patch = dict_get(p_version_dict, "patch");
                
                // error check
                if ( ! ( p_major && p_minor && p_patch ) ) goto missing_version_properties;

                // extra checking
                if ( p_major->type != JSON_VALUE_INTEGER ) goto version_major_wrong_type;
                if ( p_minor->type != JSON_VALUE_INTEGER ) goto version_minor_wrong_type;
                if ( p_patch->type != JSON_VALUE_INTEGER ) goto version_patch_wrong_type;
                
                // store each version number
                p_instance->version.major = (u16) p_major->integer,
                p_instance->version.minor = (u16) p_minor->integer,
                p_instance->version.patch = (u16) p_patch->integer;
            }
        }

        // default to 1.0.0
        else
            p_instance->version.major = (u16)1,
            p_instance->version.major = (u16)0,
            p_instance->version.patch = (u16)0;

        // initialize window system integration
        #ifdef G10_BUILD_WITH_SDL3

            // external functions
            extern int g_sdl3_init ( g_instance *p_instance );
            extern int g_sdl3_window_from_json ( g_instance *p_instance, const json_value *p_value );

            // initialize sdl3
            g_sdl3_init(p_instance);

            // create an sdl3 window
            g_sdl3_window_from_json(p_instance, p_window);
        #else

            // others? 
        #endif

        // initialize the renderer
        #ifdef G10_BUILD_WITH_SDL3

            // external functions
            extern int g_sdl3_renderer_from_json ( renderer *pp_renderer, const json_value *p_value );

            // create an sdl3 renderer
            g_sdl3_renderer_from_json(&p_instance->context.p_renderer, p_renderer);
        #else

            // others? 
        #endif

        // load a scene
        scene_from_json(&p_instance->context.p_scene, p_scene);
    }

    // return a pointer to the caller
    *pp_instance = p_instance;

    // success
    return 1;

    // error handling
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
            json_value_free(p_value, 0);
        error_after_file_allocated:
            p_file_contents = default_allocator(p_file_contents, 0);

        // Error
        return 0;
    }
}

g_instance *g_active_instance( void )
{

    // done
    return p_active_instance;
}

int poll_input ( g_instance *p_instance ) 
{

    // poll events
    while ( SDL_PollEvent(&p_instance->window.sdl3.event) )
    {
        if ( p_instance->window.sdl3.event.type == SDL_EVENT_QUIT )
            p_instance->running = false;
        if ( p_instance->window.sdl3.event.type == SDL_EVENT_KEY_DOWN )
            if ( p_instance->window.sdl3.event.key.key == SDLK_P );
                // TODO
                
    }

    // success
    return 1;
}

int program_pipeline ( const char _name[], fn_pipeline_bind_once *pfn_once, fn_pipeline_bind_each *pfn_each )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    pipeline *p_pipeline = dict_get(p_instance->cache.p_pipeline, _name);

    pipeline_set_bind_once(p_pipeline, (fn_pipeline_bind_once *)pfn_once),
    pipeline_set_bind_each(p_pipeline, (fn_pipeline_bind_each *)pfn_each);

    // success
    return 1;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // success
    return ret;

    // error handling
    {

        // argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("Null pointer provided for parameter \"path\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // File errors
        {
            invalid_file:

                // error
                return 0;
        }
    }
}
