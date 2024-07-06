// Header file
#include <g10/scene.h>

// Function declarations
/** !
 * Allocate memory for a scene
 * 
 * @param pp_scene result
 * 
 * @return 1 on success, 0 on error
 */
int scene_create ( scene **pp_scene );

// Function definitions
int scene_create ( scene **pp_scene )
{

    // Argument check
    if ( pp_scene == (void *) 0 ) goto no_scene;

    // Initialized data
    scene *p_scene = G10_REALLOC(0, sizeof(scene));

    // Error check
    if ( p_scene == 0 ) goto no_mem;

    // Initialize
    memset(p_scene, 0, sizeof(scene));

    // Return a pointer to the caller
    *pp_scene = p_scene;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"\n", __FUNCTION__);
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

int scene_from_json
(
    scene      **pp_scene,
    json_value  *p_value
)
{

    // Argument check
    if ( pp_scene      ==        (void *) 0 ) goto no_scene;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto scene_value_is_wrong_type;

    // Initialized data
    g_instance *p_instance = g_get_active_instance();
    scene       _scene = { 0 },
               *p_scene = 0;
    size_t entity_quantity = 0,
           camera_quantity = 0,
           light_quantity  = 0;
    char *_camera_names[G10_SCENE_CAMERA_MAX] = { 0 };

    // Parse the json value into a scene
    {

        // Initialized data
        dict *const p_dict = p_value->object;
        const json_value *p_name     = dict_get(p_dict, "name"),
                         *p_entities = dict_get(p_dict, "entities"),
                         *p_cameras  = dict_get(p_dict, "cameras"),
                         *p_lights   = dict_get(p_dict, "lights");

        // Extra check
        if ( dict_get(p_dict, "$schema") == 0 ) log_info("[g10] [scene] Consider adding a \"$schema\" property to the scene\n");

        // Count the quantity of entities
        entity_quantity = array_size(p_entities->list);

        // Count the quantity of cameras
        camera_quantity = dict_keys(p_cameras->object, 0);

        // Construct lookups
        if ( dict_construct(&_scene.data.entities, entity_quantity, 0) == 0 ) goto failed_to_construct_dictionary;
        if ( dict_construct(&_scene.data.cameras , camera_quantity, 0) == 0 ) goto failed_to_construct_dictionary;

        // Set the name
        {
            
            // Error check
            if ( p_name->type != JSON_VALUE_STRING ) goto name_property_is_wrong_type;

            // Initialized data
            size_t len = strlen(p_name->string);

            // Error check
            if ( len == 0   ) goto name_property_is_too_short;
            if ( len  > 255 ) goto name_property_is_too_long;

            // Copy the scene name
            strncpy(_scene._name, p_name->string, 255);

            // Store a null terminator
            _scene._name[len] = '\0';
        }

        // Construct entities
        for (size_t i = 0; i < entity_quantity; i++)
        {

            // Initialized data
            json_value *p_value  = (void *) 0;
            entity     *p_entity = (void *) 0;

            // Store the entity json
            if ( array_index(p_entities->list, i, &p_value) == 0 ) goto failed_to_index_array;

            // Construct the entity
            if ( entity_from_json(&p_entity, p_value) == 0 ) goto failed_to_construct_entity;

            // Add the entity to the scene
            dict_add(_scene.data.entities, p_entity->_name, p_entity);
        }
    
        // Store camera names
        dict_keys(p_cameras->object, &_camera_names);

        // Construct cameras
        for (size_t i = 0; i < camera_quantity; i++)
        {

            // Initialized data
            camera     *p_camera = (void *) 0;
            json_value *p_value  = dict_get(p_cameras->object, _camera_names[i]);;

            // Error check
            if ( p_value == (void *)0 ) goto failed_to_get_dictionary;

            // Construct the camera
            if ( camera_from_json(&p_camera, _camera_names[i], p_value) == 0 ) goto failed_to_construct_camera;

            // Add the camera to the scene
            dict_add(_scene.data.entities, p_camera->_name, p_camera);
        }
    }

    // Allocate memory for the scene
    if ( scene_create(&p_scene) == 0 ) goto failed_to_allocate_scene;

    // Copy the scene from the stack to the heap
    memcpy(p_scene, &_scene, sizeof(scene));

    // Return a pointer to the caller
    *pp_scene = p_scene;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_scene:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Null pointer provided for parameter \"pp_scene\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
            
            no_value:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // array errors
        {
            failed_to_index_array:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Failed to index array in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // dictionary errors
        {
            failed_to_construct_dictionary:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Failed to construct dictionary in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_get_dictionary:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Failed to get dictionary property in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // g10 errors
        {
            failed_to_allocate_scene:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Failed to allocate scene in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            failed_to_construct_entity:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Failed to construct entity in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            failed_to_construct_camera:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Failed to construct camera in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
        }

        // json errors
        {
            scene_value_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [scene] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/scene.json\n");
                #endif

                // Error
                return 0;

            name_property_is_wrong_type:
                #ifndef NDEBUG
                    log_error("[g10] [scene] \"name\" property of \"p_value\" object must be of type [ string ] in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/scene.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_long:
                #ifndef NDEBUG
                    log_error("[g10] [scene] \"name\" property of \"p_value\" object must be less than 255 characters in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/scene.json\n");
                #endif

                // Error
                return 0;
            
            name_property_is_too_short:
                #ifndef NDEBUG
                    log_error("[g10] [scene] \"name\" property of \"p_value\" object must be at least 1 character long in call to function \"%s\"\n", __FUNCTION__);
                    log_info("\tRefer to gschema: https://schema.g10.app/scene.json\n");
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

entity *scene_entity_get ( const scene *const p_scene, const char *const p_name )
{
    
    // TODO: Argument check

    // Done
    return dict_get(p_scene->data.entities, p_name);

    // TODO: Error handling
    {

        // Argument errors
        {

        }
    }
}

int scene_entity_add ( scene *p_scene, entity *p_entity )
{

    // Success
    return 1;
}

int scene_destroy ( scene **pp_scene )
{

    // Success
    return 1;
}
