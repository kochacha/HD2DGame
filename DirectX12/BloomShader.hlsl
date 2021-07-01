#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //�ʏ�e�N�X�`��
    float4 col = tex0.Sample(smp, input.uv);
    //���P�x�����̃K�E�V�A���ڂ���
    float widthPixel = 1.0f / 1280.0f;
    float hightPixel = 1.0f / 720.0f;
    float blurScale = 4;
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

    //�ʏ�e�N�X�`���ƍ��P�x�ڂ���������
    return col + blur;
}