#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    float4 depth = depthTex.Sample(smp, input.uv);

    ////�t�H�O
    float dep1 = pow(depth,800);
    dep1 = dep1 - value;
    //float4 col = tex0.Sample(smp, input.uv);
    //return lerp(col, float4(1, 1, 1, 1), dep1)   
    //return float4(dep1,dep1,dep1,1); //�t�H�O�̐[�x�m�F�p

    //��ʊE�[�x
    float dep2 = pow(depth,300);
    dep2 = abs(dep2 - 0.65f) * 9.00f;
    //return GaussianBlur(tex0, smp, input, dep2);
    //return float4(dep2,dep2,dep2,1); //�u���[�[�x�m�F�p

    //��ʊE�[�x�{�t�H�O
    return lerp(GaussianBlur(tex0, smp, input, dep2), float4(1, 1, 1, 1), dep1);
}