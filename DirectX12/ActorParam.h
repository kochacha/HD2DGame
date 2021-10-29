#pragma once
#include <string>
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	//����
	enum Attribute
	{
		FIRE,
		WATER,
		NATURE,
		LIGHT,
		DARK,
	};

	//�L�����E�G�l�~�[�̃p�����[�^
	struct ActorParam
	{
		std::string name;
		Attribute attribute;
		unsigned int level;
		int hp;
		int mp;
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