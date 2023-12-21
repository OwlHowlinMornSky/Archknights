#include "Activity_Debug.h"
#include "Activity_Game.h"
#include "Global.h"
#include "../Game/GameGlobal.h"

namespace gamegui {

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
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Num1:
			Global::instance()->data.name = "debug";
			game::Global::instance()->data.name = "dabug";
			r->changeActivity(std::make_unique<Activity_Game>());
			return true;
		}
		break;
	}
	return false;
}

void Activity_Debug::update(sf::Time dtime) {
	r->clear(sf::Color::Cyan);
	r->display();
}

void Activity_Debug::OnEnterSysloop() noexcept {}

void Activity_Debug::OnExitSysloop() noexcept {}

}
