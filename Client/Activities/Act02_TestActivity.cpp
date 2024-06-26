﻿/*
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
#include "Act02_TestActivity.h"

#ifdef _DEBUG
#include "Act01_DefaultEntrance.h"
#include <MysteryEngine/Client/Carnival.h>
#include <iostream>

namespace Activity {

Act02_TestActivity::Act02_TestActivity() :
	m_paused(false),
	m_modeI(0) {
	return;
}

Act02_TestActivity::~Act02_TestActivity() noexcept {
	return;
}

bool Act02_TestActivity::prepare(ME::Window& wnd) noexcept {
	r_wnd = wnd;
	// 加载调试信息
	m_tex.loadFromFile("assets/TestActivity.png");
	m_sp.setTexture(m_tex, true);
	return true;
}

void Act02_TestActivity::start() noexcept {
	// 复制所有屏幕模式
	m_modes = sf::VideoMode::getFullscreenModes();
	m_modeI = 0;
	noticeSelectedMode();

	// 设置中心正方形
	m_shape.setFillColor(sf::Color::Red);
	m_shape.setSize({ 100.0f, 100.0f });

	// 更新布局
	updateSize();
	return;
}

void Act02_TestActivity::stop() noexcept {
	r_wnd();
	return;
}

bool Act02_TestActivity::handleEvent(const sf::Event& evt) {
	switch (evt.type) {
	case sf::Event::Closed:
		r_wnd->setWaitingForStop();
		return 1;
	case sf::Event::KeyPressed:
		switch (evt.key.code) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			r_wnd->changeActivity(std::make_unique<Act01_DefaultEntrance>());
			return 1;
		case sf::Keyboard::Num1:
			r_wnd->setMinimizeEnabled(!r_wnd->isMinimizeEnabled());
			std::cout
				<< "Test: Minimize Enabled: "
				<< std::boolalpha << r_wnd->isMinimizeEnabled()
				<< std::endl;
			break;
		case sf::Keyboard::Num2:
			r_wnd->setResizeEnabled(!r_wnd->isResizeEnabled());
			std::cout
				<< "Test: Resize Enabled: "
				<< std::boolalpha << r_wnd->isResizeEnabled()
				<< std::endl;
			break;
		case sf::Keyboard::Num3:
			r_wnd->setCloseEnabled(!r_wnd->isCloseEnabled());
			std::cout
				<< "Test: Close Enabled: "
				<< std::boolalpha << r_wnd->isCloseEnabled()
				<< std::endl;
			break;
		case sf::Keyboard::Grave:
			r_wnd->setSizingAsResized(!r_wnd->isSizingAsResized());
			std::cout
				<< "Test: SizingAsResized: "
				<< std::boolalpha << r_wnd->isSizingAsResized()
				<< std::endl;
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
		case sf::Keyboard::F:
			ME::Carnival::Instance().emplaceWindow(std::make_unique<Act02_TestActivity>());
			break;
		case sf::Keyboard::Left:
			if (m_modeI > 0) {
				m_modeI--;
				noticeSelectedMode();
			}
			break;
		case sf::Keyboard::Right:
			if (m_modeI < m_modes.size() - 1) {
				m_modeI++;
				noticeSelectedMode();
			}
			break;
		default:
			break;
		}
		break;
	case sf::Event::Resized:
		updateSize();
		break;
	default:
		break;
	}
	return 0;
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

void Act02_TestActivity::onEnterSysloop() noexcept {
	//m_paused = true;
	return;
}

void Act02_TestActivity::onExitSysloop() noexcept {
	m_paused = false;
	return;
}

void Act02_TestActivity::updateSize() noexcept {
	auto& view = r_wnd->getView();
	sf::Vector2f size = view.getSize();
	m_shape.setPosition(size.x / 2.0f, size.y / 2.0f);
	return;
}

void Act02_TestActivity::noticeSelectedMode() noexcept {
	if (!m_modes.empty()) {
		std::cout
			<< "Test: Selected Mode: "
			<< "W: " << m_modes[m_modeI].width
			<< ", H: " << m_modes[m_modeI].height
			<< ", bPP: " << m_modes[m_modeI].bitsPerPixel
			<< std::endl;
	}
	return;
}

} // namespace Activity
#endif // _DEBUG
