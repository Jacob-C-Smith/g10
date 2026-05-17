#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
    float2 uv [[attribute(1)]];
    float3 normal [[attribute(2)]];
    float4 tangent [[attribute(3)]];
};

struct InvMat {
    float4x4 N;
};

struct VertexUniforms {
    float4x4 M;
};

struct CameraUniforms {
    float4x4 V;
    float4x4 P;
    float4 camera_pos;
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
    out.worldPos = worldPosition.xyz;
    out.uv = float2(in.uv.x, 1.0 - in.uv.y);

    // create the TBN vectors
    float3x3 normalMatrix = float3x3(inv_mat.N[0].xyz, inv_mat.N[1].xyz, inv_mat.N[2].xyz);
    float3 worldNormal = normalize(normalMatrix * in.normal.xyz);
    float3 worldTangent = normalize(normalMatrix * in.tangent.xyz);
    float3 worldBitangent = cross(worldNormal, worldTangent) * in.tangent.w;

    out.worldNormal = worldNormal;
    out.worldTangent = worldTangent;
    out.worldBitangent = worldBitangent;
    
    return out;
}