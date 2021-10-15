#include "Application.h"
#include "Window.h"
#include "Input.h"
#include "Dx12_Wrapper.h"
#include "Dx12_Blob.h"
#include "Dx12_Descriptor.h"
#include "Dx12_Texture.h"
#include "Dx12_RootSignature.h"
#include "Dx12_Pipeline.h"
#include "Dx12_Object.h"
#include "SceneManager.h"
#include "Title.h"
#include "StageSelect.h"
#include "GamePlay.h"
#include "Ending.h"
#include "GameOver.h"
#include "Texture2D.h"
#include "Object.h"
#include "Camera.h"
#include "Util.h"
#include "FBXModel.h"
#include "FBXObject.h"
#include "FBXLoader.h"
#include "CustomGui.h"
#include "EffectManager.h"
#include "LightManager.h"

#include <sstream>
#include <mmsystem.h>
#include <omp.h>

#pragma comment(lib,"winmm.lib")


using namespace DirectX;

float KochaEngine::Application::clearColor[4] = { 0.1568f,0.1176f,0.1960f,1 };

KochaEngine::Application::Application()
{
}

KochaEngine::Application::~Application()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void KochaEngine::Application::Run()
{
	InitFPS();
	window->Show();
	MSG msg = {};
	while (1)
	{
		//メッセージがあるか
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); //キー入力メッセージの処理
			DispatchMessage(&msg); //プロシージャにメッセージを送る
		}

		//終了メッセージが来たら＆ESCAPEを押したらループを抜ける
		if (msg.message == WM_QUIT) break;
		if (Input::TriggerKey(DIK_ESCAPE)) break;

		if (UpdateFPS()) continue;

		////↓毎フレーム処理↓//
		Input::Update();

		//if (Input::CheckKey(DIK_A))
		//{
		//	camera->MoveEye(Vector3(-1, 0, 0));
		//}
		//if (Input::CheckKey(DIK_D))
		//{
		//	camera->MoveEye(Vector3(1, 0, 0));
		//}
		//if (Input::CheckKey(DIK_W))
		//{
		//	camera->MoveEye(Vector3(0, 1, 0));
		//}
		//if (Input::CheckKey(DIK_S))
		//{
		//	camera->MoveEye(Vector3(0, -1, 0));
		//}
		//if (Input::CheckKey(DIK_UP))
		//{
		//	camera->MoveEye(Vector3(0, 0, 1));
		//}
		//if (Input::CheckKey(DIK_DOWN))
		//{
		//	camera->MoveEye(Vector3(0, 0, -1));
		//}
		//if (Input::CheckKey(DIK_Q))
		//{
		//	camera->MoveTarget(Vector3(-1, 0, 0));
		//}
		//if (Input::CheckKey(DIK_E))
		//{
		//	camera->MoveTarget(Vector3(1, 0, 0));
		//}

		if (Input::TriggerKey(DIK_1))
		{
			effectManager->Play("light.efk", Vector3(0, 0, 0));
		}
		if (Input::TriggerKey(DIK_2))
		{
			effectManager->Play("hit.efk", Vector3(0, 0, 0));
		}

		sceneManager->Update();
		/*camera->Update();
		auto target = camera->GetTarget();
		auto eye = camera->GetEye();
		auto lightPos = XMLoadFloat3(&target) + XMVector3Normalize({1,1,1})
			* XMVector3Length(XMVectorSubtract(XMLoadFloat3(&target), XMLoadFloat3(&eye))).m128_f32[0];
		Vector3 lightPos2 = Vector3(lightPos.m128_f32[0], lightPos.m128_f32[1], lightPos.m128_f32[2]);
		lightCamera->SetEye(lightPos2);
		lightCamera->Update();
		MeraMera();

		lightManager->SetDirectionalLightColor(0, dirLightColor);
		lightManager->SetDirectionalLightDirection(0, dirLightDirection);
		lightManager->SetDirectionalLightIsActive(0, isActiveDirLight);
		lightManager->SetPointLightPos(0, pointLightPosition);
		lightManager->SetPointLightAtten(0, pointLightAtten);
		lightManager->SetLightCamera(lightCamera);
		lightManager->Update();*/

		for (int i = 0; i < OBJ_COUNT; ++i)
		{
			//obj[i]->MoveRotate({ 0,1.5f,0 });
		}

		////↑毎フレーム処理↑//
		
		//１パス
		{
			//////////////////////////////

			//////////////////////////////

			peraDof->PreDrawScene(dx12->GetCmdList().Get());

			//Object::BeginDrawFromLight(dx12->GetCmdList().Get());
			////peraBloom->PreDrawShadow(dx12->GetCmdList().Get());
			//floor->Draw(lightCamera);
			//sceneManager->ObjDraw();
			////for (int i = 0; i < OBJ_COUNT; ++i)
			////{
			////	obj[i]->Draw(lightCamera);
			////}
			//taimatu->Draw(lightCamera);

			Object::BeginDraw(dx12->GetCmdList().Get());
			//↓ObjDraw↓//

			sceneManager->ObjDraw();

			//↑ObjDraw↑//
			Object::EndDraw();

			Object::BeginAlphaDraw(dx12->GetCmdList().Get());

			sceneManager->AlphaObjDraw();

			//↑ObjDraw↑//
			Object::EndDraw();


			//effectManager->Update(camera);

			peraDof->PostDrawScene(dx12->GetCmdList().Get());
		}

		//２パス
		{
			peraBloom->PreDrawScene(dx12->GetCmdList().Get());

			if (isDof)
			{
				peraDof->Draw(ShaderType::DEPTH_OF_FIELD_SHADER); //被写界深度
			}
			else
			{
				peraDof->Draw();
			}

			peraBloom->PostDrawScene(dx12->GetCmdList().Get());
		}

		//３パス
		{
			peraEffect->PreDrawScene(dx12->GetCmdList().Get());

			peraBloom->Draw();

			peraEffect->PostDrawScene(dx12->GetCmdList().Get());
		}

		//４パス
		{
			dx12->BeginDraw(clearColor[0], clearColor[1], clearColor[2]);

			peraEffect->Draw(peraEffectType);

			Texture2D::BeginDraw(dx12->GetCmdList().Get());
			//↓SpriteDraw↓//

			sceneManager->SpriteDraw();
			texture[0]->Draw();

			//↑SpriteDraw↑//
			Texture2D::EndDraw();

			DrawGUI();

			dx12->EndDraw();
		}

		dx12->GetSwapchain()->Present(1, 0);
	}
}

