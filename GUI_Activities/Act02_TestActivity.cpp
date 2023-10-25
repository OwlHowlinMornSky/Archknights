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
#include "Act02_TestActivity.h"
#include "Act01_DefaultEntry.h"

#include <iostream>

namespace Activity {

Act02_TestActivity::Act02_TestActivity() noexcept :
	m_paused(false),
	r_wnd(nullptr),
	m_disableResize(false),
	m_disableMinimize(false),
	m_disableClose(false) {
	std::cout << "TestActivity: Construct." << std::endl;

	auto& modes = sf::VideoMode::getFullscreenModes();
	for (const auto& mode : modes) {
		std::cout << "W: " << mode.width << ", H: " << mode.height << ", bPP: " << mode.bitsPerPixel << std::endl;
	}
	m_modes = modes;
	m_modeI = 0;
	return;
}

Act02_TestActivity::~Act02_TestActivity() noexcept {
	std::cout << "TestActivity: Destruct." << std::endl;
}

bool Act02_TestActivity::start(GUI::Window& wnd) noexcept {
	r_wnd = &wnd;

	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });
	updateSize();

	m_tex.loadFromFile("assets/TestActivity.png");
	m_sp.setTexture(m_tex, true);
	return true;
}

void Act02_TestActivity::stop() noexcept {
	std::cout << "TestActivity: stop." << std::endl;
}

void Act02_TestActivity::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->stop();
		break;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			r_wnd->setActivity(std::make_unique<Act01_DefaultEntry>());
			break;
		case sf::Keyboard::Num1:
			r_wnd->setMinimizeEnabled(!r_wnd->isMinimizeEnabled());
			break;
		case sf::Keyboard::Num2:
			r_wnd->setResizeEnabled(!r_wnd->isResizeEnabled());
			break;
		case sf::Keyboard::Num3:
			r_wnd->setCloseEnabled(!r_wnd->isCloseEnabled());
			break;
		case sf::Keyboard::Grave:
			r_wnd->setSizingAsResized(!r_wnd->isSizingAsResized());
			break;
		case sf::Keyboard::F1:
			r_wnd->setWindowed();
			break;
		case sf::Keyboard::F2:
			r_wnd->setBorderless();
			break;
		case sf::Keyboard::F3:
			r_wnd->setFullscreen(sf::VideoMode::getDesktopMode());
			break;
		case sf::Keyboard::F4:
			r_wnd->setFullscreen(m_modes.at(m_modeI));
			break;
		case sf::Keyboard::Left:
			if (m_modeI > 0) {
				m_modeI--;
				auto& mode = m_modes.at(m_modeI);
				std::cout << "W: " << mode.width << ", H: " << mode.height << ", bPP: " << mode.bitsPerPixel << std::endl;
			}
			break;
		case sf::Keyboard::Right:
			if (m_modeI < m_modes.size() - 1) {
				m_modeI++;
				auto& mode = m_modes.at(m_modeI);
				std::cout << "W: " << mode.width << ", H: " << mode.height << ", bPP: " << mode.bitsPerPixel << std::endl;
			}
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

void Act02_TestActivity::update(sf::Time deltaTime) {
	float dt = deltaTime.asSeconds();
	if (dt > 0.1f)
		dt = 0.1f;
	if (!m_paused)
		m_shape.rotate(dt * 180.0f);

	r_wnd->clear(sf::Color::Green);
	r_wnd->draw(m_shape);
	r_wnd->draw(m_sp);
	r_wnd->display();
	return;
}

void Act02_TestActivity::OnEnterSysloop() noexcept {
	//m_paused = true;
}

void Act02_TestActivity::OnExitSysloop() noexcept {
	//m_paused = false;
}

void Act02_TestActivity::updateSize() noexcept {
	auto size = r_wnd->getClientSize();
	m_shape.setPosition(size.x / 2.0f, size.y / 2.0f);
}

} // namespace Activity
