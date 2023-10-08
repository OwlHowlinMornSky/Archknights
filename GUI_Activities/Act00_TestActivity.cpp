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
#include "Act00_TestActivity.h"

#include <iostream>

namespace Activity {

TestActivity::TestActivity(uint32_t n) noexcept :
	m_id(n),
	m_paused(false),
	ref_carnival(nullptr),
	m_disableResize(false),
	m_disableMinimize(false),
	m_disableClose(false) {
	std::cout << "TestActivity " << m_id << ": Construct." << std::endl;
}

TestActivity::~TestActivity() noexcept {
	std::cout << "TestActivity " << m_id << ": Destruct." << std::endl;
}

void TestActivity::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;

	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });
	updateSize();

	std::cout << "TestActivity " << m_id << ": start, " << ref_carnival << "." << std::endl;
	return;
}

void TestActivity::stop() noexcept {
	std::cout << "TestActivity " << m_id << ": stop." << std::endl;
}

void TestActivity::pause() noexcept {
	std::cout << "TestActivity " << m_id << ": pause." << std::endl;
}

void TestActivity::resume() noexcept {
	updateSize();
	std::cout << "TestActivity " << m_id << ": resume." << std::endl;
}

uint32_t TestActivity::getID() noexcept {
	return m_id;
}

void TestActivity::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(GUI::Transition::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::F:
			ref_carnival->setTransition(evt.key.control ? -GUI::Transition::Push : GUI::Transition::Push, m_id + 1);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::E:
			ref_carnival->setTransition(evt.key.control ? -GUI::Transition::Switch : GUI::Transition::Switch, m_id + 1);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Q:
			ref_carnival->setTransition(evt.key.control ? -GUI::Transition::Pop : GUI::Transition::Pop);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Backspace:
			ref_carnival->setFullResizeMessage(true);
			break;
		case sf::Keyboard::Backslash:
			ref_carnival->setFullResizeMessage(false);
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
	case sf::Event::Resized:
		m_shape.setPosition(evt.size.width / 2.0f, evt.size.height / 2.0f);
		break;
	default:
		break;
	}
	return;
}

void TestActivity::update(sf::Time deltaTime) {
	float dt = deltaTime.asSeconds();
	if (dt > 0.1f)
		dt = 0.1f;
	if (!m_paused)
		m_shape.rotate(dt * 180.0f);

	ref_carnival->getRenderWindow().clear(sf::Color::Green);
	ref_carnival->getRenderWindow().draw(m_shape);
	ref_carnival->getRenderWindow().display();
	return;
}

void TestActivity::onEnterSysloop() noexcept {
	m_paused = true;
}

void TestActivity::onExitSysloop() noexcept {
	m_paused = false;
}

void TestActivity::updateSize() noexcept {
	auto size = ref_carnival->getRenderWindow().getSize();
	m_shape.setPosition(size.x / 2.0f, size.y / 2.0f);
	ref_carnival->getRenderWindow().setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)size.x, (float)size.y)));
}

} // namespace Activity
