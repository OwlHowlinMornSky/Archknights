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

namespace ME {

Window::Window() :
	m_created(false),
	m_sizingAsSized(false),
	m_waitToStop(false),
	m_waitToChange(false),
	m_windowStatus(ME::WindowStatus::Windowed) {}

Window::~Window() noexcept {
	Close();
	return;
}

bool Window::Create(bool foreground) noexcept {
	if (!isOpen()) return false;
	m_created = true;
	return true;
}

void Window::Close() noexcept {
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

bool Window::available() const {
	// 确保窗口已经 Create 并且含有有效 Activity。
	return m_created && (m_activity != nullptr || m_waitToChange);
}

} // namespace GUI
