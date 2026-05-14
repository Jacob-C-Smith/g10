// header
#include <g10.h>
#include <input.h>

// data
static int logger_depth = 0;
static g_instance *p_active_instance;

// key lookup
typedef struct key_lookup_s
{
    bool _active;
} key_lookup;

static key_lookup _key_lookup[SDL_SCANCODE_COUNT];

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
    dict_construct(&p_instance->cache.p_attachment, 64, NULL, (fn_key_accessor *)attachment_key_accessor, NULL);
    dict_construct(&p_instance->cache.p_pipeline, 64, NULL, (fn_key_accessor *)pipeline_key_accessor, NULL);
    dict_construct(&p_instance->cache.p_texture, 64, NULL, (fn_key_accessor *)texture_key_accessor, NULL);
    dict_construct(&p_instance->cache.p_geometry, 64, NULL, (fn_key_accessor *)geometry_key_accessor, NULL);

    // parse the json value into an instance
    {

        // initialized data
        dict *const p_dict = p_value->object;
        json_value *p_name_value      = NULL,
                   *p_version         = NULL,
                   *p_schedule        = NULL,
                   *p_renderer        = NULL,
                   *p_fixed_tick_rate = NULL,
                   *p_vulkan          = NULL,
                   *p_scene           = NULL,
                   *p_input           = NULL,
                   *p_window          = NULL;
    
        dict_get(p_dict, "name"           , (void **)&p_name_value);
        dict_get(p_dict, "version"        , (void **)&p_version);
        dict_get(p_dict, "schedule"       , (void **)&p_schedule);
        dict_get(p_dict, "renderer"       , (void **)&p_renderer);
        dict_get(p_dict, "fixed tick rate", (void **)&p_fixed_tick_rate);
        dict_get(p_dict, "vulkan"         , (void **)&p_vulkan);
        dict_get(p_dict, "scene"          , (void **)&p_scene);
        dict_get(p_dict, "input"          , (void **)&p_input);
        dict_get(p_dict, "window"         , (void **)&p_window);

                
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
                json_value *p_major = NULL,
                           *p_minor = NULL,
                           *p_patch = NULL;
                
                dict_get(p_version_dict, "major", (void **)&p_major);
                dict_get(p_version_dict, "minor", (void **)&p_minor);
                dict_get(p_version_dict, "patch", (void **)&p_patch);
                
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
            extern int g_sdl3_renderer_from_json ( renderer **pp_renderer, const json_value *p_value );

            // create an sdl3 renderer
            g_sdl3_renderer_from_json(&p_instance->context.p_renderer, p_renderer);
        #else

            // others? 
        #endif

        // input
        if ( p_input )
        {
            
            // create an sdl3 renderer
            input_from_json(&p_instance->context.p_input, p_input);
        }

        // load a scene
        scene_from_json(&p_instance->context.p_scene, p_scene);
    }

    srand(time(NULL));

    // return a pointer to the caller
    *pp_instance = p_instance;

    // logs
    log_info("[g10] init\n");

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for \"pp_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_path:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for parameter \"p_path\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // g10 errors
        {
            failed_to_load_initial_scene:
                #ifndef NDEBUG
                    log_error("[g10] Failed to construct scene in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            failed_to_json_parse_value:
                #ifndef NDEUBG
                    log_error("[g10] Failed to parse file \"%s\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // error
                goto error_after_file_allocated;

            instance_value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] Value must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;
                
            missing_name_property:
                #ifndef NDEUBG
                    log_error("[g10] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", p_path, __FUNCTION__);
                #endif

                // error
                goto error_after_json_parsed;

            fixed_tick_rate_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] Value must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;

            missing_version_properties:
                #ifndef NDEBUG
                    log_error("[g10] \"version\" property is missing required properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;

            version_major_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"major\" property of \"version\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;

            version_minor_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"minor\" property of \"version\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;

            version_patch_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"patch\" property of \"version\" property must be of type [ integer ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;

            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;
            
            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;

            wrong_version_type:
                #ifndef NDEBUG
                    log_error("[g10] \"version\" property of instance object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // error
                goto error_after_json_parsed;
        }

        // parallel errors
        {
            failed_to_load_schedule_from_json_value:
                #ifndef NDEBUG
                    log_error("[g10] Failed to construct schedule in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Memory allocator returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
            
            failed_to_load_file:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to load file in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                goto error_after_file_allocated;
        }

        // clean up
        error_after_json_parsed:
            json_value_free(p_value, 0);
        error_after_file_allocated:
            p_file_contents = default_allocator(p_file_contents, 0);

        // error
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

    // argument check
    if ( p_instance == (void *) 0 ) goto no_instance;
    
    // initialized data
    input *p_input = p_instance->context.p_input;
    int num_keys = 0;
    const bool* keyboard_state = SDL_GetKeyboardState(&num_keys);

    if ( p_input )
    {
        for (size_t i = 0; i < p_input->bind_quantity; i++)
        {
            
            // initialize data
            input_bind *p_bind = p_input->_p_binds[i];

            // clear the input
            p_bind->value = 0.0;
        }
    }
    
    // process each event
    while (SDL_PollEvent(&p_instance->window.sdl3.event))
    {
        
        // switch on the event type
        switch (p_instance->window.sdl3.event.type)
        {

            // process should exit
            case SDL_EVENT_QUIT:
            {

                // clear the running flag
                p_instance->running = false;

                // done
                break;
            }

            case SDL_EVENT_WINDOW_RESIZED:
            {

                // // external functions
                // extern int g_window_resize ( g_instance *p_instance, u32 width, u32 height );
                
                // // resize the window
                // g_window_resize
                // (
                //     p_instance,
                //     (u32) p_instance->window.sdl3.event.window.data1,
                //     (u32) p_instance->window.sdl3.event.window.data2
                // );

                // done
                break;
            }

            // mouse moves
            case SDL_EVENT_MOUSE_BUTTON_DOWN:break;
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                
                // done
                break;
            }
            case SDL_EVENT_MOUSE_MOTION:
            {

                // initialized data
                int xrel = p_instance->window.sdl3.event.motion.xrel,
                    yrel = p_instance->window.sdl3.event.motion.yrel;
                camera *p_camera = p_instance->context.p_scene->p_active_camera;
                
                // vec3 vel = p_camera->view.target;
                // vec3 svel = { 0 };
                
                // float forward_backward = 0.02f * ( yrel );
                // float left_right = 0.02f * ( xrel );
                
                // vec3_cross_product(&svel, vel, p_camera->view.up);
                
                // vec3_mul_scalar(&vel, vel, forward_backward);
                // vec3_mul_scalar(&svel, svel, left_right);

                // vec3_add_vec3(&p_camera->view.location, p_camera->view.location, vel);
                // vec3_add_vec3(&p_camera->view.location, p_camera->view.location, svel);

                // printf("[ %4.2f, %4.2f ]\n", p_instance->window.sdl3.event.motion.x, p_instance->window.sdl3.event.motion.y);
                
                // done
                break;
            }
        }
    }
    
    // update the keyboard state
    for (size_t i = 0; i < ( ( num_keys < SDL_SCANCODE_COUNT ) ? num_keys : SDL_SCANCODE_COUNT ); i++) 
        _key_lookup[i]._active = keyboard_state[i];

    // update the binds
    if ( p_input )
    {
        for (size_t i = 0; i < p_input->bind_quantity; i++)
        {
            
            // initialize data
            input_bind *p_bind = p_input->_p_binds[i];

            // iterate through each scancode
            for (size_t j = 0; j < p_bind->scancode_quantity; j++)
            {

                // initialized data
                size_t p_scancode = (size_t) p_bind->_scancodes[j];

                // update the scancode 
                if ( _key_lookup[p_scancode]._active )
                {

                    // set the input
                    p_bind->value = 1.0;

                    // done with this bind
                    break;
                }
            }
        }
    }

    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [sdl3] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int program_pipeline ( const char _name[], fn_pipeline_bind_once *pfn_once, fn_pipeline_bind_each *pfn_each, fn_pipeline_draw *pfn_draw )
{

    // initialized data
    g_instance *p_instance = g_active_instance();
    pipeline *p_pipeline = NULL;
    
    dict_get(p_instance->cache.p_pipeline, _name, (void **)&p_pipeline);

    // error check
    if ( NULL == p_pipeline ) goto failed_to_find_pipeline;

    // program the pipeline
    pipeline_set_bind_once(p_pipeline, pfn_once),
    pipeline_set_bind_each(p_pipeline, pfn_each),
    pipeline_set_draw(p_pipeline, pfn_draw);

    // success
    return 1;

    // error check
    {

        // g10 errors
        {
            failed_to_find_pipeline:
                #ifndef NDEBUG
                    log_error("[g10] Failed to find pipeline \"%s\" in call to function \"%s\"\n", _name, __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // argument checking 
    if ( path == 0 ) goto no_path;

    // initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // check if file is valid
    if ( f == NULL ) goto invalid_file;

    // find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // the file is no longer needed
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

        // file errors
        {
            invalid_file:

                // error
                return 0;
        }
    }
}

int instance_info ( g_instance *p_instance )
{

    // argument check
    if ( NULL == p_instance ) goto no_instance;

    // logs
    logger_pad(), log_info("Instance @%p\n", p_instance),
    
    logger_push(),
    logger_pad(), printf("name    - %s\n", p_instance->_name),
    logger_pad(), printf("running - %s\n", p_instance->running ? "true" : "false"),
    logger_pad(), printf("version - %u.%u.%u\n", 
        p_instance->version.major,
        p_instance->version.minor, 
        p_instance->version.patch),
    logger_pad(), printf("window  - <%u, %u> \"%s\"\n", 
        p_instance->window.width, 
        p_instance->window.height, 
        p_instance->window.title
    ),
    
    logger_pad(), printf("input: \n"),
    logger_push(),
    input_info(p_instance->context.p_input),
    logger_pop(),

    logger_pad(), printf("renderer: \n"),
    logger_push(),
    renderer_info(p_instance->context.p_renderer),
    logger_pop(),

    logger_pad(), printf("scene: \n"),
    logger_push(),
    scene_info(p_instance->context.p_scene),
    logger_pop(),

    logger_pop();
    
    // success
    return 1;

    // error handling
    {

        // argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 1;
        }
    }
}

void logger_push(){

    logger_depth++;

    return;
}

void logger_pop(){

    logger_depth--;

    return;
}

void logger_pad(){
    for (size_t i = 0; i < logger_depth; i++)
    {
        printf("  ");
    }
}
