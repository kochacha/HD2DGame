#pragma once
#include "BattleObject.h"

namespace KochaEngine
{
	class Text;
	class Texture2D;
	class Number;

	class BattleCharacter : public BattleObject
	{
	private:
		Text* nameText;
		Texture2D* battleStatusTex;
		Texture2D* slashTex[2];
		Texture2D* gaugeTex[4];
		Number* paramTex[5];
		BattleObjectType battleObjectType;

		float hpBarSize;
		float spBarSize;
		float prePosX;

		void FixParam();
		void SetGauge();
		void SetObjParam();

	public:
		BattleCharacter(const BattleObjectType& arg_battleObjectType, const Vector3& arg_position, const ActorParam& arg_param);
		~BattleCharacter() override;

		void Initialize() override;
		void Update() override;
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager) override;
		void SpriteDraw() override;
		BattleObjectType GetType() override;

		void SetDamage(const int arg_damage) override;
		void ActiveReset() override;
		void ActiveDone() override;
	};

}