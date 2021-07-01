#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include"Vector4.h"
#include"Vector3.h"
#include"Vector2.h"

using namespace Microsoft::WRL;

#define TEX_BUFF_COUNT 3

namespace KochaEngine
{
	enum ShaderType
	{
		PERA_SHADER,
		VIGNETTE_SHADER,
		BLOOM_SHADER,
		GAME_BOY_SHADER,
		CHROMATIC_ABERRATION_SHADER,
		TOON_SHADER,
		GRAY_SCALE_SHADER,
		MOSAIC_SHADER,
		GAUSSIAN_BLUR_SHADER,
	};

	class PostEffect
	{
		struct PeraVartex
		{
			Vector3 pos;
			Vector2 uv;
		};

		struct ConstBufferDataB0
		{
			Vector4 color;
			float time;
			float value;
		};

	public:
		PostEffect();
		~PostEffect();

		void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
		void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
		void Draw();
		void Draw(const ShaderType& arg_type);

		void SetValue(const float arg_value) { value = arg_value; }
		void SetColor(const Vector4& arg_color) { color = arg_color; }

		static void StaticInit(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const SIZE winSize);
	private:
		PeraVartex vertex[4];
		D3D12_VERTEX_BUFFER_VIEW vertBuffView;
		ComPtr<ID3D12Resource> vertBuff;
		ComPtr<ID3D12Resource> texBuff[TEX_BUFF_COUNT];
		ComPtr<ID3D12Resource> depthBuff;
		ComPtr<ID3D12DescriptorHeap> descHeapSRV;
		ComPtr<ID3D12DescriptorHeap> descHeapRTV;
		ComPtr<ID3D12DescriptorHeap> descHeapDSV;

		ComPtr<ID3D12Resource> constBuff;
		ConstBufferDataB0* constMap;
		Vector4 color;
		float value;
		float time;

		static ID3D12Device* device;
		static ID3D12GraphicsCommandList* cmdList;
		static SIZE winSize;

		void Initialize();
		void SetVertices();
		void CreateBufferView();
	};
}