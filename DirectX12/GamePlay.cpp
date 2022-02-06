#include "GamePlay.h"

#include "Map.h"
#include "LightManager.h"
#include "Number3DEmitter.h"
#include "Number3DManager.h"
#include "EffectManager.h"
#include "CameraManager.h"

#include "Battle.h"

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
	//�J�����̐���
	camera = new Camera();
	cameraManager = new CameraManager(*camera);

	//�Q�[���I�u�W�F�N�g�Ǘ��N���X�̐���
	gManager = new GameObjectManager();
	bManager = new BattleObjectManager();

	//�p�[�e�B�N���֘A�̐���
	pManager = new ParticleManager();
	pEmitter = new ParticleEmitter(pManager);

	//�G�t�F�N�g�Ǘ��N���X����
	effectManager = new EffectManager(dx12);
	//�G�t�F�N�g�̃��[�h
	effectManager->LoadEffect("hit.efk", 3.0f); //���G�t�F�N�g
	effectManager->LoadEffect("none.efk", 5.0f); //�ʏ�U��(�a��)�G�t�F�N�g
	effectManager->LoadEffect("fire_0.efk", 2.0f); //�t�@�C�A0(����)�G�t�F�N�g
	effectManager->LoadEffect("dark_0.efk", 3.0f); //�_�[�N0(����)�G�t�F�N�g
	effectManager->LoadEffect("jab1.efk", 4.0f); //�ʏ�U��(�˂�)�G�t�F�N�g
	effectManager->LoadEffect("levelUp.efk", 3.0f); //���x���A�b�v���G�t�F�N�g
	effectManager->LoadEffect("DeadEnemy.efk", 5.0f); //�G���S���G�t�F�N�g
	//effectManager->LoadEffect("slash1.efk", 3.0f);

	//3D��ԗp�����`��Ǘ��N���X������
	n3DManager = new Number3DManager();
	n3DEmitter = new Number3DEmitter(n3DManager);

	//�t�B�[���h�쐬�p�N���X�̐���
	map = new Map(gManager, camera);

	//���C�g�Ǘ��N���X�̐���
	lightManager = new LightManager();
	lightManager = LightManager::Create();

	//�o�g���p�N���X�̐���
	battleScene = new Battle(camera, cameraManager, gManager, bManager, effectManager, n3DEmitter, lightManager);

	EnemyData::StaticInit(); //�G�l�~�[�f�[�^�̓ǂݍ���
	SkillData::StaticInit(); //�X�L���f�[�^�̓ǂݍ���

	//�n�ʁ��V���̐���
	floor = new Object("ground");
	skyObj = new Object("skydome");

	//�t�F�[�h�A�E�g�p�e�N�X�`���̐���
	fadeTexture = new Texture2D("Resources/Texture/Color/white.png", Vector2(0, 0), SCREEN_SIZE, 0);
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
	delete battleScene;
	delete map;
	delete floor;
	delete skyObj;
	delete fadeTexture;
}

void KochaEngine::GamePlay::Initialize()
{
	isEnd = false;
	isGameOver = false;
	isChangeScene = false;

	//�O�̂��߃I�u�W�F�N�g���폜���Ă���
	gManager->Clear();
	bManager->Clear();
	//�e��I�u�W�F�N�g�Ǘ��N���X�̏�����
	gManager->Initialize();
	bManager->Initialize();

	//�J�����̏�����
	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });

	//���C�g�}�l�[�W���[�̏�����
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//�ŏ��̃t�B�[���h�̃}�b�v�f�[�^���[�h
	map->CreateMap(GameSetting::nowField);

	//�o�g���V�[���̏�����
	battleScene->Initialize();

	//�v���C���[�����炤
	fieldPlayer = gManager->GetPlayer();

	//�J�����}�l�[�W���[�Ƀv���C���[���Z�b�g
	cameraManager->SetPlayer(fieldPlayer);

	//�G�ƃG���J�E���g����t�B�[���h���ǂ���
	GameSetting::isBattleField = true;

	//�n�ʂ̃p�����[�^�[�Z�b�g
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

	//�V���̃p�����[�^�[�Z�b�g
	skyObj->SetScale(Vector3(15, 15, 15));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
	//skyObj->SetTexture("Resources/Texture/Color/black.png");

	//�t�F�[�h�A�E�g�p�e�N�X�`���̐F��ݒ�
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));
	
	frameCount = 0;
	seconds = 0;
	extraNum = 0;
	
	fadeFlag = false;
	fadeAlpha = 1.0f;
}

