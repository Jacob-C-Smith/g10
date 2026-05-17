#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
};

struct CameraUniforms {
    float4x4 V;
    float4x4 P;
    float4 camera_pos;
};

struct Light {
    float4 position;  // w = 0 for Directional, w = 1 for Point/Spot
    float4 color;     // rgb = color, a = intensity
    float4 direction; // xyz = direction, w = inner cone angle (Spot)
    float4 params;    // x = radius, y = outer cone angle (Spot), z = type, w = falloff
};

struct LightingUniforms {
    Light lights[16];
    float4 light_count_and_pad;
    float4 ambient_color;
};

fragment float4 fs_main(
    VSOut in [[stage_in]],
    constant CameraUniforms &camera [[buffer(2)]],
    constant LightingUniforms &lighting [[buffer(3)]],
    texture2d<float> colorMap [[texture(0)]],
    sampler          colorSmp [[sampler(0)]],
    texture2d<float> normalMap [[texture(1)]],
    sampler          normalSmp [[sampler(1)]]
) {
    float3 normalSample = normalMap.sample(normalSmp, in.uv).xyz;
    normalSample.y = 1.0 - normalSample.y;
    float3 tangentNormal = normalize(normalSample * 2.0 - 1.0);
    float3 T = normalize(in.worldTangent);
    float3 B = normalize(in.worldBitangent);
    float3 N = normalize(in.worldNormal);
    float3x3 TBN = float3x3(T, B, N);
    float3 worldNormal = normalize(TBN * tangentNormal);

    float3 viewDir = normalize(camera.camera_pos.xyz - in.worldPos);

    float3 totalDiffuse = 0.0;
    float3 totalSpecular = 0.0;

    int lightCount = int(lighting.light_count_and_pad.x);
    for (int i = 0; i < lightCount; i++) {
        Light l = lighting.lights[i];

        float3 lightDir;
        float attenuation = 1.0;
        int type = int(l.params.z);

        if (type == 0) { // Directional
            lightDir = normalize(-l.direction.xyz);
        } else { // Point or Spot
            float3 lightDirVector = l.position.xyz - in.worldPos;
            float d = length(lightDirVector);
            lightDir = lightDirVector / d;

            float radius = l.params.x;
            attenuation = saturate(1.0 - d / radius);
            attenuation *= attenuation;

            if (type == 2) { // Spot
                float theta = dot(lightDir, normalize(-l.direction.xyz));
                float inner = l.direction.w;
                float outer = l.params.y;
                float epsilon = inner - outer;
                float intensity = saturate((theta - outer) / epsilon);
                attenuation *= intensity;
            }
        }

        float3 lightColor = l.color.rgb * l.color.a;

        // Diffuse
        float diff = max(dot(worldNormal, lightDir), 0.0);
        totalDiffuse += diff * lightColor * attenuation;

        // Specular (Blinn-Phong)
        float3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(worldNormal, halfwayDir), 0.0), 32.0);
        totalSpecular += 0.5 * spec * lightColor * attenuation;
    }

    float4 albedo = colorMap.sample(colorSmp, in.uv);
    float3 ambient = lighting.ambient_color.rgb * albedo.rgb;
    float3 result = ambient + totalDiffuse * albedo.rgb + totalSpecular;

    return float4(result, albedo.a);
}