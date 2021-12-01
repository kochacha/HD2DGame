#pragma once
#include "BattleObject.h"
#include "Camera.h"
#include <vector>

namespace KochaEngine
{
	class Enemy;

	class BattleObjectManager
	{
	private:
		Camera* camera;
		LightManager* lightManager;
		std::vector<BattleObject*> battleObjects;

		int totalMoney;
		int totalExp;
		int turn;

	public:
		BattleObjectManager();
		~BattleObjectManager();

		void AddObject(BattleObject* arg_battleObject);
		void Initialize();
		void Update();
		void ObjDraw(Camera* arg_camera, LightManager* arg_lightManager);
		void SpriteDraw();
		void AddReward(const int arg_money, const int arg_exp);

		int GetEnemyCount();
		Enemy* GetEnemy(const int arg_enemyNum);
		BattleObject* GetCurrentActive();
		BattleObject* GetCharacter(const KochaEngine::BattleObjectType& arg_type);

		void Remove();
		void RemoveAll();
		void Reward(); //報酬計算
		void ActiveReset();

		bool IsEnemyDestroy(); //エネミーが全滅したかどうか
		bool IsCharacterDestroy(); //キャラクターが全滅したかどうか

		inline const int GetTotalMoney() { return totalMoney; }
		inline const int GetTotalExp() { return totalExp; }
		inline const int GetTurn() { return turn; }
	};
}