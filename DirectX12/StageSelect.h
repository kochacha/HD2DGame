#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Number.h"

namespace KochaEngine
{
	class StageSelect : public Scene
	{
	private:
		Camera* camera;
		GameObjectManager* gManager;

		int textCount;

		DirectX::XMFLOAT2 texPos;
		DirectX::XMFLOAT2 texSize;
		DirectX::XMFLOAT2 startPos;
		DirectX::XMFLOAT2 endPos;

		ParticleManager* pManager;
		ParticleEmitter* emitter;

		int moveCount;
		bool countFlag;

		float fadeAlpha;
		bool fadeFlag;

	public:
		StageSelect();
		~StageSelect() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void Load() override;
		Scenes Next() override;

	};
}
