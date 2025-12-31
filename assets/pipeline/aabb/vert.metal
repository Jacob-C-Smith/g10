#include "shared.metal"

struct VertexUniforms {
    float4x4 M;
};

struct CameraUniforms {
    float4x4 V;
    float4x4 P;
};

vertex VSOut vs_main(
    uint vid [[vertex_id]],
    constant VertexUniforms &transform [[buffer(1)]],
    constant CameraUniforms &camera [[buffer(2)]]
)
{

    float3 position[8] = {
        float3( 1.0,  1.0,  1.0),
        float3( 1.0,  1.0, -1.0),
        float3( 1.0, -1.0,  1.0),
        float3( 1.0, -1.0, -1.0),

        float3(-1.0,  1.0,  1.0),
        float3(-1.0,  1.0, -1.0),
        float3(-1.0, -1.0,  1.0),
        float3(-1.0, -1.0, -1.0),

    };

    VSOut out;

    float4 worldPosition = transform.M * float4(position[vid], 1.0);
    out.position = camera.P * camera.V * worldPosition;
    out.pointSize = 10.0;

    return out;
}

/*
#include "shared.metal"

vertex VSOut vs_main(uint vid [[vertex_id]])
{


    VSOut out;
    out.position = float4(positions[vid], 1.0);
    return out;
}
*/