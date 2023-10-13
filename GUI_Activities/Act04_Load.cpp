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
#include "Act04_Load.h"

#include "../Audio/BgmSFML.h"
#include "ActivityIDs.h"

#define ST_IN     (1)
#define ST_NORMAL (2)
#define ST_OUT    (3)
#define ST_OVER   (4)

namespace Activity {

Act04_Load::Act04_Load() noexcept :
	m_status(0),
	ref_carnival(nullptr) {}

void Act04_Load::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;

	m_tex.loadFromFile("res/textures/titleback.png");
	m_tex.setSmooth(true);
	m_tex.generateMipmap();
	m_sp.setTexture(m_tex, true);
	m_sp.setOrigin(m_tex.getSize().x / 2.0f, m_tex.getSize().y / 2.0f);

	updateSize(ref_carnival->getRenderWindow().getSize());

	m_blackBar[0].setFillColor(sf::Color::Black);
	m_blackBar[1].setFillColor(sf::Color::Black);

	m_bgm = std::make_unique<Audio::BgmSFML>();
	m_bgm->openFromFile("res/music/m_sys_title.ogg");
	//m_bgm->openFromFile("res/music/m_sys_title_h.ogg");
	m_bgm->play();

	m_text.setString(L"test测试あいうえお😅");
	m_text.setFont(ref_carnival->getFontMgr().getFont(GUI::FontType::CommonContext));
	m_text.setFillColor(sf::Color::White);
	m_text.setCharacterSize(36);
	return;
}

void Act04_Load::stop() noexcept {
	m_bgm->stop();

	m_status = 0;
	return;
}

void Act04_Load::pause() noexcept {}

void Act04_Load::resume() noexcept {}

uint32_t Act04_Load::getID() noexcept {
	return IDs::ID_Load;
}

void Act04_Load::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::KeyPressed:
		//m_bgm.reset();
		break;
	case sf::Event::MouseButtonPressed:
		switch (evt.mouseButton.button) {
		case sf::Mouse::Button::Left:
			if (m_status == ST_NORMAL) {
				m_status = ST_OUT;
			}
			break;
		case sf::Mouse::Button::Right:
			if (m_status == ST_OVER) {
				m_status = ST_IN;
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
	return;
}

void Act04_Load::update(sf::RenderWindow& window, sf::Time deltaTime) {
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
		break;
	default:
		m_status = ST_IN;
		m_sp.setColor(sf::Color::Black);
		break;
	}
	window.clear();
	window.draw(m_sp);
	window.draw(m_blackBar[0]);
	window.draw(m_blackBar[1]);
	window.draw(m_text);
	window.display();
	return;
}

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

	/*
	float fontSizef = wy * 0.08f;
	unsigned int fontSize = static_cast<unsigned int>(fontSizef);
	float fontRate = fontSizef / fontSize;
	m_text.setCharacterSize(fontSize);
	m_text.setScale(fontRate, fontRate);*/
	float fontSizef = wy * 0.08f;
	float fontRate = fontSizef / 36;
	m_text.setScale(fontRate, fontRate);
	return;
}

}
