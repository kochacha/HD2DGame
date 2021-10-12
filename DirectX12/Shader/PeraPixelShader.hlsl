#include "../Shader/PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    ////デフォルト
    float4 col = tex0.Sample(smp, input.uv);
    return col;

    //float dep = pow(depthTex.Sample(smp, input.uv),300);
    //return float4(dep, dep, dep, 1);

    //色反転
    //float4 col = tex0.Sample(smp, input.uv);
    //return float4(1.0f - col.rgb, col.a);
}