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
*
*/
#include "TestActivity.h"

TestActivity::TestActivity(size_t n) :
	m_id(n),
	ref_carnival(nullptr) {
	printf_s("TestActivity %zu: Construct.\n", m_id);
}

TestActivity::~TestActivity() {
	printf_s("TestActivity %zu: Destruct.\n", m_id);
}

void TestActivity::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });
	m_shape.setPosition({ 400.0f, 300.0f });
	printf_s("TestActivity %zu: start, %p.\n", m_id, ref_carnival);
	return;
}

void TestActivity::stop() {
	printf_s("TestActivity %zu: stop.\n", m_id);
}

void TestActivity::pause() {
	printf_s("TestActivity %zu: pause.\n", m_id);
}

void TestActivity::resume() {
	printf_s("TestActivity %zu: resume.\n", m_id);
}

size_t TestActivity::getID() {
	return m_id;
}

void TestActivity::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->setTransition(GUI::ICarnival::Exit);
		ref_carnival->cancelKeepRunning();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::F:
			ref_carnival->setTransition(evt.key.control ? -GUI::ICarnival::Push : GUI::ICarnival::Push, m_id + 1);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::E:
			ref_carnival->setTransition(evt.key.control ? -GUI::ICarnival::Switch : GUI::ICarnival::Switch, m_id + 1);
			ref_carnival->cancelKeepRunning();
			break;
		case sf::Keyboard::Q:
			ref_carnival->setTransition(evt.key.control ? -GUI::ICarnival::Pop : GUI::ICarnival::Pop);
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

void TestActivity::update(float dt) {
	m_shape.rotate(dt * 90.0f);

	ref_carnival->getRenderWindow().clear(sf::Color::Green);
	ref_carnival->getRenderWindow().draw(m_shape);
	ref_carnival->getRenderWindow().display();
	return;
}
