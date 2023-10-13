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

#include <iostream>

namespace Activity {

Act02_TestActivity::Act02_TestActivity(uint32_t n) noexcept :
	m_id(n),
	m_paused(false),
	ref_carnival(nullptr),
	m_disableResize(false),
	m_disableMinimize(false),
	m_disableClose(false) {
	std::cout << "TestActivity " << m_id << ": Construct." << std::endl;

	auto& modes = sf::VideoMode::getFullscreenModes();
	for (const auto& mode : modes) {
		std::cout << "W: " << mode.width << ", H: " << mode.height << ", bPP: " << mode.bitsPerPixel << std::endl;
	}
	m_modes = modes;
	m_modeI = 0;
	return;
}

Act02_TestActivity::~Act02_TestActivity() noexcept {
	std::cout << "TestActivity " << m_id << ": Destruct." << std::endl;
}

void Act02_TestActivity::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;

	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });
	updateSize();

	m_tex.loadFromFile("assets/TestActivity.png");
	m_sp.setTexture(m_tex, true);

	std::cout << "TestActivity " << m_id << ": start, " << ref_carnival << "." << std::endl;
	return;
}

void Act02_TestActivity::stop() noexcept {
	std::cout << "TestActivity " << m_id << ": stop." << std::endl;
}

void Act02_TestActivity::pause() noexcept {
	std::cout << "TestActivity " << m_id << ": pause." << std::endl;
}

void Act02_TestActivity::resume() noexcept {
	updateSize();
	std::cout << "TestActivity " << m_id << ": resume." << std::endl;
}

uint32_t Act02_TestActivity::getID() noexcept {
	return m_id;
}

void Act02_TestActivity::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			ref_carnival->meActivitySetTransition(GUI::Transition::Pop);
			ref_carnival->meDependentActivityStopRunning();
			break;
		case sf::Keyboard::Num1:
			ref_carnival->windowSetMinimizeEnabled(!ref_carnival->windowIsMinimizeEnabled());
			break;
		case sf::Keyboard::Num2:
			ref_carnival->windowSetResizeEnabled(!ref_carnival->windowIsResizeEnabled());
			break;
		case sf::Keyboard::Num3:
			ref_carnival->windowSetCloseEnabled(!ref_carnival->windowIsCloseEnabled());
			break;
		case sf::Keyboard::Grave:
			ref_carnival->setSizingAsResized(!ref_carnival->isSizingAsResized());
			break;
		case sf::Keyboard::F1:
			ref_carnival->windowSetWindowed();
			break;
		case sf::Keyboard::F2:
			ref_carnival->windowSetBorderless();
			break;
		case sf::Keyboard::F3:
			ref_carnival->windowSetFullscreen(sf::VideoMode::getDesktopMode());
			break;
		case sf::Keyboard::F4:
			ref_carnival->windowSetFullscreen(m_modes.at(m_modeI));
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

void Act02_TestActivity::update(sf::RenderWindow& window, sf::Time deltaTime) {
	float dt = deltaTime.asSeconds();
	if (dt > 0.1f)
		dt = 0.1f;
	if (!m_paused)
		m_shape.rotate(dt * 180.0f);

	window.clear(sf::Color::Green);
	window.draw(m_shape);
	window.draw(m_sp);
	window.display();
	return;
}

void Act02_TestActivity::onEnterSysloop() noexcept {
	m_paused = true;
}

void Act02_TestActivity::onExitSysloop() noexcept {
	m_paused = false;
}

void Act02_TestActivity::updateSize() noexcept {
	auto size = ref_carnival->getRenderWindow().getSize();
	m_shape.setPosition(size.x / 2.0f, size.y / 2.0f);
	//ref_carnival->getRenderWindow().setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)size.x, (float)size.y)));
}

} // namespace Activity
