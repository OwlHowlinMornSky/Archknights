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
#include "DefaultEntryDebug.h"

#ifdef _DEBUG
#include "../Audio/BgmSFML.h"
#include "../G3D/base.h"
#include "../G3D/ShaderDefault.h"
#include "ActivityIDs.h"

namespace Activity {

DefaultEntryDebug::DefaultEntryDebug() :
	ref_carnival(nullptr),
	m_disableResize(false),
	m_disableMinimize(false),
	m_disableClose(false) {
	printf_s("DefaultEntryDebug: Construct.\n");
}

DefaultEntryDebug::~DefaultEntryDebug() {
	printf_s("DefaultEntryDebug: Destruct.\n");
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

	printf_s("DefaultEntryDebug: start, %p.\n", ref_carnival);
	return;
}

void DefaultEntryDebug::stop() {
	g3d::base::drop();
	m_bgm->stop();
	printf_s("DefaultEntryDebug: stop.\n");
}

void DefaultEntryDebug::pause() {
	m_bgm->pause();
	printf_s("DefaultEntryDebug: pause.\n");
}

void DefaultEntryDebug::resume() {
	m_bgm->play();
	printf_s("DefaultEntryDebug: resume.\n");
}

size_t DefaultEntryDebug::getID() {
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

} // namespace GUI

#endif
