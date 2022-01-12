#include "GameObjectManager.h"
#include "Collision.h"
#include "Player.h"
#include "Fighter.h"
#include <map>

KochaEngine::GameObjectManager::GameObjectManager()
{
	battleCameraDefaultPos.Zero();
}

KochaEngine::GameObjectManager::~GameObjectManager()
{
	//ゲームオブジェクトの全削除
	Clear();
}

void KochaEngine::GameObjectManager::AddObject(GameObject* arg_gameObject)
{
	//ゲームオブジェクトの追加
	gameObjects.push_back(arg_gameObject);
}

void KochaEngine::GameObjectManager::Initialize()
{
	//全てのゲームオブジェクトの初期化
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		(*it)->Initialize();
	}
}

void KochaEngine::GameObjectManager::Update()
{
	//全てのゲームオブジェクトの更新
	for (int i = 0; i != gameObjects.size(); i++)
	{
		gameObjects[i]->Update();
	}
	//IsDeleteがtrueならそのオブジェクトを削除
	Remove();
}

void KochaEngine::GameObjectManager::AlphaObjDrawFieldScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//フィールドシーン用の透明オブジェクトの描画
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	auto _eye = camera->GetEye();
	float cameraNearPosZ = _eye.z + 18;
	float cameraFarPosZ = _eye.z + 400;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if (!(*it)->IsAlphaObject()) continue; //AlphaObjectじゃない場合は描画しない
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraNearPosZ || 
			(*it)->GetPosition().z > cameraFarPosZ) continue; //AlphaObjectがCameraよりも手前にあったら描画しない

		(*it)->ObjDraw(this->camera, this->lightManager);
	}

}

void KochaEngine::GameObjectManager::AlphaObjDrawBattleScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//バトルシーン用の透明オブジェクトの描画
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	float cameraNearPosZ = battleCameraDefaultPos.z + 80;
	float cameraFarPosZ = battleCameraDefaultPos.z + 400;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		auto type = (*it)->GetType();
		if (!(*it)->IsAlphaObject()) continue; //AlphaObjectじゃない場合は描画しない
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraNearPosZ ||
			(*it)->GetPosition().z > cameraFarPosZ) continue; //AlphaObjectがCameraよりも手前にあったら描画しない

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::ObjDrawFieldScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//フィールドシーン用の不透明オブジェクトの描画
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	float cameraPosZ = camera->GetEye().z - 50;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsAlphaObject()) continue; //AlphaObjectの場合は描画しない
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraPosZ) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::ObjDrawBattleScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//バトルシーン用の不透明オブジェクトの描画
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	camera = arg_camera;
	lightManager = arg_lightManager;

	float cameraPosZ = battleCameraDefaultPos.z + 100;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsAlphaObject()) continue; //AlphaObjectの場合は描画しない
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraPosZ) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::SpriteDraw()
{
	//平行投影テクスチャの描画
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;

		(*it)->SpriteDraw();
	}
}

void KochaEngine::GameObjectManager::CheckBlock(GameObject* arg_obj, const GameObjectType& arg_otherType)
{
	//自分以外のオブジェクトと当たっていないかのチェック
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
	//エネミーの数を数えて返す
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
	//isDeleteフラグがtrueのオブジェクトは削除
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		if (!(*it)->IsDelete()) { ++it; continue; }
		delete *it;
		it = gameObjects.erase(it);
	}
}

void KochaEngine::GameObjectManager::Clear()
{
	//ゲームオブジェクトの全削除
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		delete (*it);
	}
	gameObjects.clear();
}

//void KochaEngine::GameObjectManager::RemoveBattleObject()
//{
//	//バトルシーン用のオブジェクトの削除
//	//バトル終了時に呼ぶ
//	for (auto it = gameObjects.begin(); it != gameObjects.end();)
//	{
//		auto type = (*it)->GetType();
//		if (!(*it)->IsDelete() &&
//			type != ENEMY &&
//			type != BATTLE_PLAYER && 
//			type != BATTLE_FIGHTER)
//		{
//			++it; 
//			continue; 
//		}
//		delete* it;
//		it = gameObjects.erase(it);
//	}
//}

KochaEngine::Player* KochaEngine::GameObjectManager::GetPlayer()
{
	//プレイヤーを探して返す
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == FIELD_PLAYER)
		{
			Player* player = static_cast<Player*>(static_cast<void*>(*it));
			return player;
		}
	}
	return nullptr;
}

KochaEngine::Fighter* KochaEngine::GameObjectManager::GetFighter()
{
	//ファイターを探して返す
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == FIELD_FIGHTER)
		{
			Fighter* fighter = static_cast<Fighter*>(static_cast<void*>(*it));
			return fighter;
		}
	}
	return nullptr;
}
