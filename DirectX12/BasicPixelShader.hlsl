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

	//////�e�N�X�`���}�b�s���O
	//float4 texColor = tex.Sample(smp, input.uv);

	////Lambert����
	//float diffuse = saturate(dot(-dirLights[0].direction, input.normal));
	//float brightness = diffuse + 0.7f;
	//float4 shaderColor = float4(brightness, brightness, brightness, 1.0f);

	// �e�N�X�`���}�b�s���O
	float4 texColor = tex.Sample(smp, input.uv);
	// ����x
	const float SHININESS = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	// �����ˌ�
	float3 ambient = 0.1f;
	// �V�F�[�f�B���O�ɂ��F
	float4 shaderColor = float4(ambientColor * ambient, m_alpha);

	for (int i = 0; i < DIRECTIONAL_LIGHT_NUM; i++) 
	{
		if (dirLights[i].isActive) 
		{
			float3 dotLightNormal = dot(dirLights[i].direction, input.normal);
			float3 reflect = normalize(-dirLights[i].direction + 2.0f * dotLightNormal * input.normal);
			float3 diffuse = dotLightNormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), SHININESS) * m_specular;

			shaderColor.rgb += (diffuse + specular) * dirLights[i].lightColor;
		}
	}

	for (int i = 0; i < POINT_LIGHT_NUM; i++) 
	{
		if (pointLights[i].isActive) 
		{
			float3 lightVector = pointLights[i].lightPos - input.worldpos.xyz;
			float d = length(lightVector);
			lightVector = normalize(lightVector);
			float atten = 1.0f / (pointLights[i].lightAtten.x * pointLights[i].lightAtten.y * d + pointLights[i].lightAtten.z * d * d);
			float3 dotLightNormal = dot(lightVector, input.normal);
			float3 reflect = normalize(-lightVector + 2.0f * dotLightNormal * input.normal);
			float3 diffuse = saturate(dotLightNormal * m_diffuse);
			float3 specular = pow(saturate(dot(reflect, eyedir)), SHININESS) * m_specular;

			shaderColor.rgb += atten * (diffuse + specular) * pointLights[i].lightColor;
		}
	}

	//�ʏ�
    output.target0 = shaderColor * texColor * color;

	//���P�x�o��(�u���[���p)
	float y = dot(float3(0.299f, 0.587f, 0.114f), shaderColor * texColor);
    output.target1 = y > 0.8f ? shaderColor * texColor : 0.0f;

	output.target2 = float4(1, 0, 0, 1); //shaderColor * texColor;

    return output;
}