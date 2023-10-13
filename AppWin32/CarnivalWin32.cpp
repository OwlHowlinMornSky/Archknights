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
*    实现了 ICarnival 实例之一 CarnivalWin32。
*/
// SFML 的东西必须 在 Windows的 之前。
#include <SFML/Graphics/RenderWindow.hpp>
#include "framework.h"

#include "CarnivalWin32.h"

#include <iostream>

#include "../Global/TempGuard.h"
#include "../GUI/Callbacks.h"
#include "../GUI_Activities/Factory.h"


namespace {

class exception_sfml_window_init final :
	public std::exception {
public:
	_NODISCARD virtual char const* what() const {
		return "Window Initialization Failed!";
	}
};

}

namespace GUI {

CarnivalWin32::CarnivalWin32(HWND hWnd) :
	m_oldsize({ 0 }),
	m_hwnd(hWnd) {
	m_renderWindow->create(hWnd);
	if (!m_renderWindow->isOpen()) {
		throw ::exception_sfml_window_init();
	}
	return;
}

CarnivalWin32::~CarnivalWin32() {
	m_renderWindow->close();
	return;
}

void CarnivalWin32::systemMessagePump(bool callerDoWantToHandleThem) const noexcept {
	MSG msg{ 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	if (!callerDoWantToHandleThem) {
		try {
			sf::Event evt;
			while (m_renderWindow->pollEvent(evt));
		}
		catch (...) {
			;
		}
	}
	return;
}

void CarnivalWin32::systemShowMessageBox(std::string_view title, std::string_view text, MBInfo info) const noexcept {
	UINT type{ 0 };
	// 根据自定的 info 类型 决定系统层的数值。
	switch (info) {
	case MBInfo::Info:
		type = MB_ICONINFORMATION;
		break;
	case MBInfo::Error:
		type = MB_ICONERROR;
		break;
	default:
		break;
	}
	try {
		MessageBoxA(m_hwnd, text.data(), title.data(), type);
	}
	catch (...) {
		;
	}
	return;
}

void CarnivalWin32::windowSetCloseEnabled(bool enabled) noexcept {
	HMENU hmenu = GetSystemMenu(m_hwnd, FALSE);
	// 修改成功即保存。
	if (EnableMenuItem(hmenu, SC_CLOSE, enabled ? MF_ENABLED : MF_GRAYED) != -1)
		Callbacks::ButtonEnabled_Close = enabled;
	return;
}

void CarnivalWin32::windowSetResizeEnabled(bool enabled) noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	SetLastError(0);
	LONG_PTR res = 0;
	if (enabled) {
		res = SetWindowLongPtrW(m_hwnd, GWL_STYLE, style | (WS_SIZEBOX | WS_MAXIMIZEBOX));
	}
	else {
		res = SetWindowLongPtrW(m_hwnd, GWL_STYLE, style & (~(WS_SIZEBOX | WS_MAXIMIZEBOX)));
		ShowWindow(m_hwnd, SW_RESTORE); // 必须在 禁用最大化 的同时 取消掉 现有的最大化。
	}
	if (res != 0 || GetLastError() == 0)
		m_enabledResize = enabled;
	return;
}

void CarnivalWin32::windowSetMinimizeEnabled(bool enabled) noexcept {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	SetLastError(0);
	LONG_PTR res = 0;
	if (enabled) {
		res = SetWindowLongPtrW(m_hwnd, GWL_STYLE, style | WS_MINIMIZEBOX);
	}
	else {
		res = SetWindowLongPtrW(m_hwnd, GWL_STYLE, style & (~WS_MINIMIZEBOX));
		ShowWindow(m_hwnd, SW_RESTORE); // 必须在 禁用最小化 的同时 取消掉 现有的最小化。
	}
	if (res != 0 || GetLastError() == 0)
		m_enabledMinimize = enabled;
	return;
}

bool CarnivalWin32::windowSetBorderless() noexcept {
	if (m_windowType == WindowType::Borderless)
		return true;
	if (m_windowType == WindowType::Windowed) {
		m_sizeBefore = m_renderWindow->getSize();
		m_positionBefore = m_renderWindow->getPosition();
	}
	if (m_windowType == WindowType::Fullscreen) {
		ChangeDisplaySettingsW(NULL, 0);
	}
	SetWindowLongPtrW(m_hwnd, GWL_STYLE,
					  WS_OVERLAPPED | WS_SYSMENU | WS_BORDER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowPos(m_hwnd, HWND_TOP, 0, 0,
				 GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
	ShowWindow(m_hwnd, SW_SHOW);

	m_windowType = WindowType::Borderless;
	return true;
}

bool CarnivalWin32::windowSetFullscreen(sf::VideoMode mode) noexcept {
	if (m_windowType == WindowType::Fullscreen && m_lastMode == mode)
		return true;
	if (m_windowType == WindowType::Windowed) {
		m_sizeBefore = m_renderWindow->getSize();
		m_positionBefore = m_renderWindow->getPosition();
	}

	DEVMODE devMode{ 0 };
	devMode.dmSize = sizeof(devMode);
	devMode.dmPelsWidth = mode.width;
	devMode.dmPelsHeight = mode.height;
	devMode.dmBitsPerPel = mode.bitsPerPixel;
	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	// Apply fullscreen mode
	if (ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
		systemShowMessageBox("", "Failed to change display mode for fullscreen", GUI::MBInfo::Error);
		return false;
	}

	SetWindowLongPtrW(m_hwnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowPos(m_hwnd, HWND_TOP,
				 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
				 SWP_FRAMECHANGED);

	// Resize the window so that it fits the entire screen
	SetWindowPos(m_hwnd, HWND_TOP, 0, 0, static_cast<int>(mode.width), static_cast<int>(mode.height), SWP_FRAMECHANGED);
	ShowWindow(m_hwnd, SW_SHOW);

	m_lastMode = mode;
	m_windowType = WindowType::Fullscreen;
	return true;
}

void CarnivalWin32::windowSetWindowed() noexcept {
	if (m_windowType == WindowType::Windowed)
		return;
	if (m_windowType == WindowType::Fullscreen) {
		ChangeDisplaySettingsW(NULL, 0);
	}
	SetWindowLongPtrW(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	ShowWindow(m_hwnd, SW_SHOW);

	windowSetMinimizeEnabled(m_enabledMinimize);
	windowSetResizeEnabled(m_enabledResize);
	m_renderWindow->setSize(m_sizeBefore);
	m_renderWindow->setPosition(m_positionBefore);

	m_windowType = WindowType::Windowed;
	return;
}

void CarnivalWin32::systemResetSleepCounter() noexcept {
	SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	return;
}

void CarnivalWin32::systemTrySetSleepEnabled(bool allowSleep) noexcept {
	if (allowSleep) {
		SetThreadExecutionState(ES_CONTINUOUS);
	}
	else {
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	}
	return;
}

void CarnivalWin32::runTheActivity() {
	// 初始化。
	ohms::TempGuard<std::function<void()>> idleGuard(Callbacks::OnIdle);
	idleGuard = std::bind(&CarnivalWin32::OnIdle, this);
	MSG msg{ 0 };
	sf::Event evt;
	m_keepRunning = true;
	m_clk.restart();
	// 主循环。
	while (m_keepRunning) {
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		while (m_renderWindow->pollEvent(evt)) {
			switch (evt.type) {
			case sf::Event::Closed:
				meActivitySetTransition(Transition::Exit);
				meDependentActivityStopRunning();
				break;
			case sf::Event::Resized:
			{
				RECT clientrect{ 0 };
				m_renderWindow->setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)evt.size.width, (float)evt.size.height)));
				GetClientRect(m_hwnd, &clientrect);
				if (m_oldsize.x != clientrect.right || m_oldsize.y != clientrect.bottom)
					m_oldsize = { clientrect.right, clientrect.bottom };
			}
			[[fallthrough]];
			default:
				m_runningActivity->handleEvent(evt);
				break;
			}
		}
		m_runningActivity->update(*m_renderWindow, m_clk.restart());
	}
	return;
}

std::unique_ptr<IActivity> CarnivalWin32::createActivity(uint32_t id) const noexcept {
	return Activity::Factory::CreateActivity(id);
}

void CarnivalWin32::OnIdle() {
	RECT clientrect{ 0 };
	GetClientRect(m_hwnd, &clientrect);
	sf::Event evt;
	if (m_oldsize.x != clientrect.right || m_oldsize.y != clientrect.bottom) {
		m_oldsize = { clientrect.right, clientrect.bottom };
		m_renderWindow->setSize({ (unsigned int)m_oldsize.x, (unsigned int)m_oldsize.y });
		if (m_enableFullResizeMessage) {
			evt.type = sf::Event::Resized;
			evt.size.width = m_oldsize.x;
			evt.size.height = m_oldsize.y;
			m_renderWindow->setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)evt.size.width, (float)evt.size.height)));
			m_runningActivity->handleEvent(evt);
		}
	}
	while (m_renderWindow->pollEvent(evt)) {
		switch (evt.type) {
		case sf::Event::Closed:
			meActivitySetTransition(Transition::Exit);
			meDependentActivityStopRunning();
			break;
		default:
			m_runningActivity->handleEvent(evt);
			break;
		}
	}
	m_runningActivity->update(*m_renderWindow, m_clk.restart());
	// 如果Activity不再运行则强行打破sysloop。
	if (!m_keepRunning) {
		POINT p;
		GetCursorPos(&p);
		PostMessageW(m_hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(p.x, p.y));
	}
}

} // namespace GUI
