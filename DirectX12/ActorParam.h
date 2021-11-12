#pragma once
#include <string>
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	//属性
	enum Attribute
	{
		FIRE,
		WATER,
		NATURE,
		LIGHT,
		DARK,
	};

	//キャラ・エネミーのパラメータ
	struct ActorParam
	{
		std::string name;
		std::string texName;
		Attribute attribute;
		Vector3 size;
		unsigned int level;
		int hp;
		int sp;
		int attack;
		int diffence;
		int intelligence;
		int speed;
		int skillful;
		int luck;
		int exp;
		int money;
	};
}