#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
using namespace Microsoft::WRL;

namespace KochaEngine
{
	struct Blob
	{
		ComPtr<ID3DBlob> vsBlob;
		ComPtr<ID3DBlob> psBlob;
		ComPtr<ID3DBlob> gsBlob;
		ComPtr<ID3DBlob> errorBlob;
	};

	class Dx12_Blob
	{
	private:

		Blob basicBlob;
		Blob texBlob;
		Blob pmdBlob;
		Blob fbxBlob;
		Blob peraBlob;
		Blob shadowBlob;
		Blob vignetteBlob;
		Blob bloomBlob;
		Blob gameBoyBlob;
		Blob cAberrationBlob;
		Blob toonBlob;
		Blob grayScaleBlob;
		Blob mosaicBlob;
		Blob blurBlob;
		Blob dofBlob;

		HRESULT CompileShader(const std::string& arg_ShaderName, const std::string& arg_EntryPoint, const std::string& arg_ShaderModel, Blob& arg_Blob);
		void ErrorBlob(const HRESULT& result, const Blob& blob);

	public:

		Dx12_Blob();
		~Dx12_Blob();

		void Init();

		Blob GetBasicBlob() { return basicBlob; }
		Blob GetTexBlob() { return texBlob; }
		Blob GetPMDBlob() { return pmdBlob; }
		Blob GetFBXBlob() { return fbxBlob; }
		Blob GetPeraBlob() { return peraBlob; }
		Blob GetShadowBlob() { return shadowBlob; }
		Blob GetVignetteBlob() { return vignetteBlob; }
		Blob GetBloomBlob() { return bloomBlob; }
		Blob GetGameBoyBlob() { return gameBoyBlob; }
		Blob GetCAbeBlob() { return cAberrationBlob; }
		Blob GetToonBlob() { return toonBlob; }
		Blob GetGrayScaleBlob() { return grayScaleBlob; }
		Blob GetMosaicBlob() { return mosaicBlob; }
		Blob GetBlurBlob() { return blurBlob; }
		Blob GetDofBlob() { return dofBlob; }
	};
}
