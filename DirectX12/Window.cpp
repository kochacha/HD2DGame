#include "Window.h"
#include "imgui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

KochaEngine::Window::Window()
{
}

KochaEngine::Window::~Window()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}

LRESULT KochaEngine::Window::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY://ウィンドウが破棄された
		PostQuitMessage(0);//OSに対して、アプリの終了を伝える
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool KochaEngine::Window::Create()
{
	//ウィンドウクラス生成＆登録
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//コールバック関数の指定
	w.lpszClassName = L"MyEngine";//アプリケーションクラス名(適当でいいです)
	w.hInstance = GetModuleHandle(0);//ハンドルの取得
	RegisterClassEx(&w);//アプリケーションクラス(こういうの作るからよろしくってOSに予告する)

	RECT wrc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };//ウィンドウサイズを決める
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//ウィンドウのサイズはちょっと面倒なので関数を使って補正する
	//ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName,//クラス名指定
		L"MyEngine",//タイトルバーの文字
		WS_OVERLAPPEDWINDOW,//タイトルバーと境界線があるウィンドウです
		CW_USEDEFAULT,//表示X座標はOSにお任せします
		CW_USEDEFAULT,//表示Y座標はOSにお任せします
		wrc.right - wrc.left,//ウィンドウ幅
		wrc.bottom - wrc.top,//ウィンドウ高
		nullptr,//親ウィンドウハンドル
		nullptr,//メニューハンドル
		w.hInstance,//呼び出しアプリケーションハンドル
		nullptr);//追加パラメータ

	HWND hDeskWnd;
	RECT deskrc, rc;
	int x, y;

	//ウィンドウを中央に表示
	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
	GetWindowRect(hwnd, (LPRECT)&rc);
	x = (deskrc.right - (rc.right - rc.left)) / 2;
	y = (deskrc.bottom - (rc.bottom - rc.top)) / 2;
	SetWindowPos(hwnd, HWND_TOP, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);

	//ウィンドウ生成に失敗した場合
	if (hwnd == nullptr)
		return false;

	//成功したのでtrue
	return true;
}

bool KochaEngine::Window::IsCreated()
{
	if (this->hwnd == nullptr)
		return false;
	else
		return true;
}

bool KochaEngine::Window::Show()
{
	if (this->hwnd == nullptr)
		return false;

	ShowWindow(this->hwnd, SW_SHOW);
	return true;
}

HWND KochaEngine::Window::GetHwnd()
{
	return hwnd;
}

const unsigned int KochaEngine::Window::GetWIN_WIDTH()
{
	return WIN_WIDTH;
}

const unsigned int KochaEngine::Window::GetWIN_HEIGHT()
{
	return WIN_HEIGHT;
}