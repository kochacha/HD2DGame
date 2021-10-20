#include "SceneManager.h"

KochaEngine::SceneManager::SceneManager()
{

}

KochaEngine::SceneManager::~SceneManager()
{
}

void KochaEngine::SceneManager::AddScene(Scenes arg_name, Scene* arg_scene)
{
	if (scenes.count(arg_name) != 0) return;
	scenes[arg_name] = arg_scene;
}

void KochaEngine::SceneManager::ChangeScene(Scenes arg_name)
{
	currentScene = scenes[arg_name];
	currentScene->Initialize();
}

void KochaEngine::SceneManager::Load(Scenes arg_name)
{
	currentScene = scenes[arg_name];
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

void KochaEngine::SceneManager::AlphaObjDraw()
{
	currentScene->AlphaObjDraw();
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
