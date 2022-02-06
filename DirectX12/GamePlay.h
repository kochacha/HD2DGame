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
	class Player;

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
		Player* fieldPlayer;

		Object* floor;
		Object* skyObj;

		Texture2D* fadeTexture;

		//フェードアウトするかどうか
		bool fadeFlag;
		//シーン切り替え用フラグ
		bool isChangeScene;
		
		//フェードの透明度
		float fadeAlpha;

		//フレームカウント用の変数
		int frameCount;
		//秒数(60フレームで1秒)
		int seconds;
		//GUIで操作する予備の変数
		int extraNum;

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
