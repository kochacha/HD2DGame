#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Input.h"


namespace KochaEngine
{
	enum GameObjectType
	{
		PLAYER,
	};

	class GameObject
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;
	protected:
		Camera* camera;
		ParticleEmitter* emitter;
		Object* obj;

		bool isDead = false;
		bool isDelete = false;
		bool isTrigger = false;

		XMFLOAT3 position = { 0,0,0 };
		XMFLOAT3 prePosition = { 0,0,0 };
		DirectX::XMFLOAT3 velocity = { 0,0,0 };

		int deleteCount = 0;

		_Sphere sphere;
		_Box box;
		CollisionFace faceX;
		CollisionFace faceZ;
		XMFLOAT3 maxPos = { 0,0,0 };
		XMFLOAT3 minPos = { 0,0,0 };

		bool hitFlag;
		int hitCount;
		XMFLOAT3 prePos = { 0,0,0 };

	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize();
		virtual void Update();
		virtual void ObjDraw(Camera* camera);
		virtual void SpriteDraw();

		virtual void Hit();
		virtual void HitBlock(_Box box);
		virtual void SetKnockBackVel(XMFLOAT3 vel);
		virtual void Dead();
		virtual GameObjectType GetType();
		virtual float GetSpeed();

		bool IsDead() { return isDead; }
		bool IsDelete() { return isDelete; }
		bool IsTrigger() { return isTrigger; }

		XMFLOAT3 GetPosition() { return position; }
		XMFLOAT3 GetVelocity() { return velocity; }
		_Sphere GetSphere() { return sphere; }
		_Box GetBox() { return box; }
		void SetFaceX(CollisionFace faceX) { this->faceX = faceX; }
		void SetFaceZ(CollisionFace faceZ) { this->faceZ = faceZ; }
		void SetPosition(XMFLOAT3 position) { this->position = position; }
		void SetMaxPos(XMFLOAT3 position) { this->maxPos = position; }
		void SetMinPos(XMFLOAT3 position) { this->minPos = position; }
	};
}