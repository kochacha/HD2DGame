#include "../Header/Ending.h"
#include "../Header/Input.h"
#include "../Header/GamePlay.h"
#include "../Header/Util.h"
#include "../Header/CSVReader.h"
#include <fstream>

KochaEngine::Ending::Ending()
{
}

KochaEngine::Ending::~Ending()
{
}

void KochaEngine::Ending::Initialize()
{
	nextFlag = false;
	isEnd = false;

	fadeAlpha = 1;
	fadeFlag = true;
	count = 0;
	down = 0;
}

void KochaEngine::Ending::Update()
{
	count++;

	if (!(count % 29))
	{
		down = 7.0f;
	}
	float endUp = 0.0f;
	down = Util::Lerp(down, endUp, 0.2f);

	Fade();
	if (nextFlag && Input::TriggerPadButton(XINPUT_GAMEPAD_A))
	{
		fadeFlag = false;
	}
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) && fadeAlpha <= 0.0f)
	{
		nextFlag = true;
	}
	if (!fadeFlag && fadeAlpha >= 1.0f)
	{
		isEnd = true;
	}
}

void KochaEngine::Ending::SpriteDraw()
{
}

void KochaEngine::Ending::ObjDraw()
{
}

void KochaEngine::Ending::DrawGUI()
{
	ImGui::Text("Ending");
}

void KochaEngine::Ending::Load()
{
}

KochaEngine::Scenes KochaEngine::Ending::Next()
{
	return TITLE;
}

void KochaEngine::Ending::Fade()
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
