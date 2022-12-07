#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vulkan/vulkan.h>

#include <G10/GXtypedef.h>
#include <G10/G10.h>
#include <G10/GXCameraController.h>

struct GXRenderer_s
{
	char            *name;
	dict            *render_passes;
	GXRenderPass_t **render_passes_data;
	size_t           render_pass_count;
};

struct GXRenderPass_s
{
	char            *name;

	dict            *subpasses;
	GXSubpass_t    **subpasses_data;

	dict            *attachments;
	GXAttachment_t **attachments_data;
};

struct GXSubpass_s
{
	char *name;
	int i;
};

struct GXAttachment_s
{
	char* name;
	int i;

};

// Allocators
DLLEXPORT int create_renderer          ( GXRenderer_t   **pp_renderer );
DLLEXPORT int create__render_pass      ( GXRenderPass_t **pp_render_pass );
DLLEXPORT int create_subpass           ( GXSubpass_t    **pp_subpass );

// Constructors
DLLEXPORT int load_renderer            ( GXRenderer_t   **pp_renderer   , char *path );
DLLEXPORT int load_renderer_as_json    ( GXRenderer_t   **pp_renderer   , char *token_text, size_t len );

DLLEXPORT int load_render_pass         ( VkRenderPass   **pp_render_pass, char *path );
DLLEXPORT int load_render_pass_as_json ( VkRenderPass   **pp_render_pass, char *token_text, size_t len );

DLLEXPORT int load_subpass             ( VkRenderPass   **pp_subpass    , char *path );
DLLEXPORT int load_subpass_as_json     ( VkRenderPass   **pp_subpass    , char *token_text, size_t len );

// Logging
DLLEXPORT int print_renderer           ( GXRenderer_t *renderer );

// Scheduler operations
DLLEXPORT int render_frame             ( GXInstance_t    *instance );

// Destructors
DLLEXPORT int destroy_renderer         ( GXRenderer_t    *p_renderer );
DLLEXPORT int destroy_render_pass      ( GXRenderPass_t  *p_render_pass );
DLLEXPORT int destroy_subpass          ( GXSubpass_t     *p_subpass );
