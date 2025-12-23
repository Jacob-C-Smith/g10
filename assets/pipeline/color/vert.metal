#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
};

vertex VSOut vs_main(VertexInput in [[stage_in]])
{
    VSOut out;
    // Expand float3 to float4 (w = 1)
    out.position = float4(in.position, 1.0);
    return out;
}
