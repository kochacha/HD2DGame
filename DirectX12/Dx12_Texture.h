#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <map>
#include <vector>
#include <string>
using namespace Microsoft::WRL;

namespace KochaEngine
{
	class Dx12_Texture
	{
	private:

	public:

		static void LoadTexture(ID3D12Device* device, std::string texName);

		static UINT GetTexNum(const std::string& texName);
		static ComPtr<ID3D12Resource> GetTexBuff(const std::string& texName);
		static D3D12_RESOURCE_DESC GetTexResDesc(const std::string& texName);
	};
}
