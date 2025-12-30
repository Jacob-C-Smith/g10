#pragma once
#include <metal_stdlib>
using namespace metal;

struct VSOut {
    float4 position [[position]];
    float2 uv;
    float3 normal;
    float3 worldTangent;
    float3 worldBitangent;
    float3 worldNormal;
    float3 viewNormal;
};
