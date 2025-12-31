#include "shared.metal"

struct FragmentOutput {
    float4 color0 [[color(0)]];
};

fragment float4 fs_main() {
    return float4(0.0,0.0,0.0,1.0);
}