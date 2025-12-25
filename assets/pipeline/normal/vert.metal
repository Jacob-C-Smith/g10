#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
    float3 normal [[attribute(2)]];
};

struct VertexUniforms {
    float4x4 M;
};

struct CameraUniforms {
    float4x4 V;
    float4x4 P;
};

vertex VSOut vs_main(
    VertexInput in [[stage_in]],
    constant VertexUniforms &transform [[buffer(1)]],
    constant CameraUniforms &camera [[buffer(2)]]
)
{
    VSOut out;

    float4 position = transform.M * float4(in.position, 1.0);
    
    out.position = camera.P * camera.V * position;

    out.normal = position.xyz;
    return out;
}