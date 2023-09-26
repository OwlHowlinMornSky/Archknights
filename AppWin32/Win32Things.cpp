/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*
* @Description
*     Win32Things.cpp : 定义有关Win32的基本行为。
*/
#include "Win32Things.h"

#include <strsafe.h>

#include "Callbacks.h"

namespace {

void CALLBACK MyTimerProc(HWND hWnd, UINT message, UINT_PTR nTimerid, DWORD systemTick) {
	return Callbacks::OnIdle();
}

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static UINT_PTR l_timerID = 0;
	switch (message) {
	// Don't forward this to DefWindowProc because it will destroy the window.
	case WM_CLOSE:
		break;

	case WM_GETMINMAXINFO:
	{
		// Compute the minimum size of the window.
		const CREATESTRUCT& info = *(CREATESTRUCT*)lParam;
		RECT rect = { 0, 0, 120, 90 };
		AdjustWindowRectEx(&rect, info.style, info.hMenu != NULL, info.dwExStyle);
		((MINMAXINFO*)lParam)->ptMinTrackSize = { rect.right - rect.left, rect.bottom - rect.top };
		break;
	}

	case WM_NCLBUTTONDOWN:
		switch (wParam) {
		case HTCAPTION:
			PostMessageW(hWnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
			PostMessageW(hWnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, lParam);
			PostMessageW(hWnd, WM_MOUSEMOVE, MK_LBUTTON, 0);
			return 0;
		case HTMINBUTTON:
		case HTMAXBUTTON:
		case HTCLOSE:
			PostMessageW(hWnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
			return 0;
		}
		return DefWindowProcW(hWnd, message, wParam, lParam);

	case WM_NCLBUTTONUP:
		switch (wParam) {
		case HTMINBUTTON:
			PostMessageW(hWnd, WM_SYSCOMMAND,
						 (GetWindowLongPtrW(hWnd, GWL_STYLE) & WS_MINIMIZE) ? SC_RESTORE : SC_MINIMIZE, lParam);
			break;
		case HTMAXBUTTON:
			PostMessageW(hWnd, WM_SYSCOMMAND,
						 (GetWindowLongPtrW(hWnd, GWL_STYLE) & WS_MAXIMIZE) ? SC_RESTORE : SC_MAXIMIZE, lParam);
			break;
		case HTCLOSE:
			PostMessageW(hWnd, WM_SYSCOMMAND, SC_CLOSE, lParam);
			break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		break;

	case WM_NCRBUTTONDOWN:
		break;

	case WM_NCRBUTTONUP:
		PostMessageW(hWnd, WM_CONTEXTMENU, NULL, lParam);
		break;

	case WM_SYSCOMMAND:
		switch (wParam & 0xFFF0) {
		// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
		case SC_KEYMENU:
			break;

		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		break;

	case WM_ENTERMENULOOP:
		if (l_timerID) KillTimer(0, l_timerID);
		l_timerID = SetTimer(0, 0, 10, (TIMERPROC)MyTimerProc);
		Callbacks::OnEnterSysloop();
		break;

	case WM_EXITMENULOOP:
		if (l_timerID) KillTimer(0, l_timerID);
		l_timerID = 0;
		Callbacks::OnExitSysloop();
		break;

	case WM_SIZING:
		Callbacks::OnSizing();
		break;

	case WM_ENTERSIZEMOVE:
		if (l_timerID) KillTimer(0, l_timerID);
		l_timerID = SetTimer(0, 0, 10, (TIMERPROC)MyTimerProc);
		Callbacks::OnEnterSysloop();
		break;

	case WM_EXITSIZEMOVE:
		if (l_timerID) KillTimer(0, l_timerID);
		l_timerID = 0;
		Callbacks::OnExitSysloop();
		break;

	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

const WCHAR szWindowClass[] = L"WndClass.OHMS.Archknights";
const WCHAR szTitle[] = L"Archknights";

} // namespace

namespace SystemThings {

void WinCheckError(LPCWSTR lpszFunction) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf,
		0, NULL);

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
									  (static_cast<SIZE_T>(lstrlenW((LPCWSTR)lpMsgBuf)) +
									   lstrlenW((LPCWSTR)lpszFunction) + 50) * sizeof(WCHAR));
	if (lpDisplayBuf) {
		StringCchPrintfW((LPWSTR)lpDisplayBuf,
						 LocalSize(lpDisplayBuf) / sizeof(WCHAR),
						 L"\'%s\' failed with error code \'%d\': %s",
						 lpszFunction, dw, (LPCWSTR)lpMsgBuf);
		MessageBoxW(NULL, (LPCWSTR)lpDisplayBuf, L"Error", MB_ICONERROR);
	}
	else {
		MessageBoxW(NULL, (LPCWSTR)lpMsgBuf, L"Error", MB_ICONERROR);
	}

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	return;
}

bool MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex{ 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = NULL;// CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ::MyWndProc;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON0));
	wcex.hIconSm = wcex.hIcon;
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszClassName = ::szWindowClass;

	if (RegisterClassExW(&wcex) == 0) {
		WinCheckError(L"RegisterClassEx");
		return false;
	}
	return true;
}

bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd) {
	HWND res = CreateWindowExW(WS_EX_APPWINDOW, ::szWindowClass, ::szTitle,
							   WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							   //CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
							   CW_USEDEFAULT, 0, 800, 600,
							   nullptr, nullptr, hInstance, nullptr);
	if (res == NULL) {
		WinCheckError(L"CreateWindow");
		return false;
	}
	ShowWindow(res, nCmdShow);
	UpdateWindow(res);
	hWnd = res;
	return true;
}

void MyUnregisterClass(HINSTANCE hInstance) {
	UnregisterClassW(::szWindowClass, hInstance);
	return;
}

} // namespace SystemThings
