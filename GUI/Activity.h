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

#include "Window.h"

namespace GUI {

/**
 * @brief Activity: 于 Window 中执行的单个界面。
*/
class Activity {
	friend class Window;
public:
	/**
	 * @brief 默认构造函数。
	*/
	Activity() = default;
	/**
	 * @brief 默认析构函数。
	*/
	virtual ~Activity() = default;

protected:
	/**
	 * @brief Activity 被创建后、运行前的处理。
	 * @param wnd: 对所属 Window 的引用。
	*/
	virtual bool start(GUI::Window& wnd) noexcept = 0;
	/**
	 * @brief Activity 结束后、移除前的处理。
	*/
	virtual void stop() noexcept = 0;

public:
	/**
	 * @brief 处理事件。
	 * @param evt: SFML 的事件。
	 * @return 返回 false 表示一切正常，返回 true 表示立即清空消息队列。
	*/
	virtual bool handleEvent(const sf::Event& evt) = 0;
	/**
	 * @brief 更新（包括绘制）。
	 * @param dtime: 经过的时间。
	*/
	virtual void update(sf::Time dtime) = 0;

	/**
	 * @brief 进入系统循环时的通知。
	*/
	virtual void OnEnterSysloop() noexcept;
	/**
	 * @brief 退出系统循环时的通知。
	*/
	virtual void OnExitSysloop() noexcept;
}; // class IActivity

} // namespace GUI
