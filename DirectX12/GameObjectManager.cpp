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
	//�Q�[���I�u�W�F�N�g�̑S�폜
	Clear();
}

void KochaEngine::GameObjectManager::AddObject(GameObject* arg_gameObject)
{
	//�Q�[���I�u�W�F�N�g�̒ǉ�
	gameObjects.push_back(arg_gameObject);
}

void KochaEngine::GameObjectManager::Initialize()
{
	//�S�ẴQ�[���I�u�W�F�N�g�̏�����
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		(*it)->Initialize();
	}
}

void KochaEngine::GameObjectManager::Update()
{
	//�S�ẴQ�[���I�u�W�F�N�g�̍X�V
	for (int i = 0; i != gameObjects.size(); i++)
	{
		gameObjects[i]->Update();
	}
	//IsDelete��true�Ȃ炻�̃I�u�W�F�N�g���폜
	Remove();
}

void KochaEngine::GameObjectManager::AlphaObjDrawFieldScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//�t�B�[���h�V�[���p�̓����I�u�W�F�N�g�̕`��
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
		if (!(*it)->IsAlphaObject()) continue; //AlphaObject����Ȃ��ꍇ�͕`�悵�Ȃ�
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraNearPosZ || 
			(*it)->GetPosition().z > cameraFarPosZ) continue; //AlphaObject��Camera������O�ɂ�������`�悵�Ȃ�

		(*it)->ObjDraw(this->camera, this->lightManager);
	}

}

void KochaEngine::GameObjectManager::AlphaObjDrawBattleScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//�o�g���V�[���p�̓����I�u�W�F�N�g�̕`��
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
		if (!(*it)->IsAlphaObject()) continue; //AlphaObject����Ȃ��ꍇ�͕`�悵�Ȃ�
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraNearPosZ ||
			(*it)->GetPosition().z > cameraFarPosZ) continue; //AlphaObject��Camera������O�ɂ�������`�悵�Ȃ�

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::ObjDrawFieldScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//�t�B�[���h�V�[���p�̕s�����I�u�W�F�N�g�̕`��
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	this->camera = arg_camera;
	this->lightManager = arg_lightManager;

	float cameraPosZ = camera->GetEye().z - 50;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsAlphaObject()) continue; //AlphaObject�̏ꍇ�͕`�悵�Ȃ�
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraPosZ) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::ObjDrawBattleScene(Camera* arg_camera, LightManager* arg_lightManager)
{
	//�o�g���V�[���p�̕s�����I�u�W�F�N�g�̕`��
	if (arg_camera == nullptr) return;
	if (arg_lightManager == nullptr) return;

	camera = arg_camera;
	lightManager = arg_lightManager;

	float cameraPosZ = battleCameraDefaultPos.z + 100;

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsAlphaObject()) continue; //AlphaObject�̏ꍇ�͕`�悵�Ȃ�
		if ((*it)->IsDead()) continue;
		if ((*it)->GetPosition().z < cameraPosZ) continue;

		(*it)->ObjDraw(this->camera, this->lightManager);
	}
}

void KochaEngine::GameObjectManager::SpriteDraw()
{
	//���s���e�e�N�X�`���̕`��
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->IsDead()) continue;

		(*it)->SpriteDraw();
	}
}

void KochaEngine::GameObjectManager::CheckBlock(GameObject* arg_obj, const GameObjectType& arg_otherType)
{
	//�����ȊO�̃I�u�W�F�N�g�Ɠ������Ă��Ȃ����̃`�F�b�N
	_Sphere objSphere = arg_obj->GetSphere();
	GameObjectType objType = arg_obj->GetType();
	XMFLOAT3 objPos = arg_obj->GetPosition();

	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		if ((*it)->GetType() == objType) continue; //�����Ɠ����I�u�W�F�N�g�������疳��
		if ((*it)->GetType() != arg_otherType) continue; //�w��̃I�u�W�F�N�g�ȊO�������疳��
		if ((*it)->IsDead()) continue; //�I�u�W�F�N�g������ł����疳��

		if (Collision::HitSphereToBox(objSphere,(*it)->GetBox()))
		{
			arg_obj->HitBlock((*it)->GetBox());
		}
	}

}

int KochaEngine::GameObjectManager::GetEnemyCount()
{
	//�G�l�~�[�̐��𐔂��ĕԂ�
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
	//isDelete�t���O��true�̃I�u�W�F�N�g�͍폜
	for (auto it = gameObjects.begin(); it != gameObjects.end();)
	{
		if (!(*it)->IsDelete()) { ++it; continue; }
		delete *it;
		it = gameObjects.erase(it);
	}
}

void KochaEngine::GameObjectManager::Clear()
{
	//�Q�[���I�u�W�F�N�g�̑S�폜
	auto end = gameObjects.end();
	for (auto it = gameObjects.begin(); it != end; ++it)
	{
		delete (*it);
	}
	gameObjects.clear();
}

//void KochaEngine::GameObjectManager::RemoveBattleObject()
//{
//	//�o�g���V�[���p�̃I�u�W�F�N�g�̍폜
//	//�o�g���I�����ɌĂ�
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
	//�v���C���[��T���ĕԂ�
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
	//�t�@�C�^�[��T���ĕԂ�
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
