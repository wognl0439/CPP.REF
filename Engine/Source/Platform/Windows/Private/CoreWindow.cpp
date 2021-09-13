// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "CoreWindow.h"
#include <Windows.h>
#include "LogGame.h"
#include "WindowsPlatform/WindowsPlatformMouse.h"
#include "WindowsPlatform/WindowsPlatformKeyboard.h"

DEFINE_LOG_CATEGORY(LogWindowsLaunch);

inline void SetupHwndParameters(HWND hWnd, LPARAM lParam)
{
	auto lpCreateStruct = (LPCREATESTRUCTW)lParam;
	auto* coreWindow = (SCoreWindow*)lpCreateStruct->lpCreateParams;

	SetPropW(hWnd, L"this", coreWindow);
}

inline SCoreWindow* GetThis(HWND hWnd)
{
	return (SCoreWindow*)GetPropW(hWnd, L"this");
}

inline void FinallizeHwndParameters(HWND hWnd)
{
	RemovePropW(hWnd, L"this");
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATEAPP:
		SWindowsPlatformMouse::ProcessMessage(uMsg, wParam, lParam);
		SWindowsPlatformKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		SWindowsPlatformMouse::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		SWindowsPlatformKeyboard::ProcessMessage(uMsg, wParam, lParam);
		break;
	case WM_CREATE:
		SetupHwndParameters(hWnd, lParam);
		break;
	case WM_SIZE:
		if (SCoreWindow* cw = GetThis(hWnd); cw != nullptr)
		{
			cw->Size.Invoke((int16)LOWORD(lParam), (int16)HIWORD(lParam));
		}
		break;
	case WM_DESTROY:
		FinallizeHwndParameters(hWnd);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

SCoreWindow::SCoreWindow() : Super()
{
	WNDCLASSEXW wcex = {};
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName = L"CoreWindow";
	wcex.hInstance = GetModuleHandleW(nullptr);
	if (RegisterClassExW(&wcex) == 0)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"Could not register window class with error code: {}. Abort.", ::GetLastError());
		return;
	}

	HWND hWnd = CreateWindowExW(0, wcex.lpszClassName, ApplicationTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, wcex.hInstance, this);
	if (hWnd == nullptr)
	{
		SE_LOG(LogWindowsLaunch, Fatal, L"Could not create core window with error code: {}. Abort.", ::GetLastError());
		return;
	}

	_hwnd = hWnd;
}

void SCoreWindow::Start()
{
	::ShowWindow((HWND)_hwnd, SW_SHOW);

	MSG msg{ };

	_bMainLoop = true;
	while (_bMainLoop)
	{
		if (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				_bMainLoop = false;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		else
		{
			Idle.Invoke();
		}
	}
}

void* SCoreWindow::GetWindowHandle() const
{
	return _hwnd;
}

int32 SCoreWindow::GetLastError() const
{
	return _lastError;
}

void SCoreWindow::SetLastError(int32 code)
{
	_lastError = code;
}

int32 SCoreWindow::GetFrameworkWidth() const
{
	RECT rc;
	GetClientRect((HWND)_hwnd, &rc);
	return rc.right - rc.left;
}

int32 SCoreWindow::GetFrameworkHeight() const
{
	RECT rc;
	GetClientRect((HWND)_hwnd, &rc);
	return rc.bottom - rc.top;
}

void SCoreWindow::SetFrameworkTitle(const std::wstring& frameworkTitle)
{
	SetWindowTextW((HWND)_hwnd, frameworkTitle.c_str());
}