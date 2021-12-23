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
	effectManager->LoadEffect("hit.efk", 3.0f); //���G�t�F�N�g
	effectManager->LoadEffect("none.efk", 5.0f); //�ʏ�U��(�a��)�G�t�F�N�g
	effectManager->LoadEffect("fire_0.efk", 2.0f); //�t�@�C�A0(����)�G�t�F�N�g
	effectManager->LoadEffect("dark_0.efk", 3.0f); //�_�[�N0(����)�G�t�F�N�g
	//effectManager->LoadEffect("slash1.efk", 3.0f);
	effectManager->LoadEffect("jab1.efk", 4.0f); //�ʏ�U��(�˂�)�G�t�F�N�g
	effectManager->LoadEffect("levelUp.efk", 3.0f); //���x���A�b�v���G�t�F�N�g
	effectManager->LoadEffect("DeadEnemy.efk", 5.0f); //�G���S���G�t�F�N�g

	n3DManager = new Number3DManager();
	n3DEmitter = new Number3DEmitter(n3DManager);

	map = new Map(gManager, camera);
	lightManager = new LightManager();
	lightManager = LightManager::Create();

	EnemyData::StaticInit(); //�G�l�~�[�f�[�^�̓ǂݍ���
	SkillData::StaticInit(); //�X�L���f�[�^�̓ǂݍ���

	floor = new Object("ground");
	skyObj = new Object("skydome");

	fadeTexture = new Texture2D("Resources/Texture/Color/white.png", Vector2(0, 0), SCREEN_SIZE, 0);
	defaultWakuTexture = new Texture2D("Resources/Texture/UI/waku_0.png", DEFAULT_WAKU_POS, DEFAULT_WAKU_SIZE, 0);
	anotherWakuTexture = new Texture2D("Resources/Texture/UI/waku_1.png", ANOTHER_WAKU_POS, ANOTHER_WAKU_SIZE, 0);
	defaultCommandTexture = new Texture2D("Resources/Texture/UI/command_1.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	waitCommandTexture = new Texture2D("Resources/Texture/UI/command_0.png", DEFAULT_COMMAND_POS, DEFAULT_COMMAND_SIZE, 0);
	anotherCommandTexture = new Texture2D("Resources/Texture/UI/command_3.png", DEFAULT_COMMAND_POS, ANOTHER_COMMAND_SIZE, 0);
	pageCommandTexture = new Texture2D("Resources/Texture/UI/command_4.png", DEFAULT_COMMAND_POS, ANOTHER_COMMAND_SIZE + Vector2(0, 32), 0);
	spCommandTexture = new Texture2D("Resources/Texture/UI/command_5.png", ANOTHER_WAKU_POS + Vector2(0, -55), SP_COMMAND_SIZE, 0);

	cursorPos = DEFAULT_COMMAND_POS + Vector2(10, 52);
	cursorTexture = new Texture2D("Resources/Texture/UI/cursor.png", cursorPos, Vector2(16, 16), 0);

	battleLongText = new Text(TALK_LONG_TEXT_POS, Vector2(32, 32));
	battleShortText = new Text(TALK_SHORT_TEXT_POS, Vector2(32, 32));
	battleShortText->SetOneLineFonts(20);
	battleNameText = new Text(TALK_LONG_TEXT_POS, Vector2(32, 32));
	commandTitleText = new Text(DEFAULT_COMMAND_POS + Vector2(5, 5), Vector2(32, 32));
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		const Vector2 NAME_TEXT_COMMAND_POS = DEFAULT_COMMAND_POS + Vector2(30, 40) + Vector2(0, 32 * i);
		enemyNameText[i] = new Text("fail.txt",NAME_TEXT_COMMAND_POS, Vector2(32, 32));
		skillNameText[i] = new Text("fail.txt",NAME_TEXT_COMMAND_POS, Vector2(32, 32));
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
	delete pageCommandTexture;
	delete spCommandTexture;
	delete cursorTexture;
	delete battleLongText;
	delete battleShortText;
	delete battleNameText;
	delete commandTitleText;
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		delete enemyNameText[i];
		delete skillNameText[i];
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
	isActiveMotion = false;
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

	//�ŏ��̃t�B�[���h�̃}�b�v�f�[�^���[�h
	map->CreateMap(GameSetting::HAZIMARINOTI);

	floor->SetPosition(Vector3(0, 0, 0));
	floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");

	skyObj->SetScale(Vector3(8, 8, 8));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));

	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));
	currentTab = GamePlay::DEFAULT_TAB;
	previousTab = currentTab;

	motionTime = 0;
	frameCount = 0;
	seconds = 0;
	commandNum = 0;
	battleCharaCount = 0;
	battleStartWait = 0;
	resultFlowNum = 0;
	resultFlowWait = 0;
	selectSkillIndex = 0;
	skillTabPageNum = 1;
	preSkillTabPageNum = skillTabPageNum;

	preCommandNum = 0;
	
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

	const Vector3 cameraPos = camera->GetEye();

	//�����ɃG�l�~�[�G�~�b�^�[�N���X�I�Ȃ̍���ČĂяo��
	//EnemyData�Ɠ��l�Ajson�t�@�C������o���p�^�[����ǂݍ��߂�悤�ɂ���
	{
		/*���͉��ŃG�l�~�[�ǉ�*/
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

	//�o�g���L�����N�^�[�̒ǉ�����
	{
		//�L�����̃p�����[�^�[�������Ă���
		auto playerParam = gManager->GetPlayer()->GetParam();
		auto fighterParam = gManager->GetFighter()->GetParam();

		//�p�����[�^�[���Z�b�g���ăo�g���L�����N�^�[����
		bManager->AddObject(new BattleCharacter(effectManager, n3DEmitter, BATTLE_PLAYER, cameraPos + BATTLE_CHARACTOR_POS[0], playerParam));
		bManager->AddObject(new BattleCharacter(effectManager, n3DEmitter, BATTLE_FIGHTER, cameraPos + BATTLE_CHARACTOR_POS[2], fighterParam));
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

	battleLongText->SetText("Talk/Battle/EnemySpawn.txt");
	battleLongText->SetSound("Resources/Sound/text1.wav");
	commandTitleText->SetText("default.txt");
}

void KochaEngine::GamePlay::BattleUpdate()
{
	//�f�o�b�O�p
	{
	//	if (Input::TriggerKey(DIK_F1))
	//	{
	//		//�T���v���e�L�X�g�Đ�
	//		battleLongText->ReText("Talk/Battle/CharaDestroy_0.txt");
	//	}
		if (Input::TriggerKey(DIK_P))
		{
			////�T���v���e�L�X�g�Đ�
			//battleLongText->ReText("yuma.txt");
			bManager->GetCharacter(BattleObjectType::BATTLE_PLAYER)->AddExp(extraNum);
		}
	//	if (Input::TriggerKey(DIK_E))
	//	{
	//		//�e�L�X�g�X�L�b�v
	//		battleLongText->Skip();
	//	}
	//	if (Input::TriggerKey(DIK_Q))
	//	{
	//		//������Ă����G�l�~�[�̖��O�`��
	//		auto enemy = bManager->GetEnemy(1);
	//		if (enemy != nullptr)
	//		{
	//			battleLongText->ReText(enemy->GetParam().name);
	//		}
	//	}
	//	if (Input::TriggerKey(DIK_SPACE))
	//	{
	//		//�o�g���V�[���I��
	//		BattleEnd();
	//	}
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
		if (!currentActiveActor)
		{
			//�s���ł���L�����E�G�l�~�[�����Ȃ��Ȃ����̂ňꏄ���I��
			//�S�L�����E�G�l�~�[���s���\��Ԃɂ���
			bManager->ActiveReset();
			isTurnUpdate = false;
			isEnemyTurn = true;
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
	gManager->ObjDrawBattleScene(camera, lightManager);
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
	//�o�g�����̃X�v���C�g�`��
	//defaultWakuTexture->Draw();

	if (!isActiveMotion && !isEnemyTurn)
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

	if ((isActiveMotion || isDefenceMotion) && !isResultOnce)
	{
		//���O�͂V�����܂�
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

	//�X�e�[�^�X�𔽉f
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

		//�X�L���̖��O�`����X�V
		if (currentActiveActor->GetType() == BATTLE_PLAYER ||
			currentActiveActor->GetType() == BATTLE_FIGHTER)
		{
			SkillNameUpdate();
		}
	}

	//�R�}���h��ɕ\�����閼�O���X�V����t���O
	isCommandTitleUpdate = false;

	isActiveMotion = false;

	currentTab = CommandTab::DEFAULT_TAB;
	previousTab = currentTab;
	commandNum = 0;
	selectSkillIndex = 0;
	skillTabPageNum = 1;
	preSkillTabPageNum = skillTabPageNum;
	preCommandNum = 0;

	//�G�l�~�[�̖��O�`����X�V
	EnemyNameUpdate();

	//�J�����̈ʒu���o�g���J�n���̏����ʒu�ɖ߂�
	cameraManager->SetDefaultPosition();
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
			commandTitleText->SetText(currentActiveActor->GetBaseParam().name);
			//�ǂ�����H
			battleLongText->SetText("Talk/Battle/ChooseAction.txt");
			battleLongText->SetSound("Resources/Sound/text1.wav");
		}

		//�����������[�V�������͑���ł��Ȃ��悤�ɂ���
		if (!isActiveMotion)
		{
			//�R�}���h�̃J�[�\������
			MoveCursor();

			//�J�����̃t�H�[�J�X(���s)�����݂����ǂ����̃L�����ɍ��킹��
			float eyeZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
			float targetZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
			cameraManager->SetBattleEyePositionZ(eyeZ);
			cameraManager->SetBattleTargetPositionZ(targetZ);
		}


	}
	else //�o�g���I�u�W�F�N�g�^�C�v���G�l�~�[��������
	{
		isEnemyTurn = true;
		EnemyActionSelect();
	}

	ActiveMotionUpdate();
	DefenceMotionUpdate();
}

void KochaEngine::GamePlay::EnemyActionSelect()
{
	if (isActiveMotion) return;

	isActiveMotion = true;

	//�s�����̃G�l�~�[�̃X�L���̐��𒲂ׂ�
	int maxSkillIndex = currentActiveActor->GetSkillCount();
	//�����_���Ŏg�p����X�L���̑I��
	selectSkillIndex = Util::GetIntRand(1, maxSkillIndex) - 1;
	//���̃X�L���̖��O�����炤
	std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);
	//����������O��Key�ɂ��ăX�L���̃f�[�^�������Ă���
	SkillParam activeSkill = SkillData::GetSkillParam(activeSkillName);

	//�X�L���̌��ʑΏۂ𒲂ׂ�(�G��������)
	if (activeSkill.isAllies) //�G�l�~�[��
	{
		//�S�̂��P�̂����ׂ�
		if (activeSkill.isOverall) //�S��
		{

		}
		else //�P��
		{

		}
	}
	else //�L�����N�^�[��
	{
		//�S�̂��P�̂����ׂ�
		if (activeSkill.isOverall) //�S��
		{

		}
		else //�P��
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

	//����(���O)
	battleNameText->SetText(currentActiveActor->GetBaseParam().name);
	battleNameText->SetSound("Resources/Sound/text1.wav");
	//�����̂����ǂ��I
	battleLongText->SetText("Talk/Battle/AttackAction.txt");
	battleLongText->SetSound("Resources/Sound/text1.wav");

	//�J�����̃t�H�[�J�X(���s)�����݂����ǂ����̃G�l�~�[�ɍ��킹��
	float eyeZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
	float targetZ = currentActiveActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
	cameraManager->SetBattleEyePositionZ(eyeZ);
	cameraManager->SetBattleTargetPositionZ(targetZ);
}

void KochaEngine::GamePlay::ActiveMotionUpdate()
{
	if (!isActiveMotion) return;
	if (motionTime > 0)
	{
		motionTime--;
	}
	if (motionTime == FOCUS_MOTION_TIME) //�\����
	{
		currentActiveActor->SetAttackTextureIndex(0);

		//�J�����̃t�H�[�J�X(���s)���^�[�Q�b�g�ɍ��킹��
		float eyeZ = targetActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
		float targetZ = targetActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
		cameraManager->SetBattleEyePositionZ(eyeZ);
		cameraManager->SetBattleTargetPositionZ(targetZ);

		//�J�����̃t�H�[�J�X(���E)���^�[�Q�b�g�Ɋ񂹂�
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
	else if (motionTime == ACTIVE_MOTION_TIME) //�s������
	{
		std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);

		//�_���[�W�v�Z����
		//�_���[�W��^����
		targetActor->SetDamage(activeSkillName, currentActiveActor->GetBattleParam());
		currentActiveActor->CostSP(SkillData::GetSkillParam(activeSkillName).cost);
		
		//���������A�j���[�V����
		currentActiveActor->SetAttackTextureIndex(1);

		//�_���[�W�󂯂����̃J�����h��
		if (targetActor->GetType() != BattleObjectType::ENEMY)
		{
			cameraManager->SetCameraShake(5.0f);
		}

	}
	else if (motionTime == 0) //�s�����I������
	{
		isTurnUpdate = false;
		//�s���ς݂ɂ���
		currentActiveActor->ActiveDone();
		//�s������Ԃ���������
		currentActiveActor->CurrentActiveReset();

		currentActiveActor->SetDefaultWaitTexture();
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
		//�J�����̈ʒu���o�g���J�n���̏����ʒu�ɖ߂�
		cameraManager->SetDefaultPosition();
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
				battleLongText->SetText("Talk/Battle/CharaDestroy_0.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				break;
			case 1:
				//�����˂��͂�Ԃ񂤂��Ȃ���
				battleLongText->SetText("Talk/Battle/CharaDestroy_1.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
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
				battleLongText->SetText("Talk/Battle/EnemyDestroy_0.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				bManager->Reward();
				break;
			case 1:
				//�����̂������񂿂������Ƃ��I
				battleLongText->SetText("Talk/Battle/EnemyDestroy_1.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");
				RewardCalc();
				isShowNumber = true;
				break;
			case 2:
				//�����S�[���h�Ăɂ��ꂽ�I
				battleLongText->SetText("Talk/Battle/EnemyDestroy_2.txt");
				battleLongText->SetSound("Resources/Sound/text1.wav");

				break;
			case 3:
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

void KochaEngine::GamePlay::RewardCalc()
{
	BattleObject* character = nullptr;
	const float EXP_RATE = 1.1f - (float)battleCharaCount * 0.1f;
	getExp = (float)bManager->GetTotalExp() * EXP_RATE;
	if (getExp <= 0) getExp = 1; //�Œ�ł��P�̌o���l�͕ۏ�

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
	//���������R�}���h�ɕ\�������G�̖��O�̍X�V
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
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

//�y�[�W���܂����Ƃ��ɌĂяo��
void KochaEngine::GamePlay::SkillNameUpdate()
{
	//�X�L���R�}���h�ɕ\�������X�L���̖��O�̍X�V
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		int _indexNum = (skillTabPageNum - 1) * 5 + i + 1;
		auto skill = currentActiveActor->GetSkillName(_indexNum);
		if (skill == "noSkill")
		{
			skillNameText[i]->SetText("none.txt");
			continue;
		}
		skillNameText[i]->SetText(SkillData::GetSkillParam(skill).name);
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
	case KochaEngine::GamePlay::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();

		for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
		{
			enemyNameText[i]->Draw(0);
		}
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::SKILL_TAB: //�X�L���R�}���h
		anotherWakuTexture->Draw();
		pageCommandTexture->Draw();
		spCommandTexture->Draw();
		for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
		{
			skillNameText[i]->Draw(0);
		}
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h
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
	//����{�^�����������Ƃ�
	if (InputManager::TriggerDecision())
	{
		preCommandNum = commandNum; //���O�̓��͂̕ۑ�

		switch (currentTab)
		{
		case KochaEngine::GamePlay::DEFAULT_TAB: //�f�t�H���g�R�}���h
			previousTab = currentTab;
			DefaultTab();
			break;
		case KochaEngine::GamePlay::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h
			TargetSelectTab();
			break;
		case KochaEngine::GamePlay::SKILL_TAB: //�X�L���R�}���h
			previousTab = currentTab;
			SkillTab();
			break;
		case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h
			previousTab = currentTab;
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
			////�ǂ�����H
			//battleLongText->SetText("Talk/Battle/ChooseAction.txt");
			break;
		case KochaEngine::GamePlay::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h
			commandNum = preCommandNum;
			currentTab = previousTab;
			break;
		case KochaEngine::GamePlay::SKILL_TAB: //�X�L���R�}���h
			commandNum = preCommandNum;
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h
			commandNum = preCommandNum;
			currentTab = GamePlay::DEFAULT_TAB;
			break;
		default:
			break;
		}
	}

	switch (currentTab)
	{
	case KochaEngine::GamePlay::DEFAULT_TAB: //�f�t�H���g�R�}���h

		break;
	case KochaEngine::GamePlay::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h

		break;
	case KochaEngine::GamePlay::SKILL_TAB: //�X�L���R�}���h
		if (InputManager::TriggerRight())
		{
			preSkillTabPageNum = skillTabPageNum;
			//�y�[�W��i�߂�
			skillTabPageNum = CommandNumRight(skillTabPageNum, (currentActiveActor->GetSkillCount() + MAX_NAME_TEXT_COUNT_COMMAND - 2) / MAX_NAME_TEXT_COUNT_COMMAND);
			//�y�[�W���܂����ł���̂ŃX�L���y�[�W���X�V
			if (preSkillTabPageNum != skillTabPageNum)
			{
				SkillNameUpdate();
			}
		}
		else if (InputManager::TriggerLeft())
		{
			preSkillTabPageNum = skillTabPageNum;
			//�y�[�W��߂�
			skillTabPageNum = CommandNumLeft(skillTabPageNum, (currentActiveActor->GetSkillCount() + MAX_NAME_TEXT_COUNT_COMMAND - 2) / MAX_NAME_TEXT_COUNT_COMMAND);
			//�y�[�W���܂����ł���̂ŃX�L���y�[�W���X�V
			if (preSkillTabPageNum != skillTabPageNum)
			{
				SkillNameUpdate();
			}
		}

		break;
	case KochaEngine::GamePlay::ITEM_TAB: //�ǂ����R�}���h

		break;
	default:
		break;
	}

	if (currentTab == GamePlay::TARGET_SELECT_TAB)
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
	//�f�t�H���g�R�}���h�Ō��肪�����ꂽ�Ƃ�
	switch (commandNum)
	{
	case 0: //�^�[�Q�b�g�I��
		commandNum = 0;
		selectSkillIndex = 0;
		currentTab = GamePlay::TARGET_SELECT_TAB; //�^�[�Q�b�g�I���R�}���h��
		break;
	case 1: //�X�L��
	    //�X�L���y�[�W��������
		skillTabPageNum = 1;
		preSkillTabPageNum = skillTabPageNum;
		commandNum = 0;
		currentTab = GamePlay::SKILL_TAB; //�X�L���R�}���h��
		break;
	case 2: //�ǂ���
		commandNum = 0;
		currentTab = GamePlay::ITEM_TAB; //�ǂ����R�}���h��
		break;
	case 3: //�ڂ�����
		//currentActiveActor->GetParam().diffence
		isTurnUpdate = false;
		//�s���ς݂ɂ���
		currentActiveActor->ActiveDone();
		//�s������Ԃ���������
		currentActiveActor->CurrentActiveReset();
		break;
	case 4: //�ɂ���
		//�o�g���V�[���I������
		BattleEnd();
		break;
	default:
		
		break;
	}
}

void KochaEngine::GamePlay::TargetSelectTab()
{
	//�G�����������f���Ă���I��

	auto enemy = bManager->GetEnemy(commandNum + 1);
	if (enemy)
	{
		isActiveMotion = true;
		motionTime = ATTACK_MOTION_TIME;
		targetActor = enemy;
		//����(���O)
		battleNameText->SetText(currentActiveActor->GetBaseParam().name);
		battleNameText->SetSound("Resources/Sound/text1.wav");
		//�����̂����ǂ��I
		battleLongText->SetText("Talk/Battle/AttackAction.txt");
		battleLongText->SetSound("Resources/Sound/text1.wav");
	}
	else
	{
		//�Ȃ���I�������ꍇ
		//�s����炷
		int a = 0;
	}
}

void KochaEngine::GamePlay::SkillTab()
{
	selectSkillIndex = (skillTabPageNum - 1) * MAX_NAME_TEXT_COUNT_COMMAND + commandNum + 1;
	std::string _skillName = currentActiveActor->GetSkillName(selectSkillIndex);
	if (_skillName != "noSkill")
	{
		SkillParam _skillData = SkillData::GetSkillParam(_skillName);
		if (_skillData.cost > currentActiveActor->GetBaseParam().sp)
		{
			//SP������Ȃ��ꍇ
            //�s����炷&SP������Ȃ��e�L�X�g���o��
			int a = 0;
			return;
		}
		if (!_skillData.isOverall)
		{
			commandNum = 0;
			currentTab = GamePlay::TARGET_SELECT_TAB; //�^�[�Q�b�g�I���R�}���h��
		}
		else
		{
			//�S�̍U��
		}
	}
	else
	{
		//�Ȃ���I�������ꍇ
        //�s����炷
		int a = 0;
		return;
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

unsigned int KochaEngine::GamePlay::CommandNumRight(const unsigned int arg_commandNum, const unsigned int arg_maxNum)
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

unsigned int KochaEngine::GamePlay::CommandNumLeft(const unsigned int arg_commandNum, const unsigned int arg_maxNum)
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
