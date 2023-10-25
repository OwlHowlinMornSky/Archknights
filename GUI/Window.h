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

class Window :
	public sf::RenderWindow {
	friend class Carnival;
public:
	Window() noexcept;
	virtual ~Window() noexcept;

public:
	virtual bool Create() noexcept = 0;
	virtual void Close() noexcept = 0;

public:
	void update(sf::Time dtime);

	bool setActivity(std::unique_ptr<Activity>&& activity) noexcept;

	void OnSystemLoop(bool enter);

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
	virtual sf::Vector2u getClientSize() const noexcept = 0;
	/**
	 * @brief ���� �������ڵ� �رհ�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void setCloseEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ���� �������ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void setResizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ���� �������ڵ� ��С����ť �Ƿ����á�
	 * @param enabled: True �����ã�������á�
	*/
	virtual void setMinimizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief ��� �������ڵ� �رհ�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isCloseEnabled() const noexcept = 0;
	/**
	 * @brief ��� �������ڵ� Resize �߿� �� ��󻯰�ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isResizeEnabled() const noexcept = 0;
	/**
	 * @brief ��� �������ڵ� ��С����ť �Ƿ����á�
	 * @return True �������ã������ѽ��á�
	*/
	virtual bool isMinimizeEnabled() const noexcept = 0;

	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text) const noexcept = 0;
	/**
	 * @brief ��ʾһ����Ϣ��
	 * @param title: ��Ϣ��ı��⡣
	 * @param text: ��Ϣ������ݡ�
	*/
	virtual void showMessageBox(std::wstring_view title, std::wstring_view text) const noexcept = 0;

	/**
	 * @brief ��Ϊһ�㴰�ڡ�
	*/
	virtual void setWindowed() noexcept = 0;
	/**
	 * @brief ��Ϊ�ޱ߿򴰿ڡ�
	*/
	virtual bool setBorderless() noexcept = 0;
	/**
	 * @brief ��Ϊȫ����
	 * @brief ������ֱ�������Ļ��һ�£���ᶪʧ���㣬�����ҵĻ�������������OHMS��
	 * @param w: ��
	 * @param h: �ߡ�
	*/
	virtual bool setFullscreen(sf::VideoMode mode) noexcept = 0;

public:
	void stop() noexcept {
		m_waitToStop = true;
		return;
	}
	bool isWaitingForStop() noexcept {
		return m_waitToStop;
	}

protected:
	virtual void checkSizeInSystemLoop() noexcept = 0;

protected:
	bool m_created;
	bool m_sizingAsSized;
	bool m_waitToStop;
	WindowStatus m_windowStatus;
	std::unique_ptr<Activity> m_activity;
};

} // namespace GUI

