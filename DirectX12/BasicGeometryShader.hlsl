#include "BasicShaderHeader.hlsli"

[maxvertexcount(3)]
void GSmain(
	triangle VSOutput input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.svpos = input[i].svpos;
		element.worldpos = input[i].worldpos;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
		element.tpos = input[i].tpos;
		output.Append(element);
	}
}