#include "Dx12_Pipeline.h"
#include "Dx12_Wrapper.h"
#include "Dx12_Blob.h"
#include "Dx12_RootSignature.h"

ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::spritePipelineState;
ComPtr<ID3D12PipelineState> KochaEngine::Dx12_Pipeline::objPipelineState;
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

KochaEngine::Dx12_Pipeline::Dx12_Pipeline(Dx12_Wrapper& dx12, Dx12_Blob& blob) : dx12(dx12), blob(blob)
{
	CreateSpriteGraphicsPipelineState();
	CreateOBJGraphicsPipelineState();
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

	// グラフィックスパイプラインの流れを設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetTexBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetTexBlob().psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = spriteInputLayout;
	gpipeline.InputLayout.NumElements = _countof(spriteInputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	gpipeline.pRootSignature = Dx12_RootSignature::GetSpriteRootSignature().Get();

	auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelineState));
	if (FAILED(result)) { assert(0); }
}

void KochaEngine::Dx12_Pipeline::CreateOBJGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ // xy座標
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // 法線ベクトル
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // uv座標
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().psBlob.Get());
	gpipeline.GS = CD3DX12_SHADER_BYTECODE(blob.GetBasicBlob().gsBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;
	gpipeline.BlendState.RenderTarget[2] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;	// 描画対象は2つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

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

void KochaEngine::Dx12_Pipeline::CreatePMDGraphicsPipelineState()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ // xy座標
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // 法線ベクトル
		"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{ // uv座標
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
    { // ボーン番号
		"BONE_NO",0,DXGI_FORMAT_R16G16_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{ // ウェイト
		"WEIGHT",0,DXGI_FORMAT_R8_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
    { // 輪郭線フラグ
		"EDGE_FLG",0,DXGI_FORMAT_R8_UINT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	}
	};

	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetPMDBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetPMDBlob().psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//まだアンチエイリアスは使わないためfalse
	gpipeline.RasterizerState.MultisampleEnable = false;

	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //カリングしない
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; //中身を塗りつぶす
	gpipeline.RasterizerState.DepthClipEnable = true; //深度方向のクリッピングは有効に

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.BlendState.IndependentBlendEnable = false;

	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
	renderTargetBlendDesc.BlendEnable = false;
	renderTargetBlendDesc.LogicOpEnable = false;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

	//入力レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout; //レイアウト先頭アドレス
	gpipeline.InputLayout.NumElements = _countof(inputLayout); //レイアウト配列の要素数

	gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED; //カット無し
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; //三角形で構成

	//レンダーターゲットの設定
	gpipeline.NumRenderTargets = 1; //今は一つのみ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～1に正規化されたRGBA
	//↑レンダーターゲットは１つなので０番を指定

	//アンチエイリアシングのためのサンプル数設定(今回はアンチエイリアシングしない)
	gpipeline.SampleDesc.Count = 1; //サンプリングは１ピクセルにつき１
	gpipeline.SampleDesc.Quality = 0; //クオリティは最低

	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.StencilEnable = false;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; //書き込む
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS; //小さい方を採用
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
		{ // xy座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 影響を受けるボーン番号(４つ)
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // ボーンのスキンウェイト(４つ)
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetFBXBlob().vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetFBXBlob().psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blendDesc.BlendEnable = true;
	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blendDesc;
	gpipeline.BlendState.RenderTarget[1] = blendDesc;
	gpipeline.BlendState.RenderTarget[2] = blendDesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 3;    // 描画対象は3つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.RTVFormats[2] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	gpipeline.pRootSignature = Dx12_RootSignature::GetFBXRootSignature().Get();

	// グラフィックスパイプラインの生成
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

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = spriteInputLayout;
	gpipeline.InputLayout.NumElements = _countof(spriteInputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	gpipeline.pRootSignature = Dx12_RootSignature::GetPeraRootSignature().Get();

	// グラフィックスパイプラインの流れを設定
	{
		//ペラポリゴン用
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(blob.GetPeraBlob().vsBlob.Get());
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetPeraBlob().psBlob.Get());
		auto result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&peraPipelineState));
		if (FAILED(result)) { assert(0); }

		//ビネット用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetVignetteBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&vignettePipelineState));
		if (FAILED(result)) { assert(0); }

		//ブルーム用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBloomBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&bloomPipelineState));
		if (FAILED(result)) { assert(0); }

		//ゲームボーイ風用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetGameBoyBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&gameBoyPipelineState));
		if (FAILED(result)) { assert(0); }

		//色収差用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetCAbeBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&cAbePipelineState));
		if (FAILED(result)) { assert(0); }

		//階調用(トゥーン)
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetToonBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&toonPipelineState));
		if (FAILED(result)) { assert(0); }

		//グレースケール(セピア)用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetGrayScaleBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&grayScalePipelineState));
		if (FAILED(result)) { assert(0); }

		//モザイク用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetMosaicBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&mosaicPipelineState));
		if (FAILED(result)) { assert(0); }

		//ガウシアンブラー用
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(blob.GetBlurBlob().psBlob.Get());
		result = dx12.GetDevice().Get()->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&blurPipelineState));
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
