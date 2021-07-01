#pragma once

namespace KochaEngine
{
	enum Scenes
	{
		TITLE,
		STAGESELECT,
		GAMEPLAY,
		ENDING,
		GAMEOVER,
	};

	enum Color
	{
		TYPE1,
		TYPE2,
		TYPE3,
		TYPE4
	};

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();
		virtual void Update();
		virtual void SpriteDraw();
		virtual void ObjDraw();
		virtual void Initialize();
		virtual void Load();
		virtual bool IsEndFlag();
		virtual bool IsGameOverFlag();
		virtual Scenes Next();
		virtual Scenes GameOver();
	protected:
		bool isEnd;
		bool isGameOver;
		Color nowType;
	};
}