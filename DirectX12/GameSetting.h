#pragma once
#include "Vector2.h"
#include "Vector3.h"

namespace KochaEngine
{
	class GameSetting
	{
	public:

		//�}�b�v�f�[�^���[�h�p�ԍ�
		enum
		{
			//�ŏ��̃t�B�[���h
			HAZIMARINOTI,
			//�ŏ��̒�
			HAZIMARINOMATI,
		};

		//��b�̗����X�s�[�h
		static int talkSpeed;
		//�G�ƃG���J�E���g����t�B�[���h�ɂ��邩�ǂ���
		static bool isBattleField;
		//���݂̃t�B�[���h
		static int nowField;
		
		static float masterVolume;
		static float seVolume;
		static float bgmVolume;
	};

#define MAX_BATTLE_CHARA 3
#define MAX_BATTLE_ENEMY 6

	//�Q�[����ʃT�C�Y
	const Vector2 SCREEN_SIZE = Vector2(1280, 720);

	//��ʒ������̉�b�e�L�X�g�̘g
	const Vector2 DEFAULT_WAKU_POS = Vector2(210, 570);
	const Vector2 ANOTHER_WAKU_POS = Vector2(298, 570);
	const Vector2 DEFAULT_WAKU_SIZE = Vector2(768, 96);
	const Vector2 ANOTHER_WAKU_SIZE = Vector2(680, 96);

	//�o�g�����R�}���h
	const Vector2 DEFAULT_COMMAND_POS = Vector2(20, 450);
	const Vector2 DEFAULT_COMMAND_SIZE = Vector2(168, 240);
	const Vector2 ANOTHER_COMMAND_SIZE = Vector2(256, 240);
	const Vector2 ATTACK_COMMAND_SIZE = Vector2(256, 240);

	const unsigned int MAX_COMMAND_NUM = 5;

	//�o�g�����X�e�[�^�X
	const Vector2 BATTLE_STATUS_POS = Vector2(1000, 500);
	const Vector2 BATTLE_STATUS_SIZE = Vector2(256, 178);
	const Vector2 MAX_GAUGE_SIZE = Vector2(235, 5);

	//��ʒ������̉�b�e�L�X�g
	const Vector2 TALK_LONG_TEXT_POS = Vector2(225, 585);
	const Vector2 TALK_SHORT_TEXT_POS = Vector2(313, 585);

	//�o�g�����G�l�~�[�̃T�C�Y
	const Vector3 SMALL_ENEMY_SIZE = Vector3(10, 10, 10);
	const Vector3 MEDIUM_ENEMY_SIZE = Vector3(20, 20, 20);
	const Vector3 LARGE_ENEMY_SIZE = Vector3(30, 30, 30);

	//�U�����A�j���[�V��������
	const int ATTACK_MOTION_TIME = 180;
	const int SPELL_MOTION_TIME = 300;
	const int SKILL_MOTION_TIME = 240;

	//���U���g���{�^�����͑҂�����
	const int RESULT_INPUT_WAIT = 60;

	//�x�[�X(���x��1���)�ƂȂ�K�v�o���l
	const int BASE_EXP = 10;

	//�o�g�����G�o���|�W�V����
	//���T�C�Y�̓G
	const Vector3 SMALL_ENEMY_POS[4] =
	{ Vector3(-20, -20, 70),Vector3(-25, -20, 50),Vector3(-40, -20, 80),Vector3(-45, -20, 60) };
	//���T�C�Y�̓G�A���͔��ł���G
	const Vector3 MEDIUM_ENEMY_POS[4] =
	{ Vector3(-20, -15, 70),Vector3(-25, -15, 50),Vector3(-40, -15, 80),Vector3(-45, -15, 60) };
	//��T�C�Y�̓G
	const Vector3 LARGE_ENEMY_POS[4] =
	{ Vector3(-20, -10, 70),Vector3(-25, -10, 50),Vector3(-40, -10, 80),Vector3(-45, -10, 60) };

	//�o�g�����L�����N�^�[�o���|�W�V����
	const Vector3 BATTLE_CHARACTOR_POS[4] =
	{ Vector3(80, -20, 50),Vector3(80, -20, 60),Vector3(80, -20, 70),Vector3(80, -20, 80) };
}