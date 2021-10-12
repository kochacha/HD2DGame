#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
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
		void AlphaObjDraw(Camera* camera, LightManager* arg_lightManager);
		void ObjDraw(Camera* camera, LightManager* arg_lightManager);
		void SpriteDraw();
		void CheckBlock(GameObject* obj, const GameObjectType& otherType);
		int GetEnemyCount();

		Player* GetPlayer();

		void Remove();
		void RemoveAll();
	};
}