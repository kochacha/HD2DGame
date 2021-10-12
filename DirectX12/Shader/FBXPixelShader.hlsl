#include "../Shader/FBXHeader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	float4 target2 : SV_TARGET2;
};

//エントリーポイント
PSOutput PSmain(VSOutput input)/* : SV_TARGET*/
{
	PSOutput output;
	//テクスチャマッピング
	float4 texColor = tex.Sample(smp, input.uv);
	//Lambert反射
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse;
	float4 shaderColor = float4(brightness, brightness, brightness, 1.0f);
	//陰影とテクスチャの色を合成
	output.target0 = shaderColor * texColor;
	output.target1 = shaderColor * texColor;// float4(1.0f - (shaderColor * texColor).rgb, 1.0f);
	output.target2 = shaderColor * texColor;
	return output;
}