void KochaEngine::Application::Load()
{
	//Textureのロード
	Dx12_Texture::LoadTexture("Resources/Texture/white.png");
	Dx12_Texture::LoadTexture("Resources/Texture/black.png");
	Dx12_Texture::LoadTexture("Resources/Texture/player0.png");
	Dx12_Texture::LoadTexture("Resources/Texture/green.png");
	Dx12_Texture::LoadTexture("Resources/Texture/PIEN.png");
	Dx12_Texture::LoadTexture("Resources/Texture/kaku4.png");
	Dx12_Texture::LoadTexture("Resources/Texture/number.png");
	Dx12_Texture::LoadTexture("Resources/Texture/stone.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tiling_grass.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tiling_grass1.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tiling_rock1.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tiling_rock2.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tiling_water1.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tiling_water2.png");
	Dx12_Texture::LoadTexture("Resources/Texture/water_ball1.png");
	Dx12_Texture::LoadTexture("Resources/Texture/rock1.png");
	Dx12_Texture::LoadTexture("Resources/Texture/rock2.png");
	Dx12_Texture::LoadTexture("Resources/Texture/rock3.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tree1.png");
	Dx12_Texture::LoadTexture("Resources/Texture/tree2.png");
	Dx12_Texture::LoadTexture("Resources/Texture/grass1.png");

	//.objのロード
	Dx12_Object::LoadObject("LowTree");
	Dx12_Object::LoadObject("Yukidaruma");
	Dx12_Object::LoadObject("box");
	Dx12_Object::LoadObject("plane");
	Dx12_Object::LoadObject("graund");
	Dx12_Object::LoadObject("taimatu");
	Dx12_Object::LoadObject("sphere");
	Dx12_Object::LoadObject("Ground1");
	Dx12_Object::LoadObject("skydome");
	Dx12_Object::LoadObject("saku");

}

