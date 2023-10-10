/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
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
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*
* @Description
*     Win32Things.cpp : �����й�Win32�Ļ�����Ϊ��
*/
#include "Win32Things.h"

#include <strsafe.h>
#include "../GUI/Callbacks.h"
#include "resource.h"

namespace {

/**
 * @brief ��ʱ���ص���
*/
void CALLBACK MyTimerProc(HWND hWnd, UINT message, UINT_PTR nTimerid, DWORD systemTick) {
	return Callbacks::OnIdle();
}

/**
 * @brief ���ڻص���
*/
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static UINT_PTR l_timerID = 0;
	switch (message) {
	// �������ظ���Ϣ����Ϊ DefWindowProc ���ܺ�����ٴ��ڡ�
	// ��������Ҫ�� main ��������ٵġ�
	// Don't forward this to DefWindowProc because it will destroy the window.
	case WM_CLOSE:
		break;

	case WM_GETMINMAXINFO:
	{
		// ���㴰����С�ߴ硣
		// ��֤���� �ͻ��� ��СΪ 120 * 90��
		// Compute the minimum size of the window.
		const CREATESTRUCT& info = *(CREATESTRUCT*)lParam;
		RECT rect = { 0, 0, 120, 90 };
		AdjustWindowRectEx(&rect, info.style, info.hMenu != NULL, info.dwExStyle);
		((MINMAXINFO*)lParam)->ptMinTrackSize = { rect.right - rect.left, rect.bottom - rect.top };
		break;
	}

	// ���������ռ�����Ϣ����Ϊ���²����ֻ�ֱ�ӿ�����
	case WM_NCLBUTTONDOWN:
		switch (wParam) {
		case HTMINBUTTON:
		case HTMAXBUTTON:
		case HTCLOSE:
			PostMessageW(hWnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
			return 0;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		break;

	// ���������ռ��Ĺ��ܡ�
	case WM_NCLBUTTONUP:
		switch (wParam) {
		case HTMINBUTTON:
		{
			LONG_PTR style = GetWindowLongPtrW(hWnd, GWL_STYLE);
			if (style & WS_MINIMIZEBOX) {
				if (style & WS_MINIMIZE)
					PostMessageW(hWnd, WM_SYSCOMMAND, SC_RESTORE, lParam);
				else
					PostMessageW(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, lParam);
			}
			break;
		}
		case HTMAXBUTTON:
		{
			LONG_PTR style = GetWindowLongPtrW(hWnd, GWL_STYLE);
			if (style & WS_MAXIMIZEBOX) {
				if (style & WS_MAXIMIZE)
					PostMessageW(hWnd, WM_SYSCOMMAND, SC_RESTORE, lParam);
				else
					PostMessageW(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, lParam);
			}
			break;
		}
		case HTCLOSE:
			if (Callbacks::ButtonEnabled_Close)
				PostMessageW(hWnd, WM_SYSCOMMAND, SC_CLOSE, lParam);
			break;
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		break;

	// �Ҽ�ҲҪ���أ���Ϊ���²�����Ҳ�Ῠ����
	case WM_NCRBUTTONDOWN:
		break;

	// �ɿ��Ҽ�����ϵͳ�˵���
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

WCHAR szWindowClass[64]; // ����������ơ�
WCHAR szTitle[64]; // ���ڵ� ��ʼ ���ơ�

} // namespace

namespace SystemThings {

void WinCheckError(LPCWSTR lpszFunction) noexcept {
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

void InitString(HINSTANCE hInstance) noexcept {
	LoadStringW(hInstance, IDS_CLASS_NAME, szWindowClass, 64);
	LoadStringW(hInstance, IDS_INIT_WINDOW_NAME, szTitle, 64);
	return;
}

bool MyRegisterClass(HINSTANCE hInstance) noexcept {
	WNDCLASSEX wcex{ 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	// û��Ҫ�ػ档
	wcex.style = NULL;// CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ::MyWndProc;
	wcex.hInstance = hInstance;
	// ������δ��ɣ�����ͼ�꼤�������У���
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

bool MyCreateWindow(HINSTANCE hInstance, int nCmdShow, HWND& hWnd) noexcept {
	HWND res = CreateWindowExW(WS_EX_APPWINDOW, ::szWindowClass, ::szTitle,
							   WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
							   CW_USEDEFAULT, 0, 420, 420,
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

void MyUnregisterClass(HINSTANCE hInstance) noexcept {
	UnregisterClassW(::szWindowClass, hInstance);
	return;
}

} // namespace SystemThings
