#include "GamePlay.h"
#include "Input.h"
#include "Util.h"
#include "Map.h"
#include "LightManager.h"
#include "Player.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
	map = new Map(gManager, camera);
	lightManager = new LightManager();
	lightManager = LightManager::Create();

	floor = new Object("graund");
	skyObj = new Object("skydome");

	fadeTexture = new Texture2D("Resources/Texture/black.png", Vector2(0, 0), Vector2(1280, 720), 0);
}

KochaEngine::GamePlay::~GamePlay()
{
	gManager->RemoveAll();
	delete camera;
	delete lightManager;
	delete gManager;
	delete pManager;
	delete emitter;
	delete map;
	delete floor;
	delete skyObj;
	delete fadeTexture;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	isBattle = false;
	isBattleEnd = false;

	gManager->RemoveAll();
	camera->Initialize(1280, 720, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(1, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	map->CreateMap(0);

	floor->SetPosition(Vector3(0, 0, 0));
	floor->SetTexture("Resources/Texture/stone.png");

	skyObj->SetScale(Vector3(8, 8, 8));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));

	frameCount = 0;
	seconds = 0;
	
	fadeFlag = false;
	fadeAlpha = 1.0f;
	endCount = 180;
}

void KochaEngine::GamePlay::Update()
{
	FadeUpdate();
	auto player = gManager->GetPlayer();
	if (player == nullptr) return;
	if (player->IsEncount() && !fadeFlag)
	{
		fadeFlag = true;
		if (fadeAlpha >= 1.0f)
		{
			player->EncountReset();
			isBattle = true;
		}
	}
	player->SetIsBattle(isBattle);

	if (isBattle)
	{
		BattleUpdate();
	}
	else
	{
		FieldUpdate();
	}
}

void KochaEngine::GamePlay::SpriteDraw()
{
	if (isBattle)
	{
		BattleSpriteDraw();
	}
	else
	{
		FieldSpriteDraw();
	}

	fadeTexture->Draw();
}

void KochaEngine::GamePlay::ObjDraw()
{
	if (isBattle)
	{
		BattleObjDraw();
	}
	else
	{
		FieldObjDraw();
	}


}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	if (isBattle)
	{
		BattleAlphaObjDraw();
	}
	else
	{
		FieldAlphaObjDraw();
	}


}

void KochaEngine::GamePlay::DrawGUI()
{
	if (isBattle)
	{
		ImGui::Text("GamePlay_Battle");
	}
	else
	{
		ImGui::Text("GamePlay_Field");
	}
}

void KochaEngine::GamePlay::Load()
{
}

KochaEngine::Scenes KochaEngine::GamePlay::Next()
{
	gManager->RemoveAll();
	return ENDING;
}

void KochaEngine::GamePlay::CountTime()
{
	frameCount++;
	if (frameCount >= 60)
	{
		seconds++;
		frameCount = 0;
	}
}

void KochaEngine::GamePlay::FadeUpdate()
{
	const float MOVE_ALPHA = 0.05f;
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
		else
		{
			fadeFlag = false;
		}
	}
	fadeTexture->SetColor(Vector4(0, 0, 0, fadeAlpha));
}

void KochaEngine::GamePlay::BattleUpdate()
{
	if (Input::TriggerKey(DIK_SPACE))
	{
		fadeFlag = true;
		isBattleEnd = true;
	}
	if (fadeAlpha >= 1.0f && isBattleEnd)
	{
		isBattle = false;
		isBattleEnd = false;
	}
}

void KochaEngine::GamePlay::BattleObjDraw()
{

}

void KochaEngine::GamePlay::BattleAlphaObjDraw()
{

}

void KochaEngine::GamePlay::BattleSpriteDraw()
{
}

void KochaEngine::GamePlay::FieldUpdate()
{
	gManager->Update();
	pManager->Update();
	camera->Update();
	lightManager->Update();

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::FieldObjDraw()
{
	gManager->ObjDraw(camera, lightManager);
	floor->Draw(camera, lightManager);
	skyObj->Draw(camera, lightManager);
	pManager->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::FieldAlphaObjDraw()
{
	gManager->AlphaObjDraw(camera, lightManager);
}

void KochaEngine::GamePlay::FieldSpriteDraw()
{
	gManager->SpriteDraw();
}
