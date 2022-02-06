#include "EffectManager.h"
#include "Dx12_Wrapper.h"
#include "Camera.h"
#include "Util.h"

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

void KochaEngine::EffectManager::LoadEffect(const std::string& arg_efkName, const float arg_scale)
{
	std::string efkName = arg_efkName;

	std::string commonPath = "Resources/Effect";
	wchar_t wMaterialPath[128];
	MultiByteToWideChar(CP_ACP, 0, commonPath.c_str(), -1, wMaterialPath, _countof(wMaterialPath));
	auto materialPath = (const char16_t*)wMaterialPath;

	wchar_t wPath[128];
	std::string fullPath = "Resources/Effect/" + efkName;
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wPath, _countof(wPath));
	auto path = (const char16_t*)wPath;

	_effect[efkName] = Effekseer::Effect::Create(
		_efkManager,
		path,
		arg_scale,
		materialPath);
}

void KochaEngine::EffectManager::Play(const std::string& arg_efkName, const Vector3& arg_position)
{
	std::string efkName = arg_efkName;
	_efkHandle = _efkManager->Play(_effect[efkName], arg_position.x, arg_position.y, arg_position.z);
}

void KochaEngine::EffectManager::Update(Camera* arg_camera)
{
	if (!arg_camera) return;

	auto cameraEye = arg_camera->GetEye();
	auto cameraTarget = arg_camera->GetTarget();
	auto cameraUp = arg_camera->GetUp();

	auto eye = Effekseer::Vector3D(cameraEye.x, cameraEye.y, cameraEye.z);
	auto target = Effekseer::Vector3D(cameraTarget.x, cameraTarget.y, cameraTarget.z);
	auto up = Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);

	auto matrix = Effekseer::Matrix44().LookAtLH(eye, target, up);

	// カメラ行列を設定
	_efkRenderer->SetCameraMatrix(matrix);

	_efkManager->Update();
	_efkMemoryPool->NewFrame();

	EffekseerRendererDX12::BeginCommandList(_efkCmdList, dx12.GetCmdList().Get());

	_efkRenderer->BeginRendering();
	_efkManager->Draw();
	_efkRenderer->EndRendering();

	EffekseerRendererDX12::EndCommandList(_efkCmdList);
}

void KochaEngine::EffectManager::SetPosition(const Vector3& arg_position)
{
}

void KochaEngine::EffectManager::SetScale(const float arg_scale)
{
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

	auto g_position = Effekseer::Vector3D(0, 50, -150);

	// 投影行列を設定
	_efkRenderer->SetProjectionMatrix(
		Effekseer::Matrix44().PerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)dx12.GetWinSize().cx / (float)dx12.GetWinSize().cy, 0.1f, 1500.0f));

	// カメラ行列を設定
	_efkRenderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtLH(g_position, Effekseer::Vector3D(0.0f, 0.0f, 0.0f), Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}