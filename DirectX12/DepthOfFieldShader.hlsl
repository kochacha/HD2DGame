#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //float4 col = tex0.Sample(smp, input.uv);
    //return col;

    float dep = pow(depthTex.Sample(smp, input.uv),300);

    if (dep > 0.8f)
    {
        return GaussianBlur(tex0, smp, input, 1.5f);
    }
    else if (dep > 0.75f)
    {
        return GaussianBlur(tex0, smp, input, 1.0f);
    }
    else if (dep > 0.72f)
    {
        return GaussianBlur(tex0, smp, input, 0.5f);
    }
    else if (dep > 0.50f)
    {
        return tex0.Sample(smp, input.uv);
    }
    else if (dep > 0.45f)
    {
        return GaussianBlur(tex0, smp, input, 1.5f);
    }
    else if (dep > 0.35f)
    {
        return GaussianBlur(tex0, smp, input, 2.5f);
    }
    else
    {
        return GaussianBlur(tex0, smp, input, 3.5f);
    }

   /* if (dep > 0.8f)
    {
        return float4(1, 0, 0, 1);
    }
    else if (dep > 0.75f)
    {
        return float4(0, 1, 0, 1);
    }
    else if (dep > 0.72f)
    {
        return float4(0, 0, 1, 1);
    }
    else if (dep > 0.50f)
    {
        return tex0.Sample(smp, input.uv);
    }
    else if (dep > 0.45f)
     {
     return float4(0, 1, 0, 1);
     }
    else
     {
     return float4(1, 0, 0, 1);
     }*/
}