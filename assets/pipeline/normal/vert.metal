#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
    float2 uv [[attribute(1)]];
    float3 normal [[attribute(2)]];
};

struct VertexUniforms {
    float4x4 M;
};

struct CameraUniforms {
    float4x4 V;
    float4x4 P;
    float3 camera_pos;
};

vertex VSOut vs_main(
    VertexInput in [[stage_in]],
    constant VertexUniforms &transform [[buffer(1)]],
    constant CameraUniforms &camera [[buffer(2)]]
)
{
    VSOut out;

    float4 position = float4(in.position, 1.0);
    out.position = camera.P * camera.V * transform.M * position;
    out.uv = in.uv;

    float3 worldNormal = normalize((transform.M * float4(in.normal, 0.0)).xyz);
    
    // Assign to output (assuming your VSOut.normal is a float3)
    out.normal = worldNormal;
    return out;
}