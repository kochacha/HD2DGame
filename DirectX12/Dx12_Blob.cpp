#include "Dx12_Blob.h"
#include "Dx12_Wrapper.h"
#include "Util.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

KochaEngine::Dx12_Blob::Dx12_Blob()
{
	Init();
}

KochaEngine::Dx12_Blob::~Dx12_Blob()
{
}

HRESULT KochaEngine::Dx12_Blob::CompileShader(const std::string& arg_ShaderName, const std::string& arg_EntryPoint, const std::string& arg_ShaderModel, Blob& arg_Blob)
{
	auto shaderName = Util::StringToWchar_t(arg_ShaderName);
	auto result = D3DCompileFromFile(
		shaderName, //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		arg_EntryPoint.c_str(), arg_ShaderModel.c_str(), //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &arg_Blob.vsBlob, &arg_Blob.errorBlob);
	return result;
}

void KochaEngine::Dx12_Blob::ErrorBlob(const HRESULT& result, const Blob& blob)
{
	//シェーダーのエラー内容を表示
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(blob.errorBlob->GetBufferSize());
		std::copy_n((char*)blob.errorBlob->GetBufferPointer(),
			blob.errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void KochaEngine::Dx12_Blob::Init()
{
	//頂点シェーダーの読み込みとコンパイル
	auto result = D3DCompileFromFile(
		L"Shader/BasicVertexShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"VSmain", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &basicBlob.vsBlob, &basicBlob.errorBlob);
	//auto result = CompileShader("BasicVertexShader.hlsl", "VSmain", "vs_5_0", basicBlob);
	ErrorBlob(result, basicBlob);

	result = D3DCompileFromFile(
		L"Shader/TextureVertexShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"VSmain", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &texBlob.vsBlob, &texBlob.errorBlob);
	ErrorBlob(result, texBlob);

	result = D3DCompileFromFile(
		L"Shader/PMDVertexShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"VSmain", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &pmdBlob.vsBlob, &pmdBlob.errorBlob);
	ErrorBlob(result, pmdBlob);

	result = D3DCompileFromFile(
		L"Shader/FBXVertexShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"VSmain", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &fbxBlob.vsBlob, &fbxBlob.errorBlob);
	ErrorBlob(result, fbxBlob);

	result = D3DCompileFromFile(
		L"Shader/PeraVertexShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &peraBlob.vsBlob, &peraBlob.errorBlob);
	ErrorBlob(result, peraBlob);

	result = D3DCompileFromFile(
		L"Shader/BasicVertexShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"ShadowVS", "vs_5_0", //エントリーポイント名、シェーダーモデル指定
		0,0, &shadowBlob.vsBlob, &shadowBlob.errorBlob);
	ErrorBlob(result, shadowBlob);


	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Shader/BasicPixelShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"PSmain", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &basicBlob.psBlob, &basicBlob.errorBlob);
	ErrorBlob(result, basicBlob);

	result = D3DCompileFromFile(
		L"Shader/TexturePixelShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"PSmain", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &texBlob.psBlob, &texBlob.errorBlob);
	ErrorBlob(result, texBlob);

	result = D3DCompileFromFile(
		L"Shader/PMDPixelShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"PSmain", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &pmdBlob.psBlob, &pmdBlob.errorBlob);
	ErrorBlob(result, pmdBlob);

	result = D3DCompileFromFile(
		L"Shader/FBXPixelShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"PSmain", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &fbxBlob.psBlob, &fbxBlob.errorBlob);
	ErrorBlob(result, fbxBlob);

	result = D3DCompileFromFile(
		L"Shader/PeraPixelShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &peraBlob.psBlob, &peraBlob.errorBlob);
	ErrorBlob(result, peraBlob);

	result = D3DCompileFromFile(
		L"Shader/VignetteShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &vignetteBlob.psBlob, &vignetteBlob.errorBlob);
	ErrorBlob(result, vignetteBlob);

	result = D3DCompileFromFile(
		L"Shader/BloomShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &bloomBlob.psBlob, &bloomBlob.errorBlob);
	ErrorBlob(result, bloomBlob);

	result = D3DCompileFromFile(
		L"Shader/GameBoyShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &gameBoyBlob.psBlob, &gameBoyBlob.errorBlob);
	ErrorBlob(result, gameBoyBlob);

	result = D3DCompileFromFile(
		L"Shader/ChromaticAberrationShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &cAberrationBlob.psBlob, &cAberrationBlob.errorBlob);
	ErrorBlob(result, cAberrationBlob);

	result = D3DCompileFromFile(
		L"Shader/ToonShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &toonBlob.psBlob, &toonBlob.errorBlob);
	ErrorBlob(result, toonBlob);

	result = D3DCompileFromFile(
		L"Shader/GrayScaleShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &grayScaleBlob.psBlob, &grayScaleBlob.errorBlob);
	ErrorBlob(result, grayScaleBlob);

	result = D3DCompileFromFile(
		L"Shader/MosaicShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &mosaicBlob.psBlob, &mosaicBlob.errorBlob);
	ErrorBlob(result, mosaicBlob);

	result = D3DCompileFromFile(
		L"Shader/GaussianBlurShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &blurBlob.psBlob, &blurBlob.errorBlob);
	ErrorBlob(result, blurBlob);

	result = D3DCompileFromFile(
		L"Shader/DepthOfFieldShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"main", "ps_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &dofBlob.psBlob, &dofBlob.errorBlob);
	ErrorBlob(result, dofBlob);


	//ジオメトリシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Shader/BasicGeometryShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"GSmain", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &basicBlob.gsBlob, &basicBlob.errorBlob);
	ErrorBlob(result, basicBlob);

	result = D3DCompileFromFile(
		L"Shader/BasicGeometryShader.hlsl", //シェーダーファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //インクルード可能にする
		"GSmain", "gs_5_0", //エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //デバッグ用設定
		0, &shadowBlob.gsBlob, &shadowBlob.errorBlob);
	ErrorBlob(result, shadowBlob);

}