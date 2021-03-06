#include "PMDShaderHeader.hlsli"

Output VSmain(float4 pos : POSITION, float4 normal : NORMAL, float2 uv : TEXCOORD, min16uint2 boneno : BONE_NO, min16uint weight : WEIGHT)
{
    Output output;
    output.svpos = mul(mul(viewproj, world), pos); //シェーダーでは列優先なので注意
    normal.w = 0; //平行移動成分を無効にする
    output.normal = mul(world, normal); //法線にもワールド変換を行う
    output.uv = uv;
    return output;
}