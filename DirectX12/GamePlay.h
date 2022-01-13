#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "GameSetting.h"

namespace KochaEngine
{
	class Map;
	class LightManager;
	class EffectManager;
	class Text;
	class Number;
	class Number3D;
	class Audio;
	class Texture2D;
	class Number3DManager;
	class Number3DEmitter;
	class Dx12_Wrapper;
	class CameraManager;

	class GamePlay : public Scene
	{
		enum CommandTab
		{
			DEFAULT_TAB,
			TARGET_SELECT_TAB,
			SKILL_TAB,
			ITEM_TAB,
		};

	private:
		Dx12_Wrapper& dx12;

		Map* map;
		Camera* camera;
		CameraManager* cameraManager;
		GameObjectManager* gManager;
		BattleObjectManager* bManager;
		ParticleManager* pManager;
		ParticleEmitter* pEmitter;
		EffectManager* effectManager;
		Number3DManager* n3DManager;
		Number3DEmitter* n3DEmitter;
		LightManager* lightManager;
		BattleObject* currentActiveActor;
		BattleObject* targetActor;

		Object* floor;
		Object* skyObj;
		Object* house;

		Texture2D* fadeTexture;
		Texture2D* defaultWakuTexture;
		Texture2D* anotherWakuTexture;
		Texture2D* defaultCommandTexture;
		Texture2D* waitCommandTexture;
		Texture2D* anotherCommandTexture;
		Texture2D* pageCommandTexture;
		Texture2D* spCommandTexture;
		Texture2D* cursorTexture;

		Text* battleLongText;
		Text* battleShortText;
		Text* summaryText;
		Text* battleNameText;
		Text* commandTitleText;
		Text* enemyNameText[MAX_NAME_TEXT_COUNT_COMMAND];
		Text* skillNameText[MAX_NAME_TEXT_COUNT_COMMAND];

		Number* defaultNumberTex;
		Number* costSPNumberTex;
		Number* pageNumberTex;
		Number* maxPageNumberTex;

		int extraNum;

		//���݂̃R�}���h�^�u
		CommandTab currentTab;
		//���O�̃R�}���h�^�u
		CommandTab previousTab;

		//�J�[�\���̃|�W�V����
		Vector2 cursorPos;

		//�t�F�[�h�A�E�g���邩�ǂ���
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

		//�t�F�[�h�̓����x
		float fadeAlpha;

		int frameCount;
		int seconds;
		int endCount;
		//�s�����̃A�j���[�V�����̎���	
		int motionTime;
		//�o�g���Q���L�����̐�
		int battleCharaCount;
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
		void BattleUpdate();
		void BattleFlowUpdate();
		void BattleObjDraw();
		void BattleAlphaObjDraw();
		void BattleSpriteDraw();

		void FieldUpdate();
		void FieldObjDraw();
		void FieldAlphaObjDraw();
		void FieldSpriteDraw();

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
		GamePlay(Dx12_Wrapper& arg_dx12);
		~GamePlay() override;
		void Initialize() override;
		void Update() override;
		void SpriteDraw() override;
		void ObjDraw() override;
		void AlphaObjDraw() override;
		void DrawGUI() override;
		void Load() override;
		Scenes Next() override;
		void CountTime();
		void FadeUpdate();
	};
}
