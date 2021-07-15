#include "EffectManager.h"
#include "Dx12_Wrapper.h"

#ifdef _DEBUG
#pragma comment(lib,"EffekseerRendererDX12d.lib")
#pragma comment(lib,"Effekseerd.lib")
#pragma comment(lib,"LLGId.lib")
#else
#pragma comment(lib,"EffekseerRendererDX12.lib")
#pragma comment(lib,"Effekseer.lib")
#pragma comment(lib,"LLGI.lib")
#endif

KochaEngine::EffectManager::EffectManager(Dx12_Wrapper& dx12) : dx12(dx12)
{
	Initialize();
}

KochaEngine::EffectManager::~EffectManager()
{
}

void KochaEngine::EffectManager::LoadEffect()
{
	_effect = Effekseer::Effect::Create(
		_efkManager,
		(const EFK_CHAR*)L"Resources/Effect/11/light.efk",
		1.0f,
		(const EFK_CHAR*)L"Resources/Effect/11");
}

void KochaEngine::EffectManager::Play()
{
	_efkHandle = _efkManager->Play(_effect, 0, 0, 0);
}

void KochaEngine::EffectManager::Update()
{
	_efkManager->Update();
	_efkMemoryPool->NewFrame();

	EffekseerRendererDX12::BeginCommandList(_efkCmdList, dx12.GetCmdList().Get());

	_efkRenderer->BeginRendering();
	_efkManager->Draw();
	_efkRenderer->EndRendering();

	EffekseerRendererDX12::EndCommandList(_efkCmdList);
}

void KochaEngine::EffectManager::Initialize()
{
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	_efkRenderer = EffekseerRendererDX12::Create(
		dx12.GetDevice().Get(),
		dx12.GetQue().Get(),
		2,
		&format,
		1,
		DXGI_FORMAT_D32_FLOAT,
		false,
		1000);

	//最大インスタンス数
	_efkManager = Effekseer::Manager::Create(1000);

	//座標系を左手系にする
	_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	//描画用インスタンスから描画機能を設定
	_efkManager->SetSpriteRenderer(_efkRenderer->CreateSpriteRenderer());
	_efkManager->SetRibbonRenderer(_efkRenderer->CreateRibbonRenderer());
	_efkManager->SetRingRenderer(_efkRenderer->CreateRingRenderer());
	_efkManager->SetTrackRenderer(_efkRenderer->CreateTrackRenderer());
	_efkManager->SetModelRenderer(_efkRenderer->CreateModelRenderer());

	_efkManager->SetTextureLoader(_efkRenderer->CreateTextureLoader());
	_efkManager->SetModelLoader(_efkRenderer->CreateModelLoader());
	_efkManager->SetMaterialLoader(_efkRenderer->CreateMaterialLoader());
	_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	_efkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(_efkRenderer->GetGraphicsDevice());
	_efkCmdList = EffekseerRenderer::CreateCommandList(_efkRenderer->GetGraphicsDevice(), _efkMemoryPool);

	_efkRenderer->SetCommandList(_efkCmdList);

	auto g_position = Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// 投影行列を設定
	_efkRenderer->SetProjectionMatrix(
		Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)dx12.GetWinSize().cx / (float)dx12.GetWinSize().cy, 1.0f, 500.0f));

	// カメラ行列を設定
	_efkRenderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtRH(g_position, Effekseer::Vector3D(0.0f, 0.0f, 0.0f), Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}