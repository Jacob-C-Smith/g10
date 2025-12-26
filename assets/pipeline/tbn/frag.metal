#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
    float4 color1 [[color(1)]];
};

struct FragmentUniforms {
    float4 color;
};

fragment float4 fs_main(
    VSOut in [[stage_in]],
    texture2d<float> albMap [[texture(0)]],
    sampler          albSmp [[sampler(0)]],
    texture2d<float> normalMap [[texture(1)]],
    sampler          normalSmp [[sampler(1)]],
    constant FragmentUniforms &uniforms [[buffer(0)]]
) {
    
    float3 mapNormal = normalMap.sample(normalSmp, in.uv).rgb * 2.0 - 1.0;
    float3x3 TBN = float3x3(in.worldTangent, in.worldBitangent, in.worldNormal);
    float3 worldNormal = normalize(TBN * mapNormal);

    return float4(worldNormal.x, -worldNormal.y, worldNormal.z, 1.0);
    //return float4((worldNormal+1.0)/2.0,1.0);
//    return normalMap.sample(normalSmp, in.uv);
//    return float4(worldNormal.z, worldNormal.z, worldNormal.z, 1.0);
}
