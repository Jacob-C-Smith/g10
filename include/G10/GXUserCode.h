#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <G10/G10.h>

DLLEXPORT int user_code              ( GXInstance_t *instance );
DLLEXPORT int add_user_code_callback ( GXInstance_t *instance, int (*user_code_callback) (GXInstance_t* instance) );