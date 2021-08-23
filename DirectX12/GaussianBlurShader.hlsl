#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //‚Ú‚©‚µ
    return GaussianBlur(tex0, smp, input, value);
}