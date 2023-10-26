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
#include "Act04_Load.h"

#include <SFML/Graphics.hpp>

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
	r_wnd(nullptr) {}

bool Act03_Opening::start(GUI::Window& wnd) noexcept {
	r_wnd = &wnd;
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
	return true;
}

void Act03_Opening::stop() noexcept {
	m_timer = sf::Time::Zero;
	m_status = 0;
	r_wnd = nullptr;
	return;
}

void Act03_Opening::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		break;
#ifdef _DEBUG
	case sf::Event::KeyPressed:
		m_status = ST_OVER;
		r_wnd->changeActivity(std::make_unique<Act04_Load>());
		break;
#endif // _DEBUG
	default:
		break;
	}
	return;
}

void Act03_Opening::update(sf::Time deltaTime) {
	m_timer += deltaTime;
	r_wnd->clear();
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
		r_wnd->draw(circle[0]);
		break;
	case ST_PIC0_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_IN;
			circle[0].setFillColor(sf::Color::Transparent);
		}
		r_wnd->draw(circle[0]);
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
		r_wnd->draw(circle[1]);
		break;
	case ST_PIC1_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_OUT;
		}
		r_wnd->draw(circle[1]);
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
		r_wnd->draw(circle[1]);
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
		r_wnd->draw(circle[2]);
		break;
	case ST_PIC2_KEEP:
		if (m_timer >= sf::milliseconds(1750)) {
			m_timer -= sf::milliseconds(1750);
			m_status = ST_PIC2_OUT;
		}
		r_wnd->draw(circle[2]);
		break;
	case ST_PIC2_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_OVER;
			circle[2].setFillColor(sf::Color::Transparent);
		}
		else {
			circle[2].setFillColor(sf::Color(0, 0, 255 - 255 * m_timer.asMilliseconds() / 250));
		}
		r_wnd->draw(circle[2]);
		break;
	case ST_OVER:
		r_wnd->changeActivity(std::make_unique<Act04_Load>());
		return;
		break;
	default:
		m_status = ST_PIC0_IN;
		circle[0].setFillColor(sf::Color::Transparent);
		circle[1].setFillColor(sf::Color::Transparent);
		circle[2].setFillColor(sf::Color::Transparent);
		break;
	}
	return r_wnd->display();
}

void Act03_Opening::OnEnterSysloop() noexcept {}

void Act03_Opening::OnExitSysloop() noexcept {}

}
