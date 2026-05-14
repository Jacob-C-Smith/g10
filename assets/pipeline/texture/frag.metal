#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
};

fragment FragmentOutput fs_main(
    VSOut in [[stage_in]],
    texture2d<float> tex [[texture(0)]],
    sampler          smp [[sampler(0)]]
) {
    
    FragmentOutput output;

    // sample the texture using the UVs from the vertex shader
    float4 sampledColor = tex.sample(smp, in.uv);

    // combine sampled color with your uniform or UV visualization
    output.color0 = sampledColor;

    return output;
}