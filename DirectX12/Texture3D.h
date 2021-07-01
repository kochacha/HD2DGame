#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
using namespace Microsoft::WRL;

class Dx12_RootSignature;

class Texture3D
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

	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT2 position;
	float rotate;

	std::string texName;

	DirectX::XMMATRIX matView;
	DirectX::XMMATRIX matWorld;
	DirectX::XMMATRIX matRot;
	DirectX::XMMATRIX matTrans;
	DirectX::XMMATRIX matProjection;
	DirectX::XMVECTOR pos;
	float angle = 0;
	float cutSizeX;
	float cutSizeY;
	int maxTex;
	int animationCount = 0;

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> constBuff;

	Vertex* vertMap;
	unsigned short* indexMap;
	ConstBufferData* constMap;

	D3D12_HEAP_PROPERTIES heapprop = {};
	D3D12_RESOURCE_DESC resdesc = {};
	D3D12_HEAP_PROPERTIES cbHeapProp = {};
	D3D12_RESOURCE_DESC cbResDesc = {};
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = {};
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	void SetVertices();
	void SetCutVertices(UINT texNum);
	void CreateDepthStencilView();
	void CreateBufferView();

	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	static SIZE winSize;
	static UINT descriptorHandleIncrementSize;

public:
	Texture3D(std::string texName, DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT2 size, float rotate);
	Texture3D(std::string texName, UINT x, UINT y, UINT texNum, DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT2 size, float rotate);
	~Texture3D();

	void Draw();
	void Draw(DirectX::XMFLOAT3 position);
	void AnimationDraw(UINT animationRate, DirectX::XMFLOAT3 position);

	void SetTexNum(UINT texNum);

	static void StaticInit(ID3D12Device* device, SIZE winSize);
	static void BeginDraw(ID3D12GraphicsCommandList* cmdList);
	static void EndDraw();
};