#pragma once
#include "SkillParam.h"
#include <map>

namespace KochaEngine
{
	class SkillData
	{
	private:
		static std::map<std::string, SkillParam> params;
		static void AddSkillParam(const std::string& arg_skillVol, const SkillParam& arg_param);
		static Attribute GetAttribute(const int arg_num);

	public:
		static void StaticInit();
		static SkillParam GetSkillParam(const std::string& arg_skillVol);
	};
}