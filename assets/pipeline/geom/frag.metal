#include "shared.metal"

fragment float4 fs_main(VSOut in [[stage_in]])
{
    float3 color = in.position.xyz;
    return float4(color, 1.0);
}
