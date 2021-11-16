#include "GamePlay.h"
#include "Input.h"
#include "Util.h"
#include "Map.h"
#include "LightManager.h"
#include "Player.h"
#include "Fighter.h"
#include "Text.h"
#include "GameSetting.h"
#include "EnemyData.h"
#include "Enemy.h"
#include "BattleCharacter.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();
	battle_gManager = new GameObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
	map = new Map(gManager, camera);
	lightManager = new LightManager();
	lightManager = LightManager::Create();
	EnemyData::StaticInit();

	floor = new Object("graund");
	skyObj = new Object("skydome");

	fadeTexture = new Texture2D("Resources/Texture/Color/white.png", Vector2(0, 0), SCREEN_SIZE, 0);
	defaultWakuTexture = new Texture2D("Resources/Texture/UI/waku_0.png", DEFAULT_WAKU_POS, DEFAULT_WAKU_SIZE, 0);
	defaultCommandTexture = new Texture2D("Resources/Texture/UI/command_1.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	battleStatusTexture = new Texture2D("Resources/Texture/UI/command_2.png", BATTLE_STATUS_POS, BATTLE_STATUS_SIZE, 0);

	cursorTexture = new Texture2D("Resources/Texture/UI/cursor.png", DEFAULT_COMMAND_POS + Vector2(10,52), Vector2(16, 16), 0);

	text = new Text("Talk/Field/Sample1.txt", TALK_TEXT_POS, Vector2(32, 32));
}

KochaEngine::GamePlay::~GamePlay()
{
	gManager->RemoveAll();
	delete camera;
	delete lightManager;
	delete gManager;
	delete pManager;
	delete emitter;
	delete map;
	delete floor;
	delete skyObj;
	delete fadeTexture;
	delete defaultWakuTexture;
	delete battleStatusTexture;
	delete defaultCommandTexture;
	delete cursorTexture;
	delete text;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	isBattle = false;
	isBattleEnd = false;
	isBattleStart = false;

	gManager->RemoveAll();
	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(1, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//最初のフィールドのマップデータロード
	map->CreateMap(GameSetting::HAZIMARINOTI);

	floor->SetPosition(Vector3(0, 0, 0));
	floor->SetTexture("Resources/Texture/Tiling/tiling_stone1.png");

	skyObj->SetScale(Vector3(8, 8, 8));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));

	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));

	frameCount = 0;
	seconds = 0;
	
	fadeFlag = false;
	fadeAlpha = 1.0f;
	endCount = 180;
}

void KochaEngine::GamePlay::Update()
{
	FadeUpdate();
	auto player = gManager->GetPlayer();
	if (player == nullptr) return;
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

	if (isBattle)
	{
		BattleUpdate();
	}
	else
	{
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
}

void KochaEngine::GamePlay::Load()
{
}

KochaEngine::Scenes KochaEngine::GamePlay::Next()
{
	gManager->RemoveAll();
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

void KochaEngine::GamePlay::BattleUpdate()
{
	//バトルシーン開始//

	//バトル開始時に一度だけ通る処理
	if (!isBattleStart)
	{
		isBattleStart = true;

		//敵出現テキスト再生
		text->ReText("Talk/Field/Sample1.txt");

		const Vector3 cameraPos = camera->GetEye();

		//ここにエネミーエミッタークラス的なの作って呼び出す
		{
			/*今は仮でエネミー追加*/
			int aaa = Util::GetIntRand(0, 1);
			if (aaa == 0)
			{
				gManager->AddObject(new Enemy(cameraPos + MEDIUM_ENEMY_POS[0], EnemyData::GetEnemyParam(NIHUTERIZA)));
				gManager->AddObject(new Enemy(cameraPos + SMALL_ENEMY_POS[1], EnemyData::GetEnemyParam(BABYDORAGON)));
				gManager->AddObject(new Enemy(cameraPos + MEDIUM_ENEMY_POS[2], EnemyData::GetEnemyParam(NIHUTERIZA)));
				gManager->AddObject(new Enemy(cameraPos + MEDIUM_ENEMY_POS[3], EnemyData::GetEnemyParam(NIHUTERIZA)));
			}
			else
			{
				gManager->AddObject(new Enemy(cameraPos + SMALL_ENEMY_POS[0], EnemyData::GetEnemyParam(BABYDORAGON)));
				gManager->AddObject(new Enemy(cameraPos + SMALL_ENEMY_POS[1], EnemyData::GetEnemyParam(BABYDORAGON)));
			}
		}

		//バトルキャラクターの追加処理
		{
			//キャラのパラメーターをもらう
			auto playerParam = gManager->GetPlayer()->GetParam();
			auto fighterParam = gManager->GetFighter()->GetParam();

			//パラメーターをセットしてバトルキャラクター生成
			gManager->AddObject(new BattleCharacter(BATTLE_PLAYER, cameraPos + BATTLE_CHARACTOR_POS[0], playerParam));
			gManager->AddObject(new BattleCharacter(BATTLE_FIGHTER, cameraPos + BATTLE_CHARACTOR_POS[2], fighterParam));
		}

	}

	if (Input::TriggerKey(DIK_T))
	{
		text->ReText("Talk/Field/Sample1.txt");
	}

	//ここからコマンド操作等の処理




	//バトルシーン終了
	if (Input::TriggerKey(DIK_SPACE))
	{
		fadeFlag = true;
		isBattleEnd = true;
	}
	if (fadeAlpha >= 1.0f && isBattleEnd)
	{
		isBattle = false;
		isBattleEnd = false;
		gManager->RemoveBattleObject();
	}

	gManager->Update();
	pManager->Update();
	camera->Update();
	lightManager->Update();

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::BattleObjDraw()
{
	gManager->ObjDraw(camera, lightManager);
	floor->Draw(camera, lightManager);
	skyObj->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::BattleAlphaObjDraw()
{
	gManager->AlphaObjDraw2(camera, lightManager);
}

void KochaEngine::GamePlay::BattleSpriteDraw()
{
	defaultWakuTexture->Draw();
	battleStatusTexture->Draw();
	defaultCommandTexture->Draw();
	cursorTexture->Draw();
	text->Draw(KochaEngine::GameSetting::talkSpeed);
}

void KochaEngine::GamePlay::FieldUpdate()
{
	gManager->Update();
	pManager->Update();
	camera->Update();
	lightManager->Update();

	isBattleStart = false;

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::FieldObjDraw()
{
	gManager->ObjDraw(camera, lightManager);
	floor->Draw(camera, lightManager);
	skyObj->Draw(camera, lightManager);
	pManager->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::FieldAlphaObjDraw()
{
	gManager->AlphaObjDraw(camera, lightManager);
}

void KochaEngine::GamePlay::FieldSpriteDraw()
{
	gManager->SpriteDraw();
}
