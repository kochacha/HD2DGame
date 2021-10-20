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
		static ID3D12Device* device;
	public:

		static void LoadTexture(const std::string& arg_texName);

		static UINT GetTexNum(const std::string& arg_texName);
		static ComPtr<ID3D12Resource> GetTexBuff(const std::string& arg_texName);
		static D3D12_RESOURCE_DESC GetTexResDesc(const std::string& arg_texName);
		static void SetDevice(ID3D12Device* arg_device) { device = arg_device; }
	};
}
