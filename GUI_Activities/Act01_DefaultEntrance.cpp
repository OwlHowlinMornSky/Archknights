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
#include "Act01_DefaultEntrance.h"
#include "Act03_Opening.h"
#ifdef _DEBUG
#include "Act02_TestActivity.h"
#endif // _DEBUG

namespace Activity {

Act01_DefaultEntrance::Act01_DefaultEntrance() :
	r_wnd(nullptr) {
	return;
}

Act01_DefaultEntrance::~Act01_DefaultEntrance() noexcept {
	return;
}

bool Act01_DefaultEntrance::start(GUI::Window& wnd) noexcept {
	r_wnd = &wnd;
	r_wnd->setSize({ 1280, 720 });
#ifdef _DEBUG
	m_tex.loadFromFile("assets/DefaultEntry.png");
	m_sp.setTexture(m_tex, true);
#endif // _DEBUG
	return true;
}

void Act01_DefaultEntrance::stop() noexcept {
	r_wnd = nullptr;
	return;
}

bool Act01_DefaultEntrance::handleEvent(const sf::Event& evt) {
#ifdef _DEBUG
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		return 1;
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			r_wnd->setWaitingForStop();
			return 1;
			break;
		case sf::Keyboard::F:
			r_wnd->changeActivity(std::make_unique<Act03_Opening>());
			return 1;
			break;
		case sf::Keyboard::E:
			r_wnd->changeActivity(std::make_unique<Act02_TestActivity>());
			return 1;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
#endif // _DEBUG
	return 0;
}

void Act01_DefaultEntrance::update(sf::Time dtime) {
#ifdef _DEBUG
	r_wnd->clear(sf::Color::Red);
	r_wnd->draw(m_sp);
	r_wnd->display();
#else
	r_wnd->changeActivity(std::make_unique<Act03_Opening>());
#endif
	return;
}

void Act01_DefaultEntrance::OnEnterSysloop() noexcept {}

void Act01_DefaultEntrance::OnExitSysloop() noexcept {}

} // namespace Activity
