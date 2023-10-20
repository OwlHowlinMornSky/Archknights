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
#include <SFML/Graphics.hpp>
#include "../G3D/DefaultShader.h"
#include <iostream>
namespace {
std::unique_ptr<sf::Texture> g_tex;
std::unique_ptr<sf::Sprite> g_sp;
}
#endif // _DEBUG

namespace Activity {

Act01_DefaultEntry::Act01_DefaultEntry() noexcept :
	m_haveRunned(false),
	ref_carnival(nullptr) {
#ifdef _DEBUG
	g_tex = std::make_unique<sf::Texture>();
	g_sp = std::make_unique<sf::Sprite>();
	g_tex->loadFromFile("assets/DefaultEntry.png");
	g_sp->setTexture(*g_tex, true);
	std::cout << "DefaultEntryDebug: Construct." << std::endl;
#endif // _DEBUG
}

Act01_DefaultEntry::~Act01_DefaultEntry() noexcept {
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: Destruct." << std::endl;
	g_sp.reset();
	g_tex.reset();
#endif // _DEBUG
}

void Act01_DefaultEntry::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	ref_carnival->windowSetCloseEnabled(false);

	ref_carnival->windowSetClientSize(1280, 720);

	sf::RenderWindow& window = ref_carnival->getRenderWindow();
	//window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	g3d::base::setup();
#ifdef _DEBUG
	g3d::base::setActive(true);
	g3d::Shader* shader = new g3d::DefaultShader();
	shader->setup();
	std::cout << "DefaultEntryDebug: start, " << ref_carnival << "." << std::endl;
	delete shader;
#endif // _DEBUG
	g3d::base::setActive(false);
	return;
}

void Act01_DefaultEntry::stop() noexcept {
	g3d::base::drop();
	ref_carnival = nullptr;
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: stop." << std::endl;
#endif // _DEBUG
	return;
}

void Act01_DefaultEntry::pause() noexcept {
	ref_carnival->windowSetCloseEnabled(true);
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: pause." << std::endl;
#endif // _DEBUG
	return;
}

void Act01_DefaultEntry::resume() noexcept {
	ref_carnival->windowSetCloseEnabled(false);
#ifdef _DEBUG
	std::cout << "DefaultEntryDebug: resume." << std::endl;
#endif // _DEBUG
	return;
}

uint32_t Act01_DefaultEntry::getID() noexcept {
	return ID_DefaultEntry;
}

void Act01_DefaultEntry::runIndependently() {
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
				case sf::Keyboard::Escape:
				case sf::Keyboard::Q:
					ref_carnival->meActivitySetTransition(GUI::Transition::Pop);
					run = false;
					break;
				case sf::Keyboard::F:
					ref_carnival->meActivitySetTransition(-GUI::Transition::Push, IDs::ID_Opening);
					run = false;
					break;
				case sf::Keyboard::E:
					ref_carnival->meActivitySetTransition(-GUI::Transition::Push, IDs::ID_Test);
					run = false;
					break;
				default:
					break;
				}
				break;
			case sf::Event::Resized:
				window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)evt.size.width, (float)evt.size.height)));
				break;
			default:
				break;
			}
		}
		window.clear(sf::Color::Red);
		window.draw(*g_sp);
		window.display();
	}
#endif // !_DEBUG
	return;
}

} // namespace Activity
