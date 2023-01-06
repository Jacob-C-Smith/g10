#pragma once
#include <G10/G10.h>

#ifdef BUILD_G10_WITH_DISCORD
void init_discord_integration(void);

int discord_callbacks ( GXInstance_t *instance );
#endif