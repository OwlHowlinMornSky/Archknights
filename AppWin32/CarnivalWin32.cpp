

#include <SFML/Graphics.hpp>
#include "framework.h"

#include "CarnivalWin32.h"

#include "../GUI/Callbacks.h"

#include "../GUI/DefaultEntry.h"

namespace GUI {

CarnivalWin32::CarnivalWin32(HWND hwnd, sf::RenderWindow* r_window) :
	ICarnival(r_window),
	m_hwnd(hwnd) {}

CarnivalWin32::~CarnivalWin32() {}

void CarnivalWin32::run() {
	m_running = std::make_unique<DefaultEntry>();
	m_running->start(*this);

	//while (true) {
		if (m_running->isIndependent()) {
			m_running->runIndependently();
		}
		else {
			runTheActivity();
		}
		//handleTransition();
	//}

	m_running->stop();
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
		m_running->update(dt);
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
			m_running->handleEvent(evt);
		}

		dt = clk.restart().asSeconds();
		m_running->update(dt);
	}

	Callbacks::OnIdle = oldIdle;
	return;
}

void CarnivalWin32::handleTransition() {}

}
