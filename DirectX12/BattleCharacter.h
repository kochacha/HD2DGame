#pragma once
#include "BattleObject.h"

namespace KochaEngine
{
	class Text;
	class Texture2D;
	class Number;
	class EffectManager;
	class Number3DEmitter;

	class BattleCharacter : public BattleObject
	{
	private:
		Object* levelUpUI;
		EffectManager* effectManager;
		Number3DEmitter* n3DEmitter;

		Text* nameText;
		Texture2D* battleStatusTex;
		Texture2D* slashTex[2];
		Texture2D* gaugeTex[4];
		Number* paramTex[5];
		BattleObjectType battleObjectType;

		bool isLevelUpAnimationUpdate;

		int knockBackTime;
		int getExp;
		int needExp;
		int levelUpAnimationTime;
		int levelUpAnimationNum;

		float hpBarSize;
		float spBarSize;
		float startPosX;
		float prePosX;
		float activePosX;

		void Support(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void Damage(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void MagicDamage(const SkillParam& arg_activeData, const ActorParam& arg_param);
		void PhysicsDamage(const SkillParam& arg_activeData, const ActorParam& arg_param);

		void EasingPosition();
		void FixParam();
		void SetGauge();
		void SetObjParam();
		void CalcExp();
		void LevelUpStatus();

	public:
		BattleCharacter(EffectManager* arg_effectManager, Number3DEmitter* arg_n3DEmitter, const BattleObjectType& arg_battleObjectType, const Vector3& arg_position, const ActorParam& arg_param);
		~BattleCharacter() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void SpriteDraw() override;
		BattleObjectType GetType() override;

		void AddExp(const int arg_exp) override;
		void AddMoney(const int arg_money) override;
		void SetDamage(const int arg_damage) override;
		void SetDamage(const std::string& arg_skillName, const ActorParam& arg_param) override;
		void SetDefaultWaitTexture() override;
		void SetAttackTextureIndex(const int arg_index) override;
	};

}