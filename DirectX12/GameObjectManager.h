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
		bool CheckBombAlive();
		int GetEnemyCount();

		void Remove();
		void RemoveAll();
	};
}