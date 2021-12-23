#pragma once
#include <string>
#include <vector>
#include "Vector3.h"
#include "Vector4.h"

namespace KochaEngine
{
	//����
	enum Attribute
	{
		TYPE_NORMAL,
		TYPE_FIRE,
		TYPE_WATER,
		TYPE_NATURE,
		TYPE_LIGHT,
		TYPE_DARK,
	};

	//�L�����E�G�l�~�[�̃p�����[�^
	struct ActorParam
	{
		std::string name;
		std::string texName;
		Attribute attribute;
		Vector3 size;
		unsigned int level;
		int maxHP;
		int hp;
		int maxSP;
		int sp;
		int attack;
		int defence;
		int intelligence;
		int speed;
		int skillful;
		int luck;
		int exp;
		int money;
		std::vector<std::string> skills;
	};
}