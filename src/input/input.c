/** !
 * input implementation
 * 
 * @file input.c
 * 
 * @author Jacob Smith
 */

// header
#include <input.h>

// key accessor
const char *input_bind_key_accessor ( const void *const p_value )
{
    input_bind *p_input_bind = (void *)p_value;

    return p_input_bind->_name;
}

// function definitions
int input_create ( input **pp_input ) 
{

    // argument check
    if ( pp_input == (void *) 0 ) goto no_input;

    // initialized data
    input *p_input = default_allocator(0, sizeof(input));

    // error check
    if ( p_input == (void *) 0 ) goto no_mem;

    // initialize
    memset(p_input, 0, sizeof(input));

    // return a pointer to the caller
    *pp_input = p_input;

    // success
    return 1;

    // error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int input_bind_from_json ( input_bind **pp_input_bind, const char *const p_name, const json_value *const p_value )
{

    // argument check
    if ( pp_input_bind ==       (void *) 0 ) goto no_input_bind;
    if ( p_name        ==       (void *) 0 ) goto no_name;
    if ( p_value       ==       (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_ARRAY ) goto wrong_type;

    // initialized data
    input_bind *p_input_bind = (void *) 0;
    size_t input_bind_keys = array_size(p_value->list);
    size_t i = 0;
    const char *p_key = (void *) 0;

    p_input_bind = default_allocator(0, sizeof(input_bind) + input_bind_keys * sizeof(char[16]));

    // error check
    if ( p_input_bind == (void *) 0 ) goto no_mem;

    // initialize memory
    memset(p_input_bind, 0, sizeof(input_bind));

    // type check
    //

    // store the quantity of scancodes
    p_input_bind->scancode_quantity = input_bind_keys;

    // iterate through each key
    for (i = 0; i < input_bind_keys; i++)
    {

        // initialized data
        json_value *p_key_value = (void *) 0;

        // store the i'th element
        array_index(p_value->list, (signed int)i, (void **const)&p_key_value);

        // type check
        if ( p_key_value->type != JSON_VALUE_STRING ) goto wrong_key_type;

        // store the key string
        p_key = p_key_value->string;

        // lookup the key 
        #ifdef G10_BUILD_WITH_SDL3
            
            // external data
            extern dict *p_sdl2_key_lookup;
            extern dict *p_sdl2_key_scancode;

            // lookup the key
            void *p_key_scancode = NULL;
            dict_get(p_sdl2_key_scancode, p_key, (void **)&p_key_scancode);

            // error check
            if ( p_key_scancode == (void *) 0 ) goto bind_not_found;

            // Extract the scancode
            struct {
                char _name[64];
                SDL_Scancode _scancode;
                bool _active;
            } *p_entry = p_key_scancode;

            // store the scancode
            p_input_bind->_scancodes[i] = (void *) (size_t) p_entry->_scancode;
        #else
        #endif
    }

    // store the name of the bind
    {

        // initialized data
        size_t len = strlen(p_name);

        // TODO: Error check
        //

        // copy the name of the bind
        strncpy(p_input_bind->_name, p_name, len);
    }

    // return a pointer to the caller
    *pp_input_bind = p_input_bind;

    // success
    return 1;

    // error handling
    {

        // Argument errors
        {
            no_input_bind:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"pp_input_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] Parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
        
        // g10 errors
        {
            bind_not_found:
                #ifndef NDEBUG
                    log_error("[g10] [input] Failed to locate key \"%s\" in call to function \"%s\"\n", p_key, __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            wrong_key_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] Element %d of \"%s\" bind of property \"binds\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", i, p_name, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
                #endif

                // error
                return 0;
        }

        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int input_from_json ( input **pp_input, const json_value *const p_value )
{

    // argument check
    if ( pp_input == (void *) 0 ) goto no_input;
    if ( p_value  == (void *) 0 ) goto no_value;

    // initialized data
    g_instance *p_instance = g_active_instance();
    input *p_input = (void *) 0;

    // Allocate memory on the heap
    if ( input_create(&p_input) == 0 ) goto failed_to_allocate_input;

    // parse the json value into an input
    {

        // initialized data
        dict *const p_dict = p_value->object;
        json_value *p_name_value              = NULL,
                   *p_mouse_sensitivity_value = NULL,
                   *p_binds_value             = NULL;

        dict_get(p_dict, "name"             , (void **)&p_name_value);
        dict_get(p_dict, "mouse sensitivity", (void **)&p_mouse_sensitivity_value);
        dict_get(p_dict, "binds"            , (void **)&p_binds_value);

        // error check
        if ( p_name_value  == (void *) 0 ) goto no_name_property;
        if ( p_binds_value == (void *) 0 ) goto no_binds_property;

        // type check
        if ( p_name_value->type              != JSON_VALUE_STRING ) goto name_property_is_wrong_type;
        if ( p_mouse_sensitivity_value->type != JSON_VALUE_NUMBER ) goto mouse_sensitivity_property_is_wrong_type;
        if ( p_binds_value->type             != JSON_VALUE_OBJECT ) goto binds_property_is_wrong_type;

        // store the name
        {

            // initialized data
            size_t len = strlen(p_name_value->string);

            // error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // copy the instance name
            strncpy(p_input->_name, p_name_value->string, 63);

            // store a null terminator
            p_input->_name[len] = '\0';
        }

        // store the mouse sensitivity
        p_input->mouse_sensitivity = (float) p_mouse_sensitivity_value->number;

        // store the binds
        {

            // initialized data
            size_t      bind_quantity = 0;
            json_value **pp_json_values = NULL;

            dict_size(p_binds_value->object, &bind_quantity);

            // error check
            if ( bind_quantity == 0 ) goto no_mem;

            // Allocate memory for the json value pointers
            pp_json_values = default_allocator(0, bind_quantity * sizeof(json_value *));

            // get the values
            dict_values(p_binds_value->object, (void **)pp_json_values, bind_quantity);

            // grow the allocation
            p_input = default_allocator(p_input, sizeof(input) + bind_quantity * sizeof(input_bind *));

            // store the quantity of binds
            p_input->bind_quantity = bind_quantity;

            // construct a lookup for binds
            if ( dict_construct(&p_input->p_binds, bind_quantity, NULL, (fn_key_accessor *)input_bind_key_accessor, NULL) == 0 ) goto failed_to_construct_dictionary;

            // construct each bind
            for (size_t i = 0; i < bind_quantity; i++)
            {
                
                // initialized data
                input_bind       *p_input_bind = (void *) 0;
                json_value       *p_bind_json_value = pp_json_values[i];

                // construct the input bind
                if ( input_bind_from_json(&p_input_bind, p_bind_json_value->p_key, p_bind_json_value) == 0 ) goto failed_to_construct_bind;

                // store the input bind
                p_input->_p_binds[i] = p_input_bind;

                // Add the input bind to the lookup
                dict_add(p_input->p_binds, p_input->_p_binds[i]);
            }

            // free the json values
            default_allocator(pp_json_values, 0);
        }
    }

    // return a pointer to the caller 
    *pp_input = p_input;

    // success
    return 1;

    failed_to_construct_bind:
    failed_to_construct_dictionary:

        // error
        return 0;

    // error handling
    {

        // Argument errors
        {
            no_input:
            no_value:

                // error
                return 0;
        }

        // g10 errors
        {
            failed_to_allocate_input:
                #ifndef NDEBUG
                    log_error("[g10] [input] Failed to allocate input in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }

        // json errors
        {
            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"name\" property of input object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;

            mouse_sensitivity_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"mouse sensitivity\" property of input object must be of type [ number ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;

            binds_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"binds\" property of input object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;

            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [input] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;

            no_binds_property:
                #ifndef NDEBUG
                    log_error("[g10] [input] Parameter \"p_value\" is missing required property \"binds\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;

            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"name\" property of input object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;

            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"name\" property of input object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // error
                return 0;
        }
        
        // standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int input_scancode_to_name ( const char **pp_result, void *scancode )
{

    // argument check
    if ( pp_result == (void *) 0 ) goto no_result;

    // wSI dependant implementation
    #ifdef G10_BUILD_WITH_SDL2

        // external functions
        extern int g_sdl2_scancode_to_name ( void **pp_result, void *scancode );

        // get the scancode
        g_sdl2_scancode_to_name((void **) pp_result, scancode);
    #endif

    // success
    return 1;

    // error handling
    {

        // Argument errors
        {
            no_result:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for paramter \"pp_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

int input_info ( const input *const p_input )
{

    // argument check
    if ( p_input == (void *) 0 ) goto no_input;

    // print the input
    logger_pad(), log_info("Input @%p\n", p_input),

    logger_push(),
    logger_pad(), printf("name              - %s\n", p_input->_name),
    logger_pad(), printf("mouse sensitivity - %1.1f\n", p_input->mouse_sensitivity),
    logger_pad(), printf("binds: \n"),
    logger_push();
 
    for (size_t i = 0; i < p_input->bind_quantity; i++)
    {
        
        // initialized data
        input_bind *p_input_bind = p_input->_p_binds[i];

        // print the bind
        logger_pad(), printf("%s : [ ", p_input_bind->_name);

        for (size_t j = 0; j < p_input_bind->scancode_quantity; j++)
        {

            // initialized data
            const char *p_name = (void *) 0 ;

            // get the scancode
            input_scancode_to_name(&p_name, p_input_bind->_scancodes[j]);

            // print the scancode
            printf("\"%s\"%c ", p_name, (j == p_input_bind->scancode_quantity - 1) ? '\0' : ',');
        }

        printf("]\n");
    }

    logger_pop(),
    logger_pop();

    // success
    return 1;

    // error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // error
                return 0;
        }
    }
}

float input_bind_value ( const char *const p_bind_name )
{

    // argument check
    if ( p_bind_name == (void *) 0 ) goto no_bind_name; 

    // initialized data
    g_instance *p_instance = g_active_instance();
    input_bind *p_input_bind = NULL;

    // error check
    if ( p_instance->context.p_input == (void *) 0 ) return 0.0f;

    dict_get(p_instance->context.p_input->p_binds, p_bind_name, (void **)&p_input_bind);

    // error check
    if ( p_input_bind == (void *) 0 ) return 0.0f;

    // success
    return p_input_bind->value;

    // error handling
    {

        // Argument errors
        {
            no_bind_name:
                #ifndef NDEBUG
                    log_error("[input] [bind] Null pointer provided for parameter \"p_bind_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // error
                return 0;
        }
    }
}
