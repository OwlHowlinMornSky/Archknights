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
*/
#include "WindowWin32.h"

#include "Carnival.h"
#include "Activity.h"

#include <strsafe.h>
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace {

/**
 * @brief 计时器回调。
*/
void CALLBACK MyTimerProc(HWND hWnd, UINT message, UINT_PTR nTimerid, DWORD systemTick) {
	return GUI::OnIdle();
}

/**
 * @brief 窗口回调。
*/
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static UINT_PTR l_timerID = 0;
	switch (message) {
	// 必须拦截该消息，因为 DefWindowProc 接受后会销毁窗口。
	// 窗口是需要在 main 里最后销毁的。
	// Don't forward this to DefWindowProc because it will destroy the window.
	case WM_CLOSE:
		break;

	case WM_GETMINMAXINFO:
	{
		// 计算窗口最小尺寸。
		// 保证的是 客户区 最小为 120 * 90。
		// Compute the minimum size of the window.
		const CREATESTRUCT& info = *(CREATESTRUCT*)lParam;
		RECT rect = { 0, 0, 120, 90 };
		AdjustWindowRectEx(&rect, info.style, info.hMenu != NULL, info.dwExStyle);
		((MINMAXINFO*)lParam)->ptMinTrackSize = { rect.right - rect.left, rect.bottom - rect.top };
		break;
	}

	// 拦截三大金刚键的消息。因为按下不松手会直接卡死。
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

	// 触发三大金刚键的功能。
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
		{
			HMENU hmenu = GetSystemMenu(hWnd, FALSE);
			MENUITEMINFOW info = { 0 };
			info.cbSize = sizeof(info);
			info.fMask = MIIM_STATE;
			GetMenuItemInfoW(hmenu, SC_CLOSE, FALSE, &info);
			if (!(info.fState & MFS_GRAYED))
				PostMessageW(hWnd, WM_SYSCOMMAND, SC_CLOSE, lParam);
			break;
		}
		default:
			return DefWindowProcW(hWnd, message, wParam, lParam);
		}
		break;

	// 右键也要拦截，因为按下不松手也会卡死。
	case WM_NCRBUTTONDOWN:
		break;

	// 松开右键即打开系统菜单。
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
		if (l_timerID) break;
		l_timerID = SetTimer(0, 0, 10, (TIMERPROC)MyTimerProc);
		GUI::OnSystemLoop(true);
		break;

	case WM_EXITMENULOOP:
		if (l_timerID) KillTimer(0, l_timerID);
		l_timerID = 0;
		GUI::OnSystemLoop(false);
		break;

	case WM_ENTERSIZEMOVE:
		if (l_timerID) break;
		l_timerID = SetTimer(0, 0, 10, (TIMERPROC)MyTimerProc);
		GUI::OnSystemLoop(true);
		break;

	case WM_EXITSIZEMOVE:
		if (l_timerID) KillTimer(0, l_timerID);
		l_timerID = 0;
		GUI::OnSystemLoop(false);
		break;

	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

WCHAR szWindowClass[] = L"WndClass.OHMS.Archknights"; // 窗口类的名称。
WCHAR szTitle[] = L"Archknights"; // 窗口的 初始 名称。

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
		0, NULL
	);

	lpDisplayBuf = (LPVOID)LocalAlloc(
		LMEM_ZEROINIT,
		(static_cast<SIZE_T>(lstrlenW((LPCWSTR)lpMsgBuf)) +
		 lstrlenW((LPCWSTR)lpszFunction) + 50) * sizeof(WCHAR)
	);
	if (lpDisplayBuf) {
		StringCchPrintfW(
			(LPWSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(WCHAR),
			L"\'%s\' failed with error code \'%d\': %s",
			lpszFunction, dw, (LPCWSTR)lpMsgBuf
		);
		MessageBoxW(NULL, (LPCWSTR)lpDisplayBuf, L"Error", MB_ICONERROR);
	}
	else {
		MessageBoxW(NULL, (LPCWSTR)lpMsgBuf, L"Error", MB_ICONERROR);
	}

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	return;
}

