/** !
 * Header file for g10 shell
 * 
 * @file g10/shell.h
 *
 * @author Jacob Smith
 */

// Include guard
#pragma once

// array submodule
#include <array/array.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// g10
#include <g10/gtypedef.h>
#include <g10/g10.h>
#include <g10/linear.h>
#include <g10/camera_controller.h>

// Structure definitions
struct shell_s
{
    parallel_thread *p_thread;
};

// Function declarations
// Constructors
/** !
 * Construct a g10 shell
 * 
 * @param pp_shell result
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int shell_construct ( shell **pp_shell );

// Listener
/** !
 * Listen for TCP connections
 * 
 * @param p_instance the active instance
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int shell_network_listener ( g_instance *p_instance );

// Detach
/** !
 * Detach a g10 shell
 * 
 * @param p_shell the shell
 * 
 * @return 1 on success, 0 on error
*/
DLLEXPORT int shell_detach ( shell *p_shell );
