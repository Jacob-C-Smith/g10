#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
    float2 uv [[attribute(1)]];
    float3 normal [[attribute(2)]];
    float3 tangent [[attribute(3)]];
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

    float4 worldPosition = transform.M * float4(in.position, 1.0);
    out.position = camera.P * camera.V * worldPosition;
    out.uv = in.uv;

    float3 T = normalize((transform.M * float4(in.tangent, 0.0)).xyz);
    float3 N = normalize((transform.M * float4(in.normal, 0.0)).xyz);
    T = normalize(T - dot(T, N) * N);
    float3 B = cross(N, T);

    out.worldTangent = T;
    out.worldBitangent = B;
    out.worldNormal = N;
    
    return out;
}