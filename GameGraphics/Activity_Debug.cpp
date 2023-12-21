#include "Activity_Debug.h"

Activity_Debug::Activity_Debug() {}

Activity_Debug::~Activity_Debug() noexcept {}

bool Activity_Debug::start(GUI::Window& wnd) noexcept {
	r(wnd);
	return true;
}

void Activity_Debug::stop() noexcept {
	r();
}

bool Activity_Debug::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r->setWaitingForStop();
		return true;
	}
	return false;
}

void Activity_Debug::update(sf::Time dtime) {
	r->clear(sf::Color::Cyan);
	r->display();
}

void Activity_Debug::OnEnterSysloop() noexcept {}

void Activity_Debug::OnExitSysloop() noexcept {}
