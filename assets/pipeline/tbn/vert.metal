#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
    float2 uv [[attribute(1)]];
    float3 normal [[attribute(2)]];
    float4 tangent [[attribute(3)]];
};

struct InvMat {
    float3x3 N;
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
    constant InvMat &inv_mat [[buffer(0)]],
    constant VertexUniforms &transform [[buffer(1)]],
    constant CameraUniforms &camera [[buffer(2)]]
)
{
    VSOut out;

    float4 worldPosition = transform.M * float4(in.position, 1.0);
    out.position = camera.P * camera.V * worldPosition;
    out.uv = in.uv;

    // Create the TBN vectors
    float3 worldNormal = normalize(inv_mat.N * in.normal.xyz);
    float3 worldTangent = normalize(inv_mat.N * in.tangent.xyz);
    float3 worldBitangent = cross(worldNormal, worldTangent) * in.tangent.w;

    out.normal = worldNormal;
    out.worldNormal = worldNormal;
    out.worldTangent = worldTangent;
    out.worldBitangent = worldBitangent;
    out.viewNormal = (camera.V * float4(worldNormal, 0.0)).xyz;
    
    return out;
}