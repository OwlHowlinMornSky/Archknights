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
#include <SFML/Window/VideoMode.hpp>

namespace GUI {

/**
* @brief 定义了栈变类型，正数为 Stop，负数为 Pause，0 无效。
*/
namespace Transition {
enum : int {
	Switch = 1, // 1 个目标。当前 Activity 会被弹出，目标将被压入栈并运行。
	Push,       // 1 个目标。其将被压入栈中并运行。
	Pop,        // 0 个目标。当前 Activity 会被弹出，新的栈顶将运行，若栈空则退出。
	PopTo,      // 1 个目标。如果目标在栈中，则栈会被弹至目标，目标在栈顶且将运行，否则无效果。
	PopPush,    // 2 个目标。如果目标0在栈中，则栈会被弹至之，目标1被压入且将运行，否则与 Push 相同。
	Exit        // 0 个目标。强制退出。
};
} // namespace Transition

/**
* @brief 消息框的额外信息。
*/
enum class MBInfo {
	None = 0,
	Info,
	Error
};

enum class WindowType {
	Windowed = 0, // 窗口化。
	Borderless,   // 无边框。
	Fullscreen    // 全屏。
};

/**
 * @brief 接口Carnival: 在 RenderWindow 中运行 Activity 的管理类。
*/
class ICarnival {
public:
	ICarnival() noexcept = default;
	virtual ~ICarnival() = default;

/**
 * @brief 以下内容与操作系统有关。
*/
public:
	/**
	 * @brief 一个系统级消息循环，可以用来在加载时避免窗口被判断为未响应。
	 * @param callerDoWantToHandleThem: 如果为 true，则调用者还应该使用 SFML 的消息循环处理消息。
	 * 否则该函数同时把 SFML 的消息队列清空。
	*/
	virtual void systemMessagePump(bool callerDoWantToHandleThem) const noexcept = 0;

	/**
	 * @brief 显示一个消息框。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	 * @param info: 消息框的额外信息。
	*/
	virtual void systemShowMessageBox(std::string_view title,
								std::string_view text,
								MBInfo info = MBInfo::None) const noexcept = 0;

	/**
	 * @brief 设置 所管理窗口的 关闭按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void windowSetCloseEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void windowSetResizeEnabled(bool enabled) noexcept = 0;
	/**
	 * @brief 设置 所管理窗口的 最小化按钮 是否启用。
	 * @param enabled: True 则启用，否则禁用。
	*/
	virtual void windowSetMinimizeEnabled(bool enabled) noexcept = 0;

	/**
	 * @brief 设为一般窗口。
	*/
	virtual void windowSetWindowed() noexcept = 0;
	/**
	 * @brief 设为无边框窗口。
	*/
	virtual bool windowSetBorderless() noexcept = 0;
	/**
	 * @brief 设为全屏。
	 * @brief （如果分辨率与屏幕不一致，则会丢失焦点，至少我的机器上是这样）OHMS。
	 * @param w: 宽。
	 * @param h: 高。
	*/
	virtual bool windowSetFullscreen(sf::VideoMode mode) noexcept = 0;

	/**
	 * @brief 重置睡眠计数器（此处所说睡眠包括“单纯关闭屏幕”）。
	*/
	virtual void systemResetSleepCounter() noexcept = 0;

	/**
	 * @brief 设置阻止系统睡眠（此处所说睡眠包括“单纯关闭屏幕”）。
	 * @param allowSleep: 是否允许睡眠，设为 false 即 阻止睡眠 且 保持屏幕点亮。
	*/
	virtual void systemTrySetSleepEnabled(bool allowSleep) noexcept = 0;

/**
 * @brief 以下内容与系统无关，在Carnival类中已经实现。
*/
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
	virtual void meDependentActivityStopRunning() noexcept = 0;
	/**
	 * @brief 给 Activity 用的设置变迁的方法。
	 * @param t: 变迁类型。
	*/
	virtual void meActivitySetTransition(int t, uint32_t a0 = 0, uint32_t a1 = 0) noexcept = 0;

	/**
	 * @brief 设置是否把 sizing 消息当作 resized 消息让 Activity 处理。
	 * @param enabled: 是否开启。
	*/
	virtual void setSizingAsResized(bool enabled) noexcept = 0;
	/**
	 * @brief 是否已把 sizing 消息当作 resized 消息让 Activity 处理。
	 * @return 是否开启。
	*/
	virtual bool isSizingAsResized() const noexcept = 0;

	/**
	 * @brief 设置窗口大小（顺便更新View）。
	 * @param w: 宽。
	 * @param h: 高。
	*/
	virtual void windowSetClientSize(uint32_t w, uint32_t h) noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 关闭按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool windowIsCloseEnabled() const noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 Resize 边框 和 最大化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool windowIsResizeEnabled() const noexcept = 0;
	/**
	 * @brief 检测 所管理窗口的 最小化按钮 是否启用。
	 * @return True 则已启用，否则已禁用。
	*/
	virtual bool windowIsMinimizeEnabled() const noexcept = 0;

	/**
	 * @brief 获取当前窗口状态。
	 * @return 窗口状态。
	*/
	virtual WindowType windowGetWindowType() const noexcept = 0;
}; // class ICarnival

} // namespace GUI
