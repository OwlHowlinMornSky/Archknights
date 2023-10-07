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

#include <string>
#include <SFML/Graphics/RenderWindow.hpp>

namespace GUI {

/**
 * @brief 接口Carnival: 在 RenderWindow 中运行 Activity 的管理类。
*/
class ICarnival {
public:
	/**
	 * @brief 消息框的额外信息。
	*/
	enum class MBInfo {
		None = 0,
		Info,
		Error
	};

	/**
	 * @brief 定义了栈变类型，正数为 Stop，负数为 Pause，0 无效。
	*/
	enum Transition : int {
		Switch = 1, // 1 个目标。当前 Activity 会被弹出，目标将被压入栈并运行。
		Push, // 1 个目标。其将被压入栈中并运行。
		Pop, // 0 个目标。当前 Activity 会被弹出，新的栈顶将运行，若栈空则退出。
		PopTo, // 1 个目标。如果目标在栈中，则栈会被弹至目标，目标在栈顶且将运行，否则无效果。
		PopPush, // 2 个目标。如果目标0在栈中，则栈会被弹至之，目标1被压入且将运行，否则与 Push 相同。
		Exit        // 0 个目标。强制退出。
	};

public:
	ICarnival() noexcept = default;
	virtual ~ICarnival() = default;

public:
	/**
	 * @brief 开始执行。
	*/
	virtual void run() noexcept = 0;

	/**
	 * @brief 取得该 Carnival 管理的 RenderWindow。
	*/
	virtual sf::RenderWindow& getRenderWindow() noexcept = 0;

	/**
	 * @brief 给非独立 Activity 用的退出运行的方法。
	*/
	virtual void cancelKeepRunning() noexcept = 0;
	/**
	 * @brief 给 Activity 用的设置变迁的方法。
	 * @param t: 变迁类型。
	*/
	virtual void setTransition(int t, uint32_t a0 = 0, uint32_t a1 = 0) noexcept = 0;

	/**
	 * @brief 设置是否把 sizing 消息当作 resize 消息让 Activity 处理。
	 * @param enabled: 是否开启。
	*/
	virtual void setFullResizeMessage(bool enabled) noexcept = 0;

	/**
	 * @brief 显示一个消息框。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	 * @param info: 消息框的额外信息。
	*/
	virtual void showMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept = 0;

	/**
	 * @brief 检测 所管理窗口的 关闭按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isEnabledClose() const noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isEnabledResize() const noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 最小化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool isEnabledMinimize() const noexcept = 0;

	/**
	 * @brief 设置 所管理窗口的 关闭按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void enableClose(bool enabled) const noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void enableResize(bool enabled) const noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 最小化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void enableMinimize(bool enabled) const noexcept = 0;

	/**
	 * @brief 设置无边框窗口。
	 * @param full: 是否启用无边框窗口。
	*/
	virtual void setFullwindow(bool full) noexcept = 0;

	/**
	 * @brief 设置全屏。
	 * @param full: 是否启用全屏。
	*/
	virtual void setFullscreen(bool full) noexcept = 0;

	/**
	 * @brief 一个系统级消息循环，可以用来在加载时避免窗口被判断为未响应。
	*/
	virtual void systemMessagePump() const noexcept = 0;
}; // class ICarnival

} // namespace GUI