void KochaEngine::GamePlay::Update()
{
	//�t�F�[�h�֘A�̍X�V
	FadeUpdate();

	//�G���J�E���g������t�F�[�h���������ăo�g���V�[����
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

	//�f�o�b�O�L�[(��ʌ���)
	if (Input::TriggerKey(DIK_J))
	{
		//����
		floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");
		GameSetting::environmentNumber = 1;
		GameSetting::isEnvironmentUpdate = true;
	}
	else if (Input::TriggerKey(DIK_K))
	{
		//����
		floor->SetTexture("Resources/Texture/Tiling/tiling_suna1.png");
		GameSetting::environmentNumber = 2;
		GameSetting::isEnvironmentUpdate = true;
	}
	else if (Input::TriggerKey(DIK_L))
	{
		//�ጴ
		floor->SetTexture("Resources/Texture/Tiling/tiling_yuki0.png");
		GameSetting::environmentNumber = 3;
		GameSetting::isEnvironmentUpdate = true;
	}

	if (Input::TriggerKey(DIK_P))
	{
		Vector3 _kiriPos = Vector3(fieldPlayer->GetPosition().x - 20, fieldPlayer->GetPosition().y, fieldPlayer->GetPosition().z - 3);
		pEmitter->Kiri0(_kiriPos);
	}

	//�f�o�b�O�L�[(�ʃt�B�[���h�ւ̑J��)
	if (Input::TriggerKey(DIK_H))
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
		//�J�����̏�Ԃ��o�g�����̏�Ԃɐ؂�ւ�
		cameraManager->SetCameraState(CameraState::BATTLE_CAMERA_STATE);

		//�o�g�����̍X�V
		BattleUpdate();
	}
	else
	{
		//�J�����̏�Ԃ��t�B�[���h���̏�Ԃɐ؂�ւ�
		cameraManager->SetCameraState(CameraState::DEFAULT_CAMERA_STATE);

		//�o�g���ȊO�̎��̍X�V
		FieldUpdate();
	}

	//���ꂼ��̃}�l�[�W���[���X�V
	gManager->Update();
	bManager->Update();
	pManager->Update();
	n3DManager->Update();
	cameraManager->Update();
	lightManager->Update();

	//�V������������]������
	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
}

void KochaEngine::GamePlay::SpriteDraw()
{
	if (battleScene->IsBattle())
	{
		//�o�g�����̃X�v���C�g�`��
		battleScene->SpriteDraw();
	}
	else
	{
		//�t�B�[���h���̃X�v���C�g�`��
		FieldSpriteDraw();
	}

	//�t�F�[�h�A�E�g�p�X�v���C�g�`��
	fadeTexture->Draw();
}

void KochaEngine::GamePlay::ObjDraw()
{
	if (battleScene->IsBattle())
	{
		//�o�g�����̔񓧖��I�u�W�F�N�g�̕`��
		BattleObjDraw();
	}
	else
	{
		//�t�B�[���h���̔񓧖��I�u�W�F�N�g�̕`��
		FieldObjDraw();
	}

	//�n�ʂƓV���̕`��
	floor->Draw(camera, lightManager);
	skyObj->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::AlphaObjDraw()
{
	if (battleScene->IsBattle())
	{
		//�o�g�����̓����I�u�W�F�N�g�̕`��
		BattleAlphaObjDraw();
	}
	else
	{
		//�t�B�[���h���̓����I�u�W�F�N�g�̕`��
		FieldAlphaObjDraw();
	}

	//�e��}�l�[�W���[�ɂ��`��
	pManager->Draw(camera, lightManager);
	n3DManager->Draw(camera, lightManager);

	//�G�t�F�N�g�̍X�V���`��
	effectManager->Update(camera);
}

//GUI�̕`��֘A
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

//�t�F�[�h�֘A�̍X�V
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
	//�f�o�b�O�p
	{
		if (Input::TriggerKey(DIK_F1))
		{
			//�T���v���e�L�X�g�Đ�
			//battleLongText->SetText("sample99.txt");
		}
		if (Input::TriggerKey(DIK_P))
		{
			////�T���v���e�L�X�g�Đ�
			//battleLongText->SetText("yuma.txt");
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

	//�����Ƀo�g���X�V����
	battleScene->Update();

	fadeFlag = battleScene->IsFade();

	//�o�g���V�[���I������//
	if (fadeAlpha >= 1.0f && battleScene->IsBattleEnd())
	{
		battleScene->BattleEndUpdate();
	}
}

void KochaEngine::GamePlay::BattleObjDraw()
{
	gManager->ObjDrawBattleScene(camera, lightManager);
}

void KochaEngine::GamePlay::BattleAlphaObjDraw()
{
	gManager->AlphaObjDrawBattleScene(camera, lightManager);
	battleScene->AlphaObjDraw();
}

void KochaEngine::GamePlay::FieldUpdate()
{
	//��o�g����Ԃɐݒ�
	battleScene->IsBattleStartFalse();
}

void KochaEngine::GamePlay::FieldObjDraw()
{
	gManager->ObjDrawFieldScene(camera, lightManager);
}

void KochaEngine::GamePlay::FieldAlphaObjDraw()
{
	gManager->AlphaObjDrawFieldScene(camera, lightManager);
}

void KochaEngine::GamePlay::FieldSpriteDraw()
{
	gManager->SpriteDraw();
}
