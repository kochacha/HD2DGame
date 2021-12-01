#include "BattleObjectManager.h"
#include "Enemy.h"

KochaEngine::BattleObjectManager::BattleObjectManager()
{
}

KochaEngine::BattleObjectManager::~BattleObjectManager()
{
	//オブジェクトの全削除
	RemoveAll();
}

void KochaEngine::BattleObjectManager::AddObject(BattleObject* arg_battleObject)
{
	//オブジェクトの追加
	battleObjects.push_back(arg_battleObject);
}

void KochaEngine::BattleObjectManager::Initialize()
{
	totalMoney = 0;
	totalExp = 0;
	turn = 0;

	//全てのゲームオブジェクトの初期化
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		(*it)->Initialize();
	}
}

void KochaEngine::BattleObjectManager::Update()
{
	//全てのゲームオブジェクトの更新
	for (int i = 0; i != battleObjects.size(); i++)
	{
		battleObjects[i]->Update();
	}
	//IsDeleteがtrueならそのオブジェクトを削除
	Remove();
}

void KochaEngine::BattleObjectManager::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	//フィールドシーン用の透明オブジェクトの描画
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::BattleObjectManager::SpriteDraw()
{
	//平行投影テクスチャの描画
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;

		(*it)->SpriteDraw();
	}
}

void KochaEngine::BattleObjectManager::AddReward(const int arg_money, const int arg_exp)
{
	totalMoney += arg_money;
	totalExp += arg_exp;
}

int KochaEngine::BattleObjectManager::GetEnemyCount()
{
	//生きている敵の数
	int count = 0;
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;
		if ((*it)->IsKnockDown()) continue;
		if ((*it)->GetType() == ENEMY) count++;
	}
	return count;
}

KochaEngine::Enemy* KochaEngine::BattleObjectManager::GetEnemy(const int arg_enemyNum)
{
	//enemyNumは0からではなく1から
	if (arg_enemyNum > GetEnemyCount()) return nullptr;

	//エネミーを探して返す
	int count = 1;

	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;
		if ((*it)->IsKnockDown()) continue;
		if ((*it)->GetType() == ENEMY)
		{
			if (count == arg_enemyNum)
			{
				Enemy* enemy = static_cast<Enemy*>(static_cast<void*>(*it));
				return enemy;
			}
			else
			{
				count++;
			}
		}
	}

	return nullptr;
}

KochaEngine::BattleObject* KochaEngine::BattleObjectManager::GetCurrentActive()
{
	int speed = 0;
	BattleObject* object = nullptr;
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;
		if ((*it)->IsActive()) continue;
		if ((*it)->GetParam().speed >= speed )
		{
			speed = (*it)->GetParam().speed;
			object = *it;
		}	
	}
	return object;
}

KochaEngine::BattleObject* KochaEngine::BattleObjectManager::GetCharacter(const KochaEngine::BattleObjectType& arg_type)
{
	BattleObject* object = nullptr;
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;
		if ((*it)->GetType() != arg_type) continue;

		object = *it;
	}

	return object;
}

void KochaEngine::BattleObjectManager::Remove()
{
	//isDeleteフラグがtrueのオブジェクトは削除
	for (auto it = battleObjects.begin(); it != battleObjects.end();)
	{
		if (!(*it)->IsDelete()) 
		{ 
			++it; 
			continue; 
		}

		delete* it;
		it = battleObjects.erase(it);
	}
}

void KochaEngine::BattleObjectManager::RemoveAll()
{
	totalMoney = 0;
	totalExp = 0;
	turn = 0;

	//ゲームオブジェクトの全削除
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		delete (*it);
	}
	battleObjects.clear();
}

void KochaEngine::BattleObjectManager::Reward()
{
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() != ENEMY) continue;
		(*it)->Reward();
	}
}

void KochaEngine::BattleObjectManager::ActiveReset()
{
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;
		if ((*it)->IsKnockDown()) continue;
		(*it)->ActiveReset();
	}
}

bool KochaEngine::BattleObjectManager::IsEnemyDestroy()
{
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() != ENEMY) continue;
		if ((*it)->IsDead()) continue;
		if ((*it)->IsKnockDown()) continue;
		return false;
	}
	return true;
}

bool KochaEngine::BattleObjectManager::IsCharacterDestroy()
{
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() != BATTLE_PLAYER &&
			(*it)->GetType() != BATTLE_FIGHTER) continue;
		if ((*it)->IsDead()) continue;
		if ((*it)->IsKnockDown()) continue;
		return false;
	}
	return true;
}
