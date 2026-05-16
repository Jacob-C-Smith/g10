#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
};

fragment float4 fs_main(
    VSOut in [[stage_in]],
    texture2d<float> colorMap [[texture(0)]],
    sampler          colorSmp [[sampler(0)]],
    texture2d<float> normalMap [[texture(1)]],
    sampler          normalSmp [[sampler(1)]]
) {
    float3 lightPos = float3(0.0, 0.0, 10.0);
    float3 lightColor = float3(2.0, 2.0, 2.0);
    
    float ambientStrength = 0.0;
    
    float3 normalSample = normalMap.sample(normalSmp, in.uv).xyz;
    normalSample.y = 1.0 - normalSample.y;
    float3 tangentNormal = normalize(normalSample * 2.0 - 1.0);
    float3 T = normalize(in.worldTangent);
    float3 B = normalize(in.worldBitangent);
    float3 N = normalize(in.worldNormal);
    float3x3 TBN = float3x3(T, B, N);
    float3 worldNormal = normalize(TBN * tangentNormal);
    
    float3 lightDirVector = lightPos - in.worldPos;
    float distance = length(lightDirVector);
    float3 lightDir = normalize(lightDirVector);
    float dist = length(lightDirVector);
    float radius = 60.0;
    float attenuation = saturate(1.0 - dist / radius);
    attenuation *= attenuation; 
    float diff = max(dot(worldNormal, lightDir), 0.0);
    float3 diffuse = diff * lightColor * attenuation;
    float3 ambient = ambientStrength * lightColor;

    float4 albedo = colorMap.sample(colorSmp, in.uv);
    
    float3 result = (ambient + diffuse) * albedo.rgb;
    
    //return float4(T, 1.0);
    return albedo;
    // return float4(normalSample, 1.0);
    // return float4(result, albedo.a);
}