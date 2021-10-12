#include "../Header/EffectManager.h"
#include "../Header/Dx12_Wrapper.h"
#include "../Header/Camera.h"
#include "../Header/Util.h"

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

void KochaEngine::EffectManager::Update(Camera* camera)
{
	if (camera == nullptr) return;

	auto cameraEye = camera->GetEye();
	auto cameraTarget = camera->GetTarget();
	auto cameraUp = camera->GetUp();

	auto eye = Effekseer::Vector3D(cameraEye.x, cameraEye.y, cameraEye.z);
	auto target = Effekseer::Vector3D(cameraTarget.x, cameraTarget.y, cameraTarget.z);
	auto up = Effekseer::Vector3D(cameraUp.x, cameraUp.y, cameraUp.z);

	auto matrix = Effekseer::Matrix44().LookAtLH(eye, target, up);

	// �J�����s���ݒ�
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

	//�ő�C���X�^���X��
	_efkManager = Effekseer::Manager::Create(1000);

	//���W�n������n�ɂ���
	_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	//�`��p�C���X�^���X����`��@�\��ݒ�
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

	// ���e�s���ݒ�
	_efkRenderer->SetProjectionMatrix(
		Effekseer::Matrix44().PerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), (float)dx12.GetWinSize().cx / (float)dx12.GetWinSize().cy, 0.1f, 1500.0f));

	// �J�����s���ݒ�
	_efkRenderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtLH(g_position, Effekseer::Vector3D(0.0f, 0.0f, 0.0f), Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));
}