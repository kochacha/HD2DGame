#include "../Shader/FBXHeader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	float4 target2 : SV_TARGET2;
};

//�G���g���[�|�C���g
PSOutput PSmain(VSOutput input)/* : SV_TARGET*/
{
	PSOutput output;
	//�e�N�X�`���}�b�s���O
	float4 texColor = tex.Sample(smp, input.uv);
	//Lambert����
	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse;
	float4 shaderColor = float4(brightness, brightness, brightness, 1.0f);
	//�A�e�ƃe�N�X�`���̐F������
	output.target0 = shaderColor * texColor;
	output.target1 = shaderColor * texColor;// float4(1.0f - (shaderColor * texColor).rgb, 1.0f);
	output.target2 = shaderColor * texColor;
	return output;
}