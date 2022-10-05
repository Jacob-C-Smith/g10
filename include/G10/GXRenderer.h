#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <G10/G10.h>
#include <G10/GXCameraController.h>

struct GXRenderer_s
{
	int i;
};

DLLEXPORT int render_frame ( GXInstance_t *instance );
