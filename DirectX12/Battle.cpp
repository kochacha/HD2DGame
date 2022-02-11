#include "Battle.h"

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

KochaEngine::Battle::Battle(std::weak_ptr<Camera> arg_camera, std::weak_ptr<CameraManager> arg_cameraManager,
	std::weak_ptr<GameObjectManager> arg_gManager, std::weak_ptr<BattleObjectManager> arg_bManager,
	std::weak_ptr<EffectManager> arg_effectManager, std::weak_ptr<Number3DEmitter> arg_n3DEmitter,
	std::weak_ptr<LightManager> arg_lightManager)
	: camera(arg_camera), cameraManager(arg_cameraManager), gManager(arg_gManager), bManager(arg_bManager), effectManager(arg_effectManager), n3DEmitter(arg_n3DEmitter), lightManager(arg_lightManager)
{
	defaultWakuTexture = std::make_unique<Texture2D>("Resources/Texture/UI/waku_0.png", DEFAULT_WAKU_POS, DEFAULT_WAKU_SIZE, 0);
	anotherWakuTexture = std::make_unique<Texture2D>("Resources/Texture/UI/waku_1.png", ANOTHER_WAKU_POS, ANOTHER_WAKU_SIZE, 0);
	defaultCommandTexture = std::make_unique<Texture2D>("Resources/Texture/UI/command_1.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	waitCommandTexture = std::make_unique<Texture2D>("Resources/Texture/UI/command_0.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	anotherCommandTexture = std::make_unique<Texture2D>("Resources/Texture/UI/command_3.png", DEFAULT_COMMAND_POS, ANOTHER_COMMAND_SIZE, 0);
	pageCommandTexture = std::make_unique<Texture2D>("Resources/Texture/UI/command_4.png", DEFAULT_COMMAND_POS, ANOTHER_COMMAND_SIZE + Vector2(0, 32), 0);
	spCommandTexture = std::make_unique<Texture2D>("Resources/Texture/UI/command_5.png", ANOTHER_WAKU_POS + Vector2(0, -55), SP_COMMAND_SIZE, 0);

	cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 52);
	cursorTexture = std::make_unique<Texture2D>("Resources/Texture/UI/cursor.png", cursorPos, Vector2(16, 16), 0);

	battleLongText = std::make_unique<Text>(TALK_LONG_TEXT_POS, Vector2(32, 32));
	battleShortText = std::make_unique<Text>(TALK_SHORT_TEXT_POS, Vector2(32, 32));
	battleShortText->SetOneLineFonts(20);
	summaryText = std::make_unique<Text>(TALK_SHORT_TEXT_POS, Vector2(32, 32));
	summaryText->SetOneLineFonts(20);
	battleNameText = std::make_unique<Text>(TALK_LONG_TEXT_POS, Vector2(32, 32));
	commandTitleText = std::make_unique<Text>(DEFAULT_COMMAND_POS + Vector2(5, 5), Vector2(32, 32));
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		const Vector2 NAME_TEXT_COMMAND_POS = DEFAULT_COMMAND_POS + Vector2(30, 40) + Vector2(0, 32 * i);
		enemyNameText[i] = std::make_unique<Text>("fail.txt", NAME_TEXT_COMMAND_POS, Vector2(32, 32));
		skillNameText[i] = std::make_unique<Text>("fail.txt", NAME_TEXT_COMMAND_POS, Vector2(32, 32));
	}

	defaultNumberTex = std::make_unique<Number>(Vector2(TALK_LONG_TEXT_POS.x - 2, TALK_LONG_TEXT_POS.y + 4), Vector2(24, 24), 5);
	costSPNumberTex = std::make_unique<Number>(Vector2(TALK_SHORT_TEXT_POS.x + 34, TALK_SHORT_TEXT_POS.y - 58), Vector2(16, 16), 4);
	pageNumberTex = std::make_unique<Number>(Vector2(TALK_LONG_TEXT_POS.x - 2, TALK_LONG_TEXT_POS.y + 4), Vector2(24, 24), 2);
	maxPageNumberTex = std::make_unique<Number>(Vector2(TALK_LONG_TEXT_POS.x - 2, TALK_LONG_TEXT_POS.y + 4), Vector2(24, 24), 2);
}

KochaEngine::Battle::~Battle()
{
}

