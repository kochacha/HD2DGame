#include "StageSelect.h"
#include "Input.h"
#include "Util.h"

KochaEngine::StageSelect::StageSelect()
{
	camera = new Camera();
	gManager = new GameObjectManager();

	texPos = { 340,90 };
	texSize = { 650,100 };

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
}

KochaEngine::StageSelect::~StageSelect()
{
	delete camera;
	delete gManager;
	delete pManager;
	delete emitter;
}

void KochaEngine::StageSelect::Initialize()
{
	isEnd = false;
	isGameOver = false;

	nowType = TYPE1;
	countFlag = false;
	gManager->RemoveAll();
	camera->Initialize(1280, 720, 90, 80, { 0,80,-100 }, { 0,0,0 }, { 0,1,0 });

	fadeAlpha = 1;
	fadeFlag = true;
	textCount = 0;
}

void KochaEngine::StageSelect::Update()
{
	Fade();
	gManager->Update();
	pManager->Update();
	camera->Update();

	if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_A)) && fadeAlpha <= 0.0f)
	{
		fadeFlag = false;
	}
	if (!fadeFlag && fadeAlpha >= 1.0f)
	{
		isEnd = true;
	}
}

void KochaEngine::StageSelect::SpriteDraw()
{
	gManager->SpriteDraw();
}

void KochaEngine::StageSelect::ObjDraw()
{	
	//gManager->ObjDraw(camera);
	//pManager->Draw(camera);
}

void KochaEngine::StageSelect::DrawGUI()
{
	ImGui::Text("StageSelect");
}

void KochaEngine::StageSelect::Load()
{
}

KochaEngine::Scenes KochaEngine::StageSelect::Next()
{
	return GAMEPLAY;
}

void KochaEngine::StageSelect::Fade()
{
	if (fadeFlag)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= 0.02f;
		}
	}
	else
	{
		if (fadeAlpha < 1.0f)
		{
			fadeAlpha += 0.02f;
		}
	}
}