void KochaEngine::Application::LoadScene()
{
	sceneManager->Load(TITLE);
	sceneManager->Load(STAGESELECT);
	sceneManager->Load(GAMEPLAY);
	sceneManager->Load(ENDING);
	sceneManager->Load(GAMEOVER);
	sceneManager->ChangeScene(TITLE);
}

void KochaEngine::Application::InitFPS()
{
	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);
}

void KochaEngine::Application::DrawGUI()
{
	ImGui::ShowDemoWindow();

	//シーン情報
	ImGui::Begin("Scene");
	sceneManager->DrawGUI();
	ImGui::End();

	//共通設定
	ImGui::Begin("App");
	//ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
	ImGui::Text("FPS:%f", fps);
	if (ImGui::CollapsingHeader("ClearColor"))
	{
		ImGui::ColorPicker4(" Color", clearColor);
	}
	ImGui::End();

	//DirectionalLight
	ImGui::Begin("Light");
	if (ImGui::CollapsingHeader("DirectionalLight"))
	{
		ImGui::Checkbox("isActive", &isActiveDirLight);
		ImGui::Text("Direction:");
		ImGui::SameLine();
		ImGui::DragFloat3("##0", &dirLightDirection.x, 0.1f, -1.0f, 1.0f);
	}
	if (ImGui::CollapsingHeader("PointLight"))
	{
		ImGui::Text(" Position:");
		ImGui::SameLine();
		ImGui::DragFloat3("##1", &pointLightPosition.x);
		ImGui::Text("    Atten:");
		ImGui::SameLine();
		ImGui::DragFloat3("##2", &pointLightAtten.x, 0.010f, 0.001f, 1.0f);
	}
	ImGui::End();


	//画面効果切り替え
	ImGui::Begin("PostEffect");
	ImGui::Checkbox("isDof", &isDof);
	int _effectType = peraEffectType;
	ImGui::RadioButton("None", &_effectType, ShaderType::PERA_SHADER);
	ImGui::RadioButton("Vignette", &_effectType, ShaderType::VIGNETTE_SHADER);
	ImGui::RadioButton("GameBoy", &_effectType, ShaderType::GAME_BOY_SHADER);
	ImGui::RadioButton("ChromaticAberration", &_effectType, ShaderType::CHROMATIC_ABERRATION_SHADER);
	ImGui::RadioButton("Toon", &_effectType, ShaderType::TOON_SHADER);
	ImGui::RadioButton("GrayScale", &_effectType, ShaderType::GRAY_SCALE_SHADER);
	ImGui::RadioButton("Mosaic", &_effectType, ShaderType::MOSAIC_SHADER);
	ImGui::RadioButton("GaussianBlur", &_effectType, ShaderType::GAUSSIAN_BLUR_SHADER);

	if (_effectType == ShaderType::PERA_SHADER)
	{
		peraEffectType = ShaderType::PERA_SHADER;
	}
	else if (_effectType == ShaderType::VIGNETTE_SHADER)
	{
		ImGui::Text("VignetteScale");
		ImGui::SliderFloat(" ", &vignetteScale, 0.0f, 5.0f);
		peraEffectType = ShaderType::VIGNETTE_SHADER;
		peraEffect->SetValue(vignetteScale);
	}
	else if (_effectType == ShaderType::GAME_BOY_SHADER)
	{
		ImGui::Text("PixelSize");
		ImGui::SliderFloat(" ", &gBoyPixelSize, 1.0f, 50.0f);
		peraEffectType = ShaderType::GAME_BOY_SHADER;
		peraEffect->SetValue(gBoyPixelSize);
	}
	else if (_effectType == ShaderType::CHROMATIC_ABERRATION_SHADER)
	{
		ImGui::Text("Misalignment");
		ImGui::SliderFloat(" ", &cAbeScale, 0.0f, 1.0f);
		peraEffectType = ShaderType::CHROMATIC_ABERRATION_SHADER;
		peraEffect->SetValue(cAbeScale);
	}
	else if (_effectType == ShaderType::TOON_SHADER)
	{
		peraEffectType = ShaderType::TOON_SHADER;
	}
	else if (_effectType == ShaderType::GRAY_SCALE_SHADER)
	{
		ImGui::Text("SepiaScale");
		ImGui::SliderFloat(" ", &sepiaScale, 0.0f, 1.0f);
		peraEffectType = ShaderType::GRAY_SCALE_SHADER;
		peraEffect->SetValue(sepiaScale);
	}
	else if (_effectType == ShaderType::MOSAIC_SHADER)
	{
		ImGui::Text("PixelSize");
		ImGui::SliderFloat(" ", &mosaicSize, 1.0f, 50.0f);
		peraEffectType = ShaderType::MOSAIC_SHADER;
		peraEffect->SetValue(mosaicSize);
	}
	else if (_effectType == ShaderType::GAUSSIAN_BLUR_SHADER)
	{
		ImGui::Text("BlurScale");
		ImGui::SliderFloat(" ", &blurScale, 0.0f, 5.0f);
		peraEffectType = ShaderType::GAUSSIAN_BLUR_SHADER;
		peraEffect->SetValue(blurScale);
	}

	ImGui::Text("ShaderColor");
	ImGui::ColorPicker4(" Color", color);
	shaderColor.x = color[0];
	shaderColor.y = color[1];
	shaderColor.z = color[2];
	peraEffect->SetColor(shaderColor);

	ImGui::End();
}

