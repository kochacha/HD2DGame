#include "../Header/Scene.h"

int KochaEngine::Scene::mapNum = 0;

KochaEngine::Scene::Scene()
{
}

KochaEngine::Scene::~Scene()
{
}

void KochaEngine::Scene::Initialize()
{
}

void KochaEngine::Scene::Load()
{

}

void KochaEngine::Scene::Update()
{
}

void KochaEngine::Scene::SpriteDraw()
{
}

void KochaEngine::Scene::ObjDraw()
{
}

void KochaEngine::Scene::AlphaObjDraw()
{
}

void KochaEngine::Scene::DrawGUI()
{
}

bool KochaEngine::Scene::IsEndFlag()
{
	return isEnd;
}

bool KochaEngine::Scene::IsGameOverFlag()
{
	return isGameOver;
}

KochaEngine::Scenes KochaEngine::Scene::Next()
{
	return TITLE;
}

KochaEngine::Scenes KochaEngine::Scene::GameOver()
{
	return GAMEOVER;
}