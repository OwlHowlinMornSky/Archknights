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
#pragma once

#include <string_view>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

namespace GUI {

class Activity;

/**
 * @brief �������͡�
*/
enum class WindowStatus {
	Windowed = 0, // ���ڻ���
	Borderless,   // �ޱ߿�
	Fullscreen    // ȫ����
};

/**
 * @brief ���ڡ�
*/
class Window :
	protected sf::RenderWindow {
	friend class Carnival;
public:
	Window() noexcept;
	virtual ~Window() noexcept;

public:
	virtual bool Create(bool foreground) noexcept;
	virtual void Close() noexcept;

public:
	/**
	 * @brief �޸Ĵ��ڵ� Activity��
	 * @param activity: ������ Activity��
	 * @return �޸��Ƿ�ɹ���
	*/
	bool changeActivity(std::unique_ptr<Activity>&& activity) noexcept;

	/**
	 * @brief �����Ƿ�� sizing ��Ϣ���� resized ��Ϣ�� Activity ����
	 * @param enabled: �Ƿ�����
	*/
	void setSizingAsResized(bool enabled) noexcept;
	/**
	 * @brief �Ƿ��Ѱ� sizing ��Ϣ���� resized ��Ϣ�� Activity ����
	 * @return �Ƿ�����
	*/
	bool isSizingAsResized() const noexcept;

	/**
	 * @brief ���ô��ڴ�С��˳�����View����
	 * @param size: ��С��
	*/
	void setSize(sf::Vector2u size) noexcept;

	/**
	 * @brief ��ȡ��ǰ����״̬��
	 * @return ����״̬��
	*/
	WindowStatus getWindowStatus() const noexcept;

public:
	/**
	 * @brief ���� ���ڵĹرհ�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void setCloseEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ���� ���ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void setResizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ���� ���ڵ���С����ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void setMinimizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ��� ���ڵĹرհ�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isCloseEnabled() const noexcept = 0;
	/**
	 * @brief ��� ���ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isResizeEnabled() const noexcept = 0;
	/**
	 * @brief ��� ���ڵ� ��С����ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isMinimizeEnabled() const noexcept = 0;

	/**
	 * @brief ��ʾһ����Ϣ�򡣸���Ϣ�� Information ͼ�꣬�������ڴ��ڡ�
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text) const noexcept = 0;
	/**
	 * @brief ��ʾһ����Ϣ�򡣸���Ϣ�� Information ͼ�꣬�������ڴ��ڡ�
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	*/
	virtual void showMessageBox(std::wstring_view title, std::wstring_view text) const noexcept = 0;

	/**
	 * @brief ��Ϊ���ڻ����ڡ�
	*/
	virtual void setWindowed() noexcept = 0;
	/**
	 * @brief ��Ϊ�ޱ߿򴰿ڡ�
	*/
	virtual bool setBorderless() noexcept = 0;
	/**
	 * @brief ��Ϊȫ����
	 * @brief OpenGL �޷�ʵ�������Ķ�ռȫ����������ʵ�����һ���ܸı���Ļ�ֱ��ʵ��ޱ߿�
	 * @brief ������ֱ�������Ļ��һ�£���ᶪʧ���㣬�����ҵĻ�������������OHMS��
	 * @param w: ��
	 * @param h: �ߡ�
	*/
	virtual bool setFullscreen(sf::VideoMode mode) noexcept = 0;

public:
	sf::Vector2u getSize() const {
		return RenderWindow::getSize();
	}
	void setVerticalSyncEnabled(bool enabled) {
		return RenderWindow::setVerticalSyncEnabled(enabled);
	}
	void setFramerateLimit(unsigned int limit) {
		return RenderWindow::setFramerateLimit(limit);
	}
	bool setActive(bool active = true) {
		return RenderWindow::setActive(active);
	}
	void display() {
		return RenderWindow::display();
	}
	void clear(const sf::Color& color = sf::Color(0, 0, 0, 255)) {
		return RenderWindow::clear(color);
	}
	void draw(
		const sf::Drawable& drawable,
		const sf::RenderStates& states = sf::RenderStates::Default
	) {
		return RenderWindow::draw(drawable, states);
	}
	void draw(
		const sf::Vertex* vertices,
		std::size_t vertexCount,
		sf::PrimitiveType type,
		const sf::RenderStates& states = sf::RenderStates::Default
	) {
		return RenderWindow::draw(vertices, vertexCount, type, states);
	}
	void draw(
		const sf::VertexBuffer& vertexBuffer,
		const sf::RenderStates& states = sf::RenderStates::Default
	) {
		return RenderWindow::draw(vertexBuffer, states);
	}
	void draw(
		const sf::VertexBuffer& vertexBuffer,
		std::size_t firstVertex,
		std::size_t vertexCount,
		const sf::RenderStates& states = sf::RenderStates::Default
	) {
		return RenderWindow::draw(vertexBuffer, firstVertex, vertexCount, states);
	}

protected:
	void handleEvent();
	void update(sf::Time dtime);
	void onSystemLoop(bool enter);
	virtual void checkSizeInSystemLoop() noexcept = 0;
public:
	/**
	 * @brief ��ڵȴ��رա�
	*/
	void setWaitingForStop() noexcept {
		m_waitToStop = true;
	}
	bool isWaitingForStop() noexcept {
		return m_waitToStop;
	}

protected:
	bool m_waitToStop; // �ȴ��رյı�ǡ�
	bool m_created; // �Ѿ� Create �ı�ǡ�
	bool m_sizingAsSized; // �� sizing ��Ϣ��Ϊ resized ��Ϣ�� Activity ����ı�ǡ�
	WindowStatus m_windowStatus; // ����״̬��
	std::unique_ptr<Activity> m_activity; // Activity��
};

} // namespace GUI

