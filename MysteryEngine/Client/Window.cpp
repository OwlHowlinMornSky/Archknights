/*
*    Mystery Engine
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
#include <MysteryEngine/Client/Window.h>

#ifdef SFML_SYSTEM_WINDOWS

#include "WindowWin32.h"
std::unique_ptr<ME::Window> ME::Window::Create1Window(int cmd) { // this will be different on each system.
	std::unique_ptr<ME::WindowWin32> window = std::make_unique<ME::WindowWin32>();
	// Create window and run.
	if (window->Create(cmd)) {
		return std::move(window);
	}
	window.reset();
	return nullptr;
}

#endif

namespace ME {

Window::Window() :
	m_created(false),
	m_sizingAsSized(false),
	m_waitToStop(false),
	m_waitToChange(false),
	m_windowStatus(ME::WindowStatus::Windowed) {}

Window::~Window() noexcept {
	close();
	return;
}

bool Window::create(bool foreground) noexcept {
	if (!isOpen()) return false;
	m_created = true;
	return true;
}

void Window::close() noexcept {
	if (m_activity != nullptr) {
		m_activity->stop();
		m_activity.reset();
	}
	RenderWindow::close();
	return;
}

bool Window::changeActivity(std::unique_ptr<Activity>&& activity) noexcept {
	if (activity == nullptr)
		return false;
	m_waitToChange = true;
	m_nextActivity = std::move(activity);
	return true;
}

void Window::setSizingAsResized(bool enabled) noexcept {
	m_sizingAsSized = enabled;
	return;
}

bool Window::isSizingAsResized() const noexcept {
	return m_sizingAsSized;
}

void Window::setSize(sf::Vector2u size) noexcept {
	if (m_windowStatus == WindowStatus::Windowed) {
		RenderWindow::setSize(size);
		setView(
			sf::View(
				sf::FloatRect(
					0.0f, 0.0f,
					static_cast<float>(size.x),
					static_cast<float>(size.y)
				)
			)
		);
	}
	return;
}

WindowStatus Window::getWindowStatus() const noexcept {
	return m_windowStatus;
}

void Window::setIcon(const sf::Image& icon) {
	return RenderWindow::setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

bool Window::available() const {
	// 确保窗口已经 Create 并且含有有效 Activity。
	return m_created && (m_activity != nullptr || m_waitToChange);
}

void Window::handleEvent() {
	while (m_waitToChange) {
		m_waitToChange = false;
		std::unique_ptr<Activity> tmp = std::move(m_nextActivity);
		if (tmp->prepare(*this)) {
			if (m_activity != nullptr)
				m_activity->stop();
			m_activity = std::move(tmp);
			m_activity->start();
		}
	}
	sf::Event evt;
	while (pollEvent(evt)) {
		if (evt.type == sf::Event::Resized) {
			setView(
				sf::View(
					sf::FloatRect(
						0.0f, 0.0f,
						static_cast<float>(evt.size.width),
						static_cast<float>(evt.size.height)
					)
				)
			);
		}
		if (m_activity->handleEvent(evt)) {
			while (pollEvent(evt))
				;
			break;
		}
	}
	return;
}

void Window::update(sf::Time dtime) {
	return m_activity->update(dtime);
}

void Window::onSystemLoop(bool enter) {
	if (enter)
		m_activity->OnEnterSysloop();
	else
		m_activity->OnExitSysloop();
	return;
}

void Window::setWaitingForStop() noexcept {
	m_waitToStop = true;
}

bool Window::isWaitingForStop() const noexcept {
	return m_waitToStop;
}

} // namespace ME
