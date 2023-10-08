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
#include <SFML/Graphics.hpp>
#include "framework.h"

#include "CarnivalWin32.h"

#include "../GUI/Callbacks.h"
#include "../GUI_Activities/ActivityIDs.h"
#include "../GUI_Activities/Factory.h"

namespace GUI {

CarnivalWin32::CarnivalWin32(HWND hWnd) :
	m_hwnd(hWnd),
	m_windowType(WindowType::Windowed),
	m_enabledResize(true),
	m_enabledMinimize(true) {
	m_renderWindow = std::make_unique<sf::RenderWindow>();
	m_renderWindow->create(hWnd);
	if (!m_renderWindow->isOpen()) {
		throw std::exception("Window Initialization Failed!");
	}
	return;
}

CarnivalWin32::~CarnivalWin32() {
	m_renderWindow->close();
	m_renderWindow.reset();
	return;
}

void CarnivalWin32::run() noexcept {
	// 创建并唤起 默认入口 Activity。
	m_runningActivity = this->createActivity(Activity::ID_DefaultEntry);
	if (m_runningActivity == nullptr) {
		return;
	}
	try {
		m_runningActivity->start(*this);
	}
	catch (...) {
		return;
	}

	std::function<void()> oldEnter = Callbacks::OnEnterSysloop;
	Callbacks::OnEnterSysloop = [this]()-> void {
		m_runningActivity->onEnterSysloop();
	};
	std::function<void()> oldExit = Callbacks::OnExitSysloop;
	Callbacks::OnExitSysloop = [this]()-> void {
		m_runningActivity->onExitSysloop();
	};

	// 核心循环。
	while (handleTransition()) {
		try {
			if (m_runningActivity->isIndependent()) {
				m_runningActivity->runIndependently();
			}
			else {
				runTheActivity();
			}
		}
		catch (std::exception& e) {
			std::string err("Activity Exception:\n");
			err.append(e.what());
			this->systemShowMessageBox("Archnights: Error", err, MBInfo::Error);
			this->meActivitySetTransition(Transition::Pop);
		}
		catch (...) {
			std::string err("Activity Exception:\n");
			err.append("Unknown Exception.");
			this->systemShowMessageBox("Archnights: Error", err, MBInfo::Error);
			this->meActivitySetTransition(Transition::Pop);
		}
	}

	Callbacks::OnExitSysloop = oldExit;
	Callbacks::OnEnterSysloop = oldEnter;

	// 退出后 清空 Activity 栈。
	while (!m_activityStack.empty()) {
		m_activityStack.pop();
	}
	// 终止所有 Activity。
	for (const auto& i : m_pausedActivities) {
		i.second->stop();
	}
	// 释放所有 Activity。
	m_pausedActivities.clear();
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

void CarnivalWin32::windowSetClientSize(uint32_t w, uint32_t h) noexcept {
	if (m_windowType == WindowType::Windowed) {
		m_renderWindow->setSize(sf::Vector2u(w, h));
		m_renderWindow->setView(sf::View(sf::FloatRect(0.0f, 0.0f,
													   static_cast<float>(w),
													   static_cast<float>(h))));
	}
	return;
}

bool CarnivalWin32::windowIsCloseEnabled() const noexcept {
	return Callbacks::ButtonEnabled_Close;
}

bool CarnivalWin32::windowIsResizeEnabled() const noexcept {
	return m_enabledResize;
	//LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	//return style & WS_SIZEBOX;
}

bool CarnivalWin32::windowIsMinimizeEnabled() const noexcept {
	return m_enabledMinimize;
	//LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	//return style & WS_MINIMIZEBOX;
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
	UpdateWindow(m_hwnd);

	m_windowType = WindowType::Borderless;
	return true;
}

bool CarnivalWin32::windowSetFullscreen(sf::VideoMode mode) noexcept {
	if (m_windowType == WindowType::Fullscreen)
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
		//err() << "Failed to change display mode for fullscreen" << std::endl;
		return false;
	}

	SetWindowLongPtrW(m_hwnd, GWL_STYLE, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	SetWindowPos(m_hwnd, HWND_TOP,
				 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
				 SWP_FRAMECHANGED);

	// Resize the window so that it fits the entire screen
	SetWindowPos(m_hwnd, HWND_TOP, 0, 0, static_cast<int>(mode.width), static_cast<int>(mode.height), SWP_FRAMECHANGED);
	ShowWindow(m_hwnd, SW_SHOW);

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
	UpdateWindow(m_hwnd);

	windowSetMinimizeEnabled(m_enabledMinimize);
	windowSetResizeEnabled(m_enabledResize);
	m_renderWindow->setSize(m_sizeBefore);
	m_renderWindow->setPosition(m_positionBefore);

	m_windowType = WindowType::Windowed;
	return;
}

WindowType CarnivalWin32::windowGetWindowType() const noexcept {
	return m_windowType;
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

void CarnivalWin32::runTheActivity() {
	// 初始化。
	m_keepRunning = true;

	// 用于每帧的更新时间。
	sf::Clock clk;
	// 用于事件循环。
	sf::Event evt;

	POINT oldsize{ 0 };
	RECT clientrect{ 0 };
	GetClientRect(m_hwnd, &clientrect);
	oldsize = { clientrect.right, clientrect.bottom };

	// 修改 Idle 回调，要把旧的回调保存下来。
	std::function<void()> oldIdle = Callbacks::OnIdle;
	Callbacks::OnIdle = [&]() -> void {
		GetClientRect(m_hwnd, &clientrect);
		if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom) {
			oldsize = { clientrect.right, clientrect.bottom };
			m_renderWindow->setSize({ (unsigned int)oldsize.x, (unsigned int)oldsize.y });
			if (m_enableFullResizeMessage) {
				evt.type = sf::Event::Resized;
				evt.size.width = oldsize.x;
				evt.size.height = oldsize.y;
				m_renderWindow->setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)evt.size.width, (float)evt.size.height)));
				m_runningActivity->handleEvent(evt);
			}
		}
		while (m_renderWindow->pollEvent(evt)) {
			m_runningActivity->handleEvent(evt);
		}
		m_runningActivity->update(*m_renderWindow, clk.restart());
	};

	// 主循环。
	MSG msg{ 0 };
	clk.restart();
	while (m_keepRunning) {
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		while (m_renderWindow->pollEvent(evt)) {
			if (evt.type == sf::Event::Resized) {
				m_renderWindow->setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)evt.size.width, (float)evt.size.height)));
				GetClientRect(m_hwnd, &clientrect);
				if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom)
					oldsize = { clientrect.right, clientrect.bottom };
			}
			m_runningActivity->handleEvent(evt);
		}

		m_runningActivity->update(*m_renderWindow, clk.restart());
	}

	// 恢复旧的 Idle 回调。
	Callbacks::OnIdle = oldIdle;
	return;
}

std::unique_ptr<IActivity> CarnivalWin32::createActivity(uint32_t id) const noexcept {
	return Activity::Factory::CreateActivity(id);
}

} // namespace GUI
