// Header
#include <g10/ply.h>

// Preprocessor definitions
#define PLY_LOOKUP_MAX 1024
#define PLY_ELEMENT_NAME_MAX 128
#define PLY_PROPERTY_NAME_MAX 16
#define PLY_ELEMENT_PROPERTY_MAX 16 // See: GL_MAX_VERTEX_ATTRIBS

// Enumeration definitions
enum ply_token_e
{

    // Header
    PLY_MAGIC,
    PLY_FORMAT,
    PLY_ASCII,
    PLY_BINARY,
    PLY_VER_1,
    PLY_COMMENT,
    PLY_END_HEADER,

    // Attributes
    PLY_ELEMENT,
    PLY_PROPERTY,
    PLY_LIST,

    // Common
    PLY_VERTEX,
    PLY_EDGE,
    PLY_FACE,

    // Error
    PLY_INVALID
};

enum ply_type_e 
{
    
    // Types
    PLY_S8,
    PLY_U8,
    PLY_S16,
    PLY_U16,
    PLY_S32,
    PLY_U32,
    PLY_F32,
    PLY_F64,
};

// Structure definitions
struct ply_element_s
{
    size_t  quantity;
    char   _name [PLY_ELEMENT_NAME_MAX];
    struct
    {
        char _name[PLY_PROPERTY_NAME_MAX];
        enum ply_type_e type;
    } attributes [PLY_ELEMENT_PROPERTY_MAX]
};

struct ply_file_s
{
    size_t element_quantity;
    struct ply_element_s _elements[];
};

// Data
enum ply_token_e _token_lookup [ PLY_LOOKUP_MAX ] = { 0 };

int ply_parse ( struct ply_file_s **pp_file, FILE *p_f )
{
    
    // Argument check 
    if ( p_f == (void *) 0 ) goto no_file;

    // Initialized data
    char _token[1024] = { 0 };
    bool end = true;
    bool magic_okay = true;
    struct ply_file_s *p_file = G10_REALLOC(0, sizeof(struct ply_file_s));

    // Read the magic number
    magic_okay &= ( 'p'  == getc(p_f) );
    magic_okay &= ( 'l'  == getc(p_f) );
    magic_okay &= ( 'y'  == getc(p_f) );
    magic_okay &= ( '\n' == getc(p_f) );

    // Error check
    if ( magic_okay == false ) goto invalid_magic_number;

    // Parse the header
    while ( end == false )
    {
        
        // Initialized data
        size_t token_len = 0;
        hash64 h = 0;
        enum ply_token_e token_type = PLY_INVALID;

        done:

        // Read a line
        fscanf(p_f, "%s", _token);

        // Compute the length of the token
        token_len = strlen(_token);

        // TODO: Skip blank lines?
        //

        // Hash the token
        h = hash_crc64(_token, token_len);

        // Look up the token 
        token_type = _token_lookup[h % sizeof(_token_lookup)];

        // Look up the token
        switch ( token_type )
        {

            
            // Parse a comment
            case PLY_COMMENT:

                // Parse the comment
                goto parse_comment;

            // Parse an element
            case PLY_ELEMENT:
            
                // Parse the element
                goto parse_element;

            // Parse end header
            case PLY_END_HEADER:

                // Set the end flag
                end = true;

                // Done
                break;

            // Parse the format
            case PLY_FORMAT:
                break;

            // Default
            default:

                // Done
                break;
        }
    }

    // Success
    return 1;

    // Parse a comment
    parse_comment:
    {

        // Print the comment to standard out
        char _comment[1024] = { 0 };

        // Store the comment on the stack
        fscanf(p_f, "%1023[^\n]", &_comment);

        // Write the comment to standard out
        log_info("%s\n", &_comment);

        // Done
        goto done;
    }

    // Parse an element
    parse_element:
    {

        // Initialized data
        size_t token_len = 0;
        hash64 h = 0;
        enum ply_token_e token_type = PLY_INVALID;
        struct ply_element_s _element = { 0 };

        // Read a string
        fscanf(p_f, "%s", &_element._name);

        // Read an integer
        fscanf(p_f, "%d", &_element.quantity);

        // Read a list of properties
        while ( end == false )
        {
            
            // Initialized data
            size_t token_len = 0;
            hash64 h = 0;
            enum ply_token_e token_type = PLY_INVALID;

            // Read a line
            fscanf(p_f, "%s", _token);

            // Compute the length of the token
            token_len = strlen(_token);

            // Hash the token
            h = hash_crc64(_token, token_len);

            // Look up the token 
            token_type = _token_lookup[h % sizeof(_token_lookup)];

            // Look up the token
            switch ( token_type )
            {

                // Parse an element
                case PLY_ELEMENT:

                    // Set the end flag
                    end = true;

                    // Done
                    continue;

                // Parse a property
                case PLY_PROPERTY:

                    // Parse the property
                    goto parse_property;

                // Default
                default:

                    // Done
                    break;
            }
        }

        // Increment the quantity of elements
        p_file->element_quantity++;

        // Grow the allocation
        p_file = G10_REALLOC(p_file, p_file->element_quantity * sizeof(struct ply_element_s) + sizeof(struct ply_file_s));

        // Copy the property
        memcpy(
            &p_file->_elements[p_file->element_quantity - 1],
            &_element,
            sizeof(struct ply_element_s)
        );

        // Done
        goto done;
    }

    // Parse a property
    parse_property:
    {

        // Done
        goto done;
    }

    // Error handling
    {

        // Argument errors
        {
            no_file:

                // Error
                return 0;
        }

        // Parsing errors
        {
            invalid_magic_number:

                // Error
                return 0;
        }
    }  
}

int ply_to_json ( struct ply_file_s *p_ply_file, FILE *p_f )
{

    // Argument check
    if ( p_ply_file == (void *) 0 ) goto no_ply_file;
    if ( p_f        == (void *) 0 ) goto no_file;

    // Formatting 
    fprintf(p_f, "{\n");

    // Print each element
    for (size_t i = 0; i < p_ply_file->element_quantity; i++)
    {
        
        // Formatting 
        fprintf(p_f, "\t\"%s\" :\n{", p_ply_file->_elements->_name);

        // Print each property
        for (size_t j = 0; j < p_ply_file->_elements->quantity; j++)
        {
            
            // Formatting 
            fprintf(p_f, "\t\t\"%s\" :\n{", p_ply_file->_elements->_name);
        }

        // Formatting 
        fprintf(p_f, "}\n");
    }

    // Formatting 
    fprintf(p_f, "}\n");

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_ply_file:
            no_file:
                return 0;
        }   
    }
}
