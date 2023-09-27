

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

}
