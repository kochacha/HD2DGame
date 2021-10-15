#include "Title.h"
#include "Texture2D.h"
#include "Input.h"
#include "Util.h"
#include "GameObjectManager.h"

KochaEngine::Title::Title()
{
	backTexture = new Texture2D("Resources/Texture/white.png", Vector2(0, 0), Vector2(1280, 720), 0);
	fadeTexture = new Texture2D("Resources/Texture/white.png", Vector2(0, 0), Vector2(1280, 720), 0);
}

KochaEngine::Title::~Title()
{
	delete backTexture;
	delete fadeTexture;
}

void KochaEngine::Title::Initialize()
{
	isEnd = false;
	bgmFlag = false;

	fadeAlpha = 1.0f;
	fadeFlag = false;
	count = 0;
	down = 0;

	backTexture->SetColor(Vector4(1, 1, 1, 1));
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, 1));
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

	FadeUpdate();
	if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_A)) && fadeAlpha <= 0.0f)
	{
		fadeFlag = true;
	}
	if (fadeFlag && fadeAlpha >= 1.0f)
	{
		isEnd = true;
	}
}

void KochaEngine::Title::SpriteDraw()
{
	backTexture->Draw();
	fadeTexture->Draw();
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
	return GAMEPLAY;
}

void KochaEngine::Title::FadeUpdate()
{
	const float MOVE_ALPHA = 0.02f;
	if (!fadeFlag)
	{
		if (fadeAlpha > 0)
		{
			fadeAlpha -= MOVE_ALPHA;
		}
	}
	else
	{
		if (fadeAlpha < 1.0f)
		{
			fadeAlpha += MOVE_ALPHA;
		}
	}
	fadeTexture->SetColor(Vector4(0, 0, 0.12f, fadeAlpha));
}