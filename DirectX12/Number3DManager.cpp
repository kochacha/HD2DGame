#include "Number3DManager.h"
#include "Number3DOperation.h"
#include "Camera.h"
#include "LightManager.h"

KochaEngine::Number3DManager::Number3DManager()
{
}

KochaEngine::Number3DManager::~Number3DManager()
{
	RemoveAll();
}

void KochaEngine::Number3DManager::AddNumbers(Number3DOperation* arg_number3D)
{
	numbers.push_back(arg_number3D);
}

void KochaEngine::Number3DManager::Update()
{
	auto end = numbers.end();
	for (auto itr = numbers.begin(); itr != end; itr++)
	{
		(*itr)->Update();
	}
	Remove();
}

void KochaEngine::Number3DManager::Draw(Camera* arg_camera, LightManager* arg_lightManager)
{
	auto end = numbers.end();
	for (auto itr = numbers.begin(); itr != end; itr++)
	{
		(*itr)->Draw(arg_camera, arg_lightManager);
	}
}

void KochaEngine::Number3DManager::Remove()
{
	for (auto it = numbers.begin(); it != numbers.end();)
	{
		if ((*it)->GetLife() != 0)
		{
			++it;
			continue;
		}
		delete* it;
		it = numbers.erase(it);
	}
}

void KochaEngine::Number3DManager::RemoveAll()
{
	auto end = numbers.end();
	for (auto it = numbers.begin(); it != end; ++it)
	{
		delete* it;
	}
	numbers.clear();
}
