/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
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
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*
*/
#include "DefaultEntry.h"

#include "ActivityIDs.h"

namespace GUI {

DefaultEntry::DefaultEntry() :
	m_haveRunned(false),
	ref_carnival(nullptr)
{}

DefaultEntry::~DefaultEntry() {}

bool DefaultEntry::isIndependent() const {
	return true;
}

void DefaultEntry::runIndependently() {
	if (m_haveRunned) {
		ref_carnival->setTransition(ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		return;
	}
	m_haveRunned = true;
}

void DefaultEntry::start(ICarnival& carnival) {
	ref_carnival = &carnival;
	return;
}

void DefaultEntry::stop() {
	ref_carnival = nullptr;
	return;
}

void DefaultEntry::pause() {}

void DefaultEntry::resume() {}

size_t DefaultEntry::getID() {
	return GUI::ID_DefaultEntry;
}


#ifdef _DEBUG
DefaultEntryDebug::DefaultEntryDebug() :
	ref_carnival(nullptr) {
	printf_s("DefaultEntryDebug: Construct.\n");
}

DefaultEntryDebug::~DefaultEntryDebug() {
	printf_s("DefaultEntryDebug: Destruct.\n");
}

void DefaultEntryDebug::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::F:
			ref_carnival->setTransition(-ICarnival::Push, 2);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::E:
			ref_carnival->setTransition(-ICarnival::Switch, 2);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Q:
			ref_carnival->setTransition(ICarnival::Pop);
			ref_carnival->cancelKeepRunning();
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

void DefaultEntryDebug::update(float dt) {
	ref_carnival->getRenderWindow().clear(sf::Color::Red);
	ref_carnival->getRenderWindow().display();
	return;
}

void DefaultEntryDebug::start(ICarnival& carnival) {
	carnival.getRenderWindow().setFramerateLimit(60);
	ref_carnival = &carnival;
	printf_s("DefaultEntryDebug: start, %p.\n", ref_carnival);
	return;
}

void DefaultEntryDebug::stop() {
	printf_s("DefaultEntryDebug: stop.\n");
}

void DefaultEntryDebug::pause() {
	printf_s("DefaultEntryDebug: pause.\n");
}

void DefaultEntryDebug::resume() {
	printf_s("DefaultEntryDebug: resume.\n");
}

size_t DefaultEntryDebug::getID() {
	return GUI::ID_DefaultEntry;
}
#endif

} // namespace GUI
