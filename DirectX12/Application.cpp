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
//#include "PMDLoader.h"
//#include "PMDModel.h"
#include "FBXModel.h"
#include "FBXObject.h"
#include "FBXLoader.h"

#include <sstream>
#include <mmsystem.h>
#include <omp.h>

#pragma comment(lib,"winmm.lib")

using namespace DirectX;

float KochaEngine::Application::clearColor[4] = { 0,0,0,1 };

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

		//sceneManager->Update();


		for (int i = 0; i < OBJ_COUNT; ++i)
		{
			obj[i]->MoveRotate({ 0,0.2f,0 });
		}



		////↑毎フレーム処理↑//
		
		//１パス
		{
			peraBloom->PreDrawScene(dx12->GetCmdList().Get());
			Object::BeginDraw(dx12->GetCmdList().Get());
			//↓ObjDraw↓//

			//sceneManager->ObjDraw();
			for (int i = 0; i < OBJ_COUNT; ++i)
			{
				obj[i]->Draw(camera);
			}

			//↑ObjDraw↑//
			Object::EndDraw();

			//PMDModel::BeginDraw(dx12->GetCmdList().Get());

			////model->Draw(camera);

			//PMDModel::EndDraw();


			//for (int i = 0; i < FBX_COUNT; i++)
			//{
			//	//fbxObject[i]->MoveRotate({ 2,2,2 });
			//	fbxObject[i]->Draw(dx12->GetCmdList().Get());
			//}

			peraBloom->PostDrawScene(dx12->GetCmdList().Get());
		}

		//２パス
		{
			peraEffect->PreDrawScene(dx12->GetCmdList().Get());
			if (isBloom)
			{
				peraBloom->Draw(ShaderType::BLOOM_SHADER); //ブルーム
			}
			else
			{
				peraBloom->Draw();
			}
			peraEffect->PostDrawScene(dx12->GetCmdList().Get());
		}

		//３パス
		{
			dx12->BeginDraw(clearColor[0], clearColor[1], clearColor[2]);

			peraEffect->Draw(peraEffectType);

			Texture2D::BeginDraw(dx12->GetCmdList().Get());
			//↓SpriteDraw↓//

			sceneManager->SpriteDraw();
			texture->Draw();

			//↑SpriteDraw↑//
			Texture2D::EndDraw();

			ImGui::Begin("Sample");
			//ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);
			ImGui::Text("FPS:%f", fps);
			ImGui::Text("ClearColor");
			ImGui::ColorPicker4(" Color", clearColor);
			ImGui::End();

			ImGui::Begin("PostEffect");
			ImGui::Checkbox("isBloom", &isBloom);

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

			dx12->EndDraw();
		}

		dx12->GetSwapchain()->Present(1, 0);
	}
}

void KochaEngine::Application::Load()
{
	//Textureのロード
	Dx12_Texture::LoadTexture(dx12->GetDevice().Get(), "Resources/white.png");
	Dx12_Texture::LoadTexture(dx12->GetDevice().Get(), "Resources/PIEN.png");
	Dx12_Texture::LoadTexture(dx12->GetDevice().Get(), "Resources/kaku4.png");
	Dx12_Texture::LoadTexture(dx12->GetDevice().Get(), "Resources/number.png");

	//.objのロード
	Dx12_Object::LoadObject(dx12->GetDevice().Get(), "Yukidaruma");
	Dx12_Object::LoadObject(dx12->GetDevice().Get(), "box");
	Dx12_Object::LoadObject(dx12->GetDevice().Get(), "LowTree");

	//.pmdのロード *日本語！ダメ！絶対！*
	//PMDLoader::LoadModel(dx12->GetDevice().Get(), "Resources/Model/miku/miku.pmd");
	//PMDLoader::LoadModel(dx12->GetDevice().Get(), "Resources/Model/kirama/kirama.pmd");

	//.fbxのロード
	//FBXLoader::GetInstance()->LoadModelFromFile("cube");
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
	fps = 1.0f / frameTime;
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

	camera = new Camera();
	camera->Initialize(dx12->GetWinSize().cx, dx12->GetWinSize().cy, 90, 100, { 0,30,-80 }, { 0,5,0 }, { 0,1,0 });

	Texture2D::StaticInit(dx12->GetDevice().Get(), dx12->GetWinSize());
	PostEffect::StaticInit(dx12->GetDevice().Get(), dx12->GetCmdList().Get(), dx12->GetWinSize());
	Object::StaticInit(dx12->GetDevice().Get(), dx12->GetWinSize());
	//PMDModel::StaticInit(dx12->GetDevice().Get(), dx12->GetWinSize());
	FBXLoader::GetInstance()->Initialize(dx12->GetDevice().Get());
	FBXObject::SetDevice(dx12->GetDevice().Get());
	FBXObject::SetCamera(camera);

	Load();

	sceneManager = new SceneManager();
	sceneManager->AddScene(TITLE, new Title());
	sceneManager->AddScene(STAGESELECT, new StageSelect());
	sceneManager->AddScene(GAMEPLAY, new GamePlay());
	sceneManager->AddScene(ENDING, new Ending());
	sceneManager->AddScene(GAMEOVER, new GameOver());
	sceneManager->ChangeScene(TITLE);

	LoadScene();

	texture = new Texture2D("Resources/PIEN.png", Vector2(0, 0), Vector2(100, 100), 0);
	for (int i = 0; i < OBJ_COUNT; ++i)
	{
		obj[i] = new Object("Yukidaruma");
		obj[i]->SetRotate({ 0,180,0 });
		obj[i]->SetScale({ 6, 6, 6 });
		obj[i]->SetPosition({ (float)Util::GetIntRand(0,100) - 50.0f,0,(float)Util::GetIntRand(0,100) - 50.0f });
		obj[i]->MoveRotate({ 0,(float)Util::GetIntRand(0,360),0 });
	}
	//model = new PMDModel("Resources/Model/kirama/kirama.pmd");
	////model->SetScale({ 1.5f,1.5f,1.5f });
	//model->SetPosition({ 0,-10,0 });

	fbxModel = FBXLoader::GetInstance()->LoadModelFromFile("boneTest");

	for (int i = 0; i < FBX_COUNT; i++)
	{
		fbxObject[i] = new FBXObject();
		fbxObject[i]->SetModel(fbxModel);
		fbxObject[i]->SetPosition({ (float)Util::GetIntRand(0,40) - 20.0f,0,(float)Util::GetIntRand(0,40) - 20.0f });
		fbxObject[i]->MoveRotate({ 0,(float)Util::GetIntRand(0,360),0 });
		fbxObject[i]->PlayAnimation();
	}

	peraBloom = new PostEffect();
	peraEffect = new PostEffect();
	peraEffectType = ShaderType::PERA_SHADER;
	isBloom = true;

	vignetteScale = 0.25f;
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
	delete texture;
	delete blob;
	delete descriptor;
	delete rootSignature;
	delete pipeline;
	delete dx12;
	delete window;
	for (int i = 0; i < OBJ_COUNT; ++i)
	{
		delete obj[i];
	}
	delete model;
	delete camera;
	delete fbxModel;
	for (int i = 0; i < FBX_COUNT; i++)
	{
		delete fbxObject[i];
	}
	delete peraBloom;
	delete peraEffect;

	Input::Terminate();
	FBXLoader::GetInstance()->Finalize();
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