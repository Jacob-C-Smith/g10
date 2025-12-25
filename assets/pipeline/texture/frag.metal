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

    // Sample the texture using the UVs from the vertex shader
    float4 sampledColor = tex.sample(smp, in.uv);

    // Combine sampled color with your uniform or UV visualization
    //output.color0 = sampledColor;
    output.color0 = float4(in.uv, 0.0, 1.0);

    return output;
}