void KochaEngine::Battle::Initialize()
{
	fadeFlag = false;
	isBattle = false;
	isBattleEnd = false;
	isBattleStart = false;
	isCommandTitleUpdate = false;
	isTurnUpdate = false;
	isActiveMotion = false;
	isDefenceMotion = false;
	isEnemyTurn = false;
	isCharacterDestroy = false;
	isEnemyDestroy = false;
	isResultOnce = false;
	isShowNumber = false;
	isUpdateCommandExplanation = false;

	currentTab = Battle::DEFAULT_TAB;
	previousTab = currentTab;

	motionTime = 0;
	commandNum = 0;
	battleCharaCount = 0;
	battleStartWait = 0;
	resultFlowNum = 0;
	resultFlowWait = 0;
	selectSkillIndex = 0;
	skillTabPageNum = 1;
	skillTabMaxPageNum = skillTabPageNum;
	preSkillTabPageNum = skillTabPageNum;
	costSP = 0;
	preCommandNum = 0;
}

void KochaEngine::Battle::Update()
{
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

}

void KochaEngine::Battle::SpriteDraw()
{
	if (!isActiveMotion && !isEnemyTurn)
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

	if ((isActiveMotion || isDefenceMotion) && !isResultOnce)
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
			defaultNumberTex->Draw(bManager.lock()->GetTotalMoney());
		}
	}

	bManager.lock()->SpriteDraw();
}

void KochaEngine::Battle::ObjDraw()
{
}

void KochaEngine::Battle::AlphaObjDraw()
{
	bManager.lock()->ObjDraw(camera.lock().get(), lightManager.lock().get());
}

void KochaEngine::Battle::DrawGUI()
{
}

void KochaEngine::Battle::Load()
{
}

//バトル終了時の処理
void KochaEngine::Battle::BattleEndUpdate()
{
	fadeFlag = false;
	isBattle = false;
	isBattleEnd = false;
	isCharacterDestroy = false;
	isEnemyDestroy = false;
	isResultOnce = false;
	bManager.lock()->Clear();
}

