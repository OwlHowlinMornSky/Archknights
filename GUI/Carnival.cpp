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
#include <assert.h>

namespace {

void fEmpty_vv() noexcept {}
void fEmpty_vb(bool) noexcept {}

} // namespace

namespace GUI {

std::function<void()> OnIdle(&::fEmpty_vv);
std::function<void(bool)> OnSystemLoop(&::fEmpty_vb);

std::unique_ptr<Carnival> Carnival::s_instance(nullptr);

Carnival& Carnival::instance() noexcept {
	assert(s_instance != nullptr);
	return *s_instance;
}

void Carnival::drop() noexcept {
	return s_instance.reset();
}

void Carnival::run() noexcept {
	ohms::TempGuard<std::function<void()>> idleGuard(GUI::OnIdle);
	idleGuard = std::bind(&Carnival::onIdle, this);
	ohms::TempGuard<std::function<void(bool)>> syslpGuard(GUI::OnSystemLoop);
	syslpGuard = std::bind(&Carnival::onSystemLoop, this, std::placeholders::_1);
	sf::Time dt;
	m_clk.restart();
	while (!m_wnds.empty()) {
		try {
			while (!m_wnds.empty()) {
				dt = m_clk.restart();
				for (const std::unique_ptr<Window>& wnd : m_wnds) {
					wnd->update(dt);
				}
				removeStoppedWindows();
				systemMessagePump();
			}
			systemMessagePump();
		}
		catch (std::exception& e) {
			std::string err("Window Exception:\n");
			err.append(e.what());
			showErrorMessageBox("Archnights: Error", err);
		}
		catch (...) {
			std::string err("Window Exception:\n");
			err.append("Unknown Exception.");
			showErrorMessageBox("Archnights: Error", err);
		}
	}
	return;
}

void Carnival::addWindow(std::unique_ptr<Window>&& wnd) {
	assert(wnd->m_created);
	return m_wnds.push_front(std::move(wnd));
}

void Carnival::removeStoppedWindows() noexcept {
	std::list<std::unique_ptr<Window>>::iterator i = m_wnds.begin(), n = m_wnds.end();
	while (i != n) {
		if ((*i)->isWaitingForStop()) i = m_wnds.erase(i);
		else ++i;
	}
	return;
}

void Carnival::onIdle() {
	if (!m_wnds.empty()) {
		sf::Time dt = m_clk.restart();
		for (const std::unique_ptr<Window>& wnd : m_wnds) {
			wnd->checkSizeInSystemLoop();
			wnd->update(dt);
		}
		removeStoppedWindows();
	}
	return;
}

void Carnival::onSystemLoop(bool enter) {
	for (const std::unique_ptr<Window>& wnd : m_wnds) {
		wnd->OnSystemLoop(enter);
	}
	return;
}

} // namespace GUI

#ifdef _WIN32
#include "CarnivalWin32.h"
void GUI::Carnival::initialize() noexcept {
	s_instance.reset(new CarnivalWin32);
	return;
}
#endif
