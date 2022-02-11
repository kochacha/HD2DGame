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

		Vector3 battleCameraDefaultPos;

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

		void SetBattleCameraDefaultPos(const Vector3& arg_position) { battleCameraDefaultPos = arg_position; }
		int GetEnemyCount();

		Player* GetPlayer();
		Fighter* GetFighter();
		SceneChangePoint CheckSceneChangePoint(GameObject* arg_obj, const GameObjectType& arg_otherType);
		GameObject* GetSpawnPoint(const SceneChangePoint& arg_sceneChangePoint);

		void Remove();
		void Clear();

		//次のシーンのスポーン地点
		static SceneChangePoint nextFieldSpawnPoint;
	};
}