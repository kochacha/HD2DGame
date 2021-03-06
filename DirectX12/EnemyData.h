#pragma once
#include "ActorParam.h"
#include <map>

namespace KochaEngine
{
	class EnemyData
	{
	private:
		static std::map<std::string, ActorParam> params;
		static void AddEnemyParam(const std::string& arg_enemyVol, const ActorParam& arg_param);

	public:
		static void StaticInit();
		static ActorParam GetEnemyParam(const std::string& arg_enemyVol);
	};
}