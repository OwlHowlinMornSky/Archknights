/*
*    Archknights
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
#include "Activity_Debug.h"
#include "Activity_Game.h"
//#include "../Game/GameGlobal.h"

namespace Activity {

Activity_Debug::Activity_Debug() {}

Activity_Debug::~Activity_Debug() noexcept {}

bool Activity_Debug::prepare(ME::Window& wnd) noexcept {
	r(wnd);
	return true;
}

void Activity_Debug::start() noexcept {
	return;
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

} // namespace Activity
