/*
*    Mystery Engine
*
*    Copyright (C) 2023-2024  Tyler Parret True
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
#include "CarnivalWin32.h"

#include <functional>
#include <assert.h>
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <MysteryEngine/Core/TempGuard.h>

#include "WindowWin32.h"
#include "Callbacks.h"

namespace {

const char g_str_winExcept[] = "Window Exception:\n";
const char g_str_error[] = "Mystery Engine: Error";
const char g_str_unknown[] = "Unknown Exception.";

inline void g_systemMessagePump() noexcept {
	MSG msg{ 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return;
}

} // namespace

namespace ME {

CarnivalWin32::CarnivalWin32(bool mutipleWindows) :
	Carnival(mutipleWindows) {}

void CarnivalWin32::Run() noexcept {
	// 临时替换回调
	ME::TempGuard<std::function<void()>> idleGuard(ME::OnIdle);
	ME::TempGuard<std::function<void(bool)>> syslpGuard(ME::OnSystemLoop);
	sf::Time dt;
	if (m_mutipleWindows) { // 多窗口模式
		idleGuard = std::bind(&CarnivalWin32::onIdle, this);
		syslpGuard = std::bind(&CarnivalWin32::onSystemLoop, this, std::placeholders::_1);
		m_clk.restart();
		try {
			while (!m_wnds.empty()) {
				for (const std::unique_ptr<Window>& wnd : m_wnds) {
					wnd->handleEvent();
				}
				dt = m_clk.restart();
				for (const std::unique_ptr<Window>& wnd : m_wnds) {
					wnd->update(dt);
				}
				removeStoppedWindows();
				g_systemMessagePump(); // systemMessagePump();
			}
		}
		catch (std::exception& e) {
			std::string err(g_str_winExcept);
			err.append(e.what());
			showErrorMessageBox(g_str_error, err);
		}
		catch (...) {
			std::string err(g_str_winExcept);
			err.append(g_str_unknown);
			showErrorMessageBox(g_str_error, err);
		}
	}
	else { // 单窗口模式
		idleGuard = std::bind(&CarnivalWin32::onIdleSingle, this);
		syslpGuard = std::bind(&CarnivalWin32::onSystemLoopSingle, this, std::placeholders::_1);
		m_clk.restart();
		try {
			while (!m_singleWnd->isWaitingForStop()) {
				m_singleWnd->handleEvent();
				dt = m_clk.restart();
				m_singleWnd->update(dt);
				g_systemMessagePump(); // systemMessagePump();
			}
			m_singleWnd.reset();
		}
		catch (std::exception& e) {
			std::string err(g_str_winExcept);
			err.append(e.what());
			showErrorMessageBox(g_str_error, err);
		}
		catch (...) {
			std::string err(g_str_winExcept);
			err.append(g_str_unknown);
			showErrorMessageBox(g_str_error, err);
		}
	}
	return;
}

bool CarnivalWin32::emplaceWindow(std::unique_ptr<Activity>&& activity, bool foreground) {
	assert(activity != nullptr);
	if (activity == nullptr)
		return false;
	int nCmdShow = foreground ? SW_SHOWNORMAL : SW_SHOWNOACTIVATE;
	if (m_mutipleWindows) {
		std::unique_ptr<WindowWin32> wnd = std::make_unique<WindowWin32>();
		if (!wnd->Create(nCmdShow))
			return false;
		wnd->changeActivity(std::move(activity));
		m_wnds.push_front(std::move(wnd));
	}
	else {
		assert(m_singleWnd == nullptr);
		if (m_singleWnd != nullptr)
			return false;
		std::unique_ptr<WindowWin32> wnd = std::make_unique<WindowWin32>();
		if (!wnd->Create(nCmdShow))
			return false;
		wnd->changeActivity(std::move(activity));
		m_singleWnd = std::move(wnd);
	}
	return true;
}

void CarnivalWin32::showErrorMessageBox(std::string_view title, std::string_view text) const noexcept {
	MessageBoxA(NULL, text.data(), title.data(), MB_ICONERROR);
	return;
}

void ME::CarnivalWin32::resetSleepCounter() noexcept {
	SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	return;
}

void CarnivalWin32::setSleepEnabled(bool allowSleep) noexcept {
	if (allowSleep)
		SetThreadExecutionState(ES_CONTINUOUS);
	else
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
	return;
}

void CarnivalWin32::systemMessagePump() const noexcept {
	MSG msg{ 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return;
}

} // namespace GUI
