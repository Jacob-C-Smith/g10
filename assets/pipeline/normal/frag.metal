#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
};

struct FragmentUniforms {
    float4 color;
};

fragment FragmentOutput fs_main(
    VSOut in [[stage_in]],
    constant FragmentUniforms &uniforms [[buffer(0)]]
) {
    
    FragmentOutput output;
    //float3 normal = normalize(in.normal);
    float3 normal = in.normal;
    //output.color0 = float4(normal * 0.5 + 0.5, 1.0);
    output.color0 = float4(normal.xyz, 1.0);

    // done
    return output;
}
