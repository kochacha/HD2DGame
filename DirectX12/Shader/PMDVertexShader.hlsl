#include "../Shader/PMDShaderHeader.hlsli"

Output VSmain(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint2 boneno : BONE_NO, min16uint weight : WEIGHT)
{
    Output output;
    output.svpos = mul(mul(viewproj, world), pos); //�V�F�[�_�[�ł͗�D��Ȃ̂Œ���
    normal.w = 0; //���s�ړ������𖳌��ɂ���
    output.normal = mul(world, normal); //�@���ɂ����[���h�ϊ����s��
    output.uv = uv;
    return output;
}