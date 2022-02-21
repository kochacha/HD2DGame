#include "GamePlay.h"

#include "Battle.h"

#include "Map.h"
#include "LightManager.h"
#include "Number3DEmitter.h"
#include "Number3DManager.h"
#include "EffectManager.h"
#include "CameraManager.h"

#include "Enemy.h"
#include "Player.h"
#include "Fighter.h"
#include "BattleCharacter.h"
#include "EnemyData.h"
#include "SkillData.h"

#include "Audio.h"
#include "Number.h"
#include "Number3D.h"
#include "Texture2D.h"
#include "InputManager.h"
#include "Util.h"

KochaEngine::GamePlay::GamePlay(Dx12_Wrapper& arg_dx12) : dx12(arg_dx12)
{
	//カメラの生成
	camera = std::make_shared<Camera>();
	cameraManager = std::make_shared <CameraManager>(camera);

	//ゲームオブジェクト管理クラスの生成
	gManager = std::make_shared<GameObjectManager>();
	bManager = std::make_shared<BattleObjectManager>();

	//パーティクル関連の生成
	pManager = std::make_shared<ParticleManager>();
	pEmitter = std::make_shared<ParticleEmitter>(pManager.get());

	//エフェクト管理クラス生成
	effectManager = std::make_shared<EffectManager>(dx12);
	//エフェクトのロード
	effectManager->LoadEffect("hit.efk", 3.0f); //仮エフェクト
	effectManager->LoadEffect("none.efk", 5.0f); //通常攻撃(斬撃)エフェクト
	effectManager->LoadEffect("fire_0.efk", 2.0f); //ファイア0(呪文)エフェクト
	effectManager->LoadEffect("dark_0.efk", 3.0f); //ダーク0(呪文)エフェクト
	effectManager->LoadEffect("jab1.efk", 4.0f); //通常攻撃(突き)エフェクト
	effectManager->LoadEffect("levelUp.efk", 3.0f); //レベルアップ時エフェクト
	effectManager->LoadEffect("DeadEnemy.efk", 5.0f); //敵死亡時エフェクト
	//effectManager->LoadEffect("slash1.efk", 3.0f);

	//3D空間用数字描画管理クラス等生成
	n3DManager = std::make_shared<Number3DManager>();
	n3DEmitter = std::make_shared<Number3DEmitter>(n3DManager.get());

	//フィールド作成用クラスの生成
	map = std::make_unique<Map>(gManager, camera);

	//ライト管理クラスの生成
	lightManager = std::make_shared<LightManager>();
	lightManager.reset(LightManager::Create());

	//バトル用クラスの生成
	battleScene = std::make_unique<Battle>(camera, cameraManager, gManager, bManager, effectManager, n3DEmitter, lightManager);

	EnemyData::StaticInit(); //エネミーデータの読み込み
	SkillData::StaticInit(); //スキルデータの読み込み

	//地面＆天球の生成
	floor = std::make_unique<Object>("ground");
	skyObj = std::make_unique<Object>("skydome");

	//フェードアウト用テクスチャの生成
	fadeTexture = std::make_unique<Texture2D>("Resources/Texture/Color/white.png", Vector2(0, 0), SCREEN_SIZE, 0);
}

KochaEngine::GamePlay::~GamePlay()
{
	gManager->Clear();
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	isChangeScene = false;

	//念のためオブジェクトを削除しておく
	gManager->Clear();
	bManager->Clear();
	//各種オブジェクト管理クラスの初期化
	gManager->Initialize();
	bManager->Initialize();

	//カメラの初期化
	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });

	//ライトマネージャーの初期化
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//最初のフィールドのマップデータロード
	map->CreateMap(GameSetting::nowField);

	//フィールドにプレイヤーの生成
	auto _spawnPoint = gManager.get()->GetSpawnPoint(GameObjectManager::nextFieldSpawnPoint);
	if (!_spawnPoint)
	{
		//スポーン地点が無かった場合は(0,0)地点にとりあえず生成
		gManager.get()->AddObject(new Player(gManager, Vector3(0, 5, 0)));
		gManager.get()->AddObject(new Fighter(Vector3(0, 5, 0)));
	}
	else
	{
		//スポーン地点に生成
		auto _spawnPos = _spawnPoint->GetPosition();
		gManager.get()->AddObject(new Player(gManager, _spawnPos));
		gManager.get()->AddObject(new Fighter(_spawnPos));
	}

	//バトルシーンの初期化
	battleScene->Initialize();

	//プレイヤーをもらう
	fieldPlayer = gManager->GetPlayer();

	//カメラマネージャーにプレイヤーをセット
	cameraManager->SetPlayer(fieldPlayer);

	//敵とエンカウントするフィールドかどうか
	GameSetting::isBattleField = true;

	//地面のパラメーターセット
	floor->SetPosition(Vector3(0, 0, 0));
	if (GameSetting::nowField == GameSetting::HAZIMARINOTI)
	{
		floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");
	}
	else
	{
		floor->SetTexture("Resources/Texture/Tiling/tiling_floor5.png");
	}
	//floor->SetTexture("Resources/Texture/Tiling/tiling_yuki1.png");

	//天球のパラメーターセット
	skyObj->SetScale(Vector3(15, 15, 15));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
	//skyObj->SetTexture("Resources/Texture/Color/black.png");

	//フェードアウト用テクスチャの色を設定
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));
	
	frameCount = 0;
	seconds = 0;
	extraNum = 0;
	
	fadeFlag = false;
	fadeAlpha = 1.0f;
}

