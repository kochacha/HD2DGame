#include "BasicShaderHeader.hlsli"

VSOutput VSmain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

    VSOutput output;
    output.svpos = mul(mat, pos);
    output.worldpos = wpos;
    output.normal = wnormal;
    output.uv = uv;
    output.tpos = mul(lightCamera, output.svpos);
    return output;
}

VSOutput ShadowVS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);

    VSOutput output;
    output.svpos = mul(lightCamera, pos);
    output.worldpos = wpos;
    output.normal = wnormal;
    output.uv = uv;
    output.tpos = mul(lightCamera, output.svpos);
    return output;
}