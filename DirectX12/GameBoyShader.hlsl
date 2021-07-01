#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //ゲームボーイ風
    const float dx = 1.00f / 1280.00f;
    const float dy = 1.00f / 720.00f;
    float pixelSizeX = dx * value;
    float pixelSizeY = dy * value;
    float ratioX = (int)(input.uv.x / pixelSizeX) * pixelSizeX;
    float ratioY = (int)(input.uv.y / pixelSizeY) * pixelSizeY;
    float4 col = tex0.Sample(smp, float2(ratioX, ratioY));
    col = dot(col.rgb, float3(0.3, 0.59, 0.11));
    if (col.r <= 0.25)
    {
        col = float4(0.06, 0.22, 0.06, 1.0);
    }
    else if (col.r > 0.75)
    {
        col = float4(0.6, 0.74, 0.06, 1.0);
    }
    else if (col.r > 0.25 && col.r <= 0.5)
    {
        col = float4(0.19, 0.38, 0.19, 1.0);
    }
    else
    {
        col = float4(0.54, 0.67, 0.06, 1.0);
    }
    return col;
}