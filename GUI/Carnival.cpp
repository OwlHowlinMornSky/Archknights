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
#include "Carnival.h"

#include "TempGuard.h"
#include "Callbacks.h"
#include <assert.h>

namespace {

const char g_str_winExcept[] = "Window Exception:\n";
const char g_str_error[] = "Archnights: Error";
const char g_str_unknown[] = "Unknown Exception.";

} // namespace

namespace GUI {

// 唯一实例
std::unique_ptr<Carnival> Carnival::s_instance(nullptr);

Carnival& Carnival::instance() noexcept {
	assert(s_instance != nullptr);
	return *s_instance;
}

void Carnival::drop() noexcept {
	return s_instance.reset();
}

GUI::Carnival::Carnival(bool mutipleWindows) :
	m_mutipleWindows(mutipleWindows) {}

GUI::Carnival::~Carnival() noexcept {
	m_wnds.clear();
	return;
}

void Carnival::run() noexcept {
	ohms::TempGuard<std::function<void()>> idleGuard(GUI::OnIdle);
	ohms::TempGuard<std::function<void(bool)>> syslpGuard(GUI::OnSystemLoop);
	sf::Time dt;
	if (m_mutipleWindows) {
		idleGuard = std::bind(&Carnival::onIdle, this);
		syslpGuard = std::bind(&Carnival::onSystemLoop, this, std::placeholders::_1);
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
				systemMessagePump();
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
	else {
		idleGuard = std::bind(&Carnival::onIdleSingle, this);
		syslpGuard = std::bind(&Carnival::onSystemLoopSingle, this, std::placeholders::_1);
		m_clk.restart();
		try {
			while (!m_singleWnd->isWaitingForStop()) {
				m_singleWnd->handleEvent();
				dt = m_clk.restart();
				m_singleWnd->update(dt);
				systemMessagePump();
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

bool Carnival::pushWindow(std::unique_ptr<Window>&& wnd) {
	// 确保窗口已经 Create 并且含有有效 Activity。
	assert(wnd->available());
	if (!wnd->available())
		return false;
	if (m_mutipleWindows) {
		m_wnds.push_front(std::move(wnd));
	}
	else {
		// 如果已经有窗口就失败。
		assert(m_singleWnd == nullptr);
		if (m_singleWnd != nullptr)
			return false;
		m_singleWnd = std::move(wnd);
	}
	return true;
}

void Carnival::removeStoppedWindows() noexcept {
	std::list<std::unique_ptr<Window>>::iterator
		i = m_wnds.begin(),
		n = m_wnds.end();
	while (i != n) {
		if ((*i)->isWaitingForStop()) i = m_wnds.erase(i);
		else ++i;
	}
	return;
}

void Carnival::onIdle() {
	if (!m_wnds.empty()) {
		for (const std::unique_ptr<Window>& wnd : m_wnds) {
			wnd->handleEvent();
			wnd->checkSizeInSystemLoop();
		}
		sf::Time dt = m_clk.restart();
		for (const std::unique_ptr<Window>& wnd : m_wnds) {
			wnd->update(dt);
		}
		removeStoppedWindows();
	}
	return;
}

void Carnival::onIdleSingle() {
	if (!m_singleWnd->isWaitingForStop()) {
		m_singleWnd->handleEvent();
		m_singleWnd->checkSizeInSystemLoop();
		sf::Time dt = m_clk.restart();
		m_singleWnd->update(dt);
	}
	else {
		m_singleWnd->Close();
	}
	return;
}

void Carnival::onSystemLoop(bool enter) {
	for (const std::unique_ptr<Window>& wnd : m_wnds) {
		wnd->onSystemLoop(enter);
	}
	return;
}

void Carnival::onSystemLoopSingle(bool enter) {
	return m_singleWnd->onSystemLoop(enter);
}

} // namespace GUI

#ifdef _WIN32
#include "CarnivalWin32.h"
void GUI::Carnival::setup(bool mutipleWindows) noexcept {
	s_instance.reset(new CarnivalWin32(mutipleWindows));
	return;
}
#endif
