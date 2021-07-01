#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //‚Ú‚©‚µ
    float widthPixel = 1.0f / 1280.0f;
    float hightPixel = 1.0f / 720.0f;
    float blurScale = value;
    float4 blur = { 0,0,0,0 };
    blur += tex0.Sample(smp, input.uv + float2(-blurScale * widthPixel, -blurScale * hightPixel));
    blur += tex0.Sample(smp, input.uv + float2(0, -blurScale * hightPixel));
    blur += tex0.Sample(smp, input.uv + float2(blurScale * widthPixel, -blurScale * hightPixel));
    blur += tex0.Sample(smp, input.uv + float2(-blurScale * widthPixel, 0));
    blur += tex0.Sample(smp, input.uv);
    blur += tex0.Sample(smp, input.uv + float2(blurScale * widthPixel, 0));
    blur += tex0.Sample(smp, input.uv + float2(-blurScale * widthPixel, blurScale * hightPixel));
    blur += tex0.Sample(smp, input.uv + float2(0, blurScale * hightPixel));
    blur += tex0.Sample(smp, input.uv + float2(blurScale * widthPixel, blurScale * hightPixel));
    blur = blur / 9.0f;

    return blur;
}