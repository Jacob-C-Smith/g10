#include "shared.metal"

fragment float4 fs_quad(VSOut in [[stage_in]])
{
    return float4(in.uv, 0.0, 1.0);
}
