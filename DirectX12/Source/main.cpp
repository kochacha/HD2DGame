#include <Windows.h>
#include "../Header/Application.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int nCmdShow)
{
	auto& app = KochaEngine::Application::Instance();
	if (!app.Initialize())
		return -1;
	app.Run();
	app.Terminate();
	return 0;
}