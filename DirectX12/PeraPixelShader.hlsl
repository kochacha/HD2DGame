#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //�f�t�H���g
    float4 col = tex0.Sample(smp, input.uv);
    return col;

    //�F���]
    //float4 col = tex0.Sample(smp, input.uv);
    //return float4(1.0f - col.rgb, col.a);
}