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
	
	size_t           clear_color_count;
	VkClearValue    *clear_colors;
};

struct GXRenderPass_s
{
	char            *name;

	VkRenderPass     render_pass;

	dict            *subpasses;
	GXSubpass_t    **subpasses_data;
	size_t           subpasses_count;

	GXFramebuffer_t *framebuffers;

	dict            *attachments;
	VkImageView     *image_attachments;
};

struct GXSubpass_s
{
	char *name;
};

struct GXAttachment_s
{
	char        *name;
	GXTexture_t *texture;
};

struct GXFramebuffer_s
{
	char          *name;
	VkFramebuffer  framebuffer;

};

// Allocators

/* !
 *  Allocate memory for a renderer
 *
 * @param pp_renderer : return
 *
 * @sa destroy_renderrer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_renderer          ( GXRenderer_t   **pp_renderer );

/* !
 *  Allocate memory for a render pass
 *
 * @param pp_render_pass : return
 *
 * @sa destroy_render_pass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_render_pass       ( GXRenderPass_t **pp_render_pass );

/* !
 *  Allocate memory for a subpass
 *
 * @param pp_subpass : return
 *
 * @sa destroy_subpass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int create_subpass           ( GXSubpass_t    **pp_subpass );

// Constructors

/* !
 *  Load a renderer from a JSON file
 *
 * @param pp_renderer : return
 * @param path        : The path to a JSON file containing a renderer object
 *
 * @sa load_renderer_as_json
 * @sa create_renderer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_renderer            ( GXRenderer_t   **pp_renderer   , char *path );

/* !
 *  Load an renderer from JSON text
 *
 * @param pp_renderer : return
 * @param token_text  : The renderer JSON object text
 * @param len         : The length of the renderer JSON object text
 *
 * @sa load_renderer_as_json
 * @sa create_renderer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_renderer_as_json    ( GXRenderer_t   **pp_renderer   , char *token_text, size_t len );

/* !
 *  Load a render pass from a JSON file
 *
 * @param pp_render_pass : return
 * @param path           : The path to a JSON file containing a render pass object
 *
 * @sa load_render_pass_as_json
 * @sa create_render_pass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_render_pass         ( GXRenderPass_t **pp_render_pass, char *path );

/* !
 *  Load a render pass from JSON text
 *
 * @param pp_render_pass : return
 * @param token_text     : The render pass JSON object text
 * @param len            : The length of the render pass JSON object text
 *
 * @sa load_render_pass_as_json
 * @sa create_render_pass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_render_pass_as_json ( GXRenderPass_t **pp_render_pass, char *token_text, size_t len );

/* !
 *  Load a subpass from JSON text
 *
 * @param pp_subpass : return
 * @param token_text : The subpass JSON object text
 * @param len        : The length of the subpass JSON object text
 *
 * @sa load_subpass_as_json
 * @sa create_subpass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_subpass             ( GXSubpass_t    **pp_subpass    , char *path );

/* !
 *  Load a subpass from JSON text
 *
 * @param pp_subpass : return
 * @param token_text : The subpass JSON object text
 * @param len        : The length of the subpass JSON object text
 *
 * @sa load_subpass_as_json
 * @sa create_subpass
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int load_subpass_as_json     ( GXSubpass_t    **pp_subpass    , char *token_text, size_t len );

// Logging

/* !
 *  Print info about a renderer
 *
 * @param p_renderer : the renderer
 *
 * @sa load_renderer
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int renderer_info            ( GXRenderer_t    *p_renderer );

// Add subpass callback
DLLEXPORT int add_subpass_callback     ( char *name, void (*function_pointer)());

// Scheduler operations

/* !
 *  Called once a frame by the scheduler to draw the game to the window. 
 * 
 * Draws every render pass in the instances active renderer to a swapchain, presents the last frame. 
 * 
 * @param instance         : Pointer to instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int render_frame             ( GXInstance_t    *instance );

/* !
 *  Presents the last frame, after the current frame is done drawing
 *
 * @param instance         : Pointer to instance
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int present_frame            ( GXInstance_t    *instance );

// Renderer operations

// Destructors
DLLEXPORT int destroy_renderer         ( GXRenderer_t    *p_renderer );
DLLEXPORT int destroy_render_pass      ( GXRenderPass_t  *p_render_pass );
DLLEXPORT int destroy_subpass          ( GXSubpass_t     *p_subpass );
