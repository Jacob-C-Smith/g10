#pragma once

#include <stdio.h>
#include <stdbool.h>

#include <vulkan/vulkan.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <dict/dict.h>

#include <JSON/JSON.h>

#include <G10/GXtypedef.h>
#include <G10/GXInput.h>
#include <G10/GXMaterial.h>

#define JSON_VALUE ( token, type ) (token ? ((token->type == type) ? token->value.n_where : 0) : 0)

struct GXInstance_s
{
	// Name 
	char             *name;
	
	// Ticks
	size_t            ticks;

	// SDL2 data
	SDL_Window       *window;
	SDL_Event         event;

	// Vulkan data
	VkInstance        instance;
	VkPhysicalDevice  physical_device;
	VkDevice          device;
	VkQueue           graphics_queue;
	VkSurfaceKHR      surface;

	// G10 data
	GXInput_t        *input;

	// Loaded scenes
	dict             *scenes;

	// Server 
	//GXServer_t    *server;

	// Cached assets
	dict             *cached_parts;
	dict             *cached_materials;
	dict             *cached_shaders;

	// Delta time
	u32               d, 
		              lastTime;
	float             delta_time;

	u32 	          window_width,
		              window_height;

	size_t            loading_thread_count;

    bool              running;
	float             priority;
};

int           g_init           ( GXInstance_t     **instance, const char *path);
size_t        g_load_file      ( const char        *path    , void       *buffer, bool binaryMode); 

// Debug loggins
int           g_print_error    ( const char *const  format  , ... );
int           g_print_warning  ( const char *const  format  , ... );
int           g_print_log      ( const char *const  format  , ... );

GXInstance_t* g_get_active_instance ( void );

// Cache operations
int           g_cache_material ( GXInstance_t        *instance, GXMaterial_t *material );
int           g_cache_part     ( GXInstance_t        *instance, GXPart_t     *part );
int           g_cache_shader   ( GXInstance_t        *instance, GXShader_t   *shader );

GXMaterial_t *g_find_material  ( GXInstance_t        *instance, char         *name );
GXPart_t     *g_find_part      ( GXInstance_t        *instance, char         *name );
GXShader_t   *g_find_shader    ( GXInstance_t        *instance, char         *name );


int           g_exit           ( GXInstance_t      *instance );