#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //Fû·
    float misaligned = value * 0.010f;
    float2 red = float2(0, 0);
    float2 green = float2(misaligned, 0);
    float2 blue = float2(-misaligned, 0);
    float4 col = tex0.Sample(smp, input.uv);
    float2 red_uv = input.uv + red;
    float2 green_uv = input.uv + green;
    float2 blue_uv = input.uv + blue;
    col.r = tex0.Sample(smp, red_uv).r;
    col.g = tex0.Sample(smp, green_uv).g;
    col.b = tex0.Sample(smp, blue_uv).b;
    return col;
}