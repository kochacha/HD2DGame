#include "Title.h"
#include "Texture2D.h"
#include "Input.h"
#include "Util.h"
#include "GameObjectManager.h"

KochaEngine::Title::Title()
{
	camera = new Camera();
}

KochaEngine::Title::~Title()
{
	delete camera;
}

void KochaEngine::Title::Initialize()
{
	isEnd = false;
	bgmFlag = false;

	camera->Initialize(1280, 720, 90, 0, { 0,0,-50 }, { 0,0,0 }, { 0,1,0 });

	fadeAlpha = 1;
	fadeFlag = true;
	count = 0;
	down = 0;
}

void KochaEngine::Title::Update()
{
	if (count < 100000)
	{
		count++;
	}
	else
	{
		count = 0;
	}

	if (!(count % 29))
	{
		down = 7.0f;
	}

	float endUp = 0.0f;
	down = Util::Lerp(down, endUp, 0.2f);

	Fade();
	if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_A)) && fadeAlpha <= 0.0f)
	{
		fadeFlag = false;
	}
	if (!fadeFlag && fadeAlpha >= 1.0f)
	{
		isEnd = true;
	}
	camera->Update();
}

void KochaEngine::Title::SpriteDraw()
{
}

void KochaEngine::Title::ObjDraw()
{
}

void KochaEngine::Title::DrawGUI()
{
	ImGui::Text("Title");
}

void KochaEngine::Title::Load()
{
	
}

KochaEngine::Scenes KochaEngine::Title::Next()
{
	return STAGESELECT;
}

void KochaEngine::Title::Fade()
{
	if (fadeFlag)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= 0.02f;
		}
	}
	else
	{
		if (fadeAlpha < 1.0f)
		{
			fadeAlpha += 0.02f;
		}
	}
}
