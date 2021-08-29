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
		LightManager* lightManager;
		std::vector<GameObject*> gameObjects;

		void Zsort(const int arg_count);

	public:
		GameObjectManager();
		~GameObjectManager();

		void AddObject(GameObject* gameObject);
		void Initialize();
		void Update();
		void AlphaObjDraw(Camera* camera);
		void ObjDraw(Camera* camera, LightManager* arg_lightManager);
		void SpriteDraw();
		void CheckBlock(GameObject* obj, GameObjectType otherType);
		bool CheckBombAlive();
		int GetEnemyCount();

		void Remove();
		void RemoveAll();
	};
}