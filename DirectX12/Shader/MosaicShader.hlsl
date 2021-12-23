#include "../Shader/PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //ÉÇÉUÉCÉN
    const float dx = 1.00f / 1920.00f;
    const float dy = 1.00f / 1080.00f;
    float pixelSizeX = dx * value;
    float pixelSizeY = dy * value;
    float2 uv = float2((int)(input.uv.x / pixelSizeX) * pixelSizeX, (int)(input.uv.y / pixelSizeY) * pixelSizeY);
    return tex0.Sample(smp, uv);
}