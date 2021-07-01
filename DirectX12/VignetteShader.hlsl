#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //ビネット
    const float VIGNETTE_SCALE = value;
    float4 col = tex0.Sample(smp, input.uv.xy);
    float2 viewPort = (input.uv.zw);
    col.rgb -= dot(viewPort, viewPort) * VIGNETTE_SCALE;
    return col;
}