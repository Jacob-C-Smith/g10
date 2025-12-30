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
    texture2d<float> colorMap [[texture(0)]],
    sampler          colorSmp [[sampler(0)]],
    texture2d<float> normalMap [[texture(1)]],
    sampler          normalSmp [[sampler(1)]],
    constant FragmentUniforms &uniforms [[buffer(0)]]
) {
    // Hard-coded point light properties
    float3 lightPos = float3(-18.0, 13.5, 3.5);
    float3 lightColor = float3(1.0, 1.0, 1.0);
    float ambientStrength = 0.5;

    // Sample textures
    float4 albedo = colorMap.sample(colorSmp, in.uv);
    float3 normalSample = normalMap.sample(normalSmp, in.uv).xyz;

    // Unpack normal from [0, 1] to [-1, 1]
    float3 tangentNormal = normalize(normalSample * 2.0 - 1.0);

    // Construct TBN Matrix
    float3 T = normalize(in.worldTangent);
    float3 B = normalize(in.worldBitangent);
    float3 N = normalize(in.worldNormal);
    float3x3 TBN = float3x3(T, B, N);

    // Transform normal from tangent space to world space
    float3 worldNormal = normalize(TBN * tangentNormal);

    // Debug: Visualize World Normal
    return float4(worldNormal * 0.5 + 0.5, 1.0);

    /*
    // Lighting calculation
    float3 lightDirVector = lightPos - in.worldPos;
    // ... rest of lighting code
    */
}