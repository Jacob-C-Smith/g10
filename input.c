/** !
 * Input implementation
 * 
 * @file input.c
 * 
 * @author Jacob Smith
 */

// Header
#include <g10/input.h>

// Function definitions
int input_create ( input **pp_input ) 
{

    // Argument check
    if ( pp_input == (void *) 0 ) goto no_input;

    // Initialized data
    input *p_input = G10_REALLOC(0, sizeof(input));

    // Error check
    if ( p_input == (void *) 0 ) goto no_mem;

    // Initialize
    memset(p_input, 0, sizeof(input));

    // Return a pointer to the caller
    *pp_input = p_input;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"pp_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int input_bind_from_json ( input_bind **pp_input_bind, const char *const p_name, const json_value *const p_value )
{

    // Argument check
    if ( pp_input_bind ==       (void *) 0 ) goto no_input_bind;
    if ( p_name        ==       (void *) 0 ) goto no_name;
    if ( p_value       ==       (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_ARRAY ) goto wrong_type;

    // Initialized data
    input_bind *p_input_bind = (void *) 0;
    size_t input_bind_keys = array_size(p_value->list);
    size_t i = 0;
    const char *p_key = (void *) 0;

    p_input_bind = G10_REALLOC(0, sizeof(input_bind) + input_bind_keys * sizeof(char[16]));

    // Error check
    if ( p_input_bind == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_input_bind, 0, sizeof(input_bind));

    // Type check
    //

    // Store the quantity of scancodes
    p_input_bind->scancode_quantity = input_bind_keys;

    // Iterate through each key
    for (i = 0; i < input_bind_keys; i++)
    {

        // Initialized data
        json_value *p_key_value = (void *) 0;

        // Store the i'th element
        array_index(p_value->list, i, &p_key_value);

        // Type check
        if ( p_key_value->type != JSON_VALUE_STRING ) goto wrong_key_type;

        // Store the key string
        p_key = p_key_value->string;

        // Lookup the key 
        #ifdef G10_BUILD_WITH_SDL2
            
            // External data
            extern dict *p_sdl2_key_lookup;
            extern dict *p_sdl2_key_scancode;

            // Lookup the key
            void *p_key_scancode = dict_get(p_sdl2_key_scancode, p_key);

            // Error check
            if ( p_key_scancode == (void *) 0 ) goto bind_not_found;

            // Store the scancode
            p_input_bind->_scancodes[i] = p_key_scancode;
        #elif
        #endif
    }

    // Store the name of the bind
    {

        // Initialized data
        size_t len = strlen(p_name);

        // TODO: Error check
        //

        // Copy the name of the bind
        strncpy(p_input_bind->_name, p_name, len);
    }

    // Return a pointer to the caller
    *pp_input_bind = p_input_bind;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input_bind:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"pp_input_bind\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] Parameter \"p_value\" must be of type [ array ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
        
        // g10 errors
        {
            bind_not_found:
                #ifndef NDEBUG
                    log_error("[g10] [input] Failed to locate key \"%s\" in call to function \"%s\"\n", p_key, __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            wrong_key_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] Element %d of \"%s\" bind of property \"binds\" of parameter \"p_value\" must be of type [ string ] in call to function \"%s\"\n", i, p_name, __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/transform.json\n");
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

int input_from_json ( input **pp_input, const json_value *const p_value )
{

    // Argument check
    if ( pp_input == (void *) 0 ) goto no_input;
    if ( p_value  == (void *) 0 ) goto no_value;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    input *p_input = (void *) 0;

    // Allocate memory on the heap
    if ( input_create(&p_input) == 0 ) goto failed_to_allocate_input;

    // Parse the json value into an input
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name_value              = dict_get(p_dict, "name"),
                         *p_mouse_sensitivity_value = dict_get(p_dict, "mouse sensitivity"),
                         *p_binds_value             = dict_get(p_dict, "binds");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) circular_buffer_push(p_instance->debug, "[g10] [input] Consider adding a \"$schema\" property to the input");

        // Error check
        if ( p_name_value  == (void *) 0 ) goto no_name_property;
        if ( p_binds_value == (void *) 0 ) goto no_binds_property;

        // Type check
        if ( p_name_value->type              != JSON_VALUE_STRING ) goto name_property_is_wrong_type;
        if ( p_mouse_sensitivity_value->type != JSON_VALUE_NUMBER ) goto mouse_sensitivity_property_is_wrong_type;
        if ( p_binds_value->type             != JSON_VALUE_OBJECT ) goto binds_property_is_wrong_type;

        // Store the name
        {

            // Initialized data
            size_t len = strlen(p_name_value->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the instance name
            strncpy(p_input->_name, p_name_value->string, 63);

            // Store a null terminator
            p_input->_name[len] = '\0';
        }

        // Store the mouse sensitivity
        p_input->mouse_sensitivity = p_mouse_sensitivity_value->number;

        // Store the binds
        {

            // Initialized data
            size_t   bind_quantity = dict_keys(p_binds_value->object, 0);
            char   **bind_keys = G10_REALLOC(0, bind_quantity * sizeof(char *));

            // Error check
            if ( bind_keys == (void *) 0 ) goto no_mem;

            // Get the keys
            dict_keys(p_binds_value->object, bind_keys);

            // Grow the allocation
            p_input = G10_REALLOC(p_input, sizeof(input) + bind_quantity * sizeof(input_bind *));

            // Store the quantity of binds
            p_input->bind_quantity = bind_quantity;

            // Construct a lookup for binds
            if ( dict_construct(&p_input->p_binds, bind_quantity, 0) == 0 ) goto failed_to_construct_dictionary;

            // Construct each bind
            for (size_t i = 0; i < bind_quantity; i++)
            {
                
                // Initialized data
                input_bind *p_input_bind = (void *) 0;
                json_value *p_bind_value = dict_get(p_binds_value->object, bind_keys[i]);

                // Construct the input bind
                if ( input_bind_from_json(&p_input_bind, bind_keys[i], p_bind_value) == 0 ) goto failed_to_construct_bind;

                // Store the input bind
                p_input->_p_binds[i] = p_input_bind;

                // Add the input bind to the lookup
                dict_add(p_input->p_binds, p_input->_p_binds[i]->_name, p_input->_p_binds[i]);
            }

            // Free the names
            bind_keys = G10_REALLOC(bind_keys, 0);
        }
    }

    // Return a pointer to the caller 
    *pp_input = p_input;

    // Success
    return 1;

    failed_to_construct_bind:
    failed_to_construct_dictionary:

        // Error
        return 0;

    // Error handling
    {

        // Argument errors
        {
            no_input:
            no_value:

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_allocate_input:
                #ifndef NDEBUG
                    log_error("[g10] [input] Failed to allocate input in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // json errors
        {
            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"name\" property of input object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;

            mouse_sensitivity_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"mouse sensitivity\" property of input object must be of type [ number ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;

            binds_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"binds\" property of input object must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;

            no_name_property:
                #ifndef NDEBUG
                    log_error("[g10] [input] Parameter \"p_value\" is missing required property \"name\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;

            no_binds_property:
                #ifndef NDEBUG
                    log_error("[g10] [input] Parameter \"p_value\" is missing required property \"binds\" in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;

            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"name\" property of input object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;

            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [input] \"name\" property of input object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/input.json\n");
                #endif

                // Error
                return 0;
        }
        
        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int input_scancode_to_name ( const char **pp_result, void *scancode )
{

    // Argument check
    if ( pp_result == (void *) 0 ) goto no_result;

    // WSI dependant implementation
    #ifdef G10_BUILD_WITH_SDL2

        // External functions
        extern int g_sdl2_scancode_to_name ( void **pp_result, void *scancode );

        // Get the scancode
        g_sdl2_scancode_to_name(pp_result, scancode);
    #endif

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_result:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for paramter \"pp_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int input_info ( const input *const p_input )
{

    // Argument check
    if ( p_input == (void *) 0 ) goto no_input;

    // Print the input
    printf("Input:\n");
    printf(" - name : %s\n", p_input->_name);
    printf(" - mouse sensitivity : %1.1f\n", p_input->mouse_sensitivity);
    printf(" - binds : \n");

    for (size_t i = 0; i < p_input->bind_quantity; i++)
    {
        
        // Initialized data
        input_bind *p_input_bind = p_input->_p_binds[i];

        // Print the bind
        printf("     \"%s\" : [ ", p_input_bind->_name);

        for (size_t j = 0; j < p_input_bind->scancode_quantity; j++)
        {

            // Initialized data
            char *p_name = (void *) 0 ;

            // Get the scancode
            input_scancode_to_name(&p_name, p_input_bind->_scancodes[j]);

            // Print the scancode
            printf("\"%s\"%c ", p_name, (j == p_input_bind->scancode_quantity - 1) ? '\0' : ',');
        }

        printf("]\n");
    }
    
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_input:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_input\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int input_poll ( g_instance *p_instance )
{
 
    // Argument check
    if ( p_instance == (void *) 0 ) goto no_instance;

    // Window system integration depenant implementation
    #ifdef BUILD_G10_WITH_SDL2
        
        // External functions
        extern int g_sdl2_window_poll ( g_instance *p_instance );

        // Poll for input
        g_sdl2_window_poll(p_instance);
    #else

        // Others? 
    #endif

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_instance:
                #ifndef NDEBUG
                    log_error("[g10] [input] Null pointer provided for parameter \"p_instance\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

float input_bind_value ( const char *const p_bind_name )
{

    // Argument check
    if ( p_bind_name == (void *) 0 ) goto no_bind_name; 

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    input_bind *p_input_bind = dict_get(p_instance->context.p_input->p_binds, p_bind_name);

    // Success
    return p_input_bind->value;

    // Error handling
    {

        // Argument errors
        {
            no_bind_name:
                #ifndef NDEBUG
                    log_error("[input] [bind] Null pointer provided for parameter \"p_bind_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

void input_mouse_lock_toggle ( void )
{

    // WSI dependant implementation
    #ifdef BUILD_G10_WITH_SDL2

        // External functions
        extern void g_sdl2_mouse_lock_toggle ( void );

        // Toggle mouse locking
        g_sdl2_mouse_lock_toggle();
    #elif

    #endif

    // Done
    return;
}
