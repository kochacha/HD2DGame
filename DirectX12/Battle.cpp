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

}

void KochaEngine::Battle::SpriteDraw()
{
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

//�o�g���I�����̏���
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

	//�퓬�J�n���ɃJ�����̏����ʒu���Z�b�g����
	gManager.lock()->SetBattleCameraDefaultPos(cameraPos);

	//�����ɃG�l�~�[�G�~�b�^�[�N���X�I�Ȃ̍���ČĂяo��
	//EnemyData�Ɠ��l�Ajson�t�@�C������o���p�^�[����ǂݍ��߂�悤�ɂ���
	{
		/*���͉��ŃG�l�~�[�ǉ�*/
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

	//�o�g���L�����N�^�[�̒ǉ�����
	{
		//�L�����̃p�����[�^�[�������Ă���
		auto playerParam = gManager.lock()->GetPlayer()->GetParam();
		auto fighterParam = gManager.lock()->GetFighter()->GetParam();

		//�p�����[�^�[���Z�b�g���ăo�g���L�����N�^�[����
		bManager.lock()->AddObject(new BattleCharacter(effectManager.lock().get(), n3DEmitter.lock().get(), BATTLE_PLAYER, cameraPos + BATTLE_CHARACTOR_POS[0], playerParam));
		bManager.lock()->AddObject(new BattleCharacter(effectManager.lock().get(), n3DEmitter.lock().get(), BATTLE_FIGHTER, cameraPos + BATTLE_CHARACTOR_POS[2], fighterParam));
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

void KochaEngine::Battle::BattleFlowUpdate()
{
	if (battleStartWait > 0) return;

	bManager.lock()->TargetOff();

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
			bManager.lock()->ActiveReset();
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

void KochaEngine::Battle::BattleEnd()
{
	fadeFlag = true;
	isBattleEnd = true;

	//�X�e�[�^�X�𔽉f
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
	//�L�����N�^�[���S�ł�����
	if (bManager.lock()->IsCharacterDestroy())
	{
		isCharacterDestroy = true;
		return;
	}
	//�G�l�~�[���S�ł�����
	if (bManager.lock()->IsEnemyDestroy())
	{
		isEnemyDestroy = true;
		return;
	}

	//���ݍs�����̃L�����E�G�l�~�[�������Ă���
	currentActiveActor = bManager.lock()->GetCurrentActive();

	//�L�����E�G�l�~�[���s������Ԃɂ���
	if (currentActiveActor)
	{
		currentActiveActor->CurrentActive();

		//�X�L���̖��O�`����X�V
		if (currentActiveActor->GetType() == BATTLE_PLAYER ||
			currentActiveActor->GetType() == BATTLE_FIGHTER)
		{
			SkillNameUpdate();

			//�X�L���^�u�̍ő�y�[�W�����X�V����
			skillTabMaxPageNum = (currentActiveActor->GetSkillCount() + MAX_NAME_TEXT_COUNT_COMMAND - 2) / MAX_NAME_TEXT_COUNT_COMMAND;
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
	cameraManager.lock()->SetDefaultPosition();
}

void KochaEngine::Battle::ActiveActorUpdate()
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
			Vector3 _currentActiveActorPos = currentActiveActor->GetPosition();
			float eyeZ = _currentActiveActorPos.z + BATTLE_FOCUS_EYE_Z;
			float targetZ = _currentActiveActorPos.z + BATTLE_FOCUS_TARGET_Z;
			cameraManager.lock()->SetBattleEyePositionZ(eyeZ);
			cameraManager.lock()->SetBattleTargetPositionZ(targetZ);
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

void KochaEngine::Battle::EnemyActionSelect()
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

	//�����ŃG�l�~�[��AI(������ʂŃN���X����ĊǗ�������)
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

	//����(���O)
	battleNameText->SetText(currentActiveActor->GetBaseParam().name);
	battleNameText->SetSound("Resources/Sound/text1.wav");
	//�����̂����ǂ��I
	battleLongText->SetText("Talk/Battle/AttackAction.txt");
	battleLongText->SetSound("Resources/Sound/text1.wav");

	//�J�����̃t�H�[�J�X(���s)�����݂����ǂ����̃G�l�~�[�ɍ��킹��
	Vector3 _currentActiveActorPos = currentActiveActor->GetPosition();
	float eyeZ = _currentActiveActorPos.z + BATTLE_FOCUS_EYE_Z;
	float targetZ = _currentActiveActorPos.z + BATTLE_FOCUS_TARGET_Z;
	cameraManager.lock()->SetBattleEyePositionZ(eyeZ);
	cameraManager.lock()->SetBattleTargetPositionZ(targetZ);
}

//�s�����[�V�������̏���
void KochaEngine::Battle::ActiveMotionUpdate()
{
	if (!isActiveMotion) return;
	if (motionTime > 0)
	{
		motionTime--;
	}
	if (motionTime == ATTACK_MOTION_TIME - 1) //���[�V�����ɓ������u��
	{
		//std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);
		//auto activeSkill = SkillData::GetSkillParam(activeSkillName);
		//if (activeSkill.isMagic) //������񂾂�����
		//{
		//	lightManager.lock()->SetDirectionalLightDirection(0, Vector3(0, -1, 1));
		//	lightManager.lock()->SetDirectionalLightColor(0, Vector3(0, 0, 0));
		//}
	}
	else if (motionTime == FOCUS_MOTION_TIME) //�\����
	{
		//std::string activeSkillName = currentActiveActor->GetSkillName(selectSkillIndex);
		//auto activeSkill = SkillData::GetSkillParam(activeSkillName);
		//if (activeSkill.isMagic) //������񂾂�����
		//{
		//	auto pointLightPos = currentActiveActor->GetPosition();
		//	pointLightPos = Vector3(pointLightPos.x, pointLightPos.y, pointLightPos.z - 2);
		//	lightManager.lock()->SetPointLightIsActive(0, true);
		//	lightManager.lock()->SetPointLightPos(0, pointLightPos);
		//	lightManager.lock()->SetPointLightColor(0, Vector3(0.1f, 0.5f, 0.5f));
		//	lightManager.lock()->SetPointLightAtten(0, Vector3(1.000f, 0.005f, 0.0001f));
		//}

		currentActiveActor->SetAttackTextureIndex(0);

		//�J�����̃t�H�[�J�X(���s)���^�[�Q�b�g�ɍ��킹��
		float eyeZ = targetActor->GetPosition().z + BATTLE_FOCUS_EYE_Z;
		float targetZ = targetActor->GetPosition().z + BATTLE_FOCUS_TARGET_Z;
		cameraManager.lock()->SetBattleEyePositionZ(eyeZ);
		cameraManager.lock()->SetBattleTargetPositionZ(targetZ);

		//�J�����̃t�H�[�J�X(���E)���^�[�Q�b�g�Ɋ񂹂�
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
		if (targetActor->GetType() != BattleObjectType::BATTLE_ENEMY)
		{
			cameraManager.lock()->SetCameraShake(5.0f);
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

		////���C�g�̌�����߂�
		//lightManager.lock()->SetDirectionalLightDirection(0, Vector3(0, 1, -1));
		//lightManager.lock()->SetDirectionalLightColor(0, Vector3(1, 1, 1));
		//lightManager.lock()->SetPointLightIsActive(0, false);
	}
}

void KochaEngine::Battle::DefenceMotionUpdate()
{

}

void KochaEngine::Battle::ResultUpdate()
{
	//���U���g�J�ڎ��Ɉ�x�����ʂ鏈��
	if (!isResultOnce)
	{
		isResultOnce = true;
		resultFlowWait = RESULT_INPUT_WAIT;
		//�J�����̈ʒu���o�g���J�n���̏����ʒu�ɖ߂�
		cameraManager.lock()->SetDefaultPosition();
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
				bManager.lock()->Reward();
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

//���U���g���̌o���l�Ƃ�����ǉ�
void KochaEngine::Battle::RewardCalc()
{
	BattleObject* character = nullptr;

	//�o�g���Q���L�������ɉ�����1�L����������̊l���o���l���ϓ�����
	//�o���l���[�g = ��b�{��1.0�{ / �Q���L������(1�l�̎����{)
	const float EXP_RATE = 1.0f/* / (float)battleCharaCount*/;
	//��l������l���o���l = ���l���o���l �~ �o���l���[�g
	getExp = (float)bManager.lock()->GetTotalExp() * EXP_RATE;

	//�o���l���}�C�i�X�ɂȂ�Ȃ��悤�ɕ␳
	if (getExp < 0) getExp = 0;

	character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_PLAYER);
	if (character)
	{
		//������Ԃ��ƌo���l������Ȃ�
		if (!character->IsKnockDown())
		{
			character->AddExp(getExp);
		}
		//�v���C���[�̂݁A������ǉ�
		character->AddMoney(bManager.lock()->GetTotalMoney());
	}
	character = bManager.lock()->GetCharacter(BattleObjectType::BATTLE_FIGHTER);
	if (character)
	{
		//������Ԃ��ƌo���l������Ȃ�
		if (!character->IsKnockDown())
		{
			character->AddExp(getExp);
		}
	}
}

void KochaEngine::Battle::EnemyNameUpdate()
{
	//���������R�}���h�ɕ\�������G�̖��O�̍X�V
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		auto enemy = bManager.lock()->GetEnemy(i + 1);
		if (!enemy)
		{
			//�G�l�~�[�����Ȃ��ꍇ�A�u�Ȃ��v��`��
			enemyNameText[i]->SetText("none.txt");
			continue;
		}
		//�G�l�~�[�̖��O��ݒ�
		enemyNameText[i]->SetText(enemy->GetBaseParam().name);
	}
}

//�y�[�W���܂����Ƃ��ɌĂяo��
void KochaEngine::Battle::SkillNameUpdate()
{
	//�X�L���R�}���h�ɕ\�������X�L���̖��O�̍X�V
	for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
	{
		int _indexNum = (skillTabPageNum - 1) * 5 + i + 1;
		auto skill = currentActiveActor->GetSkillName(_indexNum);
		if (skill == "noSkill")
		{
			//�X�L���������ꍇ�A�u�Ȃ��v��`��
			skillNameText[i]->SetText("none.txt");
			continue;
		}
		//�X�L���̖��O��ݒ�
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
		//�X�L���������̍X�V
		summaryText->SetText(_textName);
		//�\���p�X�L������SP�X�V
		costSP = _skillData.cost;
	}
	else
	{
		//�X�L���������̍X�V
		summaryText->SetText("default.txt");
		//�\���p�X�L������SP�X�V
		costSP = 0;
	}
}

void KochaEngine::Battle::CommandDraw()
{
	switch (currentTab)
	{
	case KochaEngine::Battle::DEFAULT_TAB: //�f�t�H���g�R�}���h
		defaultWakuTexture->Draw();
		defaultCommandTexture->Draw();
		battleLongText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::Battle::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h
		anotherWakuTexture->Draw();
		anotherCommandTexture->Draw();

		for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
		{
			enemyNameText[i]->Draw(0);
		}
		battleShortText->Draw(KochaEngine::GameSetting::talkSpeed);
		break;
	case KochaEngine::Battle::SKILL_TAB: //�X�L���R�}���h
		anotherWakuTexture->Draw();
		pageCommandTexture->Draw();
		spCommandTexture->Draw();

		costSPNumberTex->Draw(costSP);
		//������skillTabNumDraw�ǉ�
		//������skillTabMaxNumDraw�ǉ�

		for (int i = 0; i < MAX_NAME_TEXT_COUNT_COMMAND; i++)
		{
			skillNameText[i]->Draw(0);
		}
		summaryText->Draw(0);
		break;
	case KochaEngine::Battle::ITEM_TAB: //�ǂ����R�}���h
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
	//����{�^�����������Ƃ�
	if (InputManager::TriggerDecision())
	{
		preCommandNum = commandNum; //���O�̓��͂̕ۑ�
		isUpdateCommandExplanation = true;

		switch (currentTab)
		{
		case KochaEngine::Battle::DEFAULT_TAB: //�f�t�H���g�R�}���h
			previousTab = currentTab;
			DefaultTab();
			break;
		case KochaEngine::Battle::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h
			TargetSelectTab();
			break;
		case KochaEngine::Battle::SKILL_TAB: //�X�L���R�}���h
			previousTab = currentTab;
			SkillTab();
			break;
		case KochaEngine::Battle::ITEM_TAB: //�ǂ����R�}���h
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
		case KochaEngine::Battle::DEFAULT_TAB: //�f�t�H���g�R�}���h
			////�ǂ�����H
			//battleLongText->SetText("Talk/Battle/ChooseAction.txt");
			break;
		case KochaEngine::Battle::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h
			isUpdateCommandExplanation = true;
			commandNum = preCommandNum;
			currentTab = previousTab;
			break;
		case KochaEngine::Battle::SKILL_TAB: //�X�L���R�}���h
			isUpdateCommandExplanation = true;
			commandNum = preCommandNum;
			currentTab = Battle::DEFAULT_TAB;
			break;
		case KochaEngine::Battle::ITEM_TAB: //�ǂ����R�}���h
			commandNum = preCommandNum;
			currentTab = Battle::DEFAULT_TAB;
			break;
		default:
			break;
		}
	}

	switch (currentTab)
	{
	case KochaEngine::Battle::DEFAULT_TAB: //�f�t�H���g�R�}���h

		break;
	case KochaEngine::Battle::TARGET_SELECT_TAB: //�^�[�Q�b�g�I���R�}���h

		break;
	case KochaEngine::Battle::SKILL_TAB: //�X�L���R�}���h
		if (InputManager::TriggerRight())
		{
			//���O�̃y�[�W��ԕۑ�
			preSkillTabPageNum = skillTabPageNum;
			//�y�[�W��i�߂�
			skillTabPageNum = CommandNumRight(skillTabPageNum, skillTabMaxPageNum);
			//�y�[�W���܂����ł���̂ŃX�L���y�[�W���X�V
			if (preSkillTabPageNum != skillTabPageNum)
			{
				SkillNameUpdate();
			}

		}
		else if (InputManager::TriggerLeft())
		{
			//���O�̃y�[�W��ԕۑ�
			preSkillTabPageNum = skillTabPageNum;
			//�y�[�W��߂�
			skillTabPageNum = CommandNumLeft(skillTabPageNum, skillTabMaxPageNum);
			//�y�[�W���܂����ł���̂ŃX�L���y�[�W���X�V
			if (preSkillTabPageNum != skillTabPageNum)
			{
				SkillNameUpdate();
			}
		}
		SkillExplanationUpdate();
		break;
	case KochaEngine::Battle::ITEM_TAB: //�ǂ����R�}���h

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

//�J�[�\���̃|�W�V������ݒ�
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
	//�f�t�H���g�R�}���h�Ō��肪�����ꂽ�Ƃ�
	switch (commandNum)
	{
	case 0: //�^�[�Q�b�g�I��
		commandNum = 0;
		selectSkillIndex = 0;
		currentTab = Battle::TARGET_SELECT_TAB; //�^�[�Q�b�g�I���R�}���h��
		break;
	case 1: //�X�L��
		//�X�L���y�[�W��������
		skillTabPageNum = 1;
		preSkillTabPageNum = skillTabPageNum;
		commandNum = 0;
		currentTab = Battle::SKILL_TAB; //�X�L���R�}���h��
		break;
	case 2: //�ǂ���
		commandNum = 0;
		currentTab = Battle::ITEM_TAB; //�ǂ����R�}���h��
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

void KochaEngine::Battle::TargetSelectTab()
{
	//�G�����������f���Ă���I��

	auto enemy = bManager.lock()->GetEnemy(commandNum + 1);
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

void KochaEngine::Battle::SkillTab()
{
	//���ݑI�𒆂̃J�[�\���ԍ����Z�o
	selectSkillIndex = (skillTabPageNum - 1) * MAX_NAME_TEXT_COUNT_COMMAND + commandNum + 1;
	//�L�����̃X�L���f�[�^���炻�̔ԍ��ɓ��Ă͂܂�X�L���̖��O�������Ă���
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
			//�P�̍U��
			commandNum = 0;
			currentTab = Battle::TARGET_SELECT_TAB; //�^�[�Q�b�g�I���R�}���h��
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
