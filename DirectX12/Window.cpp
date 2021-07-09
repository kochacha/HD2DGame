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
	//���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY://�E�B���h�E���j�����ꂽ
		PostQuitMessage(0);//OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool KochaEngine::Window::Create()
{
	//�E�B���h�E�N���X�������o�^
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;//�R�[���o�b�N�֐��̎w��
	w.lpszClassName = L"MyEngine";//�A�v���P�[�V�����N���X��(�K���ł����ł�)
	w.hInstance = GetModuleHandle(0);//�n���h���̎擾
	RegisterClassEx(&w);//�A�v���P�[�V�����N���X(���������̍�邩���낵������OS�ɗ\������)

	RECT wrc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };//�E�B���h�E�T�C�Y�����߂�
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);//�E�B���h�E�̃T�C�Y�͂�����Ɩʓ|�Ȃ̂Ŋ֐����g���ĕ␳����
	//�E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(w.lpszClassName,//�N���X���w��
		L"MyEngine",//�^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,//�^�C�g���o�[�Ƌ��E��������E�B���h�E�ł�
		CW_USEDEFAULT,//�\��X���W��OS�ɂ��C�����܂�
		CW_USEDEFAULT,//�\��Y���W��OS�ɂ��C�����܂�
		wrc.right - wrc.left,//�E�B���h�E��
		wrc.bottom - wrc.top,//�E�B���h�E��
		nullptr,//�e�E�B���h�E�n���h��
		nullptr,//���j���[�n���h��
		w.hInstance,//�Ăяo���A�v���P�[�V�����n���h��
		nullptr);//�ǉ��p�����[�^

	HWND hDeskWnd;
	RECT deskrc, rc;
	int x, y;

	//�E�B���h�E�𒆉��ɕ\��
	hDeskWnd = GetDesktopWindow();
	GetWindowRect(hDeskWnd, (LPRECT)&deskrc);
	GetWindowRect(hwnd, (LPRECT)&rc);
	x = (deskrc.right - (rc.right - rc.left)) / 2;
	y = (deskrc.bottom - (rc.bottom - rc.top)) / 2;
	SetWindowPos(hwnd, HWND_TOP, x, y, (rc.right - rc.left), (rc.bottom - rc.top), SWP_SHOWWINDOW);

	//�E�B���h�E�����Ɏ��s�����ꍇ
	if (hwnd == nullptr)
		return false;

	//���������̂�true
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