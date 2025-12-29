// header guard
#pragma once

// standard library
#include <stdio.h>

// gsdk
/// core
#include <core/log.h>
#include <core/interfaces.h>

/// data
#include <data/dict.h>

// g10
#include <gtypedef.h>
#include <g10.h>

// function definitions
int user_code_set_callback ( fn_user_code *pfn_user_code );
int user_code(g_instance *p_instance);
