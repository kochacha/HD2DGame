#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "GameSetting.h"
#include <memory>

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

		//所有(共有する)
		std::shared_ptr<Camera> camera;
		std::shared_ptr<CameraManager> cameraManager;
		std::shared_ptr<GameObjectManager> gManager;
		std::shared_ptr<BattleObjectManager> bManager;
		std::shared_ptr<ParticleManager> pManager;
		std::shared_ptr<ParticleEmitter> pEmitter;
		std::shared_ptr<EffectManager> effectManager;
		std::shared_ptr<Number3DManager> n3DManager;
		std::shared_ptr<Number3DEmitter> n3DEmitter;
		std::shared_ptr<LightManager> lightManager;

		//所有(共有しない)
		std::unique_ptr<Map> map;
		std::unique_ptr<Battle> battleScene;
		std::unique_ptr<Object> floor;
		std::unique_ptr<Object> skyObj;
		std::unique_ptr<Texture2D> fadeTexture;

		//もらってくる
		Player* fieldPlayer;

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
