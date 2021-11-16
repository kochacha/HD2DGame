#pragma once
#include <string>
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	//����
	enum Attribute
	{
		TYPE_FIRE,
		TYPE_WATER,
		TYPE_NATURE,
		TYPE_LIGHT,
		TYPE_DARK,
		TYPE_NORMAL,
	};

	//�L�����E�G�l�~�[�̃p�����[�^
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