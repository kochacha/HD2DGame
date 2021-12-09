#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //float4 col = tex0.Sample(smp, input.uv);
    //return col;

    float dep = pow(depthTex.Sample(smp, input.uv),300);
    dep = abs(dep - 0.65f) * 13.00f;
    return GaussianBlur(tex0, smp, input, dep);
    //return float4(dep, dep, dep, 1);
    
   /* if (dep > 0.9f)
    {
        return GaussianBlur(tex0, smp, input, 3.5f);
    }
    else if (dep > 0.8f)
    {
        return GaussianBlur(tex0, smp, input, 2.5f);
    }
    else if (dep > 0.75f)
    {
        return GaussianBlur(tex0, smp, input, 1.5f);
    }
    else if (dep > 0.72f)
    {
        return GaussianBlur(tex0, smp, input, 1.0f);
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
        return GaussianBlur(tex0, smp, input, 3.5f);
    }
    else
    {
        return GaussianBlur(tex0, smp, input, 5.5f);
    }*/
}