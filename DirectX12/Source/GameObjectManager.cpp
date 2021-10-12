#include "../Header/GameObjectManager.h"
#include "../Header/Collision.h"
#include <map>

KochaEngine::GameObjectManager::GameObjectManager()
{
}

KochaEngine::GameObjectManager::~GameObjectManager()
{
	RemoveAll();
}

void KochaEngine::GameObjectManager::AddObject(GameObject* arg_gameObject)
{
	gameObjects.push_back(arg_gameObject);
}

void KochaEngine::GameObjectManager::Initialize()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		(*it)->Initialize();
	}
}

void KochaEngine::GameObjectManager::Update()
{
	for (int i = 0; i != gameObjects.size(); i++)
	{
		gameObjects[i]->Update();
	}
	Remove();
}

void KochaEngine::GameObjectManager::Zsort(const int arg_count)
{
	int sortCount = 0;
	while (sortCount == arg_count)
	{

	}
}

void KochaEngine::GameObjectManager::AlphaObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	float cameraPosZ = camera->GetEye().z;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if (!(*it)->IsAlphaObject()) continue; //AlphaObjectじゃない場合は描画しない
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraPosZ) continue; //AlphaObjectがCameraよりも後ろにあったら描画しない

		(*it)->ObjDraw(this->camera, this->lightManager);
	}

}

void KochaEngine::GameObjectManager::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsAlphaObject()) continue; //AlphaObjectの場合は描画しない
		if ((*it)->IsDead()) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::SpriteDraw()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead())
		{
			continue;
		}
		(*it)->SpriteDraw();
	}
}

void KochaEngine::GameObjectManager::CheckBlock(GameObject* arg_obj, const GameObjectType& arg_otherType)
{
	_Sphere objSphere = arg_obj->GetSphere();
	GameObjectType objType = arg_obj->GetType();
	XMFLOAT3 objPos = arg_obj->GetPosition();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != arg_otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToBox(objSphere,(*it)->GetBox()))
		{
			arg_obj->HitBlock((*it)->GetBox());
		}
	}

}

int KochaEngine::GameObjectManager::GetEnemyCount()
{
	int count = 0;
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		//if ((*it)->GetType() == ENEMY)
		//{
		//	count++;
		//}
	}
	return count;
}

void KochaEngine::GameObjectManager::Remove()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		if (!(*it)->IsDelete()) { ++it; continue; }
		delete *it;
		it = gameObjects.erase(it);
	}
}

void KochaEngine::GameObjectManager::RemoveAll()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		delete (*it);
	}
	gameObjects.clear();
}

KochaEngine::Player* KochaEngine::GameObjectManager::GetPlayer()
{
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == PLAYER)
		{
			Player* player = static_cast<Player*>(static_cast<void*>(*it));
			return player;
		}
	}
	return nullptr;
}
