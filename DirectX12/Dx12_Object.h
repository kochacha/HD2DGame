#pragma once
#include <string>
#include <vector>
#include <d3d12.h>
#include <DirectXTex.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

using namespace Microsoft::WRL;

namespace KochaEngine
{
	struct VertexPosNormalUv
	{
		Vector3 pos; // xyz座標
		Vector3 normal; // 法線ベクトル
		Vector2 uv;  // uv座標
	};

	//マテリアル
	struct Material
	{
		std::string name;
		Vector3 ambient;
		Vector3 diffuse;
		Vector3 specular;
		float alpha;
		std::string textureFilename;
		std::string objFilename;

		Material()
		{
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	struct Mesh
	{
		std::vector<VertexPosNormalUv> vertices;
		std::vector<unsigned short> indices;
		D3D12_VERTEX_BUFFER_VIEW vbView;
		D3D12_INDEX_BUFFER_VIEW ibView;
		ComPtr<ID3D12Resource> vertBuff;
		ComPtr<ID3D12Resource> indexBuff;
	};

	class Dx12_Object
	{
	private:
		static ID3D12Device* device;
	public:

		static void LoadTexture(const std::string& arg_directoryPath, const std::string& arg_filename);
		static void LoadMaterial(const std::string& arg_objName, const std::string& arg_directoryPath, const std::string& arg_filename);
		static void LoadObject(const std::string& arg_objName);
		static void CreateBufferView(const std::string& arg_objName);

		static Material GetMaterial(const std::string& arg_objName);
		static D3D12_VERTEX_BUFFER_VIEW GetVBView(const std::string& arg_objName);
		static D3D12_INDEX_BUFFER_VIEW GetIBView(const std::string& arg_objName);
		static std::vector<unsigned short>GetIndices(const std::string& arg_objName);
		static void SetDevice(ID3D12Device* arg_device) { device = arg_device; }
	};
}