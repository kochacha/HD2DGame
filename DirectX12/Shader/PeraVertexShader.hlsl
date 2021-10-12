#include "../Shader/PeraShaderHeader.hlsli"

Output main( float4 pos : POSITION, float2 uv : TEXCOORD )
{
	Output output;
	output.svpos = pos;
	output.uv.xy = uv;
	output.uv.zw = pos.xy;
	return output;
}