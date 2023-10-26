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
#include "Act02_TestActivity.h"
#include "Act03_Opening.h"

#ifdef _DEBUG
#include <SFML/Graphics.hpp>
#include <iostream>
#endif // _DEBUG

namespace Activity {

Act01_DefaultEntrance::Act01_DefaultEntrance() noexcept :
	m_haveRunned(false),
	r_wnd(nullptr) {
#ifdef _DEBUG
	g_tex = std::make_unique<sf::Texture>();
	g_sp = std::make_unique<sf::Sprite>();
	g_tex->loadFromFile("assets/DefaultEntry.png");
	g_sp->setTexture(*g_tex, true);
#endif // _DEBUG
	return;
}

Act01_DefaultEntrance::~Act01_DefaultEntrance() noexcept {
#ifdef _DEBUG
	g_sp.reset();
	g_tex.reset();
#endif // _DEBUG
	return;
}

bool Act01_DefaultEntrance::start(GUI::Window& wnd) noexcept {
	r_wnd = &wnd;
	r_wnd->setSize({ 1280, 720 });
	return true;
}

void Act01_DefaultEntrance::stop() noexcept {
	r_wnd = nullptr;
	return;
}

void Act01_DefaultEntrance::handleEvent(const sf::Event& evt) {
#ifdef _DEBUG
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			r_wnd->setWaitingForStop();
			break;
		case sf::Keyboard::F:
			r_wnd->changeActivity(std::make_unique<Act03_Opening>());
			break;
		case sf::Keyboard::E:
			r_wnd->changeActivity(std::make_unique<Act02_TestActivity>());
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
#endif // _DEBUG
	return;
}

void Act01_DefaultEntrance::update(sf::Time dtime) {
#ifdef _DEBUG
	r_wnd->clear(sf::Color::Red);
	r_wnd->draw(*g_sp);
	r_wnd->display();
#else
	r_wnd->changeActivity(std::make_unique<Act03_Opening>());
#endif
	return;
}

void Act01_DefaultEntrance::OnEnterSysloop() noexcept {}

void Act01_DefaultEntrance::OnExitSysloop() noexcept {}

} // namespace Activity
