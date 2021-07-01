#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <vector>

namespace KochaEngine
{
	class GameObjectManager
	{
	private:
		Camera* camera;
		std::vector<GameObject*> gameObjects;

	public:
		GameObjectManager();
		~GameObjectManager();

		void AddObject(GameObject* gameObject);
		void Initialize();
		void Update();
		void ObjDraw(Camera* camera);
		void SpriteDraw();
		void CheckBlock(GameObject* obj, GameObjectType otherType);
		void CheckHitSphere(GameObject* obj, GameObjectType otherType);
		bool HitSphereToSphere(GameObject* obj, GameObjectType otherType);
		bool HitSphereToBox(GameObject* obj, GameObjectType otherType);
		bool HitPlayerKnockBack(GameObject* obj, GameObjectType otherType);
		bool HitPlayerKnockBack2(GameObject* obj, GameObjectType otherType);
		bool CheckBombAlive();
		int GetEnemyCount();

		void Remove();
		void RemoveAll();
	};
}