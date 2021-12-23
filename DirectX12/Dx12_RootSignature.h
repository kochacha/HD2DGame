#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <d3dx12.h>
using namespace Microsoft::WRL;

namespace KochaEngine
{
	class Dx12_Wrapper;
	class Dx12_RootSignature
	{
	private:

		HRESULT CreateSpriteRootSignature();
		HRESULT CreateOBJRootSignature();
		HRESULT CreatePMDRootSignature();
		HRESULT CreateFBXRootSignature();
		HRESULT CreatePeraRootSignature();
		HRESULT CreateShadowRootSignature();

		HRESULT result;

		ComPtr<ID3DBlob> errorBlob;

		ComPtr<ID3DBlob> rootSigBlob;

		static ComPtr<ID3D12RootSignature> spriteRootSignature;
		static ComPtr<ID3D12RootSignature> objRootSignature;
		static ComPtr<ID3D12RootSignature> pmdRootSignature;
		static ComPtr<ID3D12RootSignature> fbxRootSignature;
		static ComPtr<ID3D12RootSignature> peraRootSignature;
		static ComPtr<ID3D12RootSignature> shadowRootSignature;

		Dx12_Wrapper& dx12;

	public:

		Dx12_RootSignature(Dx12_Wrapper& dx12);

		~Dx12_RootSignature();

		static ComPtr<ID3D12RootSignature> GetSpriteRootSignature() { return spriteRootSignature; }
		static ComPtr<ID3D12RootSignature> GetOBJRootSignature() { return objRootSignature; }
		static ComPtr<ID3D12RootSignature> GetPMDRootSignature() { return pmdRootSignature; }
		static ComPtr<ID3D12RootSignature> GetFBXRootSignature() { return fbxRootSignature; }
		static ComPtr<ID3D12RootSignature> GetPeraRootSignature() { return peraRootSignature; }
		static ComPtr<ID3D12RootSignature> GetShadowRootSignature() { return shadowRootSignature; }

	};
}