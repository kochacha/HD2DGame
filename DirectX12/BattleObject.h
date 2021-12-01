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
		ActorParam param;

		bool isDead = false; //Ž€‚ñ‚Å‚¢‚é‚©‚Ç‚¤‚©
		bool isDelete = false; //íœ‚·‚é‚©‚Ç‚¤‚©
		bool isTrigger = false; //“–‚½‚è”»’è‚ð•t‚¯‚é‚©‚Ç‚¤‚©

		bool isKnockDown = false; //HP‚ª0‚©‚Ç‚¤‚©
		bool isActive = false; //s“®Ï‚Ý‚©‚Ç‚¤‚©
		bool isCurrentActive = false; //Œ»Ýs“®’†‚©‚Ç‚¤‚©

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
		virtual void SetDamage(const int arg_damage);

		inline const bool IsDead() { return isDead; }
		inline const bool IsDelete() { return isDelete; }
		inline const bool IsTrigger() { return isTrigger; }
		inline const bool IsKnockDown() { return isKnockDown; }
		inline const bool IsActive() { return isActive; }
		inline const bool IsCurrentActive() { return isCurrentActive; }

		inline const Vector3 GetPosition() { return position; }
		inline const Vector3 GetVelocity() { return velocity; }
		inline void SetPosition(const Vector3& arg_position) { this->position = arg_position; }
		inline const ActorParam GetParam() { return param; }
	};
}