void KochaEngine::Application::MeraMera()
{
	const float DELTA_VALUE = 0.0015f;
	if (Util::GetRandInt() % 2)
	{
		pointLightAtten.y -= DELTA_VALUE;
	}
	else
	{
		pointLightAtten.y += DELTA_VALUE;
	}

	if (pointLightAtten.y > 0.06f)
	{
		pointLightAtten.y -= DELTA_VALUE;
	}
	if (pointLightAtten.y < 0.03f)
	{
		pointLightAtten.y += DELTA_VALUE;
	}
}

bool KochaEngine::Application::UpdateFPS()
{
	// 今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	frameTime = static_cast<double>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<double>(timeFreq.QuadPart);
	//処理時間に余裕がある場合はその分待つ
	if (frameTime < MIN_FRAME_TIME)
	{
		//Sleepの時間を計算
		DWORD sleepTime = static_cast<DWORD>((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return true;
	}
	fps = 1.000000f / frameTime;
	//std::wstringstream stream;
	//stream << "FPS:" << fps << std::endl;
	//OutputDebugString(stream.str().c_str());
	timeStart = timeEnd;
	return false;
}

bool KochaEngine::Application::Initialize()
{
	auto result = CoInitializeEx(0, COINIT_MULTITHREADED);

	window = new Window;
	window->Create();
	window->IsCreated();

	Input::Init(window->GetHwnd());

	dx12 = new Dx12_Wrapper(window->GetHwnd());
	blob = new Dx12_Blob();
	descriptor = new Dx12_Descriptor(*dx12);
	rootSignature = new Dx12_RootSignature(*dx12);
	pipeline = new Dx12_Pipeline(*dx12, *blob);

	/*camera = new Camera();
	camera->Initialize(dx12->GetWinSize().cx, dx12->GetWinSize().cy, 90, 100, { 0,50,-150 }, { 0,0,0 }, { 0,1,0 });
	lightCamera = new Camera();
	lightCamera->Initialize(dx12->GetWinSize().cx, dx12->GetWinSize().cy, 90, 100, { -100,100,-100 }, { 0,0,0 }, { 0,1,0 });*/

	auto device = dx12->GetDevice().Get();
	Texture2D::StaticInit(device, dx12->GetWinSize());
	PostEffect::StaticInit(device, dx12->GetCmdList().Get(), dx12->GetWinSize());
	Object::StaticInit(device, dx12->GetWinSize());
	LightManager::StaticInitialize(device);
	Dx12_Texture::SetDevice(device);
	Dx12_Object::SetDevice(device);

	//FBXLoader::GetInstance()->Initialize(dx12->GetDevice().Get());
	//FBXObject::SetDevice(dx12->GetDevice().Get());
	//FBXObject::SetCamera(camera);

	Load();

	sceneManager = new SceneManager();
	sceneManager->AddScene(TITLE, new Title());
	sceneManager->AddScene(STAGESELECT, new StageSelect());
	sceneManager->AddScene(GAMEPLAY, new GamePlay());
	sceneManager->AddScene(ENDING, new Ending());
	sceneManager->AddScene(GAMEOVER, new GameOver());
	//sceneManager->ChangeScene(TITLE);

	LoadScene();

	CustomGui::DefaultCustom();

	dirLightDirection = Vector3(0, 1, -1);
	dirLightColor = Vector3(1, 0.2f, 0);
	pointLightPosition = Vector3(0, 12, 0);
	pointLightColor = Vector3(1.0, 0.7, 0.0);
	pointLightAtten = Vector3(1.000f, 0.050f, 0.001f);
	isActiveDirLight = true;

	texture[0] = new Texture2D("Resources/PIEN.png", Vector2(0, 0), Vector2(100, 100), 0);

	peraBloom = new PostEffect();
	peraEffect = new PostEffect();
	peraDof = new PostEffect();
	peraEffectType = ShaderType::VIGNETTE_SHADER;
	isDof = true;

	effectManager = new EffectManager(*dx12);
	effectManager->LoadEffect("light.efk", 10.0f);
	effectManager->LoadEffect("hit.efk", 10.0f);

	//lightManager = LightManager::Create();
	//lightManager->SetLightCamera(lightCamera);
	////Object::SetLightManager(lightManager);


	/*lightManager->SetPointLightColor(0, pointLightColor);
	lightManager->SetPointLightAtten(0, pointLightAtten);*/

	vignetteScale = 0.2f;
	gBoyPixelSize = 4.0f;
	mosaicSize = 4.0f;
	sepiaScale = 0.2f;
	blurScale = 2.0f;
	cAbeScale = 0.4f;



	shaderColor = Vector4(1, 1, 1, 1);

	return true;
}

void KochaEngine::Application::Terminate()
{
	sceneManager->Terminate();
	delete sceneManager;
	delete texture[0];
	delete blob;
	delete descriptor;
	delete rootSignature;
	delete pipeline;
	delete dx12;
	delete window;
	//delete camera;
	//delete lightCamera;
	delete fbxModel;
	//for (int i = 0; i < FBX_COUNT; i++)
	//{
	//	delete fbxObject[i];
	//}
	delete peraBloom;
	delete peraEffect;
	delete peraDof;
	delete effectManager;
	//delete lightManager;

	Input::Terminate();
	//FBXLoader::GetInstance()->Finalize();
}

KochaEngine::Application& KochaEngine::Application::Instance()
{
	static Application instance;
	return instance;
}

SIZE KochaEngine::Application::GetWindowSize()const
{
	SIZE ret;
	ret.cx = window->GetWIN_WIDTH();
	ret.cy = window->GetWIN_HEIGHT();
	return ret;
}