#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "LightManager.h"
#include "Input.h"
#include "Vector3.h"

namespace KochaEngine
{
	enum GameObjectType
	{
		DEFAULT,
		FIELD_PLAYER,
		FIELD_FIGHTER,
		BATTLE_PLAYER,
		BATTLE_FIGHTER,
		ENEMY,
		BLOCK,
		COLLISION_BLOCK,
	};

	class GameObject
	{
	protected:
		Camera* camera;
		LightManager* lightManager;
		ParticleEmitter* emitter;
		Object* obj;

		bool isDead = false;
		bool isDelete = false;
		bool isTrigger = false;
		bool isAlpha = false;

		Vector3 position = { 0,0,0 };
		Vector3 prePosition = { 0,0,0 };
		Vector3 velocity = { 0,0,0 };

		_Sphere sphere;
		_Box box;
		CollisionFace faceX;
		CollisionFace faceZ;
		XMFLOAT3 maxPos = { 0,0,0 };
		XMFLOAT3 minPos = { 0,0,0 };

		bool hitFlag;
		int hitCount;
		int deleteCount = 0;
		Vector3 prePos = { 0,0,0 };

	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize();
		virtual void Update();
		virtual void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager);
		virtual void SpriteDraw();

		virtual void Hit();
		virtual void HitBlock(_Box arg_box);
		virtual void Dead();
		virtual GameObjectType GetType();
		virtual float GetSpeed();
		virtual float GetCameraDistance(Camera* arg_camera);

		inline const bool IsDead() { return isDead; }
		inline const bool IsDelete() { return isDelete; }
		inline const bool IsTrigger() { return isTrigger; }
		inline const bool IsAlphaObject() { return isAlpha; }


		inline const Vector3 GetPosition() { return position; }
		inline const Vector3 GetVelocity() { return velocity; }
		inline const _Sphere GetSphere() { return sphere; }
		inline const _Box GetBox() { return box; }
		inline void SetFaceX(const CollisionFace& arg_faceX) { this->faceX = arg_faceX; }
		inline void SetFaceZ(const CollisionFace& arg_faceZ) { this->faceZ = arg_faceZ; }
		inline void SetPosition(const Vector3& arg_position) { this->position = arg_position; }
		inline void SetMaxPos(const Vector3& arg_position) { this->maxPos = arg_position; }
		inline void SetMinPos(const Vector3& arg_position) { this->minPos = arg_position; }
	};
}