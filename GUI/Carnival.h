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

#include <list>
#include <functional>
#include "Window.h"

namespace GUI {

/**
 * @brief 闲置状态的回调。
 * @brief 这玩意存在的目的主要就是防止 移动窗口 和 改变窗口大小 时 把主逻辑卡住。
 * @brief 方法是 设置定时器，定时器的回调 MyTimerProc 会调用这个玩意。
 * @brief 所以在主逻辑变迁的时候要改这个，不过用完一定记得改回来（因为有初始空函数）。
*/
extern std::function<void()> OnIdle;
/**
 * @brief 进入或退出系统循环时的回调。参数为 true 则为 进入，false 为退出。
 * @brief 如上所述，就是 开始移动窗口 或者 开始改变窗口大小 时调用的。
 * @brief 可以通知你进行一些处理（比如暂停游戏什么的），
 * @brief 避免 退出移动或改变大小 的时候 突然出现一个 长达数秒 甚至数十秒 的 帧。
*/
extern std::function<void(bool)> OnSystemLoop;

/**
 * @brief Carnival: 在 RenderWindow 中运行 Activity 的管理类。
*/
class Carnival {
public:
	Carnival(bool mutipleWindows);
	virtual ~Carnival() noexcept;

public:
	static void setup(bool mutipleWindows = true) noexcept;
	static Carnival& instance() noexcept;
	static void drop() noexcept;

public:
	/**
	 * @brief 开始执行。
	*/
	void run() noexcept;

	bool pushWindow(std::unique_ptr<Window>&& wnd);
	virtual bool emplaceWindow(std::unique_ptr<Activity>&& activity) = 0;

public:
	/**
	 * @brief 显示一个消息框。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	*/
	virtual void showErrorMessageBox(std::string_view title, std::string_view text) const noexcept = 0;

	/**
	 * @brief 重置睡眠计数器（此处所说睡眠包括“单纯关闭屏幕”）。
	*/
	virtual void resetSleepCounter() noexcept = 0;
	/**
	 * @brief 设置阻止系统睡眠（此处所说睡眠包括“单纯关闭屏幕”）。
	 * @param allowSleep: 是否允许睡眠，设为 false 即 阻止睡眠 且 保持屏幕点亮。
	*/
	virtual void setSleepEnabled(bool allowSleep) noexcept = 0;

protected:
	void removeStoppedWindows() noexcept;
	void onIdle();
	void onIdleSingle();
	void onSystemLoop(bool enter);
	void onSystemLoopSingle(bool enter);

	virtual void systemMessagePump() const noexcept = 0;

protected:
	bool m_mutipleWindows;
	sf::Clock m_clk;
	std::unique_ptr<Window> m_singleWnd;
	std::list<std::unique_ptr<Window>> m_wnds;

	static std::unique_ptr<Carnival> s_instance;
}; // class ICarnival

} // namespace GUI
