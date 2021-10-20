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
		inline HWND GetHwnd() { return hwnd; }
		inline const unsigned int GetWinWidth(){ return WIN_WIDTH; }
		inline const unsigned int GetWinHeight() { return WIN_HEIGHT; }
	private:
		RECT wrc;
		HWND hwnd;
		WNDCLASSEX w;
		const unsigned int WIN_WIDTH = 1280;
		const unsigned int WIN_HEIGHT = 720;
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}
