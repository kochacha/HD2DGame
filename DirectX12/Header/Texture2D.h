#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
#include "Vector2.h"
using namespace Microsoft::WRL;

namespace KochaEngine
{
	class Dx12_RootSignature;
	class Texture2D
	{
		struct ConstBufferData
		{
			DirectX::XMFLOAT4 color;
			DirectX::XMMATRIX mat;
		};

		struct Vertex
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT2 uv;
		};

	private:

		Vertex vertices[4];

		unsigned short indices[6] =
		{
			0,1,2,
			1,2,3,
		};

		UINT cutX, cutY, texNum;

		Vector2 size;
		Vector2 position;
		float rotate;

		std::string texName;

		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matWorld;
		DirectX::XMMATRIX matRot;
		DirectX::XMMATRIX matTrans;
		DirectX::XMMATRIX matProjection;
		float angle = 0;
		float cutSizeX;
		float cutSizeY;
		int maxTex;
		int animationCount = 0;
		DirectX::XMFLOAT4 color = { 1,1,1,1 };

		ComPtr<ID3D12Resource> vertBuff;
		ComPtr<ID3D12Resource> constBuff;

		Vertex* vertMap;
		ConstBufferData* constMap;

		D3D12_HEAP_PROPERTIES heapprop = {};
		D3D12_RESOURCE_DESC resdesc = {};
		D3D12_HEAP_PROPERTIES cbHeapProp = {};
		D3D12_RESOURCE_DESC cbResDesc = {};
		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = {};
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		D3D12_VERTEX_BUFFER_VIEW vbView = {};

		void SetVertices();
		void SetCutVertices(UINT texNum);
		void CreateDepthStencilView();
		void CreateBufferView();

		static ID3D12Device* device;
		static ID3D12GraphicsCommandList* cmdList;
		static SIZE winSize;
		static UINT descriptorHandleIncrementSize;

	public:
		Texture2D(const std::string& texName, Vector2 position,
			Vector2 size, float rotate);
		Texture2D(const std::string& texName, UINT x, UINT y, UINT texNum, Vector2 position,
			Vector2 size, float rotate);
		~Texture2D();

		void Draw();
		void Draw(Vector2 position);
		void AnimationDraw(UINT animationRate, Vector2 position);

		void SetTexNum(UINT texNum);
		void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
		void SetSize(Vector2 size);

		static void StaticInit(ID3D12Device* device, SIZE winSize);
		static void BeginDraw(ID3D12GraphicsCommandList* cmdList);
		static void EndDraw();
	};
}