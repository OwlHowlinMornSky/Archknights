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
*/
#include <SFML/Graphics.hpp>
#include "framework.h"

#include "CarnivalWin32.h"

#include "../GUI/Callbacks.h"
#include "../GUI/ToDefaultEntry.h"

namespace GUI {

CarnivalWin32::CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window) :
	Carnival(r_window),
	m_hwnd(hwnd) {}

CarnivalWin32::~CarnivalWin32() {}

void CarnivalWin32::run() {
	m_runningActivity = createDefaultEntry();
	m_runningActivity->start(*this);

	while (handleTransition()) {
		if (m_runningActivity->isIndependent()) {
			m_runningActivity->runIndependently();
		}
		else {
			runTheActivity();
		}
	}

	while (!m_activityStack.empty()) {
		m_activityStack.pop();
	}
	for (const auto& i : m_pausedActivities) {
		i.second->stop();
	}
	m_pausedActivities.clear();
	return;
}

void CarnivalWin32::showMessageBox(std::string_view title, std::string_view text, MBInfo info) const {
	UINT type{ 0 };
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

void CarnivalWin32::setCloseButton(bool enabled) const {}

void CarnivalWin32::runTheActivity() {
	std::function<void()> oldIdle = Callbacks::OnIdle;
	m_keepRunning = true;

	sf::Clock clk;
	float dt = 0.0f;

	Callbacks::OnIdle = [this, &dt, &clk]() -> void {
		dt = clk.restart().asSeconds();
		m_runningActivity->update(dt);
	};

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

	Callbacks::OnIdle = oldIdle;
	return;
}

std::unique_ptr<IActivity> CarnivalWin32::createActivity(size_t id) const {
	return createTestActivity(id);
}

} // namespace GUI
