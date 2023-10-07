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
#include "Act01_DefaultEntryDebug.h"

#ifdef _DEBUG
#include "../Audio/BgmSFML.h"
#include "../G3D/base.h"
#include "../G3D/ShaderDefault.h"
#include "ActivityIDs.h"

#include <iostream>

namespace Activity {

DefaultEntryDebug::DefaultEntryDebug() noexcept :
	ref_carnival(nullptr),
	m_disableResize(false),
	m_disableMinimize(false),
	m_disableClose(false) {
	std::cout << "DefaultEntryDebug: Construct." << std::endl;
}

DefaultEntryDebug::~DefaultEntryDebug() noexcept {
	std::cout << "DefaultEntryDebug: Destruct." << std::endl;
}

void DefaultEntryDebug::start(GUI::ICarnival& carnival) {
	//carnival.getRenderWindow().setFramerateLimit(60);
	carnival.getRenderWindow().setVerticalSyncEnabled(true);
	ref_carnival = &carnival;
	m_bgm = std::make_unique<Audio::BgmSFML>();
	m_bgm->openFromFile("test.ogg");
	m_bgm->play();
	g3d::base::setup();
	g3d::base::setActive(true);

	g3d::Shader* shader = new g3d::ShaderDefault();
	shader->setup();

	std::cout << "DefaultEntryDebug: start, " << ref_carnival << "." << std::endl;
	return;
}

void DefaultEntryDebug::stop() noexcept {
	g3d::base::drop();
	m_bgm->stop();
	std::cout << "DefaultEntryDebug: stop." << std::endl;
}

void DefaultEntryDebug::pause() noexcept {
	//m_bgm->pause();
	std::cout << "DefaultEntryDebug: pause." << std::endl;
}

void DefaultEntryDebug::resume() noexcept {
	//m_bgm->play();
	std::cout << "DefaultEntryDebug: resume." << std::endl;
}

uint32_t DefaultEntryDebug::getID() noexcept {
	return ID_DefaultEntry;
}

void DefaultEntryDebug::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(GUI::ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::F:
			ref_carnival->setTransition(-GUI::ICarnival::Push, 2);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::E:
			ref_carnival->setTransition(-GUI::ICarnival::Switch, 2);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Q:
			ref_carnival->setTransition(GUI::ICarnival::Pop);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Space:
			ref_carnival->enableClose(!(m_disableClose = !m_disableClose));
			break;
		case sf::Keyboard::Enter:
			ref_carnival->enableResize(!(m_disableResize = !m_disableResize));
			break;
		case sf::Keyboard::Numpad0:
			ref_carnival->enableMinimize(!(m_disableMinimize = !m_disableMinimize));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return;
}

void DefaultEntryDebug::update(sf::Time deltaTime) {
	ref_carnival->getRenderWindow().clear(sf::Color::Red);
	ref_carnival->getRenderWindow().display();
	return;
}

} // namespace Activity

#endif
