#include "GamePlay.h"

#include "Map.h"
#include "LightManager.h"
#include "Number3DEmitter.h"
#include "Number3DManager.h"
#include "EffectManager.h"
#include "CameraManager.h"

#include "Player.h"
#include "Fighter.h"
#include "BattleCharacter.h"
#include "EnemyData.h"
#include "SkillData.h"
#include "Enemy.h"

#include "Text.h"
#include "Audio.h"
#include "Number.h"
#include "Number3D.h"
#include "Texture2D.h"
#include "InputManager.h"
#include "Util.h"

KochaEngine::GamePlay::GamePlay(Dx12_Wrapper& arg_dx12) : dx12(arg_dx12)
{
	camera = new Camera();
	cameraManager = new CameraManager(*camera);
	gManager = new GameObjectManager();
	bManager = new BattleObjectManager();

	pManager = new ParticleManager();
	pEmitter = new ParticleEmitter(pManager);

	effectManager = new EffectManager(dx12);
	effectManager->LoadEffect("hit.efk", 3.0f); //仮エフェクト
	effectManager->LoadEffect("none.efk", 5.0f); //通常攻撃(斬撃)エフェクト
	effectManager->LoadEffect("fire_0.efk", 2.0f); //ファイア0(呪文)エフェクト
	effectManager->LoadEffect("dark_0.efk", 3.0f); //ダーク0(呪文)エフェクト
	//effectManager->LoadEffect("slash1.efk", 3.0f);
	effectManager->LoadEffect("jab1.efk", 4.0f); //通常攻撃(突き)エフェクト
	effectManager->LoadEffect("levelUp.efk", 3.0f); //レベルアップ時エフェクト
	effectManager->LoadEffect("DeadEnemy.efk", 5.0f); //敵死亡時エフェクト

	n3DManager = new Number3DManager();
	n3DEmitter = new Number3DEmitter(n3DManager);

	map = new Map(gManager, camera);
	lightManager = new LightManager();
	lightManager = LightManager::Create();

	EnemyData::StaticInit(); //エネミーデータの読み込み
	SkillData::StaticInit(); //スキルデータの読み込み

	floor = new Object("ground");
	skyObj = new Object("skydome");

	fadeTexture = new Texture2D("Resources/Texture/Color/white.png", Vector2(0, 0), SCREEN_SIZE, 0);
	defaultWakuTexture = new Texture2D("Resources/Texture/UI/waku_0.png", DEFAULT_WAKU_POS, DEFAULT_WAKU_SIZE, 0);
	anotherWakuTexture = new Texture2D("Resources/Texture/UI/waku_1.png", ANOTHER_WAKU_POS, ANOTHER_WAKU_SIZE, 0);
	defaultCommandTexture = new Texture2D("Resources/Texture/UI/command_1.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	waitCommandTexture = new Texture2D("Resources/Texture/UI/command_0.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	anotherCommandTexture = new Texture2D("Resources/Texture/UI/command_3.png", DEFAULT_COMMAND_POS, ANOTHER_COMMAND_SIZE, 0);

	cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 52);
	cursorTexture = new Texture2D("Resources/Texture/UI/cursor.png", cursorPos, Vector2(16, 16), 0);

	battleLongText = new Text(TALK_LONG_TEXT_POS, Vector2(32, 32));
	battleShortText = new Text(TALK_SHORT_TEXT_POS, Vector2(32, 32));
	battleShortText->SetOneLineFonts(20);
	battleNameText = new Text(TALK_LONG_TEXT_POS, Vector2(32, 32));
	commandTitleText = new Text(DEFAULT_COMMAND_POS + Vector2(5, 5), Vector2(32, 32));
	for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
	{
		const Vector2 ENEMY_NAME_POS = DEFAULT_COMMAND_POS + Vector2(30, 40) + Vector2(0, 32 * i);
		enemyNameText[i] = new Text("fail.txt",ENEMY_NAME_POS, Vector2(32, 32));
	}

	defaultNumberTex = new Number(Vector2(TALK_LONG_TEXT_POS.x - 2, TALK_LONG_TEXT_POS.y + 4), Vector2(24, 24), 5);
}

