#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //モノクロ(グレースケール/セピア)
    float4 col = tex0.Sample(smp, input.uv);
    float sepia = value;
    float y = dot(col.rgb, float3(0.299f, 0.587f, 0.114f));
    col = float4(y + sepia, y, y - sepia, 1);
    return col;
}