/*
*    Archknights
*
*    Copyright (C) 2023-2024  Tyler Parret True
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

Act03_Opening::Act03_Opening() :
	m_status(0) {}

Act03_Opening::~Act03_Opening() noexcept {}

bool Act03_Opening::prepare(ME::Window& wnd) noexcept {
	r_wnd = wnd;

	m_tex[0].loadFromFile("assets/textures/work.png");
	m_tex[1].loadFromFile("assets/textures/title.png");
	m_tex[2].loadFromFile("assets/textures/bkgnd.png");
	return true;
}

void Act03_Opening::start() noexcept {
	m_status = 0;
	m_timer = sf::Time::Zero;

	for (int i = 0; i < 3; ++i) {
		m_tex[i].setSmooth(true);
		m_tex[i].generateMipmap();
		circle[i].setTexture(m_tex[i]);
		circle[i].setOrigin(m_tex[i].getSize().x / 2.0f, m_tex[i].getSize().y / 2.0f);
	}

	updateSize();
	return;
}

void Act03_Opening::stop() noexcept {
	m_timer = sf::Time::Zero;
	m_status = 0;
	r_wnd();
	return;
}

bool Act03_Opening::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		return 1;
		break;
#ifdef _DEBUG
	case sf::Event::KeyPressed:
		m_status = ST_OVER;
		r_wnd->changeActivity(std::make_unique<Act04_Load>());
		return 1;
		break;
#endif // _DEBUG
	case sf::Event::Resized:
		updateSize();
		break;
	default:
		break;
	}
	return 0;
}

void Act03_Opening::update(sf::Time deltaTime) {
	m_timer += deltaTime;
	r_wnd->clear();
	int r = 0;
	switch (m_status) {
	case ST_PIC0_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC0_KEEP;
			circle[0].setColor(sf::Color(255, 255, 255));
		}
		else {
			r = 255 * m_timer.asMilliseconds() / 250;
			circle[0].setColor(sf::Color(r, r, r));
		}
		r_wnd->draw(circle[0]);
		break;
	case ST_PIC0_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			m_status = ST_PIC1_IN;
			circle[0].setColor(sf::Color::Transparent);
		}
		r_wnd->draw(circle[0]);
		break;
	case ST_PIC1_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC1_KEEP;
			circle[1].setColor(sf::Color(255, 255, 255));
		}
		else {
			r = 255 * m_timer.asMilliseconds() / 250;
			circle[1].setColor(sf::Color(r, r, r));
		}
		r_wnd->draw(circle[1]);
		break;
	case ST_PIC1_KEEP:
		if (m_timer >= sf::milliseconds(1000)) {
			m_timer -= sf::milliseconds(1000);
			//m_status = ST_PIC1_OUT;
			m_status = ST_PIC2_IN;
		}
		r_wnd->draw(circle[1]);
		break;
	case ST_PIC1_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC2_IN;
			circle[1].setColor(sf::Color::Transparent);
		}
		else {
			r = 255 - 255 * m_timer.asMilliseconds() / 250;
			circle[1].setColor(sf::Color(r, r, r));
		}
		r_wnd->draw(circle[1]);
		break;
	case ST_PIC2_IN:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_PIC2_KEEP;
			circle[2].setColor(sf::Color(255, 255, 255));
		}
		else {
			r = 255 * m_timer.asMilliseconds() / 250;
			circle[2].setColor(sf::Color(r, r, r));
		}
		r_wnd->draw(circle[2]);
		r_wnd->draw(circle[1]);
		break;
	case ST_PIC2_KEEP:
		if (m_timer >= sf::milliseconds(1750)) {
			m_timer -= sf::milliseconds(1750);
			m_status = ST_PIC2_OUT;
		}
		r_wnd->draw(circle[2]);
		r_wnd->draw(circle[1]);
		break;
	case ST_PIC2_OUT:
		if (m_timer >= sf::milliseconds(250)) {
			m_timer -= sf::milliseconds(250);
			m_status = ST_OVER;
			circle[2].setColor(sf::Color::Transparent);
			circle[1].setColor(sf::Color::Transparent);
		}
		else {
			r = 255 - 255 * m_timer.asMilliseconds() / 250;
			circle[2].setColor(sf::Color(r, r, r));
			circle[1].setColor(sf::Color(r, r, r));
		}
		r_wnd->draw(circle[2]);
		r_wnd->draw(circle[1]);
		break;
	case ST_OVER:
		r_wnd->changeActivity(std::make_unique<Act04_Load>());
		return;
		break;
	default:
		m_status = ST_PIC0_IN;
		circle[0].setColor(sf::Color::Transparent);
		circle[1].setColor(sf::Color::Transparent);
		circle[2].setColor(sf::Color::Transparent);
		break;
	}
	return r_wnd->display();
}

void Act03_Opening::OnEnterSysloop() noexcept {}

void Act03_Opening::OnExitSysloop() noexcept {}

void Act03_Opening::updateSize() {
	auto& view = r_wnd->getView();
	sf::Vector2f size = view.getSize();
	for (int i = 0; i < 3; ++i) {
		circle[i].setPosition(size / 2.0f);
	}
	float rate;
	rate = size.y / m_tex[0].getSize().y;
	circle[0].setScale(rate, rate);
	rate = size.y / m_tex[1].getSize().y * 0.4f;
	circle[1].setScale(rate, rate);
	rate = size.y / m_tex[2].getSize().y;
	circle[2].setScale(rate, rate);
	return;
}

} // namespace Activity
