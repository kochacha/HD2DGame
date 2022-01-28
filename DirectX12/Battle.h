#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "GameSetting.h"

namespace KochaEngine
{
	class LightManager;
	class EffectManager;
	class CameraManager;
	class Texture2D;
	class Number;
	class Number3D;
	class Number3DManager;
	class Number3DEmitter;
	class Text;
	class Audio;

	class Battle
	{
		//�R�}���h���쎞�̊e�y�[�W���
		enum CommandTab
		{
			DEFAULT_TAB,
			TARGET_SELECT_TAB,
			SKILL_TAB,
			ITEM_TAB,
		};

	private:
		Camera* camera; //�J����
		CameraManager* cameraManager; //�J�����̊Ǘ��N���X
		GameObjectManager* gManager;
		BattleObjectManager* bManager; //�o�g�����L�����E�G�l�~�[���Ǘ��N���X
		EffectManager* effectManager; //�G�t�F�N�g�֘A�̊Ǘ��N���X
		Number3DEmitter* n3DEmitter; //3D��ԏ�ɐ����𐶐�����N���X
		LightManager* lightManager; //���C�g�̊Ǘ��N���X

		//�^�[�Q�b�g�⌻�ݍs�����̃L�������i�[���Ă������߂̔�
		BattleObject* currentActiveActor;
		BattleObject* targetActor;

		//�o�g�����ɕ`�悳���UI�֘A
		Texture2D* defaultWakuTexture;
		Texture2D* anotherWakuTexture;
		Texture2D* defaultCommandTexture;
		Texture2D* waitCommandTexture;
		Texture2D* anotherCommandTexture;
		Texture2D* pageCommandTexture;
		Texture2D* spCommandTexture;
		Texture2D* cursorTexture;

		//�o�g�����ɕ`�悳���e�L�X�g�֘A
		Text* battleLongText;
		Text* battleShortText;
		Text* summaryText;
		Text* battleNameText;
		Text* commandTitleText;
		Text* enemyNameText[MAX_NAME_TEXT_COUNT_COMMAND];
		Text* skillNameText[MAX_NAME_TEXT_COUNT_COMMAND];

		//UI�Ƃ��ĕ\������鐔��
		Number* defaultNumberTex;
		Number* costSPNumberTex;
		Number* pageNumberTex;
		Number* maxPageNumberTex;

		//���݂̃R�}���h�^�u
		CommandTab currentTab;
		//���O�̃R�}���h�^�u
		CommandTab previousTab;

		//�J�[�\���̃|�W�V����
		Vector2 cursorPos;

		bool fadeFlag;
		//�o�g�������ǂ���
		bool isBattle;
		//�o�g�����I����Ă��邩�ǂ���
		bool isBattleEnd;
		//�o�g���J�n�̏u�Ԃ��ǂ���
		bool isBattleStart;
		//�R�}���h��̕\�������X�V���邩�ǂ���
		bool isCommandTitleUpdate;
		//���̃^�[���ɍs�����ǂ���
		bool isTurnUpdate;
		//���[�V���������ǂ���
		bool isActiveMotion;
		//�ڂ����僂�[�V���������ǂ���
		bool isDefenceMotion;
		//�G�̃^�[�����ǂ���
		bool isEnemyTurn;
		//�L�����N�^�[���S�ł��Ă��邩�ǂ���
		bool isCharacterDestroy;
		//�G���S�ł��Ă��邩�ǂ���
		bool isEnemyDestroy;
		//���U���g���Ɉ�x�����s�������p�t���O
		bool isResultOnce;
		//�f�t�H���g�E�B���h�E�ɐ�����`�悷�邩
		bool isShowNumber;
		//�R�}���h�������X�V�p
		bool isUpdateCommandExplanation;

		//�o�g���Q���L�����̐�
		int battleCharaCount;
		//�s�����̃A�j���[�V�����̎���	
		int motionTime;
		//�o�g���X�^�[�g���̍d������
		int battleStartWait;
		//���U���g�i�s�̔ԍ�
		int resultFlowNum;
		//���U���g�Ń{�^�����������Ƃ��̑҂�����
		int resultFlowWait;
		//�l���o���l
		int getExp;
		//�I�𒆂̃X�L���ԍ�
		int selectSkillIndex;
		//�X�L���^�u�̃y�[�W�ԍ�
		int skillTabPageNum;
		//�X�L���^�u�̍ő�y�[�W�ԍ�
		int skillTabMaxPageNum;
		//�X�L���^�u�̒��O�̃y�[�W�ԍ�
		int preSkillTabPageNum;
		//�X�L���̏���SP
		int costSP;

		//�R�}���h�ԍ�
		unsigned int commandNum;
		//�R�}���h�ԍ��ۑ��p : 0,�f�t�H���g
		unsigned int preCommandNum;

		void BattleInitialize();
		void BattleFlowUpdate();
		void BattleEnd();
		void TurnInitialize();
		void ActiveActorUpdate();
		void EnemyActionSelect();
		void ActiveMotionUpdate();
		void DefenceMotionUpdate();
		void ResultUpdate();
		void RewardCalc();
		void EnemyNameUpdate();
		void SkillNameUpdate();
		void SkillExplanationUpdate();
		void CommandDraw();
		void MoveCursor();
		void CursorPosSetting();
		void DefaultTab();
		void TargetSelectTab();
		void SkillTab();
		unsigned int CommandNumUp(const unsigned int arg_commandNum);
		unsigned int CommandNumDown(const unsigned int arg_commandNum);
		unsigned int CommandNumRight(const unsigned int arg_commandNum, const unsigned int arg_maxNum);
		unsigned int CommandNumLeft(const unsigned int arg_commandNum, const unsigned int arg_maxNum);

	public:

		Battle(Camera* arg_camera, CameraManager* arg_cameraManager, GameObjectManager* arg_gManager,
			BattleObjectManager* arg_bManager, EffectManager* arg_effectManager, 
			Number3DEmitter* arg_n3DEmitter, LightManager* arg_lightManager);
		~Battle();
		void Initialize();
		void Update();
		void SpriteDraw();
		void ObjDraw();
		void AlphaObjDraw();
		void DrawGUI();
		void Load();

		void BattleEndUpdate();
		void IsBattleTrue() { isBattle = true; }
		void IsBattleFalse() { isBattle = false; }
		void IsBattleStartFalse() { isBattleStart = false; }
		inline bool IsFade() { return fadeFlag; }
		inline bool IsBattle() { return isBattle; }
		inline bool IsBattleStart() { return isBattleStart; }
		inline bool IsBattleEnd() { return isBattleEnd; }
	};
}