void KochaEngine::Battle::BattleInitialize()
{
	isActiveMotion = false;
	isTurnUpdate = false;
	isShowNumber = false;
	battleCharaCount = 0;
	commandNum = 0;
	resultFlowNum = 0;
	battleStartWait = 100;
	currentTab = CommandTab::DEFAULT_TAB;
	previousTab = currentTab;
	preCommandNum = 0;

	const Vector3 cameraPos = camera.lock()->GetEye();

	//戦闘開始時にカメラの初期位置をセットする
	gManager.lock()->SetBattleCameraDefaultPos(cameraPos);

	//ここにエネミーエミッタークラス的なの作って呼び出す
	//EnemyDataと同様、jsonファイルから出現パターンを読み込めるようにする
	{
		/*今は仮でエネミー追加*/
		int randSpawn = Util::GetIntRand(0, 1);
		if (randSpawn == 0)
		{
			bManager.lock()->AddObject(new Enemy(bManager.lock().get(), effectManager.lock().get(), n3DEmitter.lock().get(), cameraPos + SMALL_ENEMY_POS[0], EnemyData::GetEnemyParam("babydoragon")));
			bManager.lock()->AddObject(new Enemy(bManager.lock().get(), effectManager.lock().get(), n3DEmitter.lock().get(), cameraPos + MEDIUM_ENEMY_POS[1], EnemyData::GetEnemyParam("nihuteriza")));
			bManager.lock()->AddObject(new Enemy(bManager.lock().get(), effectManager.lock().get(), n3DEmitter.lock().get(), cameraPos + MEDIUM_ENEMY_POS[2], EnemyData::GetEnemyParam("nihuteriza")));
		}
		else
		{
			bManager.lock()->AddObject(new Enemy(bManager.lock().get(), effectManager.lock().get(), n3DEmitter.lock().get(), cameraPos + MEDIUM_ENEMY_POS[0], EnemyData::GetEnemyParam("nihuteriza")));
			bManager.lock()->AddObject(new Enemy(bManager.lock().get(), effectManager.lock().get(), n3DEmitter.lock().get(), cameraPos + MEDIUM_ENEMY_POS[1], EnemyData::GetEnemyParam("nihuteriza")));
		}
	}

	//バトルキャラクターの追加処理
	{
		//キャラのパラメーターを持ってくる
		auto playerParam = gManager.lock()->GetPlayer()->GetParam();
		auto fighterParam = gManager.lock()->GetFighter()->GetParam();

		//パラメーターをセットしてバトルキャラクター生成
		bManager.lock()->AddObject(new BattleCharacter(effectManager.lock().get(), n3DEmitter.lock().get(), BATTLE_PLAYER, cameraPos + BATTLE_CHARACTOR_POS[0], playerParam));
		bManager.lock()->AddObject(new BattleCharacter(effectManager.lock().get(), n3DEmitter.lock().get(), BATTLE_FIGHTER, cameraPos + BATTLE_CHARACTOR_POS[2], fighterParam));
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

void KochaEngine::Battle::BattleFlowUpdate()
{
	if (battleStartWait > 0) return;

	bManager.lock()->TargetOff();

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
			bManager.lock()->ActiveReset();
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

void KochaEngine::Battle::BattleEnd()
{
	fadeFlag = true;
	isBattleEnd = true;

	//ステータスを反映
	BattleObject* character = nullptr;

	character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_PLAYER);
	if (character)
	{
		gManager.lock()->GetPlayer()->SetParam(character->GetBaseParam());
	}
	character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
	if (character)
	{
		gManager.lock()->GetFighter()->SetParam(character->GetBaseParam());
	}

}

void KochaEngine::Battle::TurnInitialize()
{
	//キャラクターが全滅したら
	if (bManager.lock()->IsCharacterDestroy())
	{
		isCharacterDestroy = true;
		return;
	}
	//エネミーが全滅したら
	if (bManager.lock()->IsEnemyDestroy())
	{
		isEnemyDestroy = true;
		return;
	}

	//現在行動中のキャラ・エネミーを持ってくる
	currentActiveActor = bManager.lock()->GetCurrentActive();

	//キャラ・エネミーを行動中状態にする
	if (currentActiveActor)
	{
		currentActiveActor->CurrentActive();

		//スキルの名前描画を更新
		if (currentActiveActor->GetType() == BATTLE_PLAYER ||
			currentActiveActor->GetType() == BATTLE_FIGHTER)
		{
			SkillNameUpdate();

			//スキルタブの最大ページ数を更新する
			skillTabMaxPageNum = (currentActiveActor->GetSkillCount() + MAX_NAME_TEXT_COUNT_COMMAND - 2) / MAX_NAME_TEXT_COUNT_COMMAND;
		}
	}

	//コマンド上に表示する名前を更新するフラグ
	isCommandTitleUpdate = false;

	isActiveMotion = false;

	currentTab = CommandTab::DEFAULT_TAB;
	previousTab = currentTab;
	commandNum = 0;
	selectSkillIndex = 0;
	skillTabPageNum = 1;
	preSkillTabPageNum = skillTabPageNum;
	preCommandNum = 0;

	//エネミーの名前描画を更新
	EnemyNameUpdate();

	//カメラの位置をバトル開始時の初期位置に戻す
	cameraManager.lock()->SetDefaultPosition();
}

void KochaEngine::Battle::ActiveActorUpdate()
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
		if (!isActiveMotion)
		{
			//コマンドのカーソル操作
			MoveCursor();

			//カメラのフォーカス(奥行)を現在こうどう中のキャラに合わせる
			Vector3 _currentActiveActorPos = currentActiveActor->GetPosition();
			float eyeZ = _currentActiveActorPos.z + BATTLE_FOCUS_EYE_Z;
			float targetZ = _currentActiveActorPos.z + BATTLE_FOCUS_TARGET_Z;
			cameraManager.lock()->SetBattleEyePositionZ(eyeZ);
			cameraManager.lock()->SetBattleTargetPositionZ(targetZ);
		}


	}
	else //バトルオブジェクトタイプがエネミーだったら
	{
		isEnemyTurn = true;
		EnemyActionSelect();
	}

	ActiveMotionUpdate();
	DefenceMotionUpdate();
}

void KochaEngine::Battle::EnemyActionSelect()
{
	if (isActiveMotion) return;

	isActiveMotion = true;

	//行動中のエネミーのスキルの数を調べる
	int maxSkillIndex = currentActiveActor->GetSkillCount();
	//ランダムで使用するスキルの選択
	selectSkillIndex = Util::GetIntRand(1, maxSkillIndex) - 1;
	//そのスキルの名前をもらう
	std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);
	//もらった名前をKeyにしてスキルのデータを持ってくる
	SkillParam activeSkill = SkillData::GetSkillParam(activeSkillName);

	//ここでエネミーのAI(これも別でクラス作って管理したい)
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
					character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_PLAYER);
					break;
				case 1:
					character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
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
	Vector3 _currentActiveActorPos = currentActiveActor->GetPosition();
	float eyeZ = _currentActiveActorPos.z + BATTLE_FOCUS_EYE_Z;
	float targetZ = _currentActiveActorPos.z + BATTLE_FOCUS_TARGET_Z;
	cameraManager.lock()->SetBattleEyePositionZ(eyeZ);
	cameraManager.lock()->SetBattleTargetPositionZ(targetZ);
}