void KochaEngine::GamePlay::Update()
{
	//フェード関連の更新
	FadeUpdate();

	//エンカウントしたらフェード処理を入れてバトルシーンへ
	if (fieldPlayer->IsEncount() && !fadeFlag)
	{
		fadeFlag = true;
		if (fadeAlpha >= 1.0f)
		{
			fieldPlayer->EncountReset();
			battleScene->IsBattleTrue();
		}
	}
	bool _isBattle = battleScene->IsBattle();
	fieldPlayer->SetIsBattle(_isBattle);

	//デバッグキー(画面効果)
	if (Input::TriggerKey(DIK_J))
	{
		//草原
		floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");
		GameSetting::environmentNumber = 1;
		GameSetting::isEnvironmentUpdate = true;
	}
	else if (Input::TriggerKey(DIK_K))
	{
		//砂漠
		floor->SetTexture("Resources/Texture/Tiling/tiling_suna1.png");
		GameSetting::environmentNumber = 2;
		GameSetting::isEnvironmentUpdate = true;
	}
	else if (Input::TriggerKey(DIK_L))
	{
		//雪原
		floor->SetTexture("Resources/Texture/Tiling/tiling_yuki0.png");
		GameSetting::environmentNumber = 3;
		GameSetting::isEnvironmentUpdate = true;
	}

	//デバッグキー(パーティクルチェック)
	if (Input::TriggerKey(DIK_P))
	{
		Vector3 _kiriPos = Vector3(fieldPlayer->GetPosition().x - 20, fieldPlayer->GetPosition().y, fieldPlayer->GetPosition().z - 3);
		pEmitter->Kiri0(_kiriPos);
	}

	//デバッグキー(別フィールドへの遷移)
	if (Input::TriggerKey(DIK_H))
	{
		fadeFlag = true;
		isChangeScene = true;
	}
	if (fieldPlayer->IsNextFieldScene())
	{
		fadeFlag = true;
		isChangeScene = true;
	}
	if (isChangeScene && fadeAlpha >= 1.0f)
	{
		isEnd = true;
	}

	if (_isBattle)
	{
		//カメラの状態をバトル時の状態に切り替え
		cameraManager->SetCameraState(CameraState::BATTLE_CAMERA_STATE);

		//バトル時の更新
		BattleUpdate();
	}
	else
	{
		//カメラの状態をフィールド時の状態に切り替え
		cameraManager->SetCameraState(CameraState::DEFAULT_CAMERA_STATE);

		//バトル以外の時の更新
		FieldUpdate();
	}

	//それぞれのマネージャーを更新
	gManager->Update();
	bManager->Update();
	pManager->Update();
	n3DManager->Update();
	cameraManager->Update();
	lightManager->Update();

	//天球をゆっくり回転させる
	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::SpriteDraw()
{
	if (battleScene->IsBattle())
	{
		//バトル時のスプライト描画
		battleScene->SpriteDraw();
	}
	else
	{
		//フィールド時のスプライト描画
		FieldSpriteDraw();
	}

	//フェードアウト用スプライト描画
	fadeTexture->Draw();
}

void KochaEngine::GamePlay::ObjDraw()
{
	if (battleScene->IsBattle())
	{
		//バトル時の非透明オブジェクトの描画
		BattleObjDraw();
	}
	else
	{
		//フィールド時の非透明オブジェクトの描画
		FieldObjDraw();
	}

	//地面と天球の描画
	floor->Draw(camera.get(), lightManager.get());
	skyObj->Draw(camera.get(), lightManager.get());
}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	if (battleScene->IsBattle())
	{
		//バトル時の透明オブジェクトの描画
		BattleAlphaObjDraw();
	}
	else
	{
		//フィールド時の透明オブジェクトの描画
		FieldAlphaObjDraw();
	}

	//各種マネージャーによる描画
	pManager->Draw(camera.get(), lightManager.get());
	n3DManager->Draw(camera.get(), lightManager.get());

	//エフェクトの更新＆描画
	effectManager->Update(camera.get());
}

