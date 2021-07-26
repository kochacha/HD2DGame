#include "GameObjectManager.h"
#include "Collision.h"

KochaEngine::GameObjectManager::GameObjectManager()
{
}

KochaEngine::GameObjectManager::~GameObjectManager()
{
	RemoveAll();
}

void KochaEngine::GameObjectManager::AddObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
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

void KochaEngine::GameObjectManager::ObjDraw(Camera* camera)
{
	if (camera == nullptr) return;
	this->camera = camera;
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead())
		{
			continue;
		}	
		(*it)->ObjDraw(this->camera);
	}
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead())
		{
			continue;
		}
		(*it)->ObjDraw(this->camera);
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

void KochaEngine::GameObjectManager::CheckBlock(GameObject * obj, GameObjectType otherType)
{
	GameObjectType objType = obj->GetType();
	XMFLOAT3 objPos = obj->GetPosition();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //�����Ɠ����I�u�W�F�N�g�������疳��
		if ((*it)->GetType() != otherType) continue; //�w��̃I�u�W�F�N�g�ȊO�������疳��
		if ((*it)->IsDead()) continue; //�I�u�W�F�N�g������ł����疳��

		//if (Collision::HitSphereToBox(objSphere,(*it)->GetBox()))
		//{
		//	obj->HitBlock((*it)->GetBox());					
		//}
	}

}

bool KochaEngine::GameObjectManager::CheckBombAlive()
{

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{		
		//if ((*it)->GetType() == TYPE)  //�w��̃I�u�W�F�N�g�ȊO�������疳��
		//{
		//	return true;
		//}
	}
	return false;
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

//
//Player * KochaEngine::GameObjectManager::GetPlayer()
//{
//	auto end = gameObjects.end();
//	for (auto it = gameObjects.begin(); it != end; ++it)
//	{
//		if ((*it)->GetType() == PLAYER)
//		{
//			Player* player = static_cast<Player*>(static_cast<void*>(*it));
//			return player;
//		}
//	}
//	return nullptr;
//}
//
//Tower * KochaEngine::GameObjectManager::GetTower()
//{
//	auto end = gameObjects.end();
//	for (auto it = gameObjects.begin(); it != end; ++it)
//	{
//		if ((*it)->GetType() == TOWER)
//		{
//			Tower* tower = static_cast<Tower*>(static_cast<void*>(*it));
//			return tower;
//		}
//	}
//	return nullptr;
//}

//Boss * KochaEngine::GameObjectManager::GetBoss()
//{
//	auto end = gameObjects.end();
//	for (auto it = gameObjects.begin(); it != end; ++it)
//	{
//		if ((*it)->GetType() == BOSS)
//		{
//			Boss* boss = static_cast<Boss*>(static_cast<void*>(*it));
//			return boss;
//		}
//	}
//	return nullptr;
//}
