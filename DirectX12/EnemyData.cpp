#include "EnemyData.h"
#include "SkillData.h"
#include "GameSetting.h"
#include "JsonLoader.h"

std::map<std::string, KochaEngine::ActorParam> KochaEngine::EnemyData::params = {};

KochaEngine::ActorParam KochaEngine::EnemyData::GetEnemyParam(const std::string& arg_enemyVol)
{
    return params[arg_enemyVol];
}

void KochaEngine::EnemyData::AddEnemyParam(const std::string& arg_enemyVol, const ActorParam& arg_param)
{
    params.emplace(arg_enemyVol, arg_param);
}

void KochaEngine::EnemyData::StaticInit()
{
    auto obj = JsonLoader::ReadJsonObj("Resources/DataBase/EnemyData.json");
    for (const auto& p : obj) 
    {
        ActorParam param;
        std::string name = p.first;
        picojson::object& enemyID = obj[name].get<picojson::object>();
        param.name = "Enemy/" + name + ".txt";
        param.texName = name;
        param.attribute = SkillData::GetAttribute(enemyID["Attribute"].get<double>());
        float size = enemyID["Size"].get<double>();
        param.size = Vector3(size, size, size);
        param.level = enemyID["Level"].get<double>();
        param.maxHP = enemyID["HP"].get<double>();
        param.hp = enemyID["HP"].get<double>();
        param.maxSP = enemyID["SP"].get<double>();
        param.sp = enemyID["SP"].get<double>();
        param.attack = enemyID["Attack"].get<double>();
        param.defence = enemyID["Defence"].get<double>();
        param.intelligence = enemyID["Intelligence"].get<double>();
        param.speed = enemyID["Speed"].get<double>();
        param.skillful = enemyID["Skillful"].get<double>();
        param.luck = enemyID["Luck"].get<double>();
        param.exp = enemyID["Exp"].get<double>();
        param.money = enemyID["Money"].get<double>();

        //使うじゅもん・とくぎなども読み込む(arrayで管理)
        picojson::array& skills = enemyID["Skill"].get<picojson::array>();
        for (const auto& e : skills) {
            param.skills.push_back(e.get<std::string>());
        }

        AddEnemyParam(name, param);
    }

    //ここにエネミーのデータを登録していく
    //AddEnemyParam(NIHUTERIZA, "nihuteriza", TYPE_DARK, SMALL_ENEMY_SIZE, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 60, 5, 5);
    //AddEnemyParam(BABYDORAGON, "babydoragon", TYPE_FIRE, SMALL_ENEMY_SIZE, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 60, 8, 8);
}