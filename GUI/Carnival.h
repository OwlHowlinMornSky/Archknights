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
#include "Window.h"

namespace GUI {

/**
 * @brief Carnival: 管理所有窗口的类。
*/
class Carnival {
protected:
	/**
	 * @brief 构造函数。分为 单窗口模式 和 多窗口模式。
	 * @param mutipleWindows: 是否启用多窗口。
	*/
	Carnival(bool mutipleWindows);
public:
	/**
	 * @brief 析构函数。
	*/
	virtual ~Carnival() noexcept;

public:
	/**
	 * @brief 初始化。Carnival 只能有一个实例，在该方法中构造。
	 * @param mutipleWindows: 是否启用多窗口。
	*/
	static void setup(bool mutipleWindows = true) noexcept;
	/**
	 * @brief 获取 Carnival 的唯一实例。调用前必须先 setup。
	 * @return Carnival 实例的引用。
	*/
	static Carnival& instance() noexcept;
	/**
	 * @brief 清理。销毁 Carnival 实例。
	*/
	static void drop() noexcept;

public:
	/**
	 * @brief 开始执行。
	*/
	void run() noexcept;

	/**
	 * @brief 向 Carnival 添加窗口。在单窗口模式下添加第二个及以后的窗口都会失败。
	 * @param wnd: 窗口，必须已经 Create，且含有有效 Activity。
	 * @return 添加是否成功。
	*/
	bool pushWindow(std::unique_ptr<Window>&& wnd);
	/**
	 * @brief 让 Carnival 视条件创建一个 Window，并初始含有给定的 Activity。
	 * @brief 在单窗口模式下创建第二个及以后的窗口都会失败。
	 * @param activity: 给定的 Activity。
	 * @return 创建是否成功。
	*/
	virtual bool emplaceWindow(std::unique_ptr<Activity>&& activity, bool foreground = false) = 0;

public:
	/**
	 * @brief 显示一个消息框。该 消息框 将含 Error 图标，且不依附于窗口。
	 * @param title: 消息框的标题。
	 * @param text: 消息框的内容。
	*/
	virtual void showErrorMessageBox(std::string_view title, std::string_view text) const noexcept = 0;

	/**
	 * @brief 重置系统睡眠计数器（此处所说睡眠包括“单纯关闭屏幕”）。
	*/
	virtual void resetSleepCounter() noexcept = 0;
	/**
	 * @brief 设置阻止系统睡眠（此处所说睡眠包括“单纯关闭屏幕”）。
	 * @param allowSleep: 是否允许睡眠，设为 false 即 阻止睡眠 且 保持屏幕点亮。
	*/
	virtual void setSleepEnabled(bool allowSleep) noexcept = 0;

protected:
	/**
	 * @brief 移除 等待关闭 的窗口。
	*/
	void removeStoppedWindows() noexcept;
	/**
	 * @brief 系统循环中的 Idle 回调。
	*/
	void onIdle();
	/**
	 * @brief 系统循环中的 Idle 回调（单窗口模式）。
	*/
	void onIdleSingle();
	/**
	 * @brief 系统循环通知回调。
	 * @param enter: ture 为 进入，false 为退出。
	*/
	void onSystemLoop(bool enter);
	/**
	 * @brief 系统循环通知回调（单窗口模式）。
	 * @param enter: ture 为 进入，false 为退出。
	*/
	void onSystemLoopSingle(bool enter);

	/**
	 * @brief 消息循环。
	*/
	virtual void systemMessagePump() const noexcept = 0;

protected:
	bool m_mutipleWindows; // 启用多窗口模式的标记。【只在构造时修改！】
	sf::Clock m_clk; // 时钟。
	std::unique_ptr<Window> m_singleWnd; // 单窗口模式使用的。
	std::list<std::unique_ptr<Window>> m_wnds; // 多窗口模式使用的。

	static std::unique_ptr<Carnival> s_instance; // 唯一实例。
}; // class ICarnival

} // namespace GUI
