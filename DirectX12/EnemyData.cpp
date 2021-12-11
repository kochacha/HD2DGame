#include "EnemyData.h"
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

KochaEngine::Attribute KochaEngine::EnemyData::GetAttribute(const int arg_num)
{
    Attribute attribute;
    switch (arg_num)
    {
    case 0:
        attribute = Attribute::TYPE_FIRE;
        break;
    case 1:
        attribute = Attribute::TYPE_WATER;
        break;
    case 2:
        attribute = Attribute::TYPE_NATURE;
        break;
    case 3:
        attribute = Attribute::TYPE_LIGHT;
        break;
    case 4:
        attribute = Attribute::TYPE_DARK;
        break;
    default:
        attribute = Attribute::TYPE_FIRE;
        break;
    }
    return attribute;
}

void KochaEngine::EnemyData::StaticInit()
{
    auto obj = JsonLoader::ReadJsonObj("Resources/DataBase/EnemyData.json");
    ActorParam param;
    for (const auto& p : obj) 
    {
        std::string name = p.first;
        picojson::object& enemyID = obj[name].get<picojson::object>();
        param.name = "Enemy/" + name + ".txt";
        param.texName = name;
        param.attribute = GetAttribute(enemyID["Attribute"].get<double>());
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

        AddEnemyParam(name, param);
    }

    //ここにエネミーのデータを登録していく(jsonファイルから読み込めるようにする)
    //AddEnemyParam(NIHUTERIZA, "nihuteriza", TYPE_DARK, SMALL_ENEMY_SIZE, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 60, 5, 5);
    //AddEnemyParam(BABYDORAGON, "babydoragon", TYPE_FIRE, SMALL_ENEMY_SIZE, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 60, 8, 8);
}