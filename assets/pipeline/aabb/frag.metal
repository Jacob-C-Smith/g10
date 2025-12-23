#include "shared.metal"

struct FragmentUniforms {
    float4 color;
};

fragment float4 fs_main(
    VSOut in [[stage_in]],
    constant FragmentUniforms &uniforms [[buffer(0)]]
) {

    // done
    return uniforms.color;
}