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
	_Sphere objSphere = obj->GetSphere();
	GameObjectType objType = obj->GetType();
	XMFLOAT3 objPos = obj->GetPosition();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToBox(objSphere,(*it)->GetBox()))
		{
			obj->HitBlock((*it)->GetBox());					
		}
	}

}

void KochaEngine::GameObjectManager::CheckHitSphere(GameObject * obj, GameObjectType otherType)
{
	_Sphere objSphere = obj->GetSphere();
	GameObjectType objType = obj->GetType();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToSphere(objSphere, (*it)->GetSphere()))
		{
			(*it)->Hit();
		}
	}
}

bool KochaEngine::GameObjectManager::HitSphereToSphere(GameObject * obj, GameObjectType otherType)
{
	if (obj == nullptr) return false;

	_Sphere objSphere = obj->GetSphere();
	GameObjectType objType = obj->GetType();
	XMFLOAT3 objPos = obj->GetPosition();
	XMFLOAT3 objVel = obj->GetVelocity();
	float objSpeed = obj->GetSpeed();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToSphere(objSphere, (*it)->GetSphere()))
		{
			(*it)->Hit();
			return true;
		}
	}

	return false;
}

bool KochaEngine::GameObjectManager::HitSphereToBox(GameObject * obj, GameObjectType otherType)
{
	if (obj == nullptr) return false;

	_Sphere objSphere = obj->GetSphere();
	GameObjectType objType = obj->GetType();
	XMFLOAT3 objPos = obj->GetPosition();
	XMFLOAT3 objVel = obj->GetVelocity();
	float objSpeed = obj->GetSpeed();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToBox(objSphere, (*it)->GetBox()))
		{
			(*it)->Hit();
			return true;
		}
	}

	return false;
}

bool KochaEngine::GameObjectManager::HitPlayerKnockBack(GameObject * obj, GameObjectType otherType)
{
	if (obj == nullptr) return false;

	_Sphere objSphere = obj->GetSphere();
	GameObjectType objType = obj->GetType();
	XMFLOAT3 objPos = obj->GetPosition();
	XMFLOAT3 objVel = obj->GetVelocity();
	float objSpeed = obj->GetSpeed();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToSphere(objSphere, (*it)->GetSphere()))
		{	
			obj->SetKnockBackVel((*it)->GetVelocity());		
			return true;
		}
	}

	return false;
}

bool KochaEngine::GameObjectManager::HitPlayerKnockBack2(GameObject * obj, GameObjectType otherType)
{
	if (obj == nullptr) return false;

	_Sphere objSphere = obj->GetSphere();
	GameObjectType objType = obj->GetType();
	XMFLOAT3 objPos = obj->GetPosition();
	XMFLOAT3 objVel = obj->GetVelocity();
	float objSpeed = obj->GetSpeed();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //自分と同じオブジェクトだったら無視
		if ((*it)->GetType() != otherType) continue; //指定のオブジェクト以外だったら無視
		if ((*it)->IsDead()) continue; //オブジェクトが死んでいたら無視

		if (Collision::HitSphereToSphere(objSphere, (*it)->GetSphere()))
		{	
			return true;
		}
	}

	return false;
}

bool KochaEngine::GameObjectManager::CheckBombAlive()
{

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{		
		//if ((*it)->GetType() == TYPE)  //指定のオブジェクト以外だったら無視
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
//
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
