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
 * @brief 窗口类型。
*/
enum class WindowStatus {
	Windowed = 0, // 窗口化。
	Borderless,   // 无边框。
	Fullscreen    // 全屏。
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
	 * @brief 设置是否把 sizing 消息当作 resized 消息让 Activity 处理。
	 * @param enabled: 是否开启。
	*/
	void setSizingAsResized(bool enabled) noexcept;
	/**
	 * @brief 是否已把 sizing 消息当作 resized 消息让 Activity 处理。
	 * @return 是否开启。
	*/
	bool isSizingAsResized() const noexcept;

	/**
	 * @brief 设置窗口大小（顺便更新View）。
	 * @param size: 大小。
	*/
	void setSize(sf::Vector2u size) noexcept;
	/**
	 * @brief 获取当前窗口状态。
	 * @return 窗口状态。
	*/
	WindowStatus getWindowStatus() const noexcept;

public:
	virtual sf::Vector2u getClientSize() const noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 关闭按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void setCloseEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void setResizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 最小化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void setMinimizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 关闭按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isCloseEnabled() const noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isResizeEnabled() const noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 最小化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isMinimizeEnabled() const noexcept = 0;

	/**
	 * @brief 显示一个消息框。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text) const noexcept = 0;
	/**
	 * @brief 显示一个消息框。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	*/
	virtual void showMessageBox(std::wstring_view title, std::wstring_view text) const noexcept = 0;

	/**
	 * @brief 设为一般窗口。
	*/
	virtual void setWindowed() noexcept = 0;
	/**
	 * @brief 设为无边框窗口。
	*/
	virtual bool setBorderless() noexcept = 0;
	/**
	 * @brief 设为全屏。
	 * @brief （如果分辨率与屏幕不一致，则会丢失焦点，至少我的机器上是这样）OHMS。
	 * @param w: 宽。
	 * @param h: 高。
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

