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

#include "../G3D/base.h"
#include "ActivityIDs.h"

#ifdef _DEBUG
#include "../G3D/ShaderDefault.h"
#include <iostream>
#endif // _DEBUG

namespace Activity {

DefaultEntry::DefaultEntry() noexcept :
	m_haveRunned(false),
	ref_carnival(nullptr) {
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: Construct." << std::endl;
#endif // _DEBUG
}

DefaultEntry::~DefaultEntry() noexcept {
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: Destruct." << std::endl;
#endif // _DEBUG
}

void DefaultEntry::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	ref_carnival->windowSetCloseEnabled(false);

	ref_carnival->windowSetClientSize(1280, 720);

	sf::RenderWindow& window = ref_carnival->getRenderWindow();
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	g3d::base::setup();
#ifdef _DEBUG
	g3d::base::setActive(true);
	g3d::Shader* shader = new g3d::ShaderDefault();
	shader->setup();
	std::cout << "DefaultEntryDebug: start, " << ref_carnival << "." << std::endl;
	delete shader;
#endif // _DEBUG
	g3d::base::setActive(false);
	return;
}

void DefaultEntry::stop() noexcept {
	g3d::base::drop();
	ref_carnival = nullptr;
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: stop." << std::endl;
#endif // _DEBUG
	return;
}

void DefaultEntry::pause() noexcept {
	ref_carnival->windowSetCloseEnabled(true);
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: pause." << std::endl;
#endif // _DEBUG
	return;
}

void DefaultEntry::resume() noexcept {
	ref_carnival->windowSetCloseEnabled(false);
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: resume." << std::endl;
#endif // _DEBUG
	return;
}

uint32_t DefaultEntry::getID() noexcept {
	return ID_DefaultEntry;
}

void DefaultEntry::runIndependently() {
#ifndef _DEBUG
	if (m_haveRunned) {
		ref_carnival->meActivitySetTransition(GUI::Transition::Exit);
		return;
	}
	m_haveRunned = true;
	ref_carnival->meActivitySetTransition(-GUI::Transition::Push, IDs::ID_Opening);
#else
	bool run = true;
	sf::RenderWindow& window = ref_carnival->getRenderWindow();
	sf::Event evt;
	while (run) {
		ref_carnival->systemMessagePump(true);
		while (window.pollEvent(evt)) {
			switch (evt.type) {
			case sf::Event::Closed:
				ref_carnival->meActivitySetTransition(GUI::Transition::Exit);
				run = false;
				break;
			case sf::Event::KeyPressed:
				switch (evt.key.code) {
				case sf::Keyboard::F:
					ref_carnival->meActivitySetTransition(-GUI::Transition::Push, IDs::ID_Opening);
					run = false;
					break;
				case sf::Keyboard::Q:
					ref_carnival->meActivitySetTransition(GUI::Transition::Pop);
					run = false;
					break;
				case sf::Keyboard::F1:
					ref_carnival->meActivitySetTransition(-GUI::Transition::Push, IDs::ID_Test);
					run = false;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
		window.clear(sf::Color::Red);
		window.display();
	}
#endif // !_DEBUG
	return;
}

} // namespace Activity
