#include "EnemyData.h"
#include "GameSetting.h"

std::map<KochaEngine::EnemyVol, KochaEngine::ActorParam> KochaEngine::EnemyData::params = {};

KochaEngine::ActorParam KochaEngine::EnemyData::GetEnemyParam(const EnemyVol& arg_enemyVol)
{
    return params[arg_enemyVol];
}

void KochaEngine::EnemyData::AddEnemyParam(const EnemyVol& arg_enemyVol, const std::string& arg_name, 
    const Attribute& arg_attribute, const Vector3& arg_size, const unsigned int arg_level, 
    const int arg_hp, const int arg_sp, const int arg_attack, const int arg_diffence, 
    const int arg_intelligence, const int arg_speed, const int arg_skillful, const int arg_luck, 
    const int arg_exp, const int arg_money)
{
    auto name = "Enemy/" + arg_name + ".txt";

    ActorParam param;
    param.name = name; //表示用の名前のテキストデータ
    param.texName = arg_name; //textureの名前(パスは必要なし)
    param.attribute = arg_attribute; //属性
    param.size = arg_size; //大きさ
    param.level = arg_level;
    param.hp = arg_hp;
    param.sp = arg_sp;
    param.attack = arg_attack;
    param.diffence = arg_diffence;
    param.intelligence = arg_intelligence;
    param.speed = arg_speed;
    param.skillful = arg_skillful;
    param.luck = arg_luck;
    param.exp = arg_exp;
    param.money = arg_money;

    params.emplace(arg_enemyVol, param);
}

void KochaEngine::EnemyData::StaticInit()
{
    //ここにエネミーのデータを登録していく
    AddEnemyParam(NIHUTERIZA, "nihuteriza", TYPE_DARK, SMALL_ENEMY_SIZE, 1, 10, 5, 5, 5, 5, 5, 5, 50, 5, 5);
    AddEnemyParam(BABYDORAGON, "babydoragon", TYPE_FIRE, SMALL_ENEMY_SIZE, 1, 10, 5, 5, 5, 5, 5, 5, 50, 5, 5);
}