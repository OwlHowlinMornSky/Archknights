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
#include "Act00_Debug.h"
#include "Act06_Game.h"

namespace Activity {

Act00_Debug::Act00_Debug() {}

Act00_Debug::~Act00_Debug() noexcept {}

bool Act00_Debug::prepare(ME::Window& wnd) noexcept {
	r(wnd);
	return true;
}

void Act00_Debug::start() noexcept {
	return;
}

void Act00_Debug::stop() noexcept {
	r();
}

bool Act00_Debug::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r->setWaitingForStop();
		return true;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Num1:
			r->changeActivity(std::make_unique<Act06_Game>());
			return true;
		}
		break;
	}
	return false;
}

void Act00_Debug::update(sf::Time dtime) {
	r->clear(sf::Color::Cyan);
	r->display();
}

void Act00_Debug::OnEnterSysloop() noexcept {}

void Act00_Debug::OnExitSysloop() noexcept {}

} // namespace Activity
