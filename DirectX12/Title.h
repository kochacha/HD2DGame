#pragma once
#include "Scene.h"
#include "Audio.h"
#include "Object.h"
#include "Camera.h"

namespace KochaEngine
{
	class Texture2D;
	class LightManager;
	class Title : public Scene
	{
	private:
		Camera* camera;
		LightManager* lightManager;
		Object* obj;
		Object* plane;
		std::vector<Object*> _objects;
		Texture2D* backTexture;
		Texture2D* fadeTexture;

		bool bgmFlag;
		bool fadeFlag;
		float fadeAlpha;
		Vector3 cameraPos;

		void FadeUpdate();
		void LoadLevelEditorData();

	public:
		Title();
		~Title() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
	};
}