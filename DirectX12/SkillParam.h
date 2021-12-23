#pragma once
#include "ActorParam.h"

namespace KochaEngine
{
	struct SkillParam
	{
		std::string name;
		std::string effectName;
		std::string summaryName; //�Z�����e�L�X�g
		Attribute attribute; //����

		bool isSupport; //�o�t�n���ǂ���
		bool isMagic; //���@���ǂ���
		bool isOverall; //�S�̂��ǂ���
		bool isAllies; //�������ǂ���

		int frequency; //��
		int cost; //����SP
		int baseDamage; //��b�_���[�W

		//�ˑ��x(�{��)
		float attackRate; //�U����
		float defenceRate; //�h���
		float intelligenceRate; //��������
		float speedRate; //���΂₳
		float skillfulRate; //��p��
		float luckRate; //�^
	};
}