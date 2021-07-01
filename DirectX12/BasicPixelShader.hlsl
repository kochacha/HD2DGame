#include "BasicShaderHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

struct PSOutput
{
    float4 target0 : SV_TARGET0;
    float4 target1 : SV_TARGET1;
    float4 target2 : SV_TARGET2;
};

PSOutput PSmain(GSOutput input)/* : SV_TARGET*/
{
    PSOutput output;
	//テクスチャマッピング
	float4 texColor = tex.Sample(smp, input.uv);
	//Lambert反射
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.7f;
	float4 shaderColor = float4(brightness, brightness, brightness, 1.0f);

	//通常
    output.target0 = shaderColor * texColor;

	//高輝度出力(ブルーム用)
	float y = dot(float3(0.299f, 0.587f, 0.114f), /*shaderColor * */texColor);
    output.target1 = y > 0.7f ? /*shaderColor * */texColor : 0.0f;

	output.target2 = float4(1, 0, 0, 1); //shaderColor * texColor;

    return output;
}