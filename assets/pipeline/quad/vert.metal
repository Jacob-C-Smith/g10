#include "shared.metal"

vertex VSOut vs_quad(uint vid [[vertex_id]])
{
    float2 positions[6] = {
        float2(-0.5, -0.5),
        float2( 0.5, -0.5),
        float2(-0.5,  0.5),
        float2(-0.5,  0.5),
        float2( 0.5, -0.5),
        float2( 0.5,  0.5),
    };

    float2 uvs[6] = {
        float2(0.0, 1.0),
        float2(1.0, 1.0),
        float2(0.0, 0.0),
        float2(0.0, 0.0),
        float2(1.0, 1.0),
        float2(1.0, 0.0),
    };

    VSOut out;
    out.position = float4(positions[vid], 0.0, 1.0);
    out.uv = uvs[vid];
    return out;
}
