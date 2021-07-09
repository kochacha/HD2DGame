#include "GamePlay.h"
#include "Input.h"
#include "Util.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
}

KochaEngine::GamePlay::~GamePlay()
{
	delete camera;
	delete gManager;
	delete pManager;
	delete emitter;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	
	gManager->RemoveAll();
	camera->Initialize(1280, 720, 90, 80, { 0,80,-100 }, { 0,0,0 }, { 0,1,0 });

	frameCount = 0;
	seconds = 0;
	
	fadeFlag = true;
	fadeAlpha = 1;
	endCount = 180;
}

void KochaEngine::GamePlay::Update()
{

	Fade();

	gManager->Update();
	pManager->Update();
	camera->Update();
}

void KochaEngine::GamePlay::SpriteDraw()
{
	gManager->SpriteDraw();
	if (seconds > 999)
	{
		seconds = 999;
	}
	
}

void KochaEngine::GamePlay::ObjDraw()
{
	gManager->ObjDraw(camera);
	pManager->Draw(camera);
}

void KochaEngine::GamePlay::DrawGUI()
{
	ImGui::Text("GamePlay");
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

void KochaEngine::GamePlay::Fade()
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
