#include "SkillData.h"
#include "GameSetting.h"
#include "JsonLoader.h"

std::map<std::string, KochaEngine::SkillParam> KochaEngine::SkillData::params = {};

KochaEngine::SkillParam KochaEngine::SkillData::GetSkillParam(const std::string& arg_skillVol)
{
    return params[arg_skillVol];
}

void KochaEngine::SkillData::AddSkillParam(const std::string& arg_skillVol, const SkillParam& arg_param)
{
    params.emplace(arg_skillVol, arg_param);
}

KochaEngine::Attribute KochaEngine::SkillData::GetAttribute(const int arg_num)
{
    Attribute attribute;
    switch (arg_num)
    {
    case 0:
        attribute = Attribute::TYPE_NORMAL;
        break;
    case 1:
        attribute = Attribute::TYPE_FIRE;
        break;
    case 2:
        attribute = Attribute::TYPE_WATER;
        break;
    case 3:
        attribute = Attribute::TYPE_NATURE;
        break;
    case 4:
        attribute = Attribute::TYPE_LIGHT;
        break;
    case 5:
        attribute = Attribute::TYPE_DARK;
        break;
    default:
        attribute = Attribute::TYPE_NORMAL;
        break;
    }
    return attribute;
}

void KochaEngine::SkillData::StaticInit()
{
    auto obj = JsonLoader::ReadJsonObj("Resources/DataBase/SkillData.json");
    SkillParam param;
    for (const auto& p : obj)
    {
        std::string name = p.first;
        picojson::object& enemyID = obj[name].get<picojson::object>();
        param.name = "Skill/Name/" + name + "_name.txt";
        param.summaryName = "Skill/Summary/" + name + "_summary.txt";
        param.effectName = name + ".efk";
        param.attribute = GetAttribute(enemyID["Attribute"].get<double>());
        param.isSupport = GetAttribute(enemyID["IsSupport"].get<bool>());
        param.isMagic = GetAttribute(enemyID["IsMagic"].get<bool>());
        param.isOverall = GetAttribute(enemyID["IsOverall"].get<bool>());
        param.isAllies = GetAttribute(enemyID["IsAllies"].get<bool>());
        param.frequency = enemyID["Frequency"].get<double>();
        param.cost = enemyID["Cost"].get<double>();
        param.baseDamage = enemyID["BaseDamage"].get<double>();
        param.attackRate = enemyID["AttackRate"].get<double>();
        param.defenceRate = enemyID["DefenceRate"].get<double>();
        param.intelligenceRate = enemyID["IntelligenceRate"].get<double>();
        param.speedRate = enemyID["SpeedRate"].get<double>();
        param.skillfulRate = enemyID["SkillfulRate"].get<double>();
        param.luckRate = enemyID["LuckRate"].get<double>();

        AddSkillParam(name, param);
    }
}