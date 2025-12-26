#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
    float4 color1 [[color(1)]];
};

struct FragmentUniforms {
    float4 color;
};

fragment FragmentOutput fs_main(
    VSOut in [[stage_in]],
    constant FragmentUniforms &uniforms [[buffer(0)]]
) {
    
    FragmentOutput output;
    output.color0 = float4(in.normal, 1.0);
    output.color1 = float4(0.0, 0.0, 1.0, 1.0); 

    // done
    return output;
}
