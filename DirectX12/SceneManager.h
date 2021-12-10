#pragma once
#include "Scene.h"
#include <map>

namespace KochaEngine
{
	class SceneManager
	{
	private:
		std::map<Scenes, Scene*> scenes;
		Scene* currentScene;

	public:
		SceneManager();
		~SceneManager();
		void AddScene(Scenes arg_name, Scene* arg_scene);
		void ChangeScene(Scenes arg_name);
		void Load(Scenes arg_name);
		void Update();
		void SpriteDraw();
		void AlphaObjDraw();
		void ObjDraw();
		void DrawGUI();
		void Terminate();
	};
}