#include "../Shader/PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //�ڂ���
    return GaussianBlur(tex0, smp, input, value);
}