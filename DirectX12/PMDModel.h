#pragma once
#include "PMDLoader.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Camera.h"

using namespace Microsoft::WRL;

namespace KochaEngine
{
	class PMDModel
	{
		struct ConstBufferData
		{
			DirectX::XMMATRIX world; //モデル本体を回転させたり移動させたりする行列
			DirectX::XMMATRIX viewproj; //ビューとプロジェクション合成行列
		};

		//シェーダー側に投げられるマテリアルデータ
		struct MaterialForHlsl
		{
			DirectX::XMFLOAT3 diffuse;
			DirectX::XMFLOAT3 specular;
			DirectX::XMFLOAT3 ambient;
			float alpha;
			float specularity;
		};

		//それ以外のマテリアルデータ
		struct AdditionalMaterial
		{
			std::string texPath; //テクスチャファイルパス
			int toonIndex;
			bool edgeFlag;
		};

		//全体をまとめるデータ
		struct Material
		{
			unsigned int indicesNum; //インデックス数
			MaterialForHlsl material;
			AdditionalMaterial additional;
		};

	private:

		std::string pmdName;

		PMDModel* parent = nullptr;

		DirectX::XMFLOAT3 scale = { 1,1,1 };
		DirectX::XMFLOAT3 position = { 0,0,0 };
		DirectX::XMFLOAT3 rotate = { 0,0,0 };
		DirectX::XMFLOAT4 color = { 1,1,1,1 };

		DirectX::XMMATRIX matWorld;
		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matProjection;
		DirectX::XMMATRIX matScale;
		DirectX::XMMATRIX matRot;
		DirectX::XMMATRIX matTrans;
		float angle = 0;

		std::vector<Material> materials;
		ComPtr<ID3D12DescriptorHeap> materialDescHeap;
		ComPtr<ID3D12Resource> materialBuff;
		ComPtr<ID3D12Resource> constBuff;
		ConstBufferData* constMap;

		D3D12_HEAP_PROPERTIES cbHeapProp = {};
		D3D12_RESOURCE_DESC cbResDesc = {};
		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = {};
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		D3D12_DESCRIPTOR_HEAP_DESC matDescHespDesc = {};
		D3D12_CONSTANT_BUFFER_VIEW_DESC matCBVDesc = {};
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;

		UINT descriptorHandleIncrementSize;

		void CreateBufferView();

		static ID3D12Device* device;
		static ID3D12GraphicsCommandList* cmdList;
		static SIZE winSize;

	public:
		PMDModel(std::string objName);
		~PMDModel();

		void Draw(Camera* camera);

		void SetPosition(DirectX::XMFLOAT3 position);
		void SetScale(DirectX::XMFLOAT3 scale);
		void SetRotate(DirectX::XMFLOAT3 rotate);
		void SetColor(DirectX::XMFLOAT4 color);
		void SetTexture(std::string textureName);

		void MovePosition(DirectX::XMFLOAT3 move);
		void MoveScale(DirectX::XMFLOAT3 moveScale);
		void MoveRotate(DirectX::XMFLOAT3 moveRotate);
		void MoveColor(DirectX::XMFLOAT4 moveColor);

		DirectX::XMFLOAT3 GetPosition() { return position; }
		DirectX::XMFLOAT3 GetScale() { return scale; }

		static void StaticInit(ID3D12Device* device, SIZE winSize);
		static void BeginDraw(ID3D12GraphicsCommandList* cmdList);
		static void EndDraw();
	};
}