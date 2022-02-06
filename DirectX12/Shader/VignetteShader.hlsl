#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //ビネット
    const float VIGNETTE_SCALE = value;
    float4 col = tex0.Sample(smp, input.uv.xy);
    float2 viewPort = (input.uv.zw);
    col.r -= dot(viewPort, viewPort) * VIGNETTE_SCALE * color.r;
    col.g -= dot(viewPort, viewPort) * VIGNETTE_SCALE * color.g;
    col.b -= dot(viewPort, viewPort) * VIGNETTE_SCALE * color.b;
    
    col.r -= color.a;
    col.g -= spare.r;
    col.b -= spare.g;

    col.rgb *= spare.b;
    col.rgb += spare.a;

    return col;
}