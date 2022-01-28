#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "GameSetting.h"

namespace KochaEngine
{
	class Map;
	class LightManager;
	class EffectManager;
	class Text;
	class Number;
	class Number3D;
	class Audio;
	class Texture2D;
	class Number3DManager;
	class Number3DEmitter;
	class Dx12_Wrapper;
	class CameraManager;
	class Battle;

	class GamePlay : public Scene
	{
	private:
		Dx12_Wrapper& dx12;

		Map* map;
		Camera* camera;
		CameraManager* cameraManager;
		GameObjectManager* gManager;
		BattleObjectManager* bManager;
		ParticleManager* pManager;
		ParticleEmitter* pEmitter;
		EffectManager* effectManager;
		Number3DManager* n3DManager;
		Number3DEmitter* n3DEmitter;
		LightManager* lightManager;

		Battle* battleScene;

		Object* floor;
		Object* skyObj;

		Texture2D* fadeTexture;

		int extraNum;

		//フェードアウトするかどうか
		bool fadeFlag;
		
		//フェードの透明度
		float fadeAlpha;

		int frameCount;
		int seconds;
		int endCount;

		void BattleUpdate();
		void BattleObjDraw();
		void BattleAlphaObjDraw();

		void FieldUpdate();
		void FieldObjDraw();
		void FieldAlphaObjDraw();
		void FieldSpriteDraw();

	public:
		GamePlay(Dx12_Wrapper& arg_dx12);
		~GamePlay() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void AlphaObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
		void CountTime();
		void FadeUpdate();
	};
}
