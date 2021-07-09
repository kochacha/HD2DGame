#include "SceneManager.h"

KochaEngine::SceneManager::SceneManager()
{

}

KochaEngine::SceneManager::~SceneManager()
{
}

void KochaEngine::SceneManager::AddScene(Scenes name, Scene* scene)
{
	if (scenes.count(name) != 0) return;
	scenes[name] = scene;
}

void KochaEngine::SceneManager::ChangeScene(Scenes name)
{
	currentScene = scenes[name];
	currentScene->Initialize();
}

void KochaEngine::SceneManager::Load(Scenes name)
{
	currentScene = scenes[name];
	currentScene->Load();
}

void KochaEngine::SceneManager::Update()
{
	currentScene->Update();
	if (currentScene->IsEndFlag())
	{
		ChangeScene(currentScene->Next());
		return;
	}
	if (currentScene->IsGameOverFlag())
	{
		ChangeScene(currentScene->GameOver());
		return;
	}
}

void KochaEngine::SceneManager::SpriteDraw()
{
	currentScene->SpriteDraw();
}

void KochaEngine::SceneManager::ObjDraw()
{
	currentScene->ObjDraw();
}

void KochaEngine::SceneManager::DrawGUI()
{
	currentScene->DrawGUI();
}

void KochaEngine::SceneManager::Terminate()
{
	auto end = scenes.end();
	for (auto it = scenes.begin(); it != end; ++it)
	{
		delete (*it).second;
	}
	scenes.clear();

	//delete scenes[TITLE];
	//delete scenes[STAGESELECT];
	//delete scenes[GAMEPLAY];
	//delete scenes[ENDING];
	//delete scenes[GAMEOVER];
	//scenes.clear();
}