bool g_wndClsRegistered = false;

bool MyRegisterClass() noexcept {
	WNDCLASSEX wcex{ 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);
	// 没必要重绘。
	wcex.style = NULL;// CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ::MyWndProc;
	wcex.hInstance = GetModuleHandleW(NULL);
	// ！！！未完成！！！图标激情制作中（雾
	//wcex.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_ICON0));
	wcex.hIconSm = wcex.hIcon;
	wcex.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszClassName = ::szWindowClass;

	if (RegisterClassExW(&wcex) == 0) {
		WinCheckError(L"RegisterClassEx");
		return false;
	}
	g_wndClsRegistered = true;
	return true;
}

bool MyCreateWindow(int nCmdShow, HWND& hWnd) noexcept {
	HWND res = CreateWindowExW(
		WS_EX_APPWINDOW, ::szWindowClass, ::szTitle,
		WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, 420, 420,
		nullptr, nullptr, GetModuleHandleW(NULL), nullptr
	);
	if (res == NULL) {
		WinCheckError(L"CreateWindow");
		return false;
	}
	ShowWindow(res, nCmdShow);
	UpdateWindow(res);
	hWnd = res;
	return true;
}

} // namespace

namespace GUI {

WindowWin32::WindowWin32() :
	m_hwnd(0) {}

WindowWin32::~WindowWin32() noexcept {
	Close();
	return;
}

bool WindowWin32::Create(int nCmdShow) noexcept {
	if (!g_wndClsRegistered)
		if (!::MyRegisterClass())
			return false;
	if (!::MyCreateWindow(nCmdShow, m_hwnd))
		return false;
	RenderWindow::create(m_hwnd);
	if (!isOpen())
		return false;
	m_created = true;
	return true;
}

bool GUI::WindowWin32::Create() noexcept {
	return Create(SW_SHOWNORMAL);
}

void WindowWin32::Close() noexcept {
	if (m_activity != nullptr) {
		m_activity->stop();
		m_activity.reset();
	}
	RenderWindow::close();
	if (m_hwnd) {
		DestroyWindow(m_hwnd);
		m_hwnd = 0;
	}
	return;
}

sf::Vector2u WindowWin32::getClientSize() const noexcept {
	RECT rect{ 0 };
	GetClientRect(m_hwnd, &rect);
	return sf::Vector2u(
		static_cast<unsigned int>(rect.right - rect.left),
		static_cast<unsigned int>(rect.bottom - rect.top)
	);
}

void WindowWin32::setCloseEnabled(bool enabled) noexcept {
	HMENU hmenu = GetSystemMenu(m_hwnd, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, enabled ? MF_ENABLED : MF_GRAYED);
	return;
}

void WindowWin32::setResizeEnabled(bool enabled) noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	style = enabled ?
		(style | (WS_SIZEBOX | WS_MAXIMIZEBOX)) :
		(style & (~(WS_SIZEBOX | WS_MAXIMIZEBOX)));
	SetWindowLongPtrW(m_hwnd, GWL_STYLE, style);
	if (!enabled) // 必须在禁用最大化的同时取消最大化。
		ShowWindow(m_hwnd, SW_RESTORE);
	return;
}

void WindowWin32::setMinimizeEnabled(bool enabled) noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	style = enabled ? (style | WS_MINIMIZEBOX) : (style & ~WS_MINIMIZEBOX);
	SetWindowLongPtrW(m_hwnd, GWL_STYLE, style);
	if (!enabled) // 必须在禁用最小化的同时取消最小化。
		ShowWindow(m_hwnd, SW_RESTORE);
	return;
}

bool WindowWin32::isCloseEnabled() const noexcept {
	HMENU hmenu = GetSystemMenu(m_hwnd, FALSE);
	MENUITEMINFOW info = { 0 };
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	GetMenuItemInfoW(hmenu, SC_CLOSE, FALSE, &info);
	return !(info.fState & MFS_GRAYED);
}

