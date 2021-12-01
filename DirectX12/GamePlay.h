#pragma once
#include "Scene.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "BattleObjectManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Number.h"
#include "Audio.h"
#include "Texture2D.h"
#include "GameSetting.h"

namespace KochaEngine
{
	class Map;
	class LightManager;
	class Text;

	class GamePlay : public Scene
	{
		enum CommandTab
		{
			DEFAULT_TAB,
			ATTACK_TAB,
			SPELL_TAB,
			SKILL_TAB,
			ITEM_TAB,
		};

	private:
		Map* map;
		Camera* camera;
		GameObjectManager* gManager;
		BattleObjectManager* bManager;
		ParticleManager* pManager;
		ParticleEmitter* emitter;
		LightManager* lightManager;
		BattleObject* currentActiveActor;
		BattleObject* targetActor;

		Object* floor;
		Object* skyObj;

		Texture2D* fadeTexture;
		Texture2D* defaultWakuTexture;
		Texture2D* anotherWakuTexture;
		Texture2D* defaultCommandTexture;
		Texture2D* waitCommandTexture;
		Texture2D* anotherCommandTexture;
		Texture2D* cursorTexture;
		Text* battleLongText;
		Text* battleShortText;
		Text* battleNameText;
		Text* commandTitleText;
		Text* enemyNameText[MAX_BATTLE_ENEMY];

		//���݂̃R�}���h�^�u
		CommandTab currentTab;

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
		//���������E�������E�Ƃ������[�V���������ǂ���
		bool isAttackMotion;
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

		//�R�}���h�ԍ�
		unsigned int commandNum;
		//�R�}���h�ԍ��ۑ��p : 0,�f�t�H���g 1,�������� 2,������� 3,�Ƃ��� 4,�ǂ���
		unsigned int preCommandNum[5];


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
		void AttackMotionUpdate();
		void DefenceMotionUpdate();
		void ResultUpdate();
		void EnemyNameUpdate();
		void CommandDraw();
		void MoveCursor();
		void CursorPosSetting();
		void DefaultTab();
		void AttackTab();
		unsigned int CommandNumUp(const unsigned int arg_commandNum);
		unsigned int CommandNumDown(const unsigned int arg_commandNum);

	public:
		GamePlay();
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
