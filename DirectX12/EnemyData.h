#pragma once
#include "ActorParam.h"
#include <map>

namespace KochaEngine
{
	enum EnemyVol
	{
		NIHUTERIZA,
		BABYDORAGON,
	};

	class EnemyData
	{
	public:
		static void StaticInit();
		static ActorParam GetEnemyParam(const EnemyVol& arg_enemyVol);

	private:
		static std::map<EnemyVol, ActorParam> params;
		static void AddEnemyParam(const EnemyVol& arg_enemyVol, 
			const std::string& arg_name, const Attribute& arg_attribute, const Vector3& arg_size, 
			const unsigned int arg_level, const int arg_maxHP, const int arg_hp, const int arg_maxSP, 
			const int arg_sp, const int arg_attack, const int arg_defence, const int arg_intelligence, 
			const int arg_speed, const int arg_skillful, const int arg_luck, const int arg_exp, 
			const int arg_money);
	};
}