bool WindowWin32::isResizeEnabled() const noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return (style & WS_MAXIMIZEBOX);
}

bool WindowWin32::isMinimizeEnabled() const noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return (style & WS_MINIMIZEBOX);
}

void WindowWin32::showMessageBox(std::string_view title, std::string_view text) const noexcept {
	MessageBoxA(m_hwnd, text.data(), title.data(), MB_ICONINFORMATION);
	return;
}

void WindowWin32::showMessageBox(std::wstring_view title, std::wstring_view text) const noexcept {
	MessageBoxW(m_hwnd, text.data(), title.data(), MB_ICONINFORMATION);
	return;
}

void WindowWin32::setWindowed() noexcept {
	if (m_windowStatus == WindowStatus::Windowed)
		return;
	if (m_windowStatus == WindowStatus::Fullscreen) {
		ChangeDisplaySettingsW(NULL, 0);
	}
	SetWindowLongPtrW(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	ShowWindow(m_hwnd, SW_SHOW);

	m_windowStatus = WindowStatus::Windowed;
	setSize(m_lastSizeWhenWindowed);
	setPosition(m_lastPositionWhenWindowed);
	return;
}

bool WindowWin32::setBorderless() noexcept {
	if (m_windowStatus == WindowStatus::Borderless)
		return true;
	if (m_windowStatus == WindowStatus::Windowed) {
		m_lastSizeWhenWindowed = getSize();
		m_lastPositionWhenWindowed = getPosition();
	}
	if (m_windowStatus == WindowStatus::Fullscreen) {
		ChangeDisplaySettingsW(NULL, 0);
	}
	SetWindowLongPtrW(m_hwnd, GWL_STYLE,
					  WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowPos(m_hwnd, HWND_TOP, 0, 0,
				 GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
	ShowWindow(m_hwnd, SW_SHOW);

	m_windowStatus = WindowStatus::Borderless;
	return true;
}

bool WindowWin32::setFullscreen(sf::VideoMode mode) noexcept {
	if (m_windowStatus == WindowStatus::Fullscreen)
		return true;
	if (m_windowStatus == WindowStatus::Windowed) {
		m_lastSizeWhenWindowed = getSize();
		m_lastPositionWhenWindowed = getPosition();
	}

	DEVMODE devMode{ 0 };
	devMode.dmSize = sizeof(devMode);
	devMode.dmPelsWidth = mode.width;
	devMode.dmPelsHeight = mode.height;
	devMode.dmBitsPerPel = mode.bitsPerPixel;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	// Apply fullscreen mode
	if (ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
		showMessageBox(szTitle, L"Failed to change display mode for fullscreen");
		return false;
	}

	SetWindowLongPtrW(m_hwnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowPos(
		m_hwnd, HWND_TOP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		SWP_FRAMECHANGED
	);

	// Resize the window so that it fits the entire screen
	SetWindowPos(
		m_hwnd, HWND_TOP,
		0, 0,
		static_cast<int>(mode.width),
		static_cast<int>(mode.height),
		SWP_FRAMECHANGED
	);
	ShowWindow(m_hwnd, SW_SHOW);

	m_windowStatus = WindowStatus::Fullscreen;
	return true;
}

void WindowWin32::checkSizeInSystemLoop() noexcept {
	RECT rect{ 0 };
	GetClientRect(m_hwnd, &rect);
	sf::Vector2u size(
		static_cast<unsigned int>(rect.right - rect.left),
		static_cast<unsigned int>(rect.bottom - rect.top)
	);
	if (m_oldSize != size) {
		m_oldSize = size;
		if (m_sizingAsSized) {
			setSize(size);
			if (m_activity != nullptr) {
				sf::Event evt;
				evt.type = sf::Event::Resized;
				evt.size.width = size.x;
				evt.size.height = size.y;
				m_activity->handleEvent(evt);
			}
		}
		else {
			RenderWindow::setSize(size);
		}
	}
	return;
}

} // namespace GUI
