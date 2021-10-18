#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //ŠK’²
    float4 col = tex0.Sample(smp, input.uv);
    float tone = 0.20f;
    return float4(col.rgb - fmod(col.rgb, tone), col.a);
}