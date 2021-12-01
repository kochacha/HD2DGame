#include "GamePlay.h"
#include "InputManager.h"
#include "Util.h"
#include "Map.h"
#include "LightManager.h"
#include "Player.h"
#include "Fighter.h"
#include "Text.h"
#include "EnemyData.h"
#include "Enemy.h"
#include "BattleCharacter.h"

KochaEngine::GamePlay::GamePlay()
{
	camera = new Camera();
	gManager = new GameObjectManager();
	bManager = new BattleObjectManager();

	pManager = new ParticleManager();
	emitter = new ParticleEmitter(pManager);
	map = new Map(gManager, camera);
	lightManager = new LightManager();
	lightManager = LightManager::Create();
	EnemyData::StaticInit();

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
}

KochaEngine::GamePlay::~GamePlay()
{
	gManager->RemoveAll();
	delete camera;
	delete lightManager;
	delete gManager;
	delete bManager;
	delete pManager;
	delete emitter;
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

	gManager->RemoveAll();
	bManager->RemoveAll();
	gManager->Initialize();
	bManager->Initialize();

	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(1, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//�ŏ��̃t�B�[���h�̃}�b�v�f�[�^���[�h
	map->CreateMap(GameSetting::HAZIMARINOTI);

	floor->SetPosition(Vector3(0, 0, 0));
	floor->SetTexture("Resources/Texture/Tiling/tiling_stone1.png");

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

	for (int i = 0; i < 5; i++)
	{
		preCommandNum[i] = 0;
	}
	
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

void KochaEngine::GamePlay::BattleInitialize()
{
	isAttackMotion = false;
	isTurnUpdate = false;
	battleCharaCount = 0;
	commandNum = 0;
	resultFlowNum = 0;
	battleStartWait = 100;
	for (int i = 0; i < 5; i++)
	{
		preCommandNum[i] = 0;
	}

	const Vector3 cameraPos = camera->GetEye();

	//�����ɃG�l�~�[�G�~�b�^�[�N���X�I�Ȃ̍���ČĂяo��
	{
		/*���͉��ŃG�l�~�[�ǉ�*/
		int aaa = Util::GetIntRand(0, 1);
		if (aaa == 0)
		{
			bManager->AddObject(new Enemy(bManager, cameraPos + MEDIUM_ENEMY_POS[0], EnemyData::GetEnemyParam(NIHUTERIZA)));
			bManager->AddObject(new Enemy(bManager, cameraPos + SMALL_ENEMY_POS[1], EnemyData::GetEnemyParam(BABYDORAGON)));
			bManager->AddObject(new Enemy(bManager, cameraPos + MEDIUM_ENEMY_POS[2], EnemyData::GetEnemyParam(NIHUTERIZA)));
			bManager->AddObject(new Enemy(bManager, cameraPos + MEDIUM_ENEMY_POS[3], EnemyData::GetEnemyParam(NIHUTERIZA)));
		}
		else
		{
			bManager->AddObject(new Enemy(bManager, cameraPos + SMALL_ENEMY_POS[0], EnemyData::GetEnemyParam(BABYDORAGON)));
			bManager->AddObject(new Enemy(bManager, cameraPos + SMALL_ENEMY_POS[1], EnemyData::GetEnemyParam(BABYDORAGON)));
		}
	}

	//�o�g���L�����N�^�[�̒ǉ�����
	{
		//�L�����̃p�����[�^�[�������Ă���
		auto playerParam = gManager->GetPlayer()->GetParam();
		auto fighterParam = gManager->GetFighter()->GetParam();

		//�p�����[�^�[���Z�b�g���ăo�g���L�����N�^�[����
		bManager->AddObject(new BattleCharacter(BATTLE_PLAYER, cameraPos + BATTLE_CHARACTOR_POS[0], playerParam));
		bManager->AddObject(new BattleCharacter(BATTLE_FIGHTER, cameraPos + BATTLE_CHARACTOR_POS[2], fighterParam));
	}


	////�G�o���e�L�X�g�Đ�
	//if (bManager->GetEnemyCount() == 1)
	//{
	//	//�����������ꂽ
	//	battleLongText->ReText("Talk/Battle/EnemySpawn.txt");
	//}
	//else
	//{
	//	//�܂��̂̂ނꂪ�����ꂽ
	//	battleLongText->ReText("Talk/Battle/EnemySpawn.txt");
	//}

	battleLongText->ReText("Talk/Battle/EnemySpawn.txt");
	commandTitleText->ReText("default.txt");
}

void KochaEngine::GamePlay::BattleUpdate()
{
	//�f�o�b�O�p
	{
		if (Input::TriggerKey(DIK_F1))
		{
			//�T���v���e�L�X�g�Đ�
			battleLongText->ReText("Talk/Battle/CharaDestroy_0.txt");
		}
		if (Input::TriggerKey(DIK_F2))
		{
			//�T���v���e�L�X�g�Đ�
			battleLongText->ReText("Talk/Battle/CharaDestroy_1.txt");
		}
		if (Input::TriggerKey(DIK_E))
		{
			//�e�L�X�g�X�L�b�v
			battleLongText->Skip();
		}
		if (Input::TriggerKey(DIK_Q))
		{
			//������Ă����G�l�~�[�̖��O�`��
			auto enemy = bManager->GetEnemy(1);
			if (enemy != nullptr)
			{
				battleLongText->ReText(enemy->GetParam().name);
			}
		}
		if (Input::TriggerKey(DIK_SPACE))
		{
			//�o�g���V�[���I��
			BattleEnd();
		}
	}

	//�o�g���V�[���J�n//

	//�o�g���J�n���Ɉ�x�����ʂ鏈��
	if (!isBattleStart)
	{
		isBattleStart = true;
		//�o�g���V�[���̏���������
		BattleInitialize();
	}
	if (battleStartWait > 0)
	{
		battleStartWait--;
	}

	//�o�g���̈�A�̗���̏�����Update
	BattleFlowUpdate();

	//�J�[�\���̃|�W�V�������X�V
	CursorPosSetting();

	//�o�g���V�[���I������//
	if (fadeAlpha >= 1.0f && isBattleEnd)
	{
		isBattle = false;
		isBattleEnd = false;
		isCharacterDestroy = false;
		isEnemyDestroy = false;
		isResultOnce = false;
		bManager->RemoveAll();
	}

	gManager->Update();
	bManager->Update();
	pManager->Update();
	camera->Update();
	lightManager->Update();

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::BattleFlowUpdate()
{
	if (battleStartWait > 0) return;

	//�L�����E�G�l�~�[�̍s�����I�������X�V
	if (!isTurnUpdate)
	{
		isTurnUpdate = true;

		TurnInitialize();
	}


	//��������R�}���h���쓙�̏���//

	//�L�����N�^�[���G�l�~�[�ǂ��炩���S�ł��Ă��Ȃ��Ƃ�
	if (!isCharacterDestroy && !isEnemyDestroy)
	{
		//�s�����̃L���������Ȃ��ꍇ
		if (currentActiveActor == nullptr)
		{
			//�s���ł���L�����E�G�l�~�[�����Ȃ��Ȃ����̂ňꏄ���I��
			//�S�L�����E�G�l�~�[���s���\��Ԃɂ���
			bManager->ActiveReset();
			isTurnUpdate = false;
		}
		else //�s�����̃L�����E�G�l�~�[������ꍇ
		{
			ActiveActorUpdate();
		}
	}
	else
	{
		//�o�g���I�������o
		ResultUpdate();
	}
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
	bManager->ObjDraw(camera, lightManager);
}

void KochaEngine::GamePlay::BattleSpriteDraw()
{
	//�o�g�����̃X�v���C�g�`��
	//defaultWakuTexture->Draw();

	if (!isAttackMotion && !isEnemyTurn)
	{
		//�L�����N�^�[�̃^�[����
		CommandDraw();
		commandTitleText->Draw(0);
		cursorTexture->Draw(cursorPos);
	}
	else
	{
		//�L�����N�^�[�̃^�[�����ȊO�̂Ƃ�
		defaultWakuTexture->Draw();
		waitCommandTexture->Draw();
		battleLongText->Draw(KochaEngine::GameSetting::talkSpeed);
	}


	bManager->SpriteDraw();

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

void KochaEngine::GamePlay::BattleEnd()
{
	fadeFlag = true;
	isBattleEnd = true;
}

void KochaEngine::GamePlay::TurnInitialize()
{
	//�L�����N�^�[���S�ł�����
	if (bManager->IsCharacterDestroy())
	{
		isCharacterDestroy = true;
		return;
	}
	//�G�l�~�[���S�ł�����
	if (bManager->IsEnemyDestroy())
	{
		isEnemyDestroy = true;
		return;
	}

	//���ݍs�����̃L�����E�G�l�~�[�������Ă���
	currentActiveActor = bManager->GetCurrentActive();

	//�L�����E�G�l�~�[���s������Ԃɂ���
	if (currentActiveActor != nullptr)
	{
		currentActiveActor->CurrentActive();
	}

	//�R�}���h��ɕ\�����閼�O���X�V����t���O
	isCommandTitleUpdate = false;

	isAttackMotion = false;

	currentTab = CommandTab::DEFAULT_TAB;
	commandNum = 0;
	for (int i = 0; i < 5; i++)
	{
		preCommandNum[i] = 0;
	}

	//�G�l�~�[�̖��O�`����X�V
	EnemyNameUpdate();
}

void KochaEngine::GamePlay::ActiveActorUpdate()
{
	//�o�g���I�u�W�F�N�g�^�C�v�������Ă���
	auto currentActiveActorType = currentActiveActor->GetType();

	//�o�g���I�u�W�F�N�g�^�C�v���L������������
	if (currentActiveActorType == BATTLE_PLAYER ||
		currentActiveActorType == BATTLE_FIGHTER)
	{
		isEnemyTurn = false;
		if (!isCommandTitleUpdate)
		{
			isCommandTitleUpdate = true;
			//���݃R�}���h���쒆�̃L�����̖��O��\��
			commandTitleText->ReText(currentActiveActor->GetParam().name);
		}

		//�R�}���h�̃J�[�\������
		if (!isAttackMotion)
		{
			MoveCursor();
		}


	}
	else //�o�g���I�u�W�F�N�g�^�C�v���G�l�~�[��������
	{
		isEnemyTurn = true;

		if (!isAttackMotion)
		{
			isAttackMotion = true;
			motionTime = ATTACK_MOTION_TIME;
			BattleObject* character = nullptr;
			while (true)
			{
				int rand = Util::GetIntRand(0, 2);
				switch (rand)
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
				if (character != nullptr)
				{
					if (!character->IsKnockDown()) break;
				}

			}
			targetActor = character;
		}
	}

	AttackMotionUpdate();
	DefenceMotionUpdate();
}

void KochaEngine::GamePlay::AttackMotionUpdate()
{
	if (!isAttackMotion) return;
	if (motionTime > 0)
	{
		motionTime--;
	}
	if (motionTime == 60) //�_���[�W��^����(�ʏ�U��)
	{
		//�_���[�W�v�Z����
		auto targetParam = targetActor->GetParam();
		auto activeParam = currentActiveActor->GetParam();

		//��b�_���[�W
		int baseDamage = activeParam.attack * 0.5f - targetParam.defence * 0.25f;
		//�^�_���[�W�͈�
		int luckDamageRange = baseDamage * ((float)activeParam.luck * 0.001f);
		//�^�_���[�W
		int luckDamage = Util::GetIntRand(0, luckDamageRange) - luckDamageRange * 0.5f;
		//�g�[�^���_���[�W
		int totalDamage = baseDamage + luckDamage;

		if (totalDamage < 0) totalDamage = 0;

		targetActor->SetDamage(totalDamage);
	}
	if (motionTime == 0) //�s�����I������
	{
		isTurnUpdate = false;
		//�s���ς݂ɂ���
		currentActiveActor->ActiveDone();
		//�s������Ԃ���������
		currentActiveActor->CurrentActiveReset();
	}
}

void KochaEngine::GamePlay::DefenceMotionUpdate()
{

}

void KochaEngine::GamePlay::ResultUpdate()
{
	//���U���g�J�ڎ��Ɉ�x�����ʂ鏈��
	if (!isResultOnce)
	{
		isResultOnce = true;
		resultFlowWait = RESULT_INPUT_WAIT;
	}

	if (resultFlowWait <= 0)
	{
		//���U���g���{�^���������Ď��֐i��
		if (InputManager::TriggerDecision() ||
			InputManager::TriggerCancel())
		{
			//�e�L�X�g���Đ������ǂ���
			if (!battleLongText->IsPlayEnd())
			{
				//�Đ����X�L�b�v
				battleLongText->Skip();
			}
			else
			{
				//���̃��U���g��
				resultFlowNum++;
				resultFlowWait = RESULT_INPUT_WAIT;
			}
		}
	}
	else
	{
		resultFlowWait--;
	}


	//�L�����N�^�[���S�ł��Ă����ꍇ
	if (isCharacterDestroy)
	{
		//�{�^�����͎���u�����ʂ�悤�ɂ���
		if (resultFlowWait == RESULT_INPUT_WAIT - 1)
		{
			switch (resultFlowNum)
			{
			case 0:
				//�߂̂܂����܂�����ɂȂ���
				battleLongText->ReText("Talk/Battle/CharaDestroy_0.txt");
				break;
			case 1:
				//�����˂��͂�Ԃ񂤂��Ȃ���
				battleLongText->ReText("Talk/Battle/CharaDestroy_1.txt");
				break;
			case 2:
				//�o�g�����I������
				BattleEnd();
				break;
			default:
				break;
			}
		}

	}
	//�G�l�~�[���S�ł��Ă����ꍇ
	else if (isEnemyDestroy)
	{
		//�{�^�����͎���u�����ʂ�悤�ɂ���
		if (resultFlowWait == RESULT_INPUT_WAIT - 1)
		{
			switch (resultFlowNum)
			{
			case 0:
				//�܂��̂����ׂĂ��������I
				battleLongText->ReText("Talk/Battle/EnemyDestroy_0.txt");
				break;
			case 1:
				//�H�H�H�H�H
				battleLongText->ReText("Talk/Battle/EnemyDestroy_1.txt");
				break;
			case 2:
				//�o�g�����I������
				BattleEnd();
				break;
			default:
				break;
			}

		}
		//�G�l�~�[�S�Ŏ��e�L�X�g�Đ�

	}
}

void KochaEngine::GamePlay::EnemyNameUpdate()
{
	//���������R�}���h�ɕ\�������G�̖��O�̍X�V
	for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
	{
		auto enemy = bManager->GetEnemy(i + 1);
		if (enemy == nullptr)
		{
			enemyNameText[i]->ReText("none.txt");
			continue;
		}
		enemyNameText[i]->ReText(enemy->GetParam().name);
	}
}

void KochaEngine::GamePlay::CommandDraw()
{
	//auto enemyCount = bManager->GetEnemyCount();
	switch (currentTab)
	{
	case KochaEngine::GamePlay::DEFAULT_TAB: //�f�t�H���g�R�}���h
		defaultWakuTexture->Draw();
		defaultCommandTexture->Draw();
		battleLongText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::ATTACK_TAB: //���������R�}���h
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();

		for (int i = 0; i < MAX_BATTLE_ENEMY; i++)
		{
			enemyNameText[i]->Draw(0);
		}
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::SPELL_TAB: //�������R�}���h
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::SKILL_TAB: //�Ƃ����R�}���h
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h
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
		//����{�^�����������Ƃ�
		switch (currentTab)
		{
		case KochaEngine::GamePlay::DEFAULT_TAB: //�f�t�H���g�R�}���h
			preCommandNum[0] = commandNum; //���O�̓��͂̕ۑ�
			DefaultTab();
			break;
		case KochaEngine::GamePlay::ATTACK_TAB: //���������R�}���h
			preCommandNum[1] = commandNum; //���O�̓��͂̕ۑ�
			AttackTab();
			break;
		case KochaEngine::GamePlay::SPELL_TAB: //�������R�}���h
			preCommandNum[2] = commandNum; //���O�̓��͂̕ۑ�
			break;
		case KochaEngine::GamePlay::SKILL_TAB: //�Ƃ����R�}���h
			preCommandNum[3] = commandNum; //���O�̓��͂̕ۑ�
			break;
		case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h
			preCommandNum[4] = commandNum; //���O�̓��͂̕ۑ�
			break;
		default:
			break;
		}
	}
	else if (InputManager::TriggerCancel())
	{
		//�L�����Z���{�^�����������Ƃ�
		switch (currentTab)
		{
		case KochaEngine::GamePlay::DEFAULT_TAB: //�f�t�H���g�R�}���h
			//�����ǂ���I��ł��������e�L�X�g������
			break;
		case KochaEngine::GamePlay::ATTACK_TAB: //���������R�}���h
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		case KochaEngine::GamePlay::SPELL_TAB: //�������R�}���h
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		case KochaEngine::GamePlay::SKILL_TAB: //�Ƃ����R�}���h
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h
			commandNum = preCommandNum[0];
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		default:
			break;
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
	case 5:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 212);
		break;
	default:
		cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 52);
		break;
	}
}

void KochaEngine::GamePlay::DefaultTab()
{
	//�f�t�H���g�R�}���h�Ō��肪�����ꂽ�Ƃ�
	switch (commandNum)
	{
	case 0: //��������
		commandNum = preCommandNum[1]; //�ۑ����ꂽ�R�}���h�ɐ؂�ւ�
		currentTab = GamePlay::ATTACK_TAB; //���������R�}���h��
		break;
	case 1: //�������
		commandNum = preCommandNum[2]; //�ۑ����ꂽ�R�}���h�ɐ؂�ւ�
		currentTab = GamePlay::SPELL_TAB; //�������R�}���h��
		break;
	case 2: //�Ƃ���
		commandNum = preCommandNum[3]; //�ۑ����ꂽ�R�}���h�ɐ؂�ւ�
		currentTab = GamePlay::SKILL_TAB; //�Ƃ����R�}���h��
		break;
	case 3: //�ǂ���
		commandNum = preCommandNum[4]; //�ۑ����ꂽ�R�}���h�ɐ؂�ւ�
		currentTab = GamePlay::ITEM_TAB; //�ǂ����R�}���h��
		break;
	case 4: //�ڂ�����
		//currentActiveActor->GetParam().diffence
		isTurnUpdate = false;
		//�s���ς݂ɂ���
		currentActiveActor->ActiveDone();
		break;
	case 5: //�ɂ���
		//�o�g���V�[���I������
		BattleEnd();
		break;
	default:
		
		break;
	}
}

void KochaEngine::GamePlay::AttackTab()
{
	auto enemy = bManager->GetEnemy(commandNum + 1);
	if (enemy != nullptr)
	{
		isAttackMotion = true;
		motionTime = ATTACK_MOTION_TIME;
		targetActor = enemy;
	}
	else
	{
		//�Ȃ���I�������ꍇ
		//�s����炷
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
