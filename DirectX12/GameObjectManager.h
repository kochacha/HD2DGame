#pragma once
#include "GameObject.h"
#include "Camera.h"
#include <vector>

namespace KochaEngine
{
	class Player;
	class Fighter;

	class GameObjectManager
	{
	private:
		Camera* camera;
		LightManager* lightManager;
		std::vector<GameObject*> gameObjects;

	public:
		GameObjectManager();
		~GameObjectManager();

		void AddObject(GameObject* arg_gameObject);
		void Initialize();
		void Update();
		void AlphaObjDrawFieldScene(Camera* arg_camera, LightManager* arg_lightManager);
		void AlphaObjDrawBattleScene(Camera* arg_camera, LightManager* arg_lightManager);
		void ObjDrawFieldScene(Camera* arg_camera, LightManager* arg_lightManager);
		void ObjDrawBattleScene(Camera* arg_camera, LightManager* arg_lightManager);
		void SpriteDraw();
		void CheckBlock(GameObject* arg_obj, const GameObjectType& arg_otherType);
		int GetEnemyCount();

		Player* GetPlayer();
		Fighter* GetFighter();

		void Remove();
		void Clear();
	};
}