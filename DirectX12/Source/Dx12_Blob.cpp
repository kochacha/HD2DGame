#include "../Header/Dx12_Blob.h"
#include "../Header/Dx12_Wrapper.h"
#include "../Header/Util.h"
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
		shaderName, //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		arg_EntryPoint.c_str(), arg_ShaderModel.c_str(), //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &arg_Blob.vsBlob, &arg_Blob.errorBlob);
	return result;
}

void KochaEngine::Dx12_Blob::ErrorBlob(const HRESULT& result, const Blob& blob)
{
	//�V�F�[�_�[�̃G���[���e��\��
	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(blob.errorBlob->GetBufferSize());
		std::copy_n((char*)blob.errorBlob->GetBufferPointer(),
			blob.errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//�G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}
}

void KochaEngine::Dx12_Blob::Init()
{
	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	auto result = D3DCompileFromFile(
		L"BasicVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &basicBlob.vsBlob, &basicBlob.errorBlob);
	ErrorBlob(result, basicBlob);

	result = D3DCompileFromFile(
		L"TextureVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &texBlob.vsBlob, &texBlob.errorBlob);
	ErrorBlob(result, texBlob);

	result = D3DCompileFromFile(
		L"PMDVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &pmdBlob.vsBlob, &pmdBlob.errorBlob);
	ErrorBlob(result, pmdBlob);

	result = D3DCompileFromFile(
		L"FBXVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"VSmain", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &fbxBlob.vsBlob, &fbxBlob.errorBlob);
	ErrorBlob(result, fbxBlob);

	result = D3DCompileFromFile(
		L"PeraVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &peraBlob.vsBlob, &peraBlob.errorBlob);
	ErrorBlob(result, peraBlob);

	result = D3DCompileFromFile(
		L"BasicVertexShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"ShadowVS", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		0,0, &shadowBlob.vsBlob, &shadowBlob.errorBlob);
	ErrorBlob(result, shadowBlob);


	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicPixelShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &basicBlob.psBlob, &basicBlob.errorBlob);
	ErrorBlob(result, basicBlob);

	result = D3DCompileFromFile(
		L"TexturePixelShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &texBlob.psBlob, &texBlob.errorBlob);
	ErrorBlob(result, texBlob);

	result = D3DCompileFromFile(
		L"PMDPixelShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &pmdBlob.psBlob, &pmdBlob.errorBlob);
	ErrorBlob(result, pmdBlob);

	result = D3DCompileFromFile(
		L"FBXPixelShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"PSmain", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &fbxBlob.psBlob, &fbxBlob.errorBlob);
	ErrorBlob(result, fbxBlob);

	result = D3DCompileFromFile(
		L"PeraPixelShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &peraBlob.psBlob, &peraBlob.errorBlob);
	ErrorBlob(result, peraBlob);

	result = D3DCompileFromFile(
		L"VignetteShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &vignetteBlob.psBlob, &vignetteBlob.errorBlob);
	ErrorBlob(result, vignetteBlob);

	result = D3DCompileFromFile(
		L"BloomShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &bloomBlob.psBlob, &bloomBlob.errorBlob);
	ErrorBlob(result, bloomBlob);

	result = D3DCompileFromFile(
		L"GameBoyShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &gameBoyBlob.psBlob, &gameBoyBlob.errorBlob);
	ErrorBlob(result, gameBoyBlob);

	result = D3DCompileFromFile(
		L"ChromaticAberrationShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &cAberrationBlob.psBlob, &cAberrationBlob.errorBlob);
	ErrorBlob(result, cAberrationBlob);

	result = D3DCompileFromFile(
		L"ToonShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &toonBlob.psBlob, &toonBlob.errorBlob);
	ErrorBlob(result, toonBlob);

	result = D3DCompileFromFile(
		L"GrayScaleShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &grayScaleBlob.psBlob, &grayScaleBlob.errorBlob);
	ErrorBlob(result, grayScaleBlob);

	result = D3DCompileFromFile(
		L"MosaicShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &mosaicBlob.psBlob, &mosaicBlob.errorBlob);
	ErrorBlob(result, mosaicBlob);

	result = D3DCompileFromFile(
		L"GaussianBlurShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &blurBlob.psBlob, &blurBlob.errorBlob);
	ErrorBlob(result, blurBlob);

	result = D3DCompileFromFile(
		L"DepthOfFieldShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"main", "ps_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &dofBlob.psBlob, &dofBlob.errorBlob);
	ErrorBlob(result, dofBlob);


	//�W�I���g���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicGeometryShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"GSmain", "gs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &basicBlob.gsBlob, &basicBlob.errorBlob);
	ErrorBlob(result, basicBlob);

	result = D3DCompileFromFile(
		L"BasicGeometryShader.hlsl", //�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�\�ɂ���
		"GSmain", "gs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, //�f�o�b�O�p�ݒ�
		0, &shadowBlob.gsBlob, &shadowBlob.errorBlob);
	ErrorBlob(result, shadowBlob);

}