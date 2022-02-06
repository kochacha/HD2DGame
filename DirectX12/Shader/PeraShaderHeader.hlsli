Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
Texture2D<float4> texShrink : register(t2);

Texture2D<float> depthTex : register(t3);

SamplerState smp : register(s0);

cbuffer cbuff0 : register(b0)
{
	float4 color;
	float4 spare;
	float time;
	float value;
};

struct Output
{
	float4 svpos : SV_POSITION;
	float4 uv : TEXCOORD;
};

//ガウシアンブラー
float4 GaussianBlur(Texture2D<float4> tex, SamplerState smp, Output input, float value)
{
    float widthPixel = 1.00f / 1280.00f;
    float hightPixel = 1.00f / 720.00f;
    float blurScale = value;
    float4 blur = { 0,0,0,0 };
    blur += tex.Sample(smp, input.uv + float2(-blurScale * widthPixel, -blurScale * hightPixel));
    blur += tex.Sample(smp, input.uv + float2(0, -blurScale * hightPixel));
    blur += tex.Sample(smp, input.uv + float2(blurScale * widthPixel, -blurScale * hightPixel));
    blur += tex.Sample(smp, input.uv + float2(-blurScale * widthPixel, 0));
    blur += tex.Sample(smp, input.uv);
    blur += tex.Sample(smp, input.uv + float2(blurScale * widthPixel, 0));
    blur += tex.Sample(smp, input.uv + float2(-blurScale * widthPixel, blurScale * hightPixel));
    blur += tex.Sample(smp, input.uv + float2(0, blurScale * hightPixel));
    blur += tex.Sample(smp, input.uv + float2(blurScale * widthPixel, blurScale * hightPixel));
    blur = blur / 9.00f;

    return blur;
}