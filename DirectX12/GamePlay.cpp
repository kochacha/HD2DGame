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

	battleScene = new Battle(camera, cameraManager, gManager, bManager, effectManager, n3DEmitter, lightManager);

	EnemyData::StaticInit(); //�G�l�~�[�f�[�^�̓ǂݍ���
	SkillData::StaticInit(); //�X�L���f�[�^�̓ǂݍ���

	floor = new Object("ground");
	skyObj = new Object("skydome");

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

	gManager->Clear();
	bManager->Clear();
	gManager->Initialize();
	bManager->Initialize();

	camera->Initialize(SCREEN_SIZE.x, SCREEN_SIZE.y, 90, 100, { 0,1,0 }, { 0,0,0 }, { 0,1,0 });
	lightManager->SetDirectionalLightColor(0, Vector3(1, 1, 1));
	lightManager->SetDirectionalLightDirection(0, Vector3(0, 1, -1));
	lightManager->SetDirectionalLightIsActive(0, true);
	lightManager->SetLightCamera(camera);

	//�ŏ��̃t�B�[���h�̃}�b�v�f�[�^���[�h
	map->CreateMap(GameSetting::HAZIMARINOTI);

	battleScene->Initialize();

	//�G�ƃG���J�E���g����t�B�[���h���ǂ���
	GameSetting::isBattleField = true;

	floor->SetPosition(Vector3(0, 0, 0));
	floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");
	//if (GameSetting::HAZIMARINOTI)
	//{
	//	floor->SetTexture("Resources/Texture/Tiling/tiling_kusa0.png");
	//}
	//else
	//{
		//floor->SetTexture("Resources/Texture/Tiling/tiling_floor5.png");
	//}


	skyObj->SetScale(Vector3(8, 8, 8));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));

	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));
	
	frameCount = 0;
	seconds = 0;
	
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
			battleScene->IsBattleTrue();
		}
	}
	bool _isBattle = battleScene->IsBattle();
	player->SetIsBattle(_isBattle);

	//if (Input::TriggerKey(DIK_H))
	//{
	//}
	//if (Input::TriggerKey(DIK_J))
	//{
	//	effectManager->Play("slash.efk", Vector3(player->GetPosition().x, player->GetPosition().y, player->GetPosition().z - 2));
	//}

	if (_isBattle)
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
	if (battleScene->IsBattle())
	{
		//�o�g�����̃X�v���C�g�`��
		battleScene->SpriteDraw();
	}
	else
	{
		FieldSpriteDraw();
	}

	fadeTexture->Draw();
}

void KochaEngine::GamePlay::ObjDraw()
{
	if (battleScene->IsBattle())
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
	if (battleScene->IsBattle())
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

	gManager->Update();
	bManager->Update();
	pManager->Update();
	n3DManager->Update();
	cameraManager->Update();
	lightManager->Update();

	skyObj->MoveRotate(Vector3(0, 0.005f, 0));
	skyObj->SetPosition(Vector3(camera->GetEye().x, 0, camera->GetEye().z));
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
	battleScene->AlphaObjDraw();
	n3DManager->Draw(camera, lightManager);
}

void KochaEngine::GamePlay::FieldUpdate()
{
	gManager->Update();
	pManager->Update();
	n3DManager->Update();
	cameraManager->Update();
	lightManager->Update();

	battleScene->IsBattleStartFalse();

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
