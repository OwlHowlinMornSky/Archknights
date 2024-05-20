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
#pragma once

#include <string_view>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <MysteryEngine/Core/MEAPI.h>
#include <MysteryEngine/Client/Activity.h>

namespace ME {

/**
 * @brief 窗口类型。
*/
enum class ME_API WindowStatus {
	Windowed = 0, // 窗口化。
	Borderless,   // 无边框。
	Fullscreen    // 全屏。
}; // enum class WindowStatus

/**
 * @brief 窗口。
*/
class ME_API Window :
	public sf::RenderWindow {
public:
	Window();
	virtual ~Window() noexcept override;
	
	/**
	 * @brief 创建一个系统特定的窗口。
	 * @param cmd: 显示参数。
	 * @return 创建的窗口。如果失败可能为nullptr。
	*/
	static std::unique_ptr<ME::Window> Create1Window(int cmd);

public:
	/**
	 * @brief 创建窗口。
	 * @param foreground: 是否显示在前景。
	 * @return 
	*/
	virtual bool create(bool foreground) noexcept;
	/**
	 * @brief 销毁窗口。
	*/
	virtual void close() noexcept;

public:
	/**
	 * @brief 修改窗口的 Activity，将在下次 handleEvent 之前生效。
	 * @param activity: 给定的 Activity。
	 * @return 修改是否成功。
	*/
	bool changeActivity(std::unique_ptr<Activity>&& activity) noexcept;

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
	/**
	 * @brief 设置 窗口的关闭按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void setCloseEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 设置 窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void setResizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 设置 窗口的最小化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void setMinimizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 检测 窗口的关闭按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isCloseEnabled() const noexcept = 0;
	/**
	 * @brief 检测 窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isResizeEnabled() const noexcept = 0;
	/**
	 * @brief 检测 窗口的 最小化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isMinimizeEnabled() const noexcept = 0;

	/**
	 * @brief 显示一个消息框。该消息框含 Information 图标，且依附于窗口。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	*/
	virtual void showMessageBox(std::string_view title, std::string_view text) const noexcept = 0;
	/**
	 * @brief 显示一个消息框。该消息框含 Information 图标，且依附于窗口。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	*/
	virtual void showMessageBox(std::wstring_view title, std::wstring_view text) const noexcept = 0;

	/**
	 * @brief 获取窗口实际大小（消息循环中也正确）。
	 */
	virtual sf::Vector2u getRealtimeSize() const noexcept = 0;

	/**
	 * @brief 设为窗口化窗口。
	*/
	virtual void setWindowed() noexcept = 0;
	/**
	 * @brief 设为无边框窗口。
	*/
	virtual bool setBorderless() noexcept = 0;
	/**
	 * @brief 设为全屏。
	 * @brief OpenGL 无法实现真正的独占全屏，所以其实这就是一个能改变屏幕分辨率的无边框。
	 * @brief （如果分辨率与屏幕不一致，则会丢失焦点，至少我的机器上是这样）OHMS。
	 * @param w: 宽。
	 * @param h: 高。
	*/
	virtual bool setFullscreen(sf::VideoMode mode) noexcept = 0;

public:
	using sf::RenderWindow::setIcon;
	void setIcon(const sf::Image& icon);

public:
	bool available() const;
	void handleEvent();
	void update(sf::Time dtime);
	void onSystemLoop(bool enter);
	virtual void checkSizeInSystemLoop() noexcept = 0;
public:
	/**
	 * @brief 令窗口等待关闭。
	*/
	void setWaitingForStop() noexcept;
	/**
	 * @brief 读取当前窗口是否等待关闭。
	 * @return 是否等待关闭。
	*/
	bool isWaitingForStop() const noexcept;

protected:
	bool m_waitToStop; // 等待关闭的标记。
	bool m_waitToChange; // 等待改变 Activity。
	bool m_created; // 已经 Create 的标记。
	bool m_sizingAsSized; // 将 sizing 消息作为 resized 消息让 Activity 处理的标记。
	WindowStatus m_windowStatus; // 窗口状态。
	std::unique_ptr<Activity> m_activity; // Activity。
	std::unique_ptr<Activity> m_nextActivity; // 等待生效的 Activity。
}; // class Window

} // namespace GUI
