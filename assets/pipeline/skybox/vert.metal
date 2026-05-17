#include "shared.metal"

struct VertexInput {
    float3 position [[attribute(0)]];
};

struct CameraUniforms {
    float4x4 V;
    float4x4 P;
    float3 camera_pos;
};

vertex VSOut vs_main(
    VertexInput in [[stage_in]],
    constant CameraUniforms &camera [[buffer(2)]]
)
{
    VSOut out;

    // Remove translation from view matrix
    float4x4 viewNoTranslate = camera.V;
    viewNoTranslate[3][0] = 0.0;
    viewNoTranslate[3][1] = 0.0;
    viewNoTranslate[3][2] = 0.0;

    float4 worldPosition = float4(in.position, 1.0);
    float4 clipPosition = camera.P * viewNoTranslate * worldPosition;

    // Set z to w so that z/w = 1.0 (far plane)
    out.position = clipPosition.xyww;
    out.uvw = in.position;

    return out;
}