void KochaEngine::Battle::ActiveMotionUpdate()
{
	if (!isActiveMotion) return;
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
		cameraManager.lock()->SetBattleEyePositionZ(eyeZ);
		cameraManager.lock()->SetBattleTargetPositionZ(targetZ);

		//カメラのフォーカス(左右)をターゲットに寄せる
		if (currentActiveActor->GetType() == BattleObjectType::BATTLE_ENEMY)
		{
			cameraManager.lock()->MoveBattleEyePositionX(ATTACK_FOCUS_X);
			cameraManager.lock()->MoveBattleTargetPositionX(ATTACK_FOCUS_X);
		}
		else
		{
			cameraManager.lock()->MoveBattleEyePositionX(-ATTACK_FOCUS_X);
			cameraManager.lock()->MoveBattleTargetPositionX(-ATTACK_FOCUS_X);
		}
	}
	else if (motionTime == ACTIVE_MOTION_TIME) //行動する
	{
		std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);

		//ダメージ計算処理
		//ダメージを与える
		targetActor->SetDamage(activeSkillName, currentActiveActor->GetBattleParam());
		currentActiveActor->CostSP(SkillData::GetSkillParam(activeSkillName).cost);

		//こうげきアニメーション
		currentActiveActor->SetAttackTextureIndex(1);

		//ダメージ受けた時のカメラ揺れ
		if (targetActor->GetType() != BattleObjectType::BATTLE_ENEMY)
		{
			cameraManager.lock()->SetCameraShake(5.0f);
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

void KochaEngine::Battle::DefenceMotionUpdate()
{

}

void KochaEngine::Battle::ResultUpdate()
{
	//リザルト遷移時に一度だけ通る処理
	if (!isResultOnce)
	{
		isResultOnce = true;
		resultFlowWait = RESULT_INPUT_WAIT;
		//カメラの位置をバトル開始時の初期位置に戻す
		cameraManager.lock()->SetDefaultPosition();
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
				bManager.lock()->Reward();
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

//リザルト時の経験値とお金を追加
void KochaEngine::Battle::RewardCalc()
{
	BattleObject* character = nullptr;

	//バトル参加キャラ数に応じて1キャラあたりの獲得経験値が変動する
	//経験値レート = 基礎倍率1.0倍 / 参加キャラ数(1人の時等倍)
	const float EXP_RATE = 1.0f / (float)battleCharaCount;
	//一人当たり獲得経験値 = 総獲得経験値 × 経験値レート
	getExp = (float)bManager.lock()->GetTotalExp() * EXP_RATE;

	//経験値がマイナスにならないように補正
	if (getExp < 0) getExp = 0;

	character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_PLAYER);
	if (character)
	{
		//棺桶状態だと経験値が入らない
		if (!character->IsKnockDown())
		{
			character->AddExp(getExp);
		}
		//プレイヤーのみ、お金を追加
		character->AddMoney(bManager.lock()->GetTotalMoney());
	}
	character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
	if (character)
	{
		//棺桶状態だと経験値が入らない
		if (!character->IsKnockDown())
		{
			character->AddExp(getExp);
		}
	}
}

void KochaEngine::Battle::EnemyNameUpdate()
{
	//こうげきコマンドに表示される敵の名前の更新
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		auto enemy = bManager.lock()->GetEnemy(i + 1);
		if (!enemy)
		{
			//エネミーがいない場合、「なし」を描画
			enemyNameText[i]->SetText("none.txt");
			continue;
		}
		//エネミーの名前を設定
		enemyNameText[i]->SetText(enemy->GetBaseParam().name);
	}
}

//ページをまたぐときに呼び出す
void KochaEngine::Battle::SkillNameUpdate()
{
	//スキルコマンドに表示されるスキルの名前の更新
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		int _indexNum = (skillTabPageNum - 1) * 5 + i + 1;
		auto skill = currentActiveActor->GetSkillName(_indexNum);
		if (skill == "noSkill")
		{
			//スキルが無い場合、「なし」を描画
			skillNameText[i]->SetText("none.txt");
			continue;
		}
		//スキルの名前を設定
		skillNameText[i]->SetText(SkillData::GetSkillParam(skill).name);
	}
}

