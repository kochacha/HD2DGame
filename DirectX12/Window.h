#pragma once
#include <Windows.h>

namespace KochaEngine
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Show();
		bool IsCreated();
		bool Create();
		HWND GetHwnd();
		const unsigned int GetWIN_WIDTH();
		const unsigned int GetWIN_HEIGHT();
		const unsigned int WIN_WIDTH = 1280;
		const unsigned int WIN_HEIGHT = 720;
	private:
		RECT wrc;
		HWND hwnd;
		WNDCLASSEX w;
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
