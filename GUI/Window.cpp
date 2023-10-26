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
#include "Activity.h"
#include "Window.h"

namespace GUI {

Window::Window() noexcept :
	m_created(false),
	m_sizingAsSized(false),
	m_waitToStop(false),
	m_windowStatus(GUI::WindowStatus::Windowed) {}

Window::~Window() noexcept {
	if (m_activity != nullptr)
		m_activity->stop();
	m_activity.reset();
	return;
}

void Window::handleEvent() {
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
		m_activity->handleEvent(evt);
	}
	return;
}

void Window::update(sf::Time dtime) {
	return m_activity->update(dtime);
}

bool Window::setActivity(std::unique_ptr<Activity>&& activity) noexcept {
	if (activity == nullptr)
		return false;
	if (!activity->start(*this))
		return false;
	if (m_activity != nullptr)
		m_activity->stop();
	m_activity = std::move(activity);
	sf::Event evt;
	while (pollEvent(evt))
		;
	return true;
}

void Window::OnSystemLoop(bool enter) {
	if (enter)
		m_activity->OnEnterSysloop();
	else
		m_activity->OnExitSysloop();
	return;
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

} // namespace GUI
