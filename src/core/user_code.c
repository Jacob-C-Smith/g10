// header
#include <user_code.h>


int user_code_set_callback ( fn_user_code *pfn_user_code )
{

    // initialized data
    g_instance *p_instance = g_active_instance();

    // store the game logic function pointer
    p_instance->context.pfn_user_code = pfn_user_code;

    // success
    return 1;
}

int user_code ( g_instance *p_instance )
{

    // call the game logic
    p_instance->context.pfn_user_code(p_instance);

    // success
    return 1;
}