KochaEngine::GamePlay::~GamePlay()
{
	gManager->Clear();
	delete camera;
	delete cameraManager;
	delete lightManager;
	delete gManager;
	delete bManager;
	delete pManager;
	delete pEmitter;
	delete effectManager;
	delete n3DManager;
	delete n3DEmitter;
	delete map;
	delete floor;
	delete skyObj;
	delete fadeTexture;
	delete defaultWakuTexture;
	delete anotherWakuTexture;
	delete defaultCommandTexture;
	delete waitCommandTexture;
	delete anotherCommandTexture;
	delete cursorTexture;
	delete battleLongText;
	delete battleShortText;
	delete battleNameText;
	delete commandTitleText;
	for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
	{
		delete enemyNameText[i];
	}
	delete defaultNumberTex;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	isBattle = false;
	isBattleEnd = false;
	isBattleStart = false;
	isCommandTitleUpdate = false;
	isTurnUpdate = false;
	isAttackMotion = false;
	isDefenceMotion = false;
	isEnemyTurn = false;
	isCharacterDestroy = false;
	isEnemyDestroy = false;
	isResultOnce = false;
	isShowNumber = false;

	gManager->Clear();
	bManager->Clear();
	gManager->Initialize();
	bManager->Initialize();

	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(1, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//最初のフィールドのマップデータロード
	map->CreateMap(GameSetting::HAZIMARINOTI);

	floor->SetPosition(Vector3(0, 0, 0));
	floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");

	skyObj->SetScale(Vector3(8, 8, 8));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));

	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));
	currentTab = GamePlay::DEFAULT_TAB;

	motionTime = 0;
	frameCount = 0;
	seconds = 0;
	commandNum = 0;
	battleCharaCount = 0;
	battleStartWait = 0;
	resultFlowNum = 0;
	resultFlowWait = 0;
	selectSkillIndex = 0;

	for (int i = 0; i < MAX_COMMAND_NUM; i++)
	{
		preCommandNum[i] = 0;
	}
	
	fadeFlag = false;
	fadeAlpha = 1.0f;
	endCount = 180;

	extraNum = 0;
}

void KochaEngine::GamePlay::Update()
{
	FadeUpdate();
	auto player = gManager->GetPlayer();
	if (player == nullptr) return;
	cameraManager->SetPlayer(player);
	if (player->IsEncount() && !fadeFlag)
	{
		fadeFlag = true;
		if (fadeAlpha >= 1.0f)
		{
			player->EncountReset();
			isBattle = true;
		}
	}
	player->SetIsBattle(isBattle);

	//if (Input::TriggerKey(DIK_H))
	//{
	//}
	//if (Input::TriggerKey(DIK_J))
	//{
	//	effectManager->Play("slash.efk", Vector3(player->GetPosition().x, player->GetPosition().y, player->GetPosition().z - 2));
	//}

	if (isBattle)
	{
		cameraManager->SetCameraState(CameraState::BATTLE_CAMERA_STATE);
		BattleUpdate();
	}
	else
	{
		cameraManager->SetCameraState(CameraState::DEFAULT_CAMERA_STATE);
		FieldUpdate();
	}
}

void KochaEngine::GamePlay::SpriteDraw()
{
	if (isBattle)
	{
		BattleSpriteDraw();
	}
	else
	{
		FieldSpriteDraw();
	}

	fadeTexture->Draw();
}

void KochaEngine::GamePlay::ObjDraw()
{
	if (isBattle)
	{
		BattleObjDraw();
	}
	else
	{
		FieldObjDraw();
	}

}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	if (isBattle)
	{
		BattleAlphaObjDraw();
	}
	else
	{
		FieldAlphaObjDraw();
	}

	effectManager->Update(camera);
}

void KochaEngine::GamePlay::DrawGUI()
{
	if (isBattle)
	{
		ImGui::Text("GamePlay_Battle");
	}
	else
	{
		ImGui::Text("GamePlay_Field");
	}
	ImGui::InputInt("#TalkSpeed", &KochaEngine::GameSetting::talkSpeed, 1);
	ImGui::InputInt("#extraNum", &extraNum, 1);

}

