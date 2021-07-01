#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //���U�C�N
    const float dx = 1.00f / 1280.00f;
    const float dy = 1.00f / 720.00f;
    float pixelSizeX = dx * value;
    float pixelSizeY = dy * value;
    float2 uv = float2((int)(input.uv.x / pixelSizeX) * pixelSizeX, (int)(input.uv.y / pixelSizeY) * pixelSizeY);
    return tex0.Sample(smp, uv);
}