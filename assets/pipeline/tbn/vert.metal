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
    float3 T = normalize(inv_mat.N * in.tangent.xyz);
    float3 N = normalize(inv_mat.N * in.normal.xyz);
    float3 B = cross(N, T) * in.tangent.w;
    
    out.worldTangent = in.tangent.xyz;
    out.worldNormal = in.normal.xyz;

    //out.worldTangent = T;
    //out.worldNormal = N;
    out.worldBitangent = B;
    
    return out;
}