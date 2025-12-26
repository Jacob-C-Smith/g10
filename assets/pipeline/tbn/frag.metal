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
    // 1. Extract and normalize the surface normal
    float3 T = normalize(in.worldTangent);
    float3 B = normalize(in.worldBitangent);
    float3 N = normalize(in.worldNormal);
    float3x3 TBN = float3x3(T, B, N);

    float3 mapNormal = normalMap.sample(normalSmp, in.uv).rgb * 2.0 - 1.0;
    mapNormal = normalize(mapNormal * 2.0 - 1.0);
    float3 worldNormal = normalize(TBN * mapNormal);
    
    // 2. Define a hard-coded light direction (pointing down and forward)
    // We normalize it to ensure the dot product is consistent
    float3 lightDir = normalize(float3(0.5, -0.75, -1.0));

    // 3. Calculate Diffuse intensity (Lambert's Cosine Law)
    // We negate lightDir because the vector should point TO the light source
    float diffuse = max(dot(worldNormal, -lightDir), 0.0);

    // 4. Add a tiny bit of ambient light so the shadows aren't pitch black
    float ambient = 0.1;
    float lightIntensity = diffuse + ambient;

    // 5. Sample the base color (or use your uniform color)
    float4 texColor = colorMap.sample(colorSmp, in.uv);
    
    // Final color: Texture * Intensity
    //return float4(in.worldNormal, 1.0);
    return colorMap.sample(colorSmp, in.uv);
    //return float4(texColor.rgb * lightIntensity, texColor.a);
}