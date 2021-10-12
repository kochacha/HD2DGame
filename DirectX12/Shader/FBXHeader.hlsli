//�{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0)
{
	matrix viewProj;
	matrix world;
	float3 cameraPos;
};

//�{�[���̃X�L�j���O�s�񂪓���
cbuffer skinning:register(b3)
{
	matrix matSkinning[MAX_BONES];
};

//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

//���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};