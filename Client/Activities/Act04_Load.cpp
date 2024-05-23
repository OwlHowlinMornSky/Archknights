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
#include "Act04_Load.h"

#include "Act05_Title.h"
#include <MysteryEngine/Client/GlobalBGM.h>

#define ST_IN     (1)
#define ST_NORMAL (2)
#define ST_OUT    (3)
#define ST_OVER   (4)

namespace Activity {

Act04_Load::Act04_Load() :
	m_status(0) {}

Act04_Load::~Act04_Load() noexcept {}

bool Act04_Load::prepare(ME::Window& wnd) noexcept {
	r_wnd = wnd;

#ifdef ARCHKNIGHTS_LIMITED
	m_tex.loadFromFile("res/textures/bkgnd.png");
#else
	m_tex.loadFromFile("assets/textures/bkgnd.png");
#endif // ARCHKNIGHTS_LIMITED
	m_tex.setSmooth(true);
	m_tex.generateMipmap();
	return true;
}

void Act04_Load::start() noexcept {
	m_sp.setTexture(m_tex, true);
	m_sp.setOrigin(m_tex.getSize().x / 2.0f, m_tex.getSize().y / 2.0f);

	auto& view = r_wnd->getView();
	sf::Vector2f size = view.getSize();
	updateSize(
		sf::Vector2u(
			static_cast<unsigned int>(size.x),
			static_cast<unsigned int>(size.y)
		)
	);

	m_blackBar[0].setFillColor(sf::Color::Black);
	m_blackBar[1].setFillColor(sf::Color::Black);

	ME::GlobalBGM::play("res/music/m_sys_title.ogg");

	//m_text.setString(L"test测试あいうえお😅");
	//m_text.setFont(r_wnd->getFontMgr().getFont(GUI::FontType::CommonContext));
	//m_text.setFillColor(sf::Color::White);
	//m_text.setCharacterSize(36);
	return;
}

void Act04_Load::stop() noexcept {
	m_status = 0;
	r_wnd();
	return;
}

bool Act04_Load::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		break;
	case sf::Event::MouseButtonPressed:
		switch (evt.mouseButton.button) {
		case sf::Mouse::Button::Left:
			if (m_status == ST_NORMAL) {
				m_status = ST_OUT;
				ME::GlobalBGM::stop();
			}
			break;
		default:
			break;
		}
		break;
	case sf::Event::Resized:
		updateSize({ evt.size.width, evt.size.height });
		break;
	default:
		break;
	}
	return 0;
}

void Act04_Load::update(sf::Time deltaTime) {
	switch (m_status) {
	case ST_IN:
	{
		sf::Int32 dt = deltaTime.asMilliseconds() * 2;
		sf::Uint8 a = m_sp.getColor().r;
		if (dt + a >= 255) {
			m_status = ST_NORMAL;
			m_sp.setColor(sf::Color::White);
		}
		else {
			sf::Uint8 c = static_cast<sf::Uint8>(a + dt);
			m_sp.setColor(sf::Color(c, c, c));
		}
		break;
	}
	case ST_NORMAL:
		break;
	case ST_OUT:
	{
		sf::Int32 dt = deltaTime.asMilliseconds() * 3;
		sf::Uint8 a = m_sp.getColor().r;
		if (-dt + a <= 0) {
			m_status = ST_OVER;
			m_sp.setColor(sf::Color::Black);
		}
		else {
			sf::Uint8 c = static_cast<sf::Uint8>(-dt + a);
			m_sp.setColor(sf::Color(c, c, c));
		}
		break;
	}
	case ST_OVER:
		m_status = 0;
		r_wnd->changeActivity(std::make_unique<Act05_Title>());
		break;
	default:
		m_status = ST_IN;
		m_sp.setColor(sf::Color::Black);
		break;
	}
	r_wnd->clear();
	r_wnd->draw(m_sp);
	r_wnd->draw(m_blackBar[0]);
	r_wnd->draw(m_blackBar[1]);
	//r_wnd->draw(m_text);
	r_wnd->display();
	return;
}

void Act04_Load::OnEnterSysloop() noexcept {}

void Act04_Load::OnExitSysloop() noexcept {}

void Act04_Load::updateSize(sf::Vector2u newWindowSize) {
	sf::Vector2u texSize = m_tex.getSize();
	float tx = static_cast<float>(texSize.x);
	float ty = static_cast<float>(texSize.y);
	float texRate = tx / ty;
	float wx = static_cast<float>(newWindowSize.x);
	float wy = static_cast<float>(newWindowSize.y);
	float winRate = wx / (wy * 0.8f);
	float rate = 1.0f;
	if (winRate > texRate) {
		rate = wx / tx;
	}
	else {
		rate = (wy * 0.8f) / ty;
	}
	m_sp.setScale(rate, rate);
	m_sp.setPosition(wx / 2.0f, wy / 2.0f);

	m_blackBar[0].setSize(sf::Vector2f(wx * 1.1f, wy * 0.15f));
	m_blackBar[0].setOrigin(wx * 0.05f, wy * (-0.9f));
	m_blackBar[1].setSize(sf::Vector2f(wx * 1.1f, wy * 0.15f));
	m_blackBar[1].setOrigin(wx * 0.05f, wy * 0.05f);

	//float fontSizef = wy * 0.08f;
	//float fontRate = fontSizef / 36;
	//m_text.setScale(fontRate, fontRate);
	return;
}

} // namespace Activity
