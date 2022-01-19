#include "Title.h"
#include "Texture2D.h"
#include "Input.h"
#include "Util.h"
#include "LightManager.h"
#include "GameObjectManager.h"
#include "GameSetting.h"
#include "JsonLoader.h"

KochaEngine::Title::Title()
{
	camera = new Camera();
	lightManager = new LightManager();
	lightManager = LightManager::Create();

	backTexture = new Texture2D("Resources/Texture/Engine/titleTexture.png", Vector2(0, 0), Vector2(1280, 720), 0);
	fadeTexture = new Texture2D("Resources/Texture/Color/white.png", Vector2(0, 0), Vector2(1280, 720), 0);
	plane = new Object("ground");

	LoadLevelEditorData();
}

KochaEngine::Title::~Title()
{
	auto end = _objects.end();
	for (auto it = _objects.begin(); it != end; ++it)
	{
		delete (*it);
	}
	_objects.clear();
	delete camera;
	delete lightManager;
	delete backTexture;
	delete fadeTexture;
	delete plane;
}

void KochaEngine::Title::Initialize()
{
	isEnd = false;
	bgmFlag = false;

	fadeAlpha = 1.0f;
	fadeFlag = false;

	cameraPos = Vector3(0, 5, -20);
	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, cameraPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	backTexture->SetColor(Vector4(1, 1, 1, 1));
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));

	plane->SetPosition(Vector3(0, 0, 0));
	plane->SetTexture("Resources/Texture/Tiling/tiling_floor2.png");
}

void KochaEngine::Title::Update()
{

	FadeUpdate();

	camera->SetEye(cameraPos);
	camera->SetTarget(Vector3(cameraPos.x, cameraPos.y - 5, cameraPos.z + 20));
	camera->Update();
	lightManager->Update();
	//if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_A)) && fadeAlpha <= 0.0f)
	//{
	//	fadeFlag = true;
	//}
	//if (fadeFlag && fadeAlpha >= 1.0f)
	//{
	//	isEnd = true;
	//}
}

void KochaEngine::Title::SpriteDraw()
{
	//backTexture->Draw();
	//fadeTexture->Draw();
}

void KochaEngine::Title::ObjDraw()
{
	plane->Draw(camera, lightManager);

	auto end = _objects.end();
	for (auto itr = _objects.begin(); itr != end; ++itr)
	{
		(*itr)->Draw(camera, lightManager);
	}
}

void KochaEngine::Title::DrawGUI()
{
	ImGui::Text("Title");
	ImGui::InputFloat("#cameraPosX", &cameraPos.x, 1.0f);
	ImGui::InputFloat("#cameraPosY", &cameraPos.y, 1.0f);
	ImGui::InputFloat("#cameraPosZ", &cameraPos.z, 1.0f);
}

void KochaEngine::Title::Load()
{
	
}

KochaEngine::Scenes KochaEngine::Title::Next()
{
	return GAMEPLAY;
}

void KochaEngine::Title::FadeUpdate()
{
	const float MOVE_ALPHA = 0.02f;
	if (!fadeFlag)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= MOVE_ALPHA;
		}
	}
	else
	{
		if (fadeAlpha < 1.0f)
		{
			fadeAlpha += MOVE_ALPHA;
		}
	}
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, fadeAlpha));
}

void KochaEngine::Title::LoadLevelEditorData()
{
	auto levelEditorData = JsonLoader::ReadJsonObj("Resources/DataBase/LevelEditorData.json");
	for (const auto& p : levelEditorData)
	{
		if (p.first != "objects") continue;

		picojson::array& objectArray = levelEditorData[p.first].get<picojson::array>();
		for (const auto& objects : objectArray)
		{
			picojson::object _object = objects.get<picojson::object>();
			for (const auto& o : _object)
			{
				std::string objType = _object["type"].get<std::string>();
				if (objType != "MESH") break;
				Vector3 _position;
				Vector3 _rotate;
				Vector3 _scale;

				picojson::object _transform = _object["transform"].get<picojson::object>();
				for (const auto& t : _transform)
				{
					picojson::array& transformArray = _transform[t.first].get<picojson::array>();
					int count = 0;
					Vector3 anyTransform;
					for (const auto& tA : transformArray)
					{
						switch (count)
						{
						case 0:
							anyTransform.x = tA.get<double>();
							break;
						case 1:
							anyTransform.z = tA.get<double>();
							break;
						case 2:
							anyTransform.y = tA.get<double>();
							break;
						default:
							break;
						}

						count++;
					}

					if (t.first == "translation")
					{
						_position = anyTransform;
					}
					else if (t.first == "rotation")
					{
						_rotate = anyTransform;
					}
					else if (t.first == "scaling")
					{
						_scale = anyTransform;
					}
				}

				//ここで描画用オブジェクト生成
				_objects.push_back(new Object("box", _position, _rotate, _scale));

			}
		}

	}

	auto end = _objects.end();
	for (auto itr = _objects.begin(); itr != end; ++itr)
	{
		(*itr)->SetTexture("Resources/Texture/Color/white.png");
	}
}
