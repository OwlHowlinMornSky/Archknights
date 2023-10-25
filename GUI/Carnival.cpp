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

namespace {

void fEmpty_vv() noexcept {}
void fEmpty_vb(bool) noexcept {}

} // namespace

namespace GUI {

std::function<void()> OnIdle(&::fEmpty_vv);
std::function<void(bool)> OnSystemLoop(&::fEmpty_vb);

void Carnival::run() noexcept {
	ohms::TempGuard<std::function<void()>> idleGuard(GUI::OnIdle);
	idleGuard = std::bind(&Carnival::onIdle, this);
	ohms::TempGuard<std::function<void(bool)>> syslpGuard(GUI::OnSystemLoop);
	syslpGuard = std::bind(&Carnival::onSystemLoop, this, std::placeholders::_1);
	sf::Time dt;
	try {
		m_clk.restart();
		while (!m_wnds.empty()) {
			printf_s("!\n");
			dt = m_clk.restart();
			for (const std::unique_ptr<Window>& wnd : m_wnds) {
				wnd->update(dt);
			}
			std::list<std::unique_ptr<Window>>::iterator i, n;
			i = m_wnds.begin();
			n = m_wnds.end();
			printf_s("!!\n");
			while (i != n) {
				printf_s("? %p\n", (*i).get());
				if ((*i)->isWaitingForStop()) {
					printf_s("??");
					i = m_wnds.erase(i);
				}
				else {
					printf_s("???");
					++i;
				}
				printf_s("????");
			}
			printf_s("!!!\n");
			systemMessagePump();
		}
	}
	catch (std::exception& e) {
		std::string err("Activity Exception:\n");
		err.append(e.what());
		showErrorMessageBox("Archnights: Error", err);
	}
	catch (...) {
		std::string err("Activity Exception:\n");
		err.append("Unknown Exception.");
		showErrorMessageBox("Archnights: Error", err);
	}
	return;
}

void Carnival::addWindow(std::unique_ptr<Window>&& wnd) {
	return m_wnds.push_front(std::move(wnd));
}

void Carnival::onIdle() {
	if (!m_wnds.empty()) {
		sf::Time dt = m_clk.restart();
		for (const std::unique_ptr<Window>& wnd : m_wnds) {
			wnd->checkSizeInSystemLoop();
			wnd->update(dt);
		}
		std::list<std::unique_ptr<Window>>::iterator i, n;
		i = m_wnds.begin();
		n = m_wnds.end();
		while (i != n) {
			if ((*i)->isWaitingForStop()) {
				i = m_wnds.erase(i);
			}
			else {
				++i;
			}
		}
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
