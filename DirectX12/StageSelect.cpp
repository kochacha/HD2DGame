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
	gManager->Update();
	pManager->Update();
	camera->Update();
}

void KochaEngine::StageSelect::SpriteDraw()
{
	gManager->SpriteDraw();
}

void KochaEngine::StageSelect::ObjDraw()
{	
	gManager->ObjDraw(camera);
	pManager->Draw(camera);
}

void KochaEngine::StageSelect::Load()
{
}

KochaEngine::Scenes KochaEngine::StageSelect::Next()
{
	return GAMEPLAY;
}

