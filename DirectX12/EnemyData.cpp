#include "EnemyData.h"
#include "GameSetting.h"

std::map<KochaEngine::EnemyVol, KochaEngine::ActorParam> KochaEngine::EnemyData::params = {};

KochaEngine::ActorParam KochaEngine::EnemyData::GetEnemyParam(const EnemyVol& arg_enemyVol)
{
    return params[arg_enemyVol];
}

void KochaEngine::EnemyData::AddEnemyParam(const EnemyVol& arg_enemyVol, const std::string& arg_name, 
    const Attribute& arg_attribute, const Vector3& arg_size, const unsigned int arg_level, 
    const int arg_maxHP, const int arg_hp, const int arg_maxSP, const int arg_sp, const int arg_attack, 
    const int arg_defence, const int arg_intelligence, const int arg_speed, const int arg_skillful, 
    const int arg_luck, const int arg_exp, const int arg_money)
{
    auto name = "Enemy/" + arg_name + ".txt";

    ActorParam param;
    param.name = name; //�\���p�̖��O�̃e�L�X�g�f�[�^
    param.texName = arg_name; //texture�̖��O(�p�X�͕K�v�Ȃ�)
    param.attribute = arg_attribute; //����
    param.size = arg_size; //�傫��
    param.level = arg_level;
    param.maxHP = arg_maxHP;
    param.hp = arg_hp;
    param.maxSP = arg_maxSP;
    param.sp = arg_sp;
    param.attack = arg_attack;
    param.defence = arg_defence;
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
    //�����ɃG�l�~�[�̃f�[�^��o�^���Ă���
    AddEnemyParam(NIHUTERIZA, "nihuteriza", TYPE_DARK, SMALL_ENEMY_SIZE, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 60, 5, 5);
    AddEnemyParam(BABYDORAGON, "babydoragon", TYPE_FIRE, SMALL_ENEMY_SIZE, 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 60, 8, 8);
}