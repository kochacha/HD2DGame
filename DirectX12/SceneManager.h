#pragma once
#include "Scene.h"
#include <map>

namespace KochaEngine
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();
		void AddScene(Scenes name, Scene* scene);
		void ChangeScene(Scenes name);
		void Load(Scenes name);
		void Update();
		void SpriteDraw();
		void ObjDraw();
		void Terminate();
	private:
		std::map<Scenes, Scene*> scenes;
		Scene* currentScene;
	};
}