void KochaEngine::Battle::SkillExplanationUpdate()
{
	if (!isUpdateCommandExplanation) return;
	isUpdateCommandExplanation = false;

	selectSkillIndex = (skillTabPageNum - 1) * MAX_NAME_TEXT_COUNT_COMMAND + commandNum + 1;
	std::string _skillName = currentActiveActor->GetSkillName(selectSkillIndex);
	if (_skillName != "noSkill")
	{
		auto _skillData = SkillData::GetSkillParam(_skillName);
		auto _textName = _skillData.summaryName;
		//スキル説明文の更新
		summaryText->SetText(_textName);
		//表示用スキル消費SP更新
		costSP = _skillData.cost;
	}
	else
	{
		//スキル説明文の更新
		summaryText->SetText("default.txt");
		//表示用スキル消費SP更新
		costSP = 0;
	}
}

void KochaEngine::Battle::CommandDraw()
{
	switch (currentTab)
	{
	case KochaEngine::Battle::DEFAULT_TAB: //デフォルトコマンド
		defaultWakuTexture->Draw();
		defaultCommandTexture->Draw();
		battleLongText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::Battle::TARGET_SELECT_TAB: //ターゲット選択コマンド
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();

		for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
		{
			enemyNameText[i]->Draw(0);
		}
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::Battle::SKILL_TAB: //スキルコマンド
		anotherWakuTexture->Draw();
		pageCommandTexture->Draw();
		spCommandTexture->Draw();

		costSPNumberTex->Draw(costSP);
		//ここにskillTabNumDraw追加
		//ここにskillTabMaxNumDraw追加

		for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
		{
			skillNameText[i]->Draw(0);
		}
		summaryText->Draw(0);
		break;
	case KochaEngine::Battle::ITEM_TAB: //どうぐコマンド
		anotherWakuTexture->Draw();
		pageCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	default:
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	}
}

void KochaEngine::Battle::MoveCursor()
{
	if (InputManager::TriggerUp())
	{
		commandNum = CommandNumUp(commandNum);
		isUpdateCommandExplanation = true;
	}
	else if (InputManager::TriggerDown())
	{
		commandNum = CommandNumDown(commandNum);
		isUpdateCommandExplanation = true;
	}
	//決定ボタンを押したとき
	if (InputManager::TriggerDecision())
	{
		preCommandNum = commandNum; //直前の入力の保存
		isUpdateCommandExplanation = true;

		switch (currentTab)
		{
		case KochaEngine::Battle::DEFAULT_TAB: //デフォルトコマンド
			previousTab = currentTab;
			DefaultTab();
			break;
		case KochaEngine::Battle::TARGET_SELECT_TAB: //ターゲット選択コマンド
			TargetSelectTab();
			break;
		case KochaEngine::Battle::SKILL_TAB: //スキルコマンド
			previousTab = currentTab;
			SkillTab();
			break;
		case KochaEngine::Battle::ITEM_TAB: //どうぐコマンド
			previousTab = currentTab;
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
		case KochaEngine::Battle::DEFAULT_TAB: //デフォルトコマンド
			////どうする？
			//battleLongText->SetText("Talk/Battle/ChooseAction.txt");
			break;
		case KochaEngine::Battle::TARGET_SELECT_TAB: //ターゲット選択コマンド
			isUpdateCommandExplanation = true;
			commandNum = preCommandNum;
			currentTab = previousTab;
			break;
		case KochaEngine::Battle::SKILL_TAB: //スキルコマンド
			isUpdateCommandExplanation = true;
			commandNum = preCommandNum;
			currentTab = Battle::DEFAULT_TAB;
			break;
		case KochaEngine::Battle::ITEM_TAB: //どうぐコマンド
			commandNum = preCommandNum;
			currentTab = Battle::DEFAULT_TAB;
			break;
		default:
			break;
		}
	}

	switch (currentTab)
	{
	case KochaEngine::Battle::DEFAULT_TAB: //デフォルトコマンド

		break;
	case KochaEngine::Battle::TARGET_SELECT_TAB: //ターゲット選択コマンド

		break;
	case KochaEngine::Battle::SKILL_TAB: //スキルコマンド
		if (InputManager::TriggerRight())
		{
			//直前のページ状態保存
			preSkillTabPageNum = skillTabPageNum;
			//ページを進める
			skillTabPageNum = CommandNumRight(skillTabPageNum, skillTabMaxPageNum);
			//ページをまたいでいるのでスキルページを更新
			if (preSkillTabPageNum != skillTabPageNum)
			{
				SkillNameUpdate();
			}

		}
		else if (InputManager::TriggerLeft())
		{
			//直前のページ状態保存
			preSkillTabPageNum = skillTabPageNum;
			//ページを戻す
			skillTabPageNum = CommandNumLeft(skillTabPageNum, skillTabMaxPageNum);
			//ページをまたいでいるのでスキルページを更新
			if (preSkillTabPageNum != skillTabPageNum)
			{
				SkillNameUpdate();
			}
		}
		SkillExplanationUpdate();
		break;
	case KochaEngine::Battle::ITEM_TAB: //どうぐコマンド

		break;
	default:
		break;
	}

	if (currentTab == Battle::TARGET_SELECT_TAB)
	{
		auto enemy = bManager.lock()->GetEnemy(commandNum + 1);
		if (enemy)
		{
			enemy->TargetOn();
		}
	}

}

