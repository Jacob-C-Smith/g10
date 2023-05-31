#pragma once
#include <G10/G10.h>

#ifdef BUILD_G10_WITH_DISCORD

/**!
 * Initialize discord
*/
void init_discord_integration(void);

/**!
 * This task keeps discord updated
 *
 * @param p_instance : the active instance
 *
 * @return 1 on success, 0 on error
*/
int discord_callbacks ( GXInstance_t *p_instance );
#endif