//GUIの描画関連
void KochaEngine::GamePlay::DrawGUI()
{
	if (battleScene->IsBattle())
	{
		ImGui::Text("GamePlay_Battle");
	}
	else
	{
		ImGui::Text("GamePlay_Field");
	}
	ImGui::InputInt("#TalkSpeed", &KochaEngine::GameSetting::talkSpeed, 1);
	ImGui::InputInt("#extraNum", &extraNum, 1);
	ImGui::Checkbox("#isBattleField", &GameSetting::isBattleField);

}

void KochaEngine::GamePlay::Load()
{
}

KochaEngine::Scenes KochaEngine::GamePlay::Next()
{
	gManager->Clear();
	return GAMEPLAY;
}

void KochaEngine::GamePlay::CountTime()
{
	frameCount++;
	if (frameCount >= 60)
	{
		seconds++;
		frameCount = 0;
	}
}

//フェード関連の更新
void KochaEngine::GamePlay::FadeUpdate()
{
	const float MOVE_ALPHA = 0.05f;
	if (!fadeFlag)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= MOVE_ALPHA;
		}
	}
	else
	{
		if (fadeAlpha < 1.0f)
		{
			fadeAlpha += MOVE_ALPHA;
		}
		else
		{
			fadeFlag = false;
		}
	}
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, fadeAlpha));
}

void KochaEngine::GamePlay::BattleUpdate()
{
	//デバッグ用
	{
		if (Input::TriggerKey(DIK_F1))
		{
			//サンプルテキスト再生
			//battleLongText->SetText("sample99.txt");
		}
		if (Input::TriggerKey(DIK_P))
		{
			////サンプルテキスト再生
			//battleLongText->SetText("yuma.txt");
			bManager->GetCharacter(BattleObjectType::BATTLE_PLAYER)->AddExp(extraNum);
		}
	//	if (Input::TriggerKey(DIK_E))
	//	{
	//		//テキストスキップ
	//		battleLongText->Skip();
	//	}
	//	if (Input::TriggerKey(DIK_Q))
	//	{
	//		//もらってきたエネミーの名前描画
	//		auto enemy = bManager->GetEnemy(1);
	//		if (enemy != nullptr)
	//		{
	//			battleLongText->ReText(enemy->GetParam().name);
	//		}
	//	}
	//	if (Input::TriggerKey(DIK_SPACE))
	//	{
	//		//バトルシーン終了
	//		BattleEnd();
	//	}
	}

	//バトルシーン開始//

	//ここにバトル更新処理
	battleScene->Update();

	fadeFlag = battleScene->IsFade();

	//バトルシーン終了処理//
	if (fadeAlpha >= 1.0f && battleScene->IsBattleEnd())
	{
		battleScene->BattleEndUpdate();
	}
}

void KochaEngine::GamePlay::BattleObjDraw()
{
	gManager->ObjDrawBattleScene(camera.get(), lightManager.get());
}

void KochaEngine::GamePlay::BattleAlphaObjDraw()
{
	gManager->AlphaObjDrawBattleScene(camera.get(), lightManager.get());
	battleScene->AlphaObjDraw();
}

void KochaEngine::GamePlay::FieldUpdate()
{
	//非バトル状態に設定
	battleScene->IsBattleStartFalse();
}

void KochaEngine::GamePlay::FieldObjDraw()
{
	gManager->ObjDrawFieldScene(camera.get(), lightManager.get());
}

void KochaEngine::GamePlay::FieldAlphaObjDraw()
{
	gManager->AlphaObjDrawFieldScene(camera.get(), lightManager.get());
}

void KochaEngine::GamePlay::FieldSpriteDraw()
{
	gManager->SpriteDraw();
}
