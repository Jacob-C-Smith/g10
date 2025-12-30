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
    return float4(normalize(in.viewNormal) * 0.5 + 0.5, 1.0);
}