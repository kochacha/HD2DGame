#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "LightManager.h"
#include "Input.h"
#include "Vector3.h"
#include "ActorParam.h"
#include "SkillData.h"

namespace KochaEngine
{
	enum BattleObjectType
	{
		BATTLE_OBJECT_TYPE_DEFAULT,
		BATTLE_PLAYER,
		BATTLE_FIGHTER,
		ENEMY,
	};

	class BattleObject
	{
	protected:
		Camera* camera;
		LightManager* lightManager;
		ParticleEmitter* emitter;
		Object* obj;
		Object* cursor;
		ActorParam baseParam; //�f�̃p�����[�^�[(�����i�܂�)
		ActorParam battleParam; //�o�t���l�������p�����[�^�[(�o�g����)

		std::string currentActiveName;

		bool isDead = false; //����ł��邩�ǂ���
		bool isDelete = false; //�폜���邩�ǂ���
		bool isTrigger = false; //�����蔻���t���邩�ǂ���

		bool isKnockDown = false; //HP��0���ǂ���
		bool isActive = false; //�s���ς݂��ǂ���
		bool isCurrentActive = false; //���ݍs�������ǂ���
		bool isTarget = false; //�������^�[�Q�b�g�ɂ���Ă��邩�ǂ���

		Vector3 position = { 0,0,0 };
		Vector3 prePosition = { 0,0,0 };
		Vector3 velocity = { 0,0,0 };

		int deleteCount = 0;
		Vector3 prePos = { 0,0,0 };

	public:
		BattleObject();
		virtual ~BattleObject();
		virtual void Initialize();
		virtual void Update();
		virtual void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager);
		virtual void SpriteDraw();
		virtual BattleObjectType GetType();

		virtual void Dead();
		virtual void Reward();
		virtual void ActiveReset();
		virtual void ActiveDone();
		virtual void CurrentActive();
		virtual void CurrentActiveReset();
		virtual void TargetOn();
		virtual void TargetOff();
		virtual void AddExp(const int arg_exp);
		virtual void AddMoney(const int arg_money);
		virtual void SetDamage(const int arg_damage);
		virtual void SetDamage(const std::string& arg_skillName, const ActorParam& arg_param);
		virtual std::string GetSkillName(const int arg_index);
		virtual void SetDefaultWaitTexture();
		virtual void SetAttackTextureIndex(const int arg_index);

		inline const bool IsDead() { return isDead; }
		inline const bool IsDelete() { return isDelete; }
		inline const bool IsTrigger() { return isTrigger; }
		inline const bool IsKnockDown() { return isKnockDown; }
		inline const bool IsActive() { return isActive; }
		inline const bool IsCurrentActive() { return isCurrentActive; }
		inline const bool IsTarget() { return isTarget; }

		inline const Vector3 GetPosition() { return position; }
		inline const Vector3 GetVelocity() { return velocity; }
		inline void SetPosition(const Vector3& arg_position) { this->position = arg_position; }
		inline const ActorParam GetParam() { return baseParam; }
	};
}

