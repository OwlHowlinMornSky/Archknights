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
#include "Act01_DefaultEntry.h"
#include "Act02_TestActivity.h"
#include "Act03_Opening.h"

#include "../G3D/base.h"

#ifdef _DEBUG
#include <SFML/Graphics.hpp>
#include "../G3D/DefaultShader.h"
#include <iostream>
#endif // _DEBUG

namespace Activity {

Act01_DefaultEntry::Act01_DefaultEntry() noexcept :
	m_haveRunned(false),
	r_wnd(nullptr) {
#ifdef _DEBUG
	g_tex = std::make_unique<sf::Texture>();
	g_sp = std::make_unique<sf::Sprite>();
	g_tex->loadFromFile("assets/DefaultEntry.png");
	g_sp->setTexture(*g_tex, true);
	std::cout << "DefaultEntryDebug: Construct." << std::endl;
#endif // _DEBUG
	return;
}

Act01_DefaultEntry::~Act01_DefaultEntry() noexcept {
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: Destruct." << std::endl;
	g_sp.reset();
	g_tex.reset();
#endif // _DEBUG
	return;
}

void Act01_DefaultEntry::handleEvent(const sf::Event& evt) {
#ifdef _DEBUG
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->stop();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			r_wnd->stop();
			break;
		case sf::Keyboard::F:
			r_wnd->setActivity(std::make_unique<Act03_Opening>());
			break;
		case sf::Keyboard::E:
			r_wnd->setActivity(std::make_unique<Act02_TestActivity>());
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	printf_s("Act01 %p: %p, event\n", this, r_wnd);
#endif // _DEBUG
	return;
}

void Act01_DefaultEntry::update(sf::Time dtime) {
#ifdef _DEBUG
	printf_s("Act01 %p: %p, update\n", this, r_wnd);
	r_wnd->clear(sf::Color::Red);
	r_wnd->draw(*g_sp);
	r_wnd->display();
#else
	r_wnd->setActivity(std::make_unique<Act03_Opening>());
#endif
	return;
}

void Act01_DefaultEntry::OnEnterSysloop() noexcept {}

void Act01_DefaultEntry::OnExitSysloop() noexcept {}

bool Act01_DefaultEntry::start(GUI::Window& wnd) noexcept {
	r_wnd = &wnd;
#ifdef _DEBUG
	r_wnd->setCloseEnabled(false);
#endif
	r_wnd->setSize({ 1280, 720 });

	//r_wnd->setFramerateLimit(60);
	r_wnd->setVerticalSyncEnabled(true);

	g3d::base::setup();
#ifdef _DEBUG
	g3d::base::setActive(true);
	g3d::Shader* shader = new g3d::DefaultShader();
	shader->setup();
	delete shader;
#endif // _DEBUG
	g3d::base::setActive(false);
	r_wnd->setActive(true);
	return true;
}

void Act01_DefaultEntry::stop() noexcept {
#ifdef _DEBUG
	r_wnd->setCloseEnabled(true);
#endif
	//g3d::base::drop();
	//r_wnd = nullptr;
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: stop." << std::endl;
#endif // _DEBUG
	return;
}

} // namespace Activity
