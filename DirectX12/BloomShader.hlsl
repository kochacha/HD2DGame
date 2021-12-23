#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //通常テクスチャ
    float4 col = tex0.Sample(smp, input.uv);
    //高輝度部分のガウシアンぼかし
    float widthPixel = 1.0f / 1920.0f;
    float hightPixel = 1.0f / 1080.0f;
    float blurScale = 4.0f;
    float4 blur = { 0,0,0,0 };
    blur += tex1.Sample(smp, input.uv + float2(-blurScale * widthPixel, -blurScale * hightPixel));
    blur += tex1.Sample(smp, input.uv + float2(0, -blurScale * hightPixel));
    blur += tex1.Sample(smp, input.uv + float2(blurScale * widthPixel, -blurScale * hightPixel));
    blur += tex1.Sample(smp, input.uv + float2(-blurScale * widthPixel, 0));
    blur += tex1.Sample(smp, input.uv);
    blur += tex1.Sample(smp, input.uv + float2(blurScale * widthPixel, 0));
    blur += tex1.Sample(smp, input.uv + float2(-blurScale * widthPixel, blurScale * hightPixel));
    blur += tex1.Sample(smp, input.uv + float2(0, blurScale * hightPixel));
    blur += tex1.Sample(smp, input.uv + float2(blurScale * widthPixel, blurScale * hightPixel));
    blur = blur / 9.0f;

    //通常テクスチャと高輝度ぼかしを合成
    return col + blur;
}