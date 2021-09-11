#include "Dx12_Pipeline.h"
#include "Dx12_Wrapper.h"
#include "Dx12_Blob.h"
#include "Dx12_RootSignature.h"

ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::spritePipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::objPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::alphaObjPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::pmdPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::fbxPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::peraPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::shadowPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::vignettePipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::bloomPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::gameBoyPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::cAbePipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::toonPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::grayScalePipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::mosaicPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::blurPipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::dofPipelineState;

KochaEngine::Dx12_Pipeline::Dx12_Pipeline(Dx12_Wrapper& dx12, Dx12_Blob& blob) : dx12(dx12), blob(blob)
{
	CreateSpriteGraphicsPipelineState();
	CreateOBJGraphicsPipelineState();
	CreateAlphaOBJGraphicsPipelineState();
	CreatePMDGraphicsPipelineState();
	CreateFBXGraphicsPipelineState();
	CreatePeraGraphicsPipelineState();
}

KochaEngine::Dx12_Pipeline::~Dx12_Pipeline()
{
}

void KochaEngine::Dx12_Pipeline::CreateSpriteGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC spriteInputLayout[] =
	{
		{
		  "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		   D3D12_APPEND_ALIGNED_ELEMENT,
		   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
		  "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		   D3D12_APPEND_ALIGNED_ELEMENT,
		   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetTexBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetTexBlob().psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // ��ɏ㏑�����[��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = spriteInputLayout;
	gpipeline.InputLayout.NumElements = _countof(spriteInputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	gpipeline.pRootSignature = Dx12_RootSignature::GetSpriteRootSignature().Get();

	auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelineState));
	if (FAILED(result)) { assert(0); }
}

void KochaEngine::Dx12_Pipeline::CreateOBJGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ // xy���W
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // �@���x�N�g��
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // uv���W
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().gsBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;
	gpipeline.BlendState.RenderTarget[2] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;	// �`��Ώۂ�3��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.pRootSignature = Dx12_RootSignature::GetOBJRootSignature().Get();

	auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&objPipelineState));
	if (FAILED(result)) { assert(0); }

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetShadowBlob().vsBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(blob.GetShadowBlob().gsBlob.Get());
	gpipeline.PS.BytecodeLength = 0;
	gpipeline.PS.pShaderBytecode = nullptr;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
	result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&shadowPipelineState));
	if (FAILED(result)) { assert(0); }
}

void KochaEngine::Dx12_Pipeline::CreateAlphaOBJGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ // xy���W
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // �@���x�N�g��
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // uv���W
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().gsBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;
	gpipeline.BlendState.RenderTarget[2] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;	// �`��Ώۂ�3��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipeline.DepthStencilState.StencilEnable = false;

	gpipeline.pRootSignature = Dx12_RootSignature::GetOBJRootSignature().Get();

	auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&alphaObjPipelineState));
	if (FAILED(result)) { assert(0); }
}

void KochaEngine::Dx12_Pipeline::CreatePMDGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ // xy���W
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // �@���x�N�g��
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // uv���W
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
    { // �{�[���ԍ�
		"BONE_NO",0,DXGI_FORMAT_R16G16_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{ // �E�F�C�g
		"WEIGHT",0,DXGI_FORMAT_R8_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
    { // �֊s���t���O
		"EDGE_FLG",0,DXGI_FORMAT_R8_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	}
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetPMDBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetPMDBlob().psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//�܂��A���`�G�C���A�X�͎g��Ȃ�����false
	gpipeline.RasterizerState.MultisampleEnable = false;

	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //�J�����O���Ȃ�
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; //���g��h��Ԃ�
	gpipeline.RasterizerState.DepthClipEnable = true; //�[�x�����̃N���b�s���O�͗L����

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.LogicOpEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

	//���̓��C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout; //���C�A�E�g�擪�A�h���X
	gpipeline.InputLayout.NumElements = _countof(inputLayout); //���C�A�E�g�z��̗v�f��

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED; //�J�b�g����
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; //�O�p�`�ō\��

	//�����_�[�^�[�Q�b�g�̐ݒ�
	gpipeline.NumRenderTargets = 1; //���͈�̂�
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`1�ɐ��K�����ꂽRGBA
	//�������_�[�^�[�Q�b�g�͂P�Ȃ̂łO�Ԃ��w��

	//�A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�(����̓A���`�G�C���A�V���O���Ȃ�)
	gpipeline.SampleDesc.Count = 1; //�T���v�����O�͂P�s�N�Z���ɂ��P
	gpipeline.SampleDesc.Quality = 0; //�N�I���e�B�͍Œ�

	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.StencilEnable = false;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; //��������
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; //�����������̗p
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	gpipeline.pRootSignature = Dx12_RootSignature::GetPMDRootSignature().Get();

	auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pmdPipelineState));
	if (FAILED(result)) { assert(0); }
}

