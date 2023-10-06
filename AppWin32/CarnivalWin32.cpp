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

namespace GUI {

CarnivalWin32::CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window) :
	Carnival(r_window),
	m_hwnd(hwnd) {}

CarnivalWin32::~CarnivalWin32() {}

void CarnivalWin32::run() {
	// 创建并唤起 默认入口 Activity。
	m_runningActivity = this->createActivity(Activity::ID_DefaultEntry);
	m_runningActivity->start(*this);

	// 核心循环。
	while (handleTransition()) {
		if (m_runningActivity->isIndependent()) {
			m_runningActivity->runIndependently();
		}
		else {
			runTheActivity();
		}
	}

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

void CarnivalWin32::showMessageBox(std::string_view title, std::string_view text, MBInfo info) const {
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
	MessageBoxA(m_hwnd, text.data(), title.data(), type);
	return;
}

bool CarnivalWin32::isEnabledClose() const {
	return Callbacks::ButtonEnabled_Close;
}

bool CarnivalWin32::isEnabledResize() const {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return style & WS_SIZEBOX;
}

bool CarnivalWin32::isEnabledMinimize() const {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	return style & WS_MINIMIZEBOX;
}

void CarnivalWin32::enableClose(bool enabled) const {
	HMENU hmenu = GetSystemMenu(m_hwnd, FALSE);
	// 修改成功即保存。
	if (EnableMenuItem(hmenu, SC_CLOSE, enabled ? MF_ENABLED : MF_GRAYED) != -1)
		Callbacks::ButtonEnabled_Close = enabled;
	return;
}

void CarnivalWin32::enableResize(bool enabled) const {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	if (enabled) {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style | (WS_SIZEBOX | WS_MAXIMIZEBOX));
	}
	else {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style & (~(WS_SIZEBOX | WS_MAXIMIZEBOX)));
		ShowWindow(m_hwnd, SW_RESTORE); // 必须在 禁用最大化 的同时 取消掉 现有的最大化。
	}
	return;
}

void CarnivalWin32::enableMinimize(bool enabled) const {
	LONG_PTR style = GetWindowLongPtrW(m_hwnd, GWL_STYLE);
	if (enabled) {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style | WS_MINIMIZEBOX);
	}
	else {
		SetWindowLongPtrW(m_hwnd, GWL_STYLE, style & (~WS_MINIMIZEBOX));
		ShowWindow(m_hwnd, SW_RESTORE); // 必须在 禁用最小化 的同时 取消掉 现有的最小化。
	}
	return;
}

void CarnivalWin32::setFullwindow(bool full) {}

void CarnivalWin32::setFullscreen(bool full) {}

void CarnivalWin32::runTheActivity() {
	// 初始化。
	m_keepRunning = true;

	// 用于每帧的更新时间。
	sf::Clock clk;
	float dt = 0.0f;

	POINT oldsize{ 0 };
	{
		RECT clientrect{ 0 };
		GetClientRect(m_hwnd, &clientrect);
		oldsize = { clientrect.right, clientrect.bottom };
	}
	// 修改 Idle 回调，要把旧的回调保存下来。
	std::function<void()> oldIdle = Callbacks::OnIdle;
	Callbacks::OnIdle = [this, &dt, &clk, &oldsize]() -> void {
		sf::Event evt;

		RECT clientrect{ 0 };
		GetClientRect(m_hwnd, &clientrect);
		if (oldsize.x != clientrect.right || oldsize.y != clientrect.bottom) {
			oldsize = { clientrect.right, clientrect.bottom };
			ref_window->setSize({ (unsigned int)clientrect.right, (unsigned int)clientrect.bottom });
			evt.type = sf::Event::Resized;
			evt.size.width = oldsize.x;
			evt.size.height = oldsize.y;
			m_runningActivity->handleEvent(evt);
		}

		while (ref_window->pollEvent(evt)) {
			m_runningActivity->handleEvent(evt);
		}
		dt = clk.restart().asSeconds();
		m_runningActivity->update(dt);
	};

	// 主循环。
	MSG msg{ 0 };
	sf::Event evt;
	clk.restart();
	while (m_keepRunning) {
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		while (ref_window->pollEvent(evt)) {
			m_runningActivity->handleEvent(evt);
		}

		dt = clk.restart().asSeconds();
		m_runningActivity->update(dt);
	}

	// 恢复旧的 Idle 回调。
	Callbacks::OnIdle = oldIdle;
	return;
}

} // namespace GUI
