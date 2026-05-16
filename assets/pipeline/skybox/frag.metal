#include "shared.metal"

#define M_PI_F 3.14159265358979323846f

fragment float4 fs_main(
    VSOut in [[stage_in]],
    texture2d<float> skyboxTex [[texture(0)]],
    sampler          skyboxSmp [[sampler(0)]]
)
{
    float3 d = normalize(in.uvw);
    float2 uv = float2(0.5f + atan2(d.y, d.x) / (2.0f * M_PI_F), 0.5f - asin(d.z) / M_PI_F);
    return skyboxTex.sample(skyboxSmp, uv);
}
