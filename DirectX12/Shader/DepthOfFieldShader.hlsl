#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    float4 depth = depthTex.Sample(smp, input.uv);

    ////フォグ
    float dep1 = pow(depth,800);
    dep1 = dep1 - value;
    //float4 col = tex0.Sample(smp, input.uv);
    //return lerp(col, float4(1, 1, 1, 1), dep1)   
    //return float4(dep1,dep1,dep1,1); //フォグの深度確認用

    //被写界深度
    float dep2 = pow(depth,300);
    dep2 = abs(dep2 - 0.65f) * 9.00f;
    //return GaussianBlur(tex0, smp, input, dep2);
    //return float4(dep2,dep2,dep2,1); //ブラー深度確認用

    //被写界深度＋フォグ
    return lerp(GaussianBlur(tex0, smp, input, dep2), float4(1, 1, 1, 1), dep1);
}