//カーソルのポジションを設定
void KochaEngine::Battle::CursorPosSetting()
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

void KochaEngine::Battle::DefaultTab()
{
	//デフォルトコマンドで決定が押されたとき
	switch (commandNum)
	{
	case 0: //ターゲット選択
		commandNum = 0;
		selectSkillIndex = 0;
		currentTab = Battle::TARGET_SELECT_TAB; //ターゲット選択コマンドへ
		break;
	case 1: //スキル
		//スキルページを初期化
		skillTabPageNum = 1;
		preSkillTabPageNum = skillTabPageNum;
		commandNum = 0;
		currentTab = Battle::SKILL_TAB; //スキルコマンドへ
		break;
	case 2: //どうぐ
		commandNum = 0;
		currentTab = Battle::ITEM_TAB; //どうぐコマンドへ
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

void KochaEngine::Battle::TargetSelectTab()
{
	//敵か味方か判断してから選択

	auto enemy = bManager.lock()->GetEnemy(commandNum + 1);
	if (enemy)
	{
		isActiveMotion = true;
		motionTime = ATTACK_MOTION_TIME;
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

void KochaEngine::Battle::SkillTab()
{
	//現在選択中のカーソル番号を算出
	selectSkillIndex = (skillTabPageNum - 1) * MAX_NAME_TEXT_COUNT_COMMAND + commandNum + 1;
	//キャラのスキルデータからその番号に当てはまるスキルの名前をもってくる
	std::string _skillName = currentActiveActor->GetSkillName(selectSkillIndex);

	if (_skillName != "noSkill")
	{
		SkillParam _skillData = SkillData::GetSkillParam(_skillName);
		if (_skillData.cost > currentActiveActor->GetBaseParam().sp)
		{
			//SPが足りない場合
			//不可音を鳴らす&SPが足りないテキストを出す
			int a = 0;
			return;
		}
		if (!_skillData.isOverall)
		{
			//単体攻撃
			commandNum = 0;
			currentTab = Battle::TARGET_SELECT_TAB; //ターゲット選択コマンドへ
		}
		else
		{
			//全体攻撃
		}
	}
	else
	{
		//なしを選択した場合
		//不可音を鳴らす
		int a = 0;
		return;
	}
}

unsigned int KochaEngine::Battle::CommandNumUp(const unsigned int arg_commandNum)
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

unsigned int KochaEngine::Battle::CommandNumDown(const unsigned int arg_commandNum)
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

unsigned int KochaEngine::Battle::CommandNumRight(const unsigned int arg_commandNum, const unsigned int arg_maxNum)
{
	unsigned int _commandNum = arg_commandNum;
	if (_commandNum < arg_maxNum)
	{
		_commandNum++;
	}
	else
	{
		_commandNum = 1;
	}
	return _commandNum;
}

unsigned int KochaEngine::Battle::CommandNumLeft(const unsigned int arg_commandNum, const unsigned int arg_maxNum)
{
	unsigned int _commandNum = arg_commandNum;
	if (_commandNum > 1)
	{
		_commandNum--;
	}
	else
	{
		_commandNum = arg_maxNum;
	}
	return _commandNum;
}
