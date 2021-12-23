//ボーンの最大数
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0)
{
	matrix viewProj;
	matrix world;
	float3 cameraPos;
};

//ボーンのスキニング行列が入る
cbuffer skinning:register(b3)
{
	matrix matSkinning[MAX_BONES];
};

//バーテックスバッファーの入力
struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

//頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};