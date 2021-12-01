#include "BattleObjectManager.h"
#include "Enemy.h"

KochaEngine::BattleObjectManager::BattleObjectManager()
{
}

KochaEngine::BattleObjectManager::~BattleObjectManager()
{
	//�I�u�W�F�N�g�̑S�폜
	RemoveAll();
}

void KochaEngine::BattleObjectManager::AddObject(BattleObject* arg_battleObject)
{
	//�I�u�W�F�N�g�̒ǉ�
	battleObjects.push_back(arg_battleObject);
}

void KochaEngine::BattleObjectManager::Initialize()
{
	totalMoney = 0;
	totalExp = 0;
	turn = 0;

	//�S�ẴQ�[���I�u�W�F�N�g�̏�����
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		(*it)->Initialize();
	}
}

void KochaEngine::BattleObjectManager::Update()
{
	//�S�ẴQ�[���I�u�W�F�N�g�̍X�V
	for (int i = 0; i != battleObjects.size(); i++)
	{
		battleObjects[i]->Update();
	}
	//IsDelete��true�Ȃ炻�̃I�u�W�F�N�g���폜
	Remove();
}

void KochaEngine::BattleObjectManager::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
	//�t�B�[���h�V�[���p�̓����I�u�W�F�N�g�̕`��
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
	//���s���e�e�N�X�`���̕`��
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
	//�����Ă���G�̐�
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
	//enemyNum��0����ł͂Ȃ�1����
	if (arg_enemyNum > GetEnemyCount()) return nullptr;

	//�G�l�~�[��T���ĕԂ�
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
		if ((*it)->IsKnockDown()) continue;
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
	//isDelete�t���O��true�̃I�u�W�F�N�g�͍폜
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

	//�Q�[���I�u�W�F�N�g�̑S�폜
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

void KochaEngine::BattleObjectManager::TargetOff()
{
	auto end = battleObjects.end();
	for (auto it = battleObjects.begin(); it != end; ++it)
	{
		(*it)->TargetOff();
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
