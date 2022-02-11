#include "Fighter.h"
#include "JsonLoader.h"
#include "SkillData.h"

KochaEngine::Fighter::Fighter(const Vector3& arg_position)
{
	position = position;

	Initialize();
}

KochaEngine::Fighter::~Fighter()
{
	//delete obj;
}

void KochaEngine::Fighter::Initialize()
{
	isSkillUpdate = true;

	//パラメーターのセット(後々テキストデータから読み込めるようにする(セーブ機能))
	auto obj = JsonLoader::ReadJsonObj("Resources/DataBase/FighterData.json");
	param.name = "Character/fighter.txt";
	param.texName = "fighter";
	param.size = Vector3(10, 10, 10);
	param.attribute = SkillData::GetAttribute(obj["Attribute"].get<double>());
	param.level = obj["Level"].get<double>();
	param.maxHP = obj["MaxHP"].get<double>();
	param.hp = obj["HP"].get<double>();
	param.maxSP = obj["MaxSP"].get<double>();
	param.sp = obj["SP"].get<double>();
	param.attack = obj["Attack"].get<double>();
	param.defence = obj["Defence"].get<double>();
	param.intelligence = obj["Intelligence"].get<double>();
	param.speed = obj["Speed"].get<double>();
	param.skillful = obj["Skillful"].get<double>();
	param.luck = obj["Luck"].get<double>();
	param.exp = obj["Exp"].get<double>();
	param.money = obj["Money"].get<double>();

	picojson::object& skills = obj["Skill"].get<picojson::object>();
	for (const auto& e : skills) {
		skillNames.emplace(skills[e.first].get<double>(), e.first);
		skillMasterLevels.push_back(skills[e.first].get<double>());
	}
}

void KochaEngine::Fighter::Update()
{
	SkillUpdate();
}

void KochaEngine::Fighter::Hit()
{
}

void KochaEngine::Fighter::ObjDraw(Camera* arg_camera, LightManager* arg_lightManager)
{
}

KochaEngine::GameObjectType KochaEngine::Fighter::GetType()
{
	return FIELD_FIGHTER;
}

void KochaEngine::Fighter::SkillUpdate()
{
	if (!isSkillUpdate) return;
	isSkillUpdate = false;

	param.skills.clear();

	for (int i = skillMasterLevels.size() - 1; i >= 0; i--)
	{
		if (param.level >= skillMasterLevels[i])
		{
			param.skills.push_back(skillNames.at(skillMasterLevels[i]));
		}
	}
}