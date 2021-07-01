#include "BasicShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VSOutput output;
    output.svpos = mul(mat, pos);
    output.normal = mul(mat2, normal);
    output.uv = uv;
    return output;
}