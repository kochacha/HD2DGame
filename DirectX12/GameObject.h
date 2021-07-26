#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Collision.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Vector3.h"


namespace KochaEngine
{
	enum GameObjectType
	{
		PLAYER,
		BLOCK,
	};

	class GameObject
	{
	protected:
		Camera* camera;
		ParticleEmitter* emitter;
		Object* obj;

		bool isDead = false;
		bool isDelete = false;
		bool isTrigger = false;

		Vector3 position = { 0,0,0 };
		Vector3 prePosition = { 0,0,0 };
		Vector3 velocity = { 0,0,0 };

		_Square square;
		Vector3 maxPos = { 0,0,0 };
		Vector3 minPos = { 0,0,0 };

		bool hitFlag;
		int hitCount;
		int deleteCount = 0;
		Vector3 prePos = { 0,0,0 };

	public:
		GameObject();
		virtual ~GameObject();
		virtual void Initialize();
		virtual void Update();
		virtual void ObjDraw(Camera* camera);
		virtual void SpriteDraw();

		virtual void Hit();
		virtual void HitBlock(_Square arg_square);
		virtual void Dead();
		virtual GameObjectType GetType();
		virtual float GetSpeed();

		bool IsDead() { return isDead; }
		bool IsDelete() { return isDelete; }
		bool IsTrigger() { return isTrigger; }

		Vector3 GetPosition() { return position; }
		Vector3 GetVelocity() { return velocity; }
		_Square GetSquare() { return square; }
		void SetPosition(const Vector3& arg_position) { this->position = arg_position; }
		void SetMaxPos(const Vector3& arg_position) { this->maxPos = arg_position; }
		void SetMinPos(const Vector3& arg_position) { this->minPos = arg_position; }
	};
}