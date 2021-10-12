#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vector>

class Dx12_Wrapper;
class Figure2D
{
	struct ConstBufferData
	{
		DirectX::XMFLOAT4 color;
	};

private:

	SIZE winSize;
	Dx12_Wrapper& dx12;

	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<unsigned short> indices;

};