void KochaEngine::Dx12_Pipeline::CreateFBXGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �@���x�N�g��
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �e�����󂯂�{�[���ԍ�(�S��)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // �{�[���̃X�L���E�F�C�g(�S��)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetFBXBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetFBXBlob().psBlob.Get());

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA�S�Ẵ`�����l����`��
	blendDesc.BlendEnable = true;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blendDesc;
	gpipeline.BlendState.RenderTarget[1] = blendDesc;
	gpipeline.BlendState.RenderTarget[2] = blendDesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // �`��Ώۂ�3��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	gpipeline.pRootSignature = Dx12_RootSignature::GetFBXRootSignature().Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&fbxPipelineState));
	if (FAILED(result)) { assert(0); }
}

void KochaEngine::Dx12_Pipeline::CreatePeraGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC spriteInputLayout[] =
	{
		{
		  "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		   D3D12_APPEND_ALIGNED_ELEMENT,
		   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{
		  "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
		   D3D12_APPEND_ALIGNED_ELEMENT,
		   D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
	};

	// �T���v���}�X�N
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
	// ���X�^���C�U�X�e�[�g
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// �f�v�X�X�e���V���X�e�[�g
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // ��ɏ㏑�����[��

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// �u�����h�X�e�[�g�̐ݒ�
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = spriteInputLayout;
	gpipeline.InputLayout.NumElements = _countof(spriteInputLayout);

	// �}�`�̌`��ݒ�i�O�p�`�j
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	gpipeline.pRootSignature = Dx12_RootSignature::GetPeraRootSignature().Get();

	// �O���t�B�b�N�X�p�C�v���C���̗����ݒ�
	{
		//�y���|���S���p
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetPeraBlob().vsBlob.Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetPeraBlob().psBlob.Get());
		auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&peraPipelineState));
		if (FAILED(result)) { assert(0); }

		//�r�l�b�g�p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetVignetteBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&vignettePipelineState));
		if (FAILED(result)) { assert(0); }

		//�u���[���p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBloomBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&bloomPipelineState));
		if (FAILED(result)) { assert(0); }

		//�Q�[���{�[�C���p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetGameBoyBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&gameBoyPipelineState));
		if (FAILED(result)) { assert(0); }

		//�F�����p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetCAbeBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&cAbePipelineState));
		if (FAILED(result)) { assert(0); }

		//�K���p(�g�D�[��)
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetToonBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&toonPipelineState));
		if (FAILED(result)) { assert(0); }

		//�O���[�X�P�[��(�Z�s�A)�p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetGrayScaleBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&grayScalePipelineState));
		if (FAILED(result)) { assert(0); }

		//���U�C�N�p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetMosaicBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&mosaicPipelineState));
		if (FAILED(result)) { assert(0); }

		//�K�E�V�A���u���[�p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBlurBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&blurPipelineState));
		if (FAILED(result)) { assert(0); }

		//��ʊE�[�x�p
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetDofBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&dofPipelineState));
		if (FAILED(result)) { assert(0); }

		//gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetShadowBlob().vsBlob.Get());
		//gpipeline.PS.BytecodeLength = 0;
		//gpipeline.PS.pShaderBytecode = nullptr;
		//gpipeline.NumRenderTargets = 0;
		//gpipeline.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
		//result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&shadowPipelineState));
		//if (FAILED(result)) { assert(0); }
	}
}
