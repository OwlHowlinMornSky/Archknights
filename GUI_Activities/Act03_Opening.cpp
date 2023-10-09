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
#include "Act03_Opening.h"

#include <SFML/Graphics.hpp>

#include "ActivityIDs.h"

#include <assert.h>

#define ST_PIC0_IN   (1)
#define ST_PIC0_KEEP (2)
#define ST_PIC1_IN   (3)
#define ST_PIC1_KEEP (4)
#define ST_PIC1_OUT  (5)
#define ST_PIC2_IN   (6)
#define ST_PIC2_KEEP (7)
#define ST_PIC2_OUT  (8)
#define ST_OVER      (9)

namespace Activity {

Act03_Opening::Act03_Opening() noexcept :
	m_status(0),
	ref_carnival(nullptr) {}

void Act03_Opening::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	m_status = 0;
	m_timer = sf::Time::Zero;

	circle[0].setRadius(100.0f);
	circle[0].setOrigin(100.0f, 100.0f);
	circle[1].setRadius(100.0f);
	circle[1].setOrigin(100.0f, 100.0f);
	circle[2].setRadius(100.0f);
	circle[2].setOrigin(100.0f, 100.0f);

	circle[0].setPosition(560.0f, 360.0f);
	circle[1].setPosition(720.0f, 360.0f);
	circle[2].setPosition(640.0f, 360.0f);
	return;
}

void Act03_Opening::stop() noexcept {
	m_timer = sf::Time::Zero;
	m_status = 0;
	ref_carnival = nullptr;
	return;
}

void Act03_Opening::pause() noexcept {
	assert(false);
}

void Act03_Opening::resume() noexcept {
	assert(false);
}

uint32_t Act03_Opening::getID() noexcept {
	return IDs::ID_Opening;
}

void Act03_Opening::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		ref_carnival->meActivitySetTransition(GUI::Transition::Exit);
		ref_carnival->meDependentActivityStopRunning();
		break;
#ifdef _DEBUG
	case sf::Event::KeyPressed:
		m_status = ST_OVER;
		ref_carnival->meActivitySetTransition(GUI::Transition::Switch, IDs::ID_Load);
		ref_carnival->meDependentActivityStopRunning();
		break;
#endif // _DEBUG
	default:
		break;
	}
	return;
}

void Act03_Opening::update(sf::RenderWindow& window, sf::Time deltaTime) {
	m_timer += deltaTime;
	window.clear();
	switch (m_status) {
	case ST_PIC0_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC0_KEEP;
			circle[0].setFillColor(sf::Color(255, 0, 0));
		}
		else {
			circle[0].setFillColor(sf::Color(255 * m_timer.asMilliseconds() / 250, 0, 0));
		}
		window.draw(circle[0]);
		break;
	case ST_PIC0_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_IN;
			circle[0].setFillColor(sf::Color::Transparent);
		}
		window.draw(circle[0]);
		break;
	case ST_PIC1_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC1_KEEP;
			circle[1].setFillColor(sf::Color(0, 255, 0));
		}
		else {
			circle[1].setFillColor(sf::Color(0, 255 * m_timer.asMilliseconds() / 250, 0));
		}
		window.draw(circle[1]);
		break;
	case ST_PIC1_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_OUT;
		}
		window.draw(circle[1]);
		break;
	case ST_PIC1_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC2_IN;
			circle[1].setFillColor(sf::Color::Transparent);
		}
		else {
			circle[1].setFillColor(sf::Color(0, 255 - 255 * m_timer.asMilliseconds() / 250, 0));
		}
		window.draw(circle[1]);
		break;
	case ST_PIC2_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC2_KEEP;
			circle[2].setFillColor(sf::Color(0, 0, 255));
		}
		else {
			circle[2].setFillColor(sf::Color(0, 0, 255 * m_timer.asMilliseconds() / 250));
		}
		window.draw(circle[2]);
		break;
	case ST_PIC2_KEEP:
		if (m_timer >= sf::milliseconds(1750)) {
			m_timer -= sf::milliseconds(1750);
			m_status = ST_PIC2_OUT;
		}
		window.draw(circle[2]);
		break;
	case ST_PIC2_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_OVER;
			ref_carnival->meActivitySetTransition(GUI::Transition::Switch, IDs::ID_Load);
			ref_carnival->meDependentActivityStopRunning();
			circle[2].setFillColor(sf::Color::Transparent);
		}
		else {
			circle[2].setFillColor(sf::Color(0, 0, 255 - 255 * m_timer.asMilliseconds() / 250));
		}
		window.draw(circle[2]);
		break;
	case ST_OVER:
		break;
	default:
		m_status = ST_PIC0_IN;
		circle[0].setFillColor(sf::Color::Transparent);
		circle[1].setFillColor(sf::Color::Transparent);
		circle[2].setFillColor(sf::Color::Transparent);
		break;
	}
	return window.display();
}

}
