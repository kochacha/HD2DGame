#pragma once
#include "Dx12_Object.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>
#include "Camera.h"
#include "Vector4.h"

using namespace Microsoft::WRL;

namespace KochaEngine
{
	class LightManager;
	class Object
	{
		// 定数バッファ用データ構造体B0
		struct ConstBufferDataB0
		{
			Vector4 color;	// 色 (RGBA)
			DirectX::XMMATRIX mat;	// ３Ｄ変換行列
			DirectX::XMMATRIX world;
			Vector3 cameraPos;
			float pad1;
		};

		struct ConstBufferDataB1
		{
			Vector3 ambient;
			float pad1;
			Vector3 diffuse;
			float pad2;
			Vector3 specular;
			float alpha;
		};

	public:
		enum BillboardType
		{
			NONE,
			BILLBOARD,
			BILLBOARD_Y,
		};

	private:
		Object* parent = nullptr;

		ComPtr<ID3D12Resource> constBuffB0;
		ComPtr<ID3D12Resource> constBuffB1;
		ComPtr<ID3D12Resource> depthBuff;
		//ComPtr<ID3D12DescriptorHeap> dsvHeap;

		std::string objName;
		std::string texName;

		Vector3 scale = { 1,1,1 };
		Vector3 position = { 0,0,0 };
		Vector3 rotate = { 0,0,0 };
		Vector4 color = { 1,1,1,1 };
		BillboardType billboardType = BillboardType::NONE;

		DirectX::XMMATRIX matWorld;
		DirectX::XMMATRIX matView;
		DirectX::XMMATRIX matProjection;
		float angle = 0;

		ConstBufferDataB0* constMap0;
		ConstBufferDataB1* constMap1;

		D3D12_HEAP_PROPERTIES cbHeapProp = {};
		D3D12_RESOURCE_DESC cbResDesc = {};
		D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = {};
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;

		UINT descriptorHandleIncrementSize;

		void CreateBufferView();
		void CreateDepthStencilView();

		//static LightManager* lightManager;
		static ID3D12Device* device;
		static ID3D12GraphicsCommandList* cmdList;
		static SIZE winSize;

	public:
		Object(std::string objName);
		~Object();

		void Draw(Camera* camera, LightManager* arg_lightManager);
		void Draw(Camera* camera, LightManager* arg_lightManager, Vector3 position, Vector3 scale, Vector3 rotate);

		void SetPosition(const Vector3& position);
		void SetScale(const Vector3& scale);
		void SetRotate(const Vector3& rotate);
		void SetColor(const Vector4& color);
		void SetAlpha(const float arg_alpha);
		void SetTexture(const std::string& textureName);
		void SetBillboardType(const BillboardType& arg_type);

		void MovePosition(const Vector3& move);
		void MoveScale(const Vector3& moveScale);
		void MoveRotate(const Vector3& moveRotate);
		void MoveColor(const Vector4& moveColor);

		Vector3 GetScale() { return scale; }

		//static void SetLightManager(LightManager* arg_lightManager);
		static void StaticInit(ID3D12Device* device, SIZE winSize);
		static void BeginDraw(ID3D12GraphicsCommandList* arg_cmdList);
		static void BeginAlphaDraw(ID3D12GraphicsCommandList* arg_cmdList);
		static void BeginDrawFromLight(ID3D12GraphicsCommandList* arg_cmdList);
		static void EndDraw();
	};
}