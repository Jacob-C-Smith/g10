/** !
 * Implements general G10 functionality
 * 
 * @file g10.c
 * @author Jacob Smith
 */

// Headers
#include <g10/g10.h>

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
    g_init_early( );

    // Argument Check
    if ( pp_instance == (void *) 0 ) goto no_instance;
    if ( p_path      == (void *) 0 ) goto no_path;

    // Initialized data
    g_instance  _instance = { 0 },
               *p_instance = 0;
    size_t file_len = g_load_file(p_path, (void *) 0, true);
    char *const p_file_contents = G10_REALLOC(0, (file_len + 1) * sizeof(char));
    const json_value *p_value = 0;

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
        const json_value *p_name_value = 0;

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] Consider adding a \"$schema\" property to the instance config\n");

        // Get the name
        p_name_value = dict_get(p_dict, "name");

        // Check for missing properties
        if ( ! ( p_name_value ) ) goto missing_properties;

        // Error checking
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
                return 0;

            instance_value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] Value must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;
            
            missing_properties:
                #ifndef NDEBUG
                    log_error("[g10] Missing required properties in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] \"name\" property of instance object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/instance.json\n");
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
                return 0;
        }
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