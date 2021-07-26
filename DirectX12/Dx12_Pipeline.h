#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <wrl.h>
using namespace Microsoft::WRL;

namespace KochaEngine
{
	class Dx12_Blob;
	class Dx12_Wrapper;

	class Dx12_Pipeline
	{
	private:

		Dx12_Blob& blob;
		Dx12_Wrapper& dx12;

		void CreateSpriteGraphicsPipelineState();
		void CreateOBJGraphicsPipelineState();
		void CreatePMDGraphicsPipelineState();
		void CreateFBXGraphicsPipelineState();
		void CreatePeraGraphicsPipelineState();

	public:
		Dx12_Pipeline(Dx12_Wrapper& dx12, Dx12_Blob& blob);
		~Dx12_Pipeline();

		static ComPtr<ID3D12PipelineState> spritePipelineState;
		static ComPtr<ID3D12PipelineState> objPipelineState;
		static ComPtr<ID3D12PipelineState> pmdPipelineState;
		static ComPtr<ID3D12PipelineState> fbxPipelineState;
		static ComPtr<ID3D12PipelineState> peraPipelineState;
		static ComPtr<ID3D12PipelineState> shadowPipelineState;
		static ComPtr<ID3D12PipelineState> vignettePipelineState;
		static ComPtr<ID3D12PipelineState> bloomPipelineState;
		static ComPtr<ID3D12PipelineState> gameBoyPipelineState;
		static ComPtr<ID3D12PipelineState> cAbePipelineState;
		static ComPtr<ID3D12PipelineState> toonPipelineState;
		static ComPtr<ID3D12PipelineState> grayScalePipelineState;
		static ComPtr<ID3D12PipelineState> mosaicPipelineState;
		static ComPtr<ID3D12PipelineState> blurPipelineState;
	};
}
