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

// structure definitions
struct texture_s
{
    char     _name[63+1];
    u32      width;
    u32      height;
    u32      channels;
    void *p_handle;
};

// function declarations
