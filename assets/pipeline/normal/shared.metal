#pragma once
#include <metal_stdlib>
using namespace metal;

struct VSOut {
    float4 position [[position]];
    float3 normal;
};