void KochaEngine::GamePlay::Load()
{
}

KochaEngine::Scenes KochaEngine::GamePlay::Next()
{
	gManager->Clear();
	return ENDING;
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

void KochaEngine::GamePlay::BattleInitialize()
{
	isAttackMotion = false;
	isTurnUpdate = false;
	isShowNumber = false;
	battleCharaCount = 0;
	commandNum = 0;
	resultFlowNum = 0;
	battleStartWait = 100;
	currentTab = CommandTab::DEFAULT_TAB;
	for (int i = 0; i < MAX_COMMAND_NUM; i++)
	{
		preCommandNum[i] = 0;
	}

	const Vector3 cameraPos = camera->GetEye();

	//ここにエネミーエミッタークラス的なの作って呼び出す
	//EnemyDataと同様、jsonファイルから出現パターンを読み込めるようにする
	{
		/*今は仮でエネミー追加*/
		int randSpawn = Util::GetIntRand(0, 1);
		if (randSpawn == 0)
		{
			bManager->AddObject(new Enemy(bManager, effectManager, n3DEmitter, cameraPos + SMALL_ENEMY_POS[0], EnemyData::GetEnemyParam("babydoragon")));
			bManager->AddObject(new Enemy(bManager, effectManager, n3DEmitter, cameraPos + MEDIUM_ENEMY_POS[1], EnemyData::GetEnemyParam("nihuteriza")));
			bManager->AddObject(new Enemy(bManager, effectManager, n3DEmitter, cameraPos + MEDIUM_ENEMY_POS[2], EnemyData::GetEnemyParam("nihuteriza")));
		}
		else
		{
			bManager->AddObject(new Enemy(bManager, effectManager, n3DEmitter, cameraPos + MEDIUM_ENEMY_POS[0], EnemyData::GetEnemyParam("nihuteriza")));
			bManager->AddObject(new Enemy(bManager, effectManager, n3DEmitter, cameraPos + MEDIUM_ENEMY_POS[1], EnemyData::GetEnemyParam("nihuteriza")));
		}
	}

	//バトルキャラクターの追加処理
	{
		//キャラのパラメーターを持ってくる
		auto playerParam = gManager->GetPlayer()->GetParam();
		auto fighterParam = gManager->GetFighter()->GetParam();

		//パラメーターをセットしてバトルキャラクター生成
		bManager->AddObject(new BattleCharacter(effectManager, n3DEmitter, BATTLE_PLAYER, cameraPos + BATTLE_CHARACTOR_POS[0], playerParam));
		bManager->AddObject(new BattleCharacter(effectManager, n3DEmitter, BATTLE_FIGHTER, cameraPos + BATTLE_CHARACTOR_POS[2], fighterParam));
	}


	////敵出現テキスト再生
	//if (bManager->GetEnemyCount() == 1)
	//{
	//	//○○があらわれた
	//	battleLongText->ReText("Talk/Battle/EnemySpawn.txt");
	//}
	//else
	//{
	//	//まもののむれがあらわれた
	//	battleLongText->ReText("Talk/Battle/EnemySpawn.txt");
	//}

	battleLongText->SetText("Talk/Battle/EnemySpawn.txt");
	battleLongText->SetSound("Resources/Sound/text1.wav");
	commandTitleText->SetText("default.txt");
}

void KochaEngine::GamePlay::BattleUpdate()
{
	//デバッグ用
	{
	//	if (Input::TriggerKey(DIK_F1))
	//	{
	//		//サンプルテキスト再生
	//		battleLongText->ReText("Talk/Battle/CharaDestroy_0.txt");
	//	}
		if (Input::TriggerKey(DIK_P))
		{
			////サンプルテキスト再生
			//battleLongText->ReText("yuma.txt");
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

	//バトル開始時に一度だけ通る処理
	if (!isBattleStart)
	{
		isBattleStart = true;
		//バトルシーンの初期化処理
		BattleInitialize();
	}
	if (battleStartWait > 0)
	{
		battleStartWait--;
	}

	//バトルの一連の流れの処理のUpdate
	BattleFlowUpdate();

	//カーソルのポジションを更新
	CursorPosSetting();

	//バトルシーン終了処理//
	if (fadeAlpha >= 1.0f && isBattleEnd)
	{
		isBattle = false;
		isBattleEnd = false;
		isCharacterDestroy = false;
		isEnemyDestroy = false;
		isResultOnce = false;
		bManager->Clear();
	}

	gManager->Update();
	bManager->Update();
	pManager->Update();
	n3DManager->Update();
	cameraManager->Update();
	lightManager->Update();

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::BattleFlowUpdate()
{
	if (battleStartWait > 0) return;

	bManager->TargetOff();

	//キャラ・エネミーの行動が終わったら更新
	if (!isTurnUpdate)
	{
		isTurnUpdate = true;

		TurnInitialize();
	}


	//ここからコマンド操作等の処理//

	//キャラクターかエネミーどちらかが全滅していないとき
	if (!isCharacterDestroy && !isEnemyDestroy)
	{
		//行動中のキャラがいない場合
		if (!currentActiveActor)
		{
			//行動できるキャラ・エネミーが居なくなったので一巡が終了
			//全キャラ・エネミーを行動可能状態にする
			bManager->ActiveReset();
			isTurnUpdate = false;
			isEnemyTurn = true;
		}
		else //行動中のキャラ・エネミーがいる場合
		{
			ActiveActorUpdate();
		}
	}
	else
	{
		//バトル終了時演出
		ResultUpdate();
	}
}

void KochaEngine::GamePlay::BattleObjDraw()
{
	gManager->ObjDrawFieldScene(camera, lightManager);
	floor->Draw(camera, lightManager);
	skyObj->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::BattleAlphaObjDraw()
{
	gManager->AlphaObjDrawBattleScene(camera, lightManager);
	bManager->ObjDraw(camera, lightManager);
	n3DManager->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::BattleSpriteDraw()
{
	//バトル時のスプライト描画
	//defaultWakuTexture->Draw();

	if (!isAttackMotion && !isEnemyTurn)
	{
		//キャラクターのターン中
		CommandDraw();
		commandTitleText->Draw(0);
		cursorTexture->Draw(cursorPos);
	}
	else
	{
		//キャラクターのターン中以外のとき
		defaultWakuTexture->Draw();
		waitCommandTexture->Draw();
		battleLongText->Draw(KochaEngine::GameSetting::talkSpeed);
	}

	if ((isAttackMotion || isDefenceMotion) && !isResultOnce)
	{
		//名前は７文字まで
		battleNameText->Draw(KochaEngine::GameSetting::talkSpeed);
	}

	if (isShowNumber)
	{
		if (resultFlowNum == 1)
		{
			defaultNumberTex->Draw(getExp);
		}
		else
		{
			defaultNumberTex->Draw(bManager->GetTotalMoney());
		}
	}


	bManager->SpriteDraw();

}

void KochaEngine::GamePlay::FieldUpdate()
{
	gManager->Update();
	pManager->Update();
	n3DManager->Update();
	cameraManager->Update();
	lightManager->Update();

	isBattleStart = false;

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::FieldObjDraw()
{
	gManager->ObjDrawFieldScene(camera, lightManager);
	floor->Draw(camera, lightManager);
	skyObj->Draw(camera, lightManager);
	pManager->Draw(camera, lightManager);
	n3DManager->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::FieldAlphaObjDraw()
{
	gManager->AlphaObjDrawFieldScene(camera, lightManager);
}

void KochaEngine::GamePlay::FieldSpriteDraw()
{
	gManager->SpriteDraw();
}

void KochaEngine::GamePlay::BattleEnd()
{
	fadeFlag = true;
	isBattleEnd = true;

	//ステータスを反映
	BattleObject* character = nullptr;

	character = bManager->GetCharacter(BattleObjectType::BATTLE_PLAYER);
	if (character)
	{
		gManager->GetPlayer()->SetParam(character->GetBaseParam());
	}
	character = bManager->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
	if (character)
	{
		gManager->GetFighter()->SetParam(character->GetBaseParam());
	}

}

void KochaEngine::GamePlay::TurnInitialize()
{
	//キャラクターが全滅したら
	if (bManager->IsCharacterDestroy())
	{
		isCharacterDestroy = true;
		return;
	}
	//エネミーが全滅したら
	if (bManager->IsEnemyDestroy())
	{
		isEnemyDestroy = true;
		return;
	}

	//現在行動中のキャラ・エネミーを持ってくる
	currentActiveActor = bManager->GetCurrentActive();

	//キャラ・エネミーを行動中状態にする
	if (currentActiveActor != nullptr)
	{
		currentActiveActor->CurrentActive();
	}

	//コマンド上に表示する名前を更新するフラグ
	isCommandTitleUpdate = false;

	isAttackMotion = false;

	currentTab = CommandTab::DEFAULT_TAB;
	commandNum = 0;
	selectSkillIndex = 0;
	for (int i = 0; i < MAX_COMMAND_NUM; i++)
	{
		preCommandNum[i] = 0;
	}

	//エネミーの名前描画を更新
	EnemyNameUpdate();

	//カメラの位置をバトル開始時の初期位置に戻す
	cameraManager->SetDefaultPosition();
}

void KochaEngine::GamePlay::ActiveActorUpdate()
{
	//バトルオブジェクトタイプを持ってくる
	auto currentActiveActorType = currentActiveActor->GetType();

	//バトルオブジェクトタイプがキャラだったら
	if (currentActiveActorType == BATTLE_PLAYER ||
		currentActiveActorType == BATTLE_FIGHTER)
	{
		isEnemyTurn = false;
		if (!isCommandTitleUpdate)
		{
			isCommandTitleUpdate = true;
			//現在コマンド操作中のキャラの名前を表示
			commandTitleText->SetText(currentActiveActor->GetBaseParam().name);
			//どうする？
			battleLongText->SetText("Talk/Battle/ChooseAction.txt");
			battleLongText->SetSound("Resources/Sound/text1.wav");
		}

		//こうげきモーション中は操作できないようにする
		if (!isAttackMotion)
		{
			//コマンドのカーソル操作
			MoveCursor();

			//カメラのフォーカス(奥行)を現在こうどう中のキャラに合わせる
			float eyeZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
			float targetZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
			cameraManager->SetBattleEyePositionZ(eyeZ);
			cameraManager->SetBattleTargetPositionZ(targetZ);
		}


	}
	else //バトルオブジェクトタイプがエネミーだったら
	{
		isEnemyTurn = true;
		EnemyActionSelect();
	}

	AttackMotionUpdate();
	DefenceMotionUpdate();
}

void KochaEngine::GamePlay::EnemyActionSelect()
{
	if (isAttackMotion) return;

	isAttackMotion = true;

	//行動中のエネミーのスキルの数を調べる
	int maxSkillIndex = currentActiveActor->GetSkillCount();
	//ランダムで使用するスキルの選択
	selectSkillIndex = Util::GetIntRand(1, maxSkillIndex) - 1;
	//そのスキルの名前をもらう
	std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);
	//もらった名前をKeyにしてスキルのデータを持ってくる
	SkillParam activeSkill = SkillData::GetSkillParam(activeSkillName);

	//スキルの効果対象を調べる(敵か味方か)
	if (activeSkill.isAllies) //エネミー側
	{
		//全体か単体か調べる
		if (activeSkill.isOverall) //全体
		{

		}
		else //単体
		{

		}
	}
	else //キャラクター側
	{
		//全体か単体か調べる
		if (activeSkill.isOverall) //全体
		{

		}
		else //単体
		{
			BattleObject* character = nullptr;
			while (true)
			{
				int charaSelectRandom = Util::GetIntRand(0, 2);
				switch (charaSelectRandom)
				{
				case 0:
					character = bManager->GetCharacter(BattleObjectType::BATTLE_PLAYER);
					break;
				case 1:
					character = bManager->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
					break;
				case 2:

					break;
				default:
					break;
				}

				if (character)
				{
					if (!character->IsKnockDown()) break;
				}

			}
			targetActor = character;
		}
	}

	motionTime = ATTACK_MOTION_TIME;

	//○○(名前)
	battleNameText->SetText(currentActiveActor->GetBaseParam().name);
	battleNameText->SetSound("Resources/Sound/text1.wav");
	//○○のこうどう！
	battleLongText->SetText("Talk/Battle/AttackAction.txt");
	battleLongText->SetSound("Resources/Sound/text1.wav");

	//カメラのフォーカス(奥行)を現在こうどう中のエネミーに合わせる
	float eyeZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
	float targetZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
	cameraManager->SetBattleEyePositionZ(eyeZ);
	cameraManager->SetBattleTargetPositionZ(targetZ);
}

void KochaEngine::GamePlay::AttackMotionUpdate()
{
	if (!isAttackMotion) return;
	if (motionTime > 0)
	{
		motionTime--;
	}
	if (motionTime == FOCUS_MOTION_TIME) //構える
	{
		currentActiveActor->SetAttackTextureIndex(0);

		//カメラのフォーカス(奥行)をターゲットに合わせる
		float eyeZ = targetActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
		float targetZ = targetActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
		cameraManager->SetBattleEyePositionZ(eyeZ);
		cameraManager->SetBattleTargetPositionZ(targetZ);

		//カメラのフォーカス(左右)をターゲットに寄せる
		if (currentActiveActor->GetType() == BattleObjectType::ENEMY)
		{
			cameraManager->MoveBattleEyePositionX(ATTACK_FOCUS_X);
			cameraManager->MoveBattleTargetPositionX(ATTACK_FOCUS_X);
		}
		else
		{
			cameraManager->MoveBattleEyePositionX(-ATTACK_FOCUS_X);
			cameraManager->MoveBattleTargetPositionX(-ATTACK_FOCUS_X);
		}
	}
	else if (motionTime == ACTIVE_MOTION_TIME) //行動する
	{
		std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);

		//ダメージ計算処理
		//ダメージを与える
		targetActor->SetDamage(activeSkillName, currentActiveActor->GetBattleParam());
		
		//こうげきアニメーション
		currentActiveActor->SetAttackTextureIndex(1);

		//こうげきエフェクト
		if (targetActor->GetType() != BattleObjectType::ENEMY)
		{
			cameraManager->SetCameraShake(5.0f);
		}

	}
	else if (motionTime == 0) //行動を終了する
	{
		isTurnUpdate = false;
		//行動済みにする
		currentActiveActor->ActiveDone();
		//行動中状態を解除する
		currentActiveActor->CurrentActiveReset();

		currentActiveActor->SetDefaultWaitTexture();
	}
}

void KochaEngine::GamePlay::DefenceMotionUpdate()
{

}

void KochaEngine::GamePlay::ResultUpdate()
{
	//リザルト遷移時に一度だけ通る処理
	if (!isResultOnce)
	{
		isResultOnce = true;
		resultFlowWait = RESULT_INPUT_WAIT;
		//カメラの位置をバトル開始時の初期位置に戻す
		cameraManager->SetDefaultPosition();
	}

	if (resultFlowWait <= 0)
	{
		//リザルト中ボタンを押して次へ進む
		if (InputManager::TriggerDecision() ||
			InputManager::TriggerCancel())
		{
			//テキストが再生中かどうか

			if (!battleLongText->IsPlayEnd())
			{
				//再生をスキップ
				battleLongText->Skip();
			}
			else
			{
				//次のリザルトへ
				resultFlowNum++;
				resultFlowWait = RESULT_INPUT_WAIT;
			}
		}
	}
	else
	{
		resultFlowWait--;
	}


	//キャラクターが全滅していた場合
	if (isCharacterDestroy)
	{
		//ボタン入力時一瞬だけ通るようにする
		if (resultFlowWait == RESULT_INPUT_WAIT - 1)
		{
			switch (resultFlowNum)
			{
			case 0:
				//めのまえがまっくらになった
				battleLongText->SetText("Talk/Battle/CharaDestroy_0.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				break;
			case 1:
				//おかねをはんぶんうしなった
				battleLongText->SetText("Talk/Battle/CharaDestroy_1.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				break;
			case 2:
				//バトルを終了する
				BattleEnd();
				break;
			default:
				break;
			}
		}

	}
	//エネミーが全滅していた場合
	else if (isEnemyDestroy)
	{
		//ボタン入力時一瞬だけ通るようにする
		if (resultFlowWait == RESULT_INPUT_WAIT - 1)
		{
			switch (resultFlowNum)
			{
			case 0:
				//まものをすべてたおした！
				battleLongText->SetText("Talk/Battle/EnemyDestroy_0.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				bManager->Reward();
				break;
			case 1:
				//○○のけいけんちをかくとく！
				battleLongText->SetText("Talk/Battle/EnemyDestroy_1.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				RewardCalc();
				isShowNumber = true;
				break;
			case 2:
				//○○ゴールドてにいれた！
				battleLongText->SetText("Talk/Battle/EnemyDestroy_2.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");

				break;
			case 3:
				//バトルを終了する
				BattleEnd();
				break;
			default:
				break;
			}

		}
		//エネミー全滅時テキスト再生

	}
}

void KochaEngine::GamePlay::RewardCalc()
{
	BattleObject* character = nullptr;
	const float EXP_RATE = 1.1f - (float)battleCharaCount * 0.1f;
	getExp = (float)bManager->GetTotalExp() * EXP_RATE;
	if (getExp <= 0) getExp = 1; //最低でも１の経験値は保障

	character = bManager->GetCharacter(BattleObjectType::BATTLE_PLAYER);
	if (character)
	{
		if(!character->IsKnockDown())
		{
			character->AddExp(getExp);
		}
		character->AddMoney(bManager->GetTotalMoney());
	}
	character = bManager->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
	if (character && !character->IsKnockDown())
	{
		character->AddExp(getExp);
	}
	//character = bManager->GetCharacter(BattleObjectType::BATTLE_PLAYER);
	//if (character != nullptr)
	//{
	//	character->AddExp(getExp);
	//}
	
}

void KochaEngine::GamePlay::EnemyNameUpdate()
{
	//こうげきコマンドに表示される敵の名前の更新
	for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
	{
		auto enemy = bManager->GetEnemy(i + 1);
		if (enemy == nullptr)
		{
			enemyNameText[i]->SetText("none.txt");
			continue;
		}
		enemyNameText[i]->SetText(enemy->GetBaseParam().name);
	}
}

void KochaEngine::GamePlay::CommandDraw()
{
	//auto enemyCount = bManager->GetEnemyCount();
	switch (currentTab)
	{
	case KochaEngine::GamePlay::DEFAULT_TAB: //デフォルトコマンド
		defaultWakuTexture->Draw();
		defaultCommandTexture->Draw();
		battleLongText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::ATTACK_TAB: //こうげきコマンド
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();

		for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
		{
			enemyNameText[i]->Draw(0);
		}
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::SKILL_TAB: //スキルコマンド
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::ITEM_TAB: //どうぐコマンド
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	default:
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	}
}

void KochaEngine::GamePlay::MoveCursor()
{
	if (InputManager::TriggerUp())
	{	
		commandNum = CommandNumUp(commandNum);
	}
	else if (InputManager::TriggerDown())
	{
		commandNum = CommandNumDown(commandNum);
	}
	if (InputManager::TriggerDecision())
	{
		//決定ボタンを押したとき
		switch (currentTab)
		{
		case KochaEngine::GamePlay::DEFAULT_TAB: //デフォルトコマンド
			preCommandNum[0] = commandNum; //直前の入力の保存
			DefaultTab();
			break;
		case KochaEngine::GamePlay::ATTACK_TAB: //こうげきコマンド
			preCommandNum[1] = commandNum; //直前の入力の保存
			AttackTab();
			break;
		case KochaEngine::GamePlay::SKILL_TAB: //じゅもんコマンド
			preCommandNum[2] = commandNum; //直前の入力の保存
			break;
		case KochaEngine::GamePlay::ITEM_TAB: //とくぎコマンド
			preCommandNum[3] = commandNum; //直前の入力の保存
			break;
		default:
			break;
		}
	}
	else if (InputManager::TriggerCancel())
	{
		//キャンセルボタンを押したとき
		switch (currentTab)
		{
		case KochaEngine::GamePlay::DEFAULT_TAB: //デフォルトコマンド
			////どうする？
			//battleLongText->SetText("Talk/Battle/ChooseAction.txt");
			break;
		case KochaEngine::GamePlay::ATTACK_TAB: //こうげきコマンド
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		case KochaEngine::GamePlay::SKILL_TAB: //スキルコマンド
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		case KochaEngine::GamePlay::ITEM_TAB: //どうぐコマンド
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		default:
			break;
		}
	}

	if (currentTab == GamePlay::ATTACK_TAB)
	{
		auto enemy = bManager->GetEnemy(commandNum + 1);
		if (enemy)
		{
			enemy->TargetOn();
		}
	}

}

void KochaEngine::GamePlay::CursorPosSetting()
{
	switch (commandNum)
	{
	case 0:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 52);
		break;
	case 1:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 84);
		break;
	case 2:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 116);
		break;
	case 3:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 148);
		break;
	case 4:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 180);
		break;
	default:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 52);
		break;
	}
}

void KochaEngine::GamePlay::DefaultTab()
{
	//デフォルトコマンドで決定が押されたとき
	switch (commandNum)
	{
	case 0: //こうげき
		commandNum = preCommandNum[1]; //保存されたコマンドに切り替え
		currentTab = GamePlay::ATTACK_TAB; //こうげきコマンドへ
		break;
	case 1: //スキル
		commandNum = preCommandNum[2]; //保存されたコマンドに切り替え
		currentTab = GamePlay::SKILL_TAB; //スキルコマンドへ
		break;
	case 2: //どうぐ
		commandNum = preCommandNum[3]; //保存されたコマンドに切り替え
		currentTab = GamePlay::ITEM_TAB; //どうぐコマンドへ
		break;
	case 3: //ぼうぎょ
		//currentActiveActor->GetParam().diffence
		isTurnUpdate = false;
		//行動済みにする
		currentActiveActor->ActiveDone();
		//行動中状態を解除する
		currentActiveActor->CurrentActiveReset();
		break;
	case 4: //にげる
		//バトルシーン終了処理
		BattleEnd();
		break;
	default:
		
		break;
	}
}

void KochaEngine::GamePlay::AttackTab()
{
	auto enemy = bManager->GetEnemy(commandNum + 1);
	if (enemy)
	{
		isAttackMotion = true;
		motionTime = ATTACK_MOTION_TIME;
		selectSkillIndex = 0;
		targetActor = enemy;
		//○○(名前)
		battleNameText->SetText(currentActiveActor->GetBaseParam().name);
		battleNameText->SetSound("Resources/Sound/text1.wav");
		//○○のこうどう！
		battleLongText->SetText("Talk/Battle/AttackAction.txt");
		battleLongText->SetSound("Resources/Sound/text1.wav");
	}
	else
	{
		//なしを選択した場合
		//不可音を鳴らす
		int a = 0;
	}
}

unsigned int KochaEngine::GamePlay::CommandNumUp(const unsigned int arg_commandNum)
{
	unsigned int _commandNum = arg_commandNum;
	if (_commandNum > 0)
	{
		_commandNum--;
	}
	else
	{
		_commandNum = MAX_COMMAND_NUM;
	}
	return _commandNum;
}

unsigned int KochaEngine::GamePlay::CommandNumDown(const unsigned int arg_commandNum)
{
	unsigned int _commandNum = arg_commandNum;
	if (_commandNum < MAX_COMMAND_NUM)
	{
		_commandNum++;
	}
	else
	{
		_commandNum = 0;
	}
	return _commandNum;
}
