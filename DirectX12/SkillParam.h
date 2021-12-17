#pragma once
#include "ActorParam.h"

namespace KochaEngine
{
	struct SkillParam
	{
		std::string name;
		std::string effectName;
		std::string summaryName; //技説明テキスト
		Attribute attribute; //属性

		bool isSupport; //バフ系かどうか
		bool isMagic; //魔法かどうか
		bool isOverall; //全体かどうか
		bool isAllies; //味方かどうか

		int frequency; //回数
		int cost; //消費SP
		int baseDamage; //基礎ダメージ

		//依存度(倍率)
		float attackRate; //攻撃力
		float defenceRate; //防御力
		float intelligenceRate; //かしこさ
		float speedRate; //すばやさ
		float skillfulRate; //器用さ
		float luckRate